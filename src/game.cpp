/*
 *   NinjaJump
 *
 *   Copyright (C) 2009 T. Stokkeland, D. Tverbak,
 *   E. Bradley and oyvindln
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "game.h"

#include <cassert>
#include <cmath>

#include <QTimerEvent>

#include "constants.h"
#include "dummybox.h"
#include "ingameview.h"
#include "player.h"
#include "platform.h"
#include "powerup.h"
#include "level.h"
#include "makeunique.h"

/*!
  Constructor for the game object.
  Loads the pixmaps, and sets some variables.
  */
Game::Game(int maxPlayers, unsigned rounds, quint32 updateRate)
    : m_maxPlayers(maxPlayers), m_rounds(rounds),
      m_playerSprite(constants::FILENAME_Ninja),
      m_platformSprite(constants::FILENAME_Platform),
      m_slashSprite("slash.png"), m_updateRate(updateRate) {

  m_mapName =
      constants::FILENAME_Levels_Prefix + constants::FILENAME_Default_Level;

  level = Utils::make_unique<Level>();
  m_view = 0;
  m_scene = 0;
  //	//std::cout << m_playerSprite.isNull() << std::endl;

  // powerup variables should be connected with menu options
  m_maxPowerUps = 5;
  m_minPowerUpRespawnTime = 10000;
  m_maxPowerUpRespawnTime = 80000;
  m_PowerUpTimeCounter = 0;
  m_PowerUpSpawnChance = 1;
  m_CurrentRand = 0;
  m_CurrentPosRand = 0;
  m_CurrentPowerRand = 0;
  m_powerupID = 0;

  m_powerupSprites[GameObject::PT_POWER] =
      QPixmap(constants::FILENAME_Powerup_Sword);
  m_powerupSprites[GameObject::PT_SPEED] =
      QPixmap(constants::FILENAME_Powerup_Speed);
  m_powerupSprites[GameObject::PT_SHIELD] =
      QPixmap(constants::FILENAME_Powerup_Shield);

  m_localPlayer = 0;

  balanceModifier = 1.0;
  lastBalanceTime = 0;

#ifndef QT_NO_DEBUG

  QImage box(constants::FILENAME_Dummy1);
  m_serverPos = new DummyBox(QPixmap::fromImage(box));
  box.invertPixels();
  m_predPos = new DummyBox(QPixmap::fromImage(box));
  box = box.rgbSwapped();
  m_playerPos = new DummyBox(QPixmap::fromImage(box));
#endif
}

Game::~Game() {
  // Delete scene and view.
  delete m_scene;
  delete m_view;
}

IngameView *Game::getGraphicsView() const { return m_view; }

/*!
  Overriden function to make the timer call the update function.
  */
void Game::timerEvent(QTimerEvent *event) {
  if (event->timerId() == m_timer.timerId()) {
    update();
  }
}

/*!
  Load the graphics and map, generate powerup spawn locations, and prepare for
  starting.
  */
void Game::start(qint64 seed) {

  // Replay functionality is currently disabled.
  m_save = false; // saveReplay
  m_load = !m_save; // loadReplay
  m_load = false;

  if (m_load) {
    m_replayFile = new QFile("replay.txt");
    loadReplay();
  }
  if (m_load) {
    addPlayer(QString("test"));
  }

  // Load the level
  level->loadLevel(m_mapName);
  qDebug() << "Game::Loading map:" << m_mapName;

  //    qDebug() << platforms.size();
  // Put get the platforms from the level.
  for (auto &platformData : level->getPlatforms()) {
    Platform *platform = new Platform(m_platformSprite);
    platform->setPos(
        platformData.line.p1()); // TODO, implement morphed platforms.
    m_platforms.push_back(platform);
  }

  // Initialize the level with the given size.
  m_scene = new QGraphicsScene(0, 0, level->width, level->height);

  // Set background.
  QPixmap background(constants::FILENAME_Background);

  m_scene->setBackgroundBrush(background);
  m_scene->setItemIndexMethod(QGraphicsScene::NoIndex);

  // Set up graphics view widget.
  m_view = new IngameView(m_scene, m_localPlayer, m_players /*, parent*/);
  m_view->setInteractive(false);
  m_view->setCacheMode(QGraphicsView::CacheBackground);
  //    m_view->scale(0.5, 0.5);

  connect(m_view, SIGNAL(localMove(const QLine &)), this,
          SLOT(viewPlayerMove(const QLine &)));

  // TODO: Predefined spawn-points.
  // Add players to the scene.
  qreal i = 20.0;
  Player *player;
  foreach (player, m_players) {
    m_scene->addItem(player);
    player->setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
    player->setPos(i, 90);
    i += 100.0;
    player->setVelocity(0, 0);
    player->setMoving(true);

    m_playerUpdateRects.append(player->sceneBoundingRect());
  }

  // Add the platforms to the scene.
  Platform *item;
  foreach (item, m_platforms) { m_scene->addItem(item); }

  m_gravity = constants::Default_Gravity;

  // Generate powerup spawn points.
  if (seed != 0) {
    srand(seed);
  } else {
    srand(time(0));
  }
  int tmp_width = (int)m_scene->sceneRect().width();
  int tmp_height = (int)m_scene->sceneRect().height();
  // std::cout << "height ;: " << tmp_height << std::endl;
  for (int i = 0; i < 1000; i++) {
    m_rand[i] = rand() % 100;
    m_randY[i] = rand() % (tmp_height - 100) + 50;
    m_randX[i] = rand() % (tmp_width - 100) + 50;
  }

  for (int i = 0; i < 100; i++) {
    m_randPowerType[i] = rand() % GameObject::PT_NUMBER;
  }

  m_waitMessage = new QGraphicsTextItem(tr("Waiting"), 0);
  m_waitMessage->setScale(5.0);
  m_waitMessage->setPos(100.0, 100.0);
  m_waitMessage->setZValue(20);
  m_scene->addItem(m_waitMessage);

  m_view->centerOn(0, 0);

  m_firstUpdate = true;
}

/*!
  Add a player to the game, used by network classes.
  */
int Game::addPlayer(const QString &name) {
  if (m_players.size() < m_maxPlayers) {
    m_players.push_back(new Player(name, m_players.size(), m_playerSprite));
    return m_players.size() - 1;
  }

  // else
  return -1;
}

/*!
  Set what player is the local player.
  */
bool Game::setPlayer(int playerNumber) {
  assert(playerNumber < m_players.size());
  m_localPlayer = playerNumber;
  qDebug() << "Local player " << playerNumber;
  return true;
}

/*!
  Move the given player
  */
void Game::movePlayer(QLine moveVector, int playerNumber) {
  // std::cout << "Moving player" << playerNumber << std::endl;
  if (playerNumber <= m_players.size()) {
    QLineF temp = QLineF(moveVector);
    temp.setLength(constants::Default_Speed_Vector_Length);

    m_players[playerNumber]->setVelocity(
        static_cast<qreal>(temp.x2() - temp.x1()),
        static_cast<qreal>(temp.y2() - temp.y1()));
    ////std::cout << " Something " << moveVector.x1()-moveVector.x2() << "  " <<
    ///moveVector.y1()-moveVector.y2() << std::endl;
    m_players[playerNumber]->setMoving(true);
    qDebug() << "Game::Player move, new speed: "
             << m_players[playerNumber]->getVelocity() << " id "
             << playerNumber;
  }

  if (m_save) {
    // TODO - When enabling replays, do this properly, not with malloc
    /*
    char * simontest;
    simontest = (char *)malloc(200);
    sprintf(simontest, "%i:%i:%6.2f:%6.2f\n", m_gameTimer.elapsed(),
    playerNumber, moveVector.x2()-moveVector.x1(),
    moveVector.y2()-moveVector.y1());
    m_replayData.append(simontest);
    replaySave(m_replayData);
    */
  }
}

/*!
  Kill a player and put the player at the new respawn point.
  */
void Game::killRespawn(quint8 killedPlayerNumber, quint8 killerPlayerNumber,
                       const QPointF &newPos) {
  qDebug() << "Player killed, spawns at: " << newPos;
  m_players[killedPlayerNumber]->setPos(newPos);
  m_players[killedPlayerNumber]->setVelocity(0, 0);
  m_players[killedPlayerNumber]->setMoving(true);
  m_players[killedPlayerNumber]->setBalance(0);
  m_players[killerPlayerNumber]->addScore();
  m_scene->invalidate();
  m_view->updateScore();
}

/*!
  Save the given replay.
  */
void Game::replaySave(QByteArray replayData) {
  if (m_save) {

    if (!m_replayFile->open(QIODevice::WriteOnly | QIODevice::Text)) {
      return;
    }
    QTextStream m_replayStream(m_replayFile);
    m_replayStream << replayData;
    m_replayFile->close();
  }
}

/*!
  Load a replay.
  */
void Game::loadReplay() {
  if (!m_replayFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
    qDebug() << "Error loading replay: " << m_replayFile->errorString();
    return;
  }

  while (!m_replayFile->atEnd()) {
    QByteArray line = m_replayFile->readLine();
    line.chop(1);

    QList<QByteArray> loadList = line.split(':');
    replay.push_back(Replay(
        int(loadList.takeLast().toInt()), int(loadList.takeLast().toInt()),
        int(loadList.takeLast().toFloat()), int(loadList.takeLast().toFloat()),
        15)); // int(*loadList.takeFirst().data()),int(*loadList.takeFirst().data()),int(*loadList.takeFirst().data()),25));
  }
}

/*
  Public slots
  */

/*!
  Move the local player, called from ingameView.
  \param moveVector The players new direction.
  */
void Game::viewPlayerMove(const QLine &moveVector) {
  movePlayer(moveVector, m_localPlayer);
}

/*!
  Start the game.

  */
void Game::begin() {

  // Start the timers.
  m_timer.start(m_updateRate, this);
  qDebug() << m_updateRate;
  m_frameTimer.start();
  m_gameTimer.start();

  m_scene->removeItem(m_waitMessage);
  delete m_waitMessage;

  // Dummy boxes for network testing.
  if (m_localPlayer != 0) {
    m_scene->addItem(m_playerPos);
    m_scene->addItem(m_serverPos);
    m_scene->addItem(m_predPos);
  }
}

/*
  Protected
  */

/*!
  Add a new powerup to the scene.
  */
void Game::addPowerUp() {

  // Allocate a new powerup object.
  m_powerups.push_back(
      new PowerUp(m_powerupSprites[m_randPowerType[m_CurrentPowerRand]],
                  m_randPowerType[m_CurrentPowerRand], m_powerupID));

  m_powerupID++;
  m_CurrentPowerRand++;
  if (m_CurrentPowerRand == 100) {
    m_CurrentPowerRand = 0;
  }

  m_powerups.back()->setPos(m_randX[m_CurrentPosRand],
                            m_randY[m_CurrentPosRand]);
  m_powerups.back()->setScale(0.5);

  m_scene->addItem(m_powerups.last());

  m_CurrentPosRand++;
  if (m_CurrentPosRand == 1000) {
    m_CurrentPosRand = 0;
  }
}

/*!
  Main game update function.
  */
void Game::update() {
  // Make sure the whole scene is redrawn in the first frame update.
  if (m_firstUpdate) {
    m_scene->invalidate();
    m_view->centerOn(m_players[m_localPlayer]);
    m_firstUpdate = false;
  }
  int tmp_elapsed = m_frameTimer.elapsed();

  // If the time used is less than 1 ms, we don't do anything, no point trying
  // to move with a delta of 0.
  if (tmp_elapsed < 1) {
    return;
  }
  m_delta = static_cast<qreal>(tmp_elapsed) / 1000.0;
  m_PowerUpTimeCounter += tmp_elapsed;
  m_players[m_localPlayer]->reduceTime(tmp_elapsed);

  updateBalance();

  m_frameTimer.restart();

  // Move replay player. Disabled.
  /*    if (m_load)
      {
          if(!replay.empty())
          {
              if(replay.front().time < m_gameTimer.elapsed())
              {
                  movePlayer(QLineF(QLine(0,0,replay.front().x,replay.front().y)),replay.front().player);
                  replay.pop_front();
              }
          }
      }*/

  checkCollisions(m_delta);
  //    m_view->centerOn(m_players[m_localPlayer]); //TODO, only if player
  //    moved.

  // Scroll the view to the player.
  // Creates some artifacts so we have to tell the scene to update some sections
  // after calling this.

  if (m_players[m_localPlayer]->isMoving()) {
    m_view->centerOn(m_players[m_localPlayer]);
  }
  // Schedule a repaint in the areas where the player moved.
  QList<QRectF>::const_iterator it;
  for (it = m_playerUpdateRects.begin(); it != m_playerUpdateRects.end();
       ++it) {
    const QRectF &rect = *it;
    m_scene->update(rect);
  }

  // for powerup spawning
  if (m_maxPowerUps + 1 > m_powerups.size()) {
    if (m_PowerUpTimeCounter > m_maxPowerUpRespawnTime) {
      //    //std::cout << " counter : " << m_PowerUpTimeCounter << " maxTimer :
      //    " << m_maxPowerUpRespawnTime << std::endl;
      addPowerUp();
      m_PowerUpTimeCounter = 0;

    } else if (m_PowerUpTimeCounter > m_minPowerUpRespawnTime) {
      //   //std::cout << "counter : " << m_PowerUpTimeCounter << " minTimer : "
      //   << m_minPowerUpRespawnTime << std::endl;

      if (getRand() < m_PowerUpSpawnChance) {
        addPowerUp();
        m_PowerUpTimeCounter = 0;
      }
    }
  }

  // Make sure energy bar is updated.
  QRectF barRect = m_view->getEnergyBarSceneRect();
  m_scene->update(barRect);

  // Make sure score board is updated.
  m_scene->update(QRectF(m_view->mapToScene(QPoint(0, 0)),
                         m_view->mapToScene(QPoint(m_view->width(), 25))));

  // Print update rate.
  if (m_frameRateTimer.elapsed() >= 1000) {
    qreal framerate = m_updates / (m_frameRateTimer.elapsed() / 1000.0);
    m_updates = 0;
    m_frameRateTimer.restart();
    m_view->setFrameRate(framerate);
    //        qDebug() << framerate;
  } else {
    m_updates++;
  }
}

/*!
  Checks for collisions between players and objects, using the given delta.
  */
void Game::checkCollisions(qreal delta) {

  Player *player;
  int j;

  for (int i = 0; i < m_players.size(); ++i) {
    player = m_players[i];

    m_playerUpdateRects[i] = player->sceneBoundingRect();

    if (player->isMoving()) {

      // Checks walls first and generate movement.
      QPointF movement = checkWallCollide(*player, delta);
      // Move to check if the player collides with anything, then move back

      // Checks if player collides with any of the platforms, and generate
      // further movement.
      for (j = 0; j < m_platforms.size(); j++) {
        playerPlatformCollide(player, *m_platforms[j], movement);
      }

      player->moveBy(movement.x(), movement.y());

      // Checks if the player collides with a powerup.
      for (j = 0; j < m_powerups.size(); j++) {
        if (player->collidesWithItem(m_powerups[j])) {
          player->addPower(m_powerups[j]->getType());
          removePowerUp(m_powerups[j]->getID());
        }
      }

    } else {
      QPointF move = player->adjust(delta);
      player->moveBy(move.x(), move.y());
    }

    // Check if the player collided with someone.
    for (j = i + 1; j < m_players.size(); j++) {
      playerPlayerCollide(player, m_players[j]);
    }
  }
}

/*!
  Kills the given player, and generates a new position.
  (Used for host.)
  */
void Game::killPlayer(Player *player) {
  //    emit wantSync();

  while (player->collidingItems().size() != 0) {
    qreal x =
        rand() % (static_cast<int>(m_scene->sceneRect().width()) - 200) + 100;
    qreal y =
        rand() % (static_cast<int>(m_scene->sceneRect().height()) - 200) + 10;
    player->setPos(x, y);
    player->setMoving(true);
    player->setVelocity(0, 0);
    player->setBalance(0);
    m_scene->invalidate();
    m_view->updateScore();
  }
}

/*!
  Check collision with walls
  */
QPointF Game::checkWallCollide(Player &player, qreal delta) {
  player.addVelocity(0, m_gravity * delta);
  QPointF move = player.getVelocity() * delta;
  move += player.adjust(delta);
  //
  QRectF sceneRect = m_scene->sceneRect();
  // TODO Lose speed when hitting things.

  if (player.sceneBoundingRect().left() + move.x() < sceneRect.left()) {
    // Collision with left wall
    player.reverseMovementX();
    move.rx() = (2 * sceneRect.left()) -
                (2 * player.sceneBoundingRect().left()) - move.x();
    emit wantSync();
    //		xmove = x() + x() + xmove; Not sure if border allways is 0.

  } else if (move.x() + player.sceneBoundingRect().right() >
             sceneRect.right()) {
    // Collision with right wall
    player.reverseMovementX();
    move.rx() = (2 * sceneRect.right()) -
                (2 * player.sceneBoundingRect().right()) - move.x();
    emit wantSync();
  }

  if (player.sceneBoundingRect().top() + move.y() < sceneRect.top()) {
    // Collision with top wall.
    player.reverseMovementY();
    move.ry() = (2 * sceneRect.top()) - (2 * player.sceneBoundingRect().top()) -
                move.y();
    emit wantSync();
  }
  if (move.y() + player.sceneBoundingRect().bottom() > sceneRect.bottom()) {
    // Collision with bottom wall.
    // player.reverseMovementY();
    player.stop();
    // player.setPos(player.x(), sceneRect.bottom() -
    // player.sceneBoundingRect().height());
    QPointF newPos(player.x() + ((sceneRect.bottom() -
                                  player.sceneBoundingRect().bottom()) *
                                 (move.x() / move.y())),
                   sceneRect.bottom() - player.sceneBoundingRect().height());
    player.setPos(newPos);
    move.rx() = 0;
    move.ry() = 0;
    //		move.ry() = -0.1;

    // move.ry() = (2 * sceneRect.bottom()) - (2 *
    // player.sceneBoundingRect().bottom()) - move.y();

    emit wantSync();
  }

  return move;
}

/*!
  Check collision with players.
  */
void Game::playerPlatformCollide(Player *player, const Platform &platform,
                                 QPointF &movement) {
  // Handles collision, moves the player accordingly,
  // and changes direction or stops player depending on where he collides.

  QRectF playerR = player->sceneBoundingRect();
  playerR.translate(movement);
  QRectF platformR = platform.sceneBoundingRect();

  if (playerR.top() < platformR.bottom() &&
      playerR.bottom() > platformR.top() &&
      playerR.left() < platformR.right() &&
      playerR.right() >
          platformR.left()) // Checks if player is colliding with platform
  {
    if (std::fabs(playerR.top() - platformR.bottom()) <
        std::fabs(playerR.bottom() - platformR.top())) {
      if (std::fabs(playerR.left() - platformR.right()) <
          std::fabs(playerR.right() - platformR.left())) {
        // Bottom right
        if (std::fabs(playerR.top() - platformR.bottom()) <
            std::fabs(playerR.left() - platformR.right())) {
          //                                        player->setPos(player->x(),platformR.bottom()+1);
          player->reverseMovementY();
          qDebug("A");
          movement.ry() =
              (2 * platformR.bottom()) - (2 * playerR.top()) - movement.y();
        }
        // Right lower
        else {
          //                                        player->setPos(platformR.right()+1,player->y());
          player->reverseMovementX();
          qDebug("B");
          movement.rx() =
              (2 * platformR.right()) - (2 * playerR.left()) - movement.x();
        }
      } else {
        // Bottom left
        if (std::fabs(playerR.top() - platformR.bottom()) <
            std::fabs(playerR.right() - platformR.left())) {
          //                                        player->setPos(player->x(),platformR.bottom()+1);
          player->reverseMovementY();
          qDebug("C");
          movement.ry() =
              (2 * platformR.bottom()) - (2 * playerR.top()) - movement.y();
        }
        // Left lower
        else {
          // player->setPos(platformR.left()-playerR.width(),player->y());
          player->reverseMovementX();
          qDebug("D");

          movement.rx() =
              (2 * platformR.left()) - (2 * playerR.right()) - movement.x();
        }
      }
    } else {
      if (std::fabs(playerR.left() - platformR.right()) <
          std::fabs(playerR.right() - platformR.left())) {
        // Top right
        if (std::fabs(playerR.bottom() - platformR.top()) <
            std::fabs(playerR.left() - platformR.right())) {
          player->stop();
          QPointF newPos(
              player->x() +
                  ((platformR.top() - player->sceneBoundingRect().bottom()) *
                   (movement.x() / movement.y())),
              platformR.top() - player->sceneBoundingRect().height());
          player->setPos(newPos);
          movement.setX(0);
          movement.setY(0);
          //                              player->setPos(player->x(),
          //                              platformR.top()-playerR.height());
          player->setVelocity(0, 0); //-player->m_velocityY;
          qDebug("G");
        }
        // Right upper
        else {
          qDebug("E");
          //                                        player->setPos(platformR.right()+1,
          //                                        player->y() );
          movement.rx() =
              (2 * platformR.right()) - (2 * playerR.left()) - movement.x();
          player->reverseMovementX();
        }
      } else {
        // Top left
        if (std::fabs(playerR.bottom() - platformR.top()) <
            std::fabs(playerR.right() - platformR.left())) {
          player->stop();

          //                              player->setPos(player->x(),
          //                              platformR.top()-playerR.height());
          QPointF newPos(
              player->x() +
                  ((platformR.top() - player->sceneBoundingRect().bottom()) *
                   (movement.x() / movement.y())),
              platformR.top() - player->sceneBoundingRect().height());
          player->setPos(newPos);
          movement.setX(0);
          movement.setY(0);
          player->setVelocity(0, 0);

          qDebug("H");
        }
        // Left upper
        else {
          qDebug("F");

          movement.rx() =
              (2 * platformR.left()) - (2 * playerR.right()) - movement.x();
          //                              player->setPos(platformR.left()-playerR.width()-1,
          //                              player->y() );
          player->reverseMovementX();
        }
      }
    }
    emit wantSync();
  }
}

/*!
  Check collision with players.
  */
void Game::playerPlayerCollide(Player *player, Player *player2) {
  QRectF playerR = player->sceneBoundingRect();
  QRectF itemR = player2->sceneBoundingRect();

  if (QLineF(playerR.bottomLeft(), itemR.bottomLeft()).length() <
      QLineF(playerR.bottomLeft(), playerR.bottomRight()).length()) {

    if (player->getLoadedPower() > player2->getLoadedPower()) {

      if (m_localPlayer == 0) {
        player->addScore();
        //   //std::cout << "haha" << std::endl;
        player->setPower(0);
        killPlayer(player2);
        emit playerKilled(player2->getPlayerNumber(), player->getPlayerNumber(),
                          player2->scenePos());
      }

    } else if (player->getLoadedPower() < player2->getLoadedPower()) {
      if (m_localPlayer == 0) {
        player2->addScore();
        //            //std::cout << "haha" << std::endl;
        player2->setPower(0);
        killPlayer(player);
        emit playerKilled(player->getPlayerNumber(), player2->getPlayerNumber(),
                          player->scenePos());
      }
    }
    // If power is the same, no one dies. maybe we should do somethin else.
    /*        for (int i=0; i<m_playerInfo.size(); i++)
            {
                m_playerInfo[i]->setPlainText(m_players[i]->getPlayerName()+":
       "+QString::number(int(m_players[i]->getScore())));
            }*/
  }
}

/*!
  get the next generated random number.
  */
int Game::getRand() {
  int tmp = m_CurrentRand;
  m_CurrentRand++;
  if (m_CurrentRand == 1000) {
    m_CurrentRand = 0;
  }

  return m_rand[tmp];
}

/*!
  Remove a powerup from the scene.
  Called if someone picks up a powerup.
  */
void Game::removePowerUp(int id) {
  for (int i = 0; i < m_powerups.size(); i++) {
    if (m_powerups[i]->getID() == id) {
      m_scene->removeItem(m_powerups[i]);
      PowerUp *powerup = m_powerups[i];
      m_powerups.removeAt(i);
      delete powerup;
    }
  }
}

/*!
  Updates the players balance.
  */
void Game::updateBalance() {
  QList<Player *>::iterator it;
  for (it = m_players.begin(); it != m_players.end(); ++it) {
    Player *player = *it;
    // Balance modifier changes depending on powerups
    if (player->isMoving() && player->getBalance() < 50) {
      if (player->getBalance() < 0) {
        player->addBalance((m_frameTimer.elapsed() * player->getWindMod()) /
                           100);
      } else {
        player->addBalance(m_frameTimer.elapsed() /
                           (100 * player->getEarthMod()));
      }
    } else if (!player->isMoving() && player->getBalance() > -50) {
      if (player->getBalance() > 0) {
        player->addBalance(-(m_frameTimer.elapsed() * player->getWindMod()) /
                           50);
      } else {
        player->addBalance(
            -(m_frameTimer.elapsed() / (50 * player->getEarthMod())));
      }
    }
  }
}
