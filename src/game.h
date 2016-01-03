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

#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QBasicTimer>
#include <QGraphicsScene>
#include <QList>
#include <QTime> //std::clock is not very precise.
#include <QFile> //for saving/loading replay
#include <QTextStream>
#include <QtDebug>
#include <QGraphicsTextItem>

#include <memory>
#include <vector>

#include "replay.h"
#include "gameobject.h"

class DummyBox;
class IngameView;
class Level;
class Player;
class Platform;
class PowerUp;

/*!
    Class containing the main game logic.
*/
class Game : public QObject {
  Q_OBJECT
public:
  Game(int maxPlayers, unsigned rounds,
       /*, QString mapName */ quint32 updateRate = 17);

  ~Game();

  int addPlayer(const QString &name);
  bool setPlayer(int playerNumber);
  void start(qint64 seed = 0);

  // To set the widget in the main window
  IngameView *getGraphicsView() const;

  void timerEvent(QTimerEvent *event);
  void movePlayer(QLine moveVector, int playerNumber);
  void setMap(const QString &filename);
  const QString &getMap() const;
  QList<Player *> &getPlayers();

  void killRespawn(quint8 killedPlayerNumber, quint8 killerPlayerNumber,
                   const QPointF &newPos);
  int getElapsedTime() const;

  void setDelay(int serverTime);
  int getDelay();
  qreal getGravity();

// NOTE:
// For testing, remove these afterwards.
#ifndef QT_NO_DEBUG
  DummyBox *m_serverPos;
  DummyBox *m_predPos;
  DummyBox *m_playerPos;
#endif

signals:
  //[somestatdata] gameFinished();
  void wantSync();
  void playerKilled(quint8 killedPlayerNumber, quint8 killerPlayerNumber,
                    const QPointF &newPos);
public slots:
  void begin();
  void viewPlayerMove(const QLine &moveVector);

protected:
  void update();

  void checkCollisions(qreal delta);
  void killPlayer(Player *player);

  // player get's altered
  QPointF checkWallCollide(Player &player, qreal delta);
  void playerPlatformCollide(Player *player, const Platform &platform,
                             QPointF &movement);
  void playerPlayerCollide(Player *player, Player *player2);

private:
  QList<Platform *> m_platforms; // To be moved into a map object.

  QList<QRectF> m_playerUpdateRects;
  QList<Player *> m_players;
  QList<PowerUp *> m_powerups;
  QGraphicsTextItem *m_waitMessage;
  std::unique_ptr<Level> level;

  // Game options
  int m_maxPlayers;
  unsigned m_rounds;
  qreal m_gravity;
  int m_localPlayer;

  QGraphicsScene *m_scene;
  IngameView *m_view;
  QBasicTimer m_timer;

  // powerup variables
  short m_maxPowerUps;
  int m_minPowerUpRespawnTime;
  int m_maxPowerUpRespawnTime;
  int m_PowerUpTimeCounter;
  short m_PowerUpSpawnChance;
  short m_rand[1000];
  short m_randX[1000];
  short m_randY[1000];
  short m_CurrentPosRand;
  short m_CurrentRand;
  short m_randPowerType[100];
  short m_CurrentPowerRand;
  int m_powerupID;

  // Should have a list with more sprites.
  QPixmap m_playerSprite;
  QPixmap m_platformSprite;
  QPixmap m_powerupSprites[GameObject::PT_NUMBER];
  QPixmap m_ninjaSprites[5];
  QPixmap m_slashSprite;
  QList<QPixmap> sprites;

  QList<Replay> replay;

  QTime m_frameTimer;
  QTime m_gameTimer;
  int m_lastFrame;
  int m_currentFrame;
  qreal m_delta; // Should find a way to avoid too many float values.

  bool m_save;
  bool m_load;
  QFile *m_replayFile;
  QString m_replayName;
  QTextStream m_replayStream;
  QByteArray m_replayData;

  void replaySave(QByteArray replayData);
  void loadReplay();
  void addPowerUp();
  int getRand();
  void removePowerUp(int id);

  void updateBalance();
  float balanceModifier;
  double lastBalanceTime;

  QTime m_frameRateTimer;
  quint32 m_updates;
  bool m_firstUpdate;
  quint32 m_updateRate;
  int m_delay;
  QString m_mapName;
};

inline QList<Player *> &Game::getPlayers() { return m_players; }

inline void Game::setMap(const QString &filename) { m_mapName = filename; }

inline const QString &Game::getMap() const { return m_mapName; }

inline int Game::getElapsedTime() const { return m_gameTimer.elapsed(); }

/*
  Sets the current delay based on the server time.
  */
inline void Game::setDelay(int serverTime) {
  m_delay = m_gameTimer.elapsed() - serverTime;
  if (m_delay < 0) {
    m_delay = 0;
  }
  qDebug() << "Server time:" << serverTime
           << "local: " << m_gameTimer.elapsed();
}

inline int Game::getDelay() { return m_delay; }

inline qreal Game::getGravity() { return m_gravity; }
#endif
