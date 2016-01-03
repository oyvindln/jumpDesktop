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

#include "parser.h"

#include <cassert>

#include <QLine>

#include "constants.h"
#include "game.h"
#include "player.h"
#include "playerdata.h"

Parser::Parser(bool isServer, QObject *)
    : m_bufferStream(&m_buffer, QIODevice::WriteOnly), m_game(0),
      m_isServer(isServer) {
  m_bufferStream.setVersion(
      constants::DATASTREAM_VERSION); // Have to decide what version to support.
  // m_bufferStream.setDevice(
}

void Parser::setGame(Game *game) { m_game = game; }

/*!
  Prepare buffer for sending.
  */
void Parser::prepareBuffer(qint8 command) {
  m_buffer.clear();
  m_bufferStream.skipRawData(10); // Make sure buffer is empty.
  m_bufferStream << (quint16)0;
  m_bufferStream << command;
  if (m_isServer) {
    if (m_game) {
      m_bufferStream << static_cast<qint32>(m_game->getElapsedTime());
    } else {
      m_bufferStream << static_cast<qint32>(-1);
    }
  }
}

/*!
  Set first two bytes of buffer to the length of the buffer.
  FIXME: fix function name to reflect what it does.
  */
void Parser::sendBuffer() {
  m_bufferStream.device()->seek(0);
  m_bufferStream << (quint16)(m_buffer.size() - sizeof(quint16));
}

/*!
  Player asking to join.
  */
QByteArray Parser::playerReqJoin(const QString &name) {
  prepareBuffer(commands::COMMAND_CL_REQJOIN);

  m_bufferStream << name;
  sendBuffer();
  return m_buffer;
}

void Parser::playerReqJoinReceived(QDataStream &input, quint8 id) {
  QString name;
  input >> name;
  // std::cout << name.constData() << " is requesting to join" << std::endl;
  // TODO, send name
  //    m_networkHandler->joinRequest(id);
  emit serverJoinRequest(id, name);
}

/*!
  Player left.
  //Not used
  */
QByteArray Parser::playerLeave() {
  /*!    m_buffer.clear();
      quint8 command = commands::COMMAND_CL_DROP;
      m_bufferStream << command;*/
  prepareBuffer(commands::COMMAND_CL_DROP);
  sendBuffer();
  return m_buffer;
}

void Parser::playerLeaveReveived(QDataStream &, quint8) {
  /*
    qint8 playerNumber;
    input >> playerNumber;
    */
  // network->handleDisconnect();
}

/*!
  Movement from player.
  */
QByteArray Parser::playerMove(quint8 playerNumber, const QLine &movement) {
  prepareBuffer(commands::COMMAND_CL_JUMP);
  m_buffer.reserve(17);
  /*    data.append(command);
      data.append(m_packer.intToData(movement.p1().x() ) );//Append
      data.append(m_packer.intToData(movement.p1().y() ) );
      data.append(m_packer.intToData(movement.p2().x() ) );//Append
      data.append(m_packer.intToData(movement.p2().y() ) );*/
  m_bufferStream << playerNumber;
  m_bufferStream << movement;
  sendBuffer();
  return m_buffer;
}

void Parser::playerMoveReceived(QDataStream &input,
                                quint8 playerId) // TODO add player number
{
  quint8 playerNumber;
  input >> playerNumber;
  QLine line;
  input >> line;

  if (playerNumber != playerId) {
    qDebug(
        "Warning: playerId (%u) is different from what the client sendt (%u)",
        static_cast<quint32>(playerId), static_cast<quint32>(playerNumber));
  }

  if (m_game) // Make sure game exists. Should be possible to remove this later.
  {
    qDebug() << "Parser::Player move from client: " << line;
    m_game->movePlayer(line, playerId);
    emit playerMoveNeedsSending(playerId, line);
  }
}

/*!
  Player want a sync message.
  //Not used
  */
QByteArray Parser::playerWantSync() {
  prepareBuffer(commands::COMMAND_CL_WANTSYNC);
  sendBuffer();
  return m_buffer;
}

void Parser::playerWantSyncReceived(QDataStream &, quint8) {
  /*
qint8 playerNumber;
input >> playerNumber;
*/
  // network->handleSync
}

QByteArray Parser::playerReady() {
  prepareBuffer(commands::COMMAND_CL_READY);
  sendBuffer();
  return m_buffer;
}

void Parser::playerReadyReceived(QDataStream &, quint8 playerId) {
  emit readyPlayer(playerId);
  qDebug("Player ready");
}

/*!
  Broadcast message
  //Not implemented
  */
/*
QByteArray Parser::serverBroadCast()
{
}
*/

/*!
  Test message....... for testing!
  */
QByteArray Parser::playerTestMessage(const QString &message) {
  prepareBuffer(commands::COMMAND_CL_TEST);
  m_bufferStream << message;
  sendBuffer();
  return m_buffer;
}

/*!
  Server accepted join request, sends info back.
  */
QByteArray Parser::serverAcceptJoin(quint8 playerNumber, qint64 seed) {
  prepareBuffer(commands::COMMAND_SV_ACCEPTJOIN);
  m_bufferStream << playerNumber;
  m_bufferStream << seed;
  sendBuffer();
  return m_buffer;
}

void Parser::serverAcceptJoinReceived(QDataStream &input) {

  quint8 playerNumber;
  qint64 seed;
  input >> playerNumber;
  input >> seed;
  qDebug() << "Connection accepted, player id: "
           << static_cast<quint32>(playerNumber) << "Seed: " << seed;
  emit clientSetPlayerId(playerNumber, seed);

  // Done in networkClient.
  /*      if(m_game != 0)
        {
          m_game->setPlayer(playerNumber);
        }
        else
        {
            //std::cout << "Game does not exist! " << std::endl;
        }*/
  // send message to GUI
}

/*!
  Info about the clients currently connected to the server.
  */
QByteArray Parser::serverClientInfo(const QList<QString> &names,
                                    const QList<QHostAddress> &ips) {
  prepareBuffer(commands::COMMAND_SV_CLIENTINFO);
  m_bufferStream << names;
  m_bufferStream << ips;
  sendBuffer();
  return m_buffer;
}

void Parser::serverClientInfoReceived(QDataStream &input) {
  QList<QString> names;
  QList<QHostAddress> ips;

  input >> names;
  input >> ips;
  emit clientInfo(names, ips);
  // Send data to GUI
}

/*!
  Load in the map, prepare to start.
  */
QByteArray Parser::serverLoadIn(const QString &mapName) {
  prepareBuffer(commands::COMMAND_SV_LOADIN);
  m_bufferStream << mapName;
  sendBuffer();
  return m_buffer;
}

void Parser::serverLoadInReceived(QDataStream &input) {
  QString mapName;
  input >> mapName;
  emit gameStarting(mapName);
}

/*!
  Start the game.
  */
QByteArray Parser::serverStart() {
  prepareBuffer(commands::COMMAND_SV_START);
  //    m_bufferStream << mapName;
  sendBuffer();
  return m_buffer;
}

void Parser::serverStartReceived(QDataStream &) {
  //   QString mapName;
  //    input >> mapName;
  assert(m_game != 0);
  m_game->begin();
}

/*
    //Not used.
  */
QByteArray Parser::serverShutdown() {
  prepareBuffer(commands::COMMAND_SV_SHUTDOWN);
  sendBuffer();
  return m_buffer;
}

/*
    //Not used.
  */
QByteArray Parser::serverIsFull() {
  m_buffer.clear();
  qint8 command = commands::COMMAND_SV_FULL;
  m_bufferStream << command;
  sendBuffer();
  return m_buffer;
}

/*!
  Another client joined, send info about that client to this client.
  */
QByteArray Parser::serverPlayerJoined(const QString &name,
                                      const QHostAddress &ip, quint8) {
  prepareBuffer(commands::COMMAND_SV_PLAYERJOINED);
  m_bufferStream << name;
  m_bufferStream << ip;
  //    m_bufferStream << playerNumber;
  sendBuffer();
  return m_buffer;
}

void Parser::serverPlayerJoinedReceived(QDataStream &input) {
  QString playerName;
  QHostAddress ip;
  //    quint8 playerNumber;
  //    input >> playerNumber;
  input >> playerName;
  input >> ip;
  qDebug() << playerName << " joined";
  emit playerJoined(playerName, ip);
  // Send message to GUI
}

/*!
  Another player moved, data from server.
  */
QByteArray Parser::serverPlayerMove(quint8 playerNumber,
                                    const QLine &movement) {
  prepareBuffer(commands::COMMAND_SV_PLAYERMOVE);
  m_bufferStream << playerNumber;
  m_bufferStream << movement;
  sendBuffer();
  return m_buffer;
}

void Parser::serverPlayerMoveReceived(QDataStream &input) {
  quint8 playerNumber;
  QLine line;
  input >> playerNumber;
  input >> line;
  if (m_game != 0) {
    m_game->movePlayer(line, playerNumber);
    qDebug() << "Parser::Playermove from server: " << line;
  } else {
    qDebug() << "Game does not exist!";
  }
}

/*!
  Message from server, a player got killed.
  */
QByteArray Parser::serverPlayerKillRespawn(quint8 killedPlayerNumber,
                                           quint8 killerPlayerNumber,
                                           const QPointF &newPos) {
  prepareBuffer(commands::COMMAND_SV_PLAYERKILLRESPAWN);
  m_bufferStream << killedPlayerNumber;
  m_bufferStream << killerPlayerNumber;
  m_bufferStream << newPos;
  sendBuffer();
  return m_buffer;
}

void Parser::serverPlayerKillRespawnReceived(QDataStream &input) {
  quint8 killedPlayerNumber, killerPlayerNumber;
  QPointF newPos;
  input >> killedPlayerNumber;
  input >> killerPlayerNumber;
  input >> newPos;
  if (m_game != 0) {
    m_game->killRespawn(killedPlayerNumber, killerPlayerNumber, newPos);
    qDebug("player kill");
  } else {
    qDebug() << "Game does not exist!";
  }
  // Send message to Game
}

/*!
  Full syncronization message from server.
  If force is false, the syncing is extrapolated.
  If force is true, the game will snap everything into the recieved values.
  */
QByteArray Parser::serverSync(/*const QList<Player*>& players, */ bool force) {
  prepareBuffer(commands::COMMAND_SV_SYNC);

  m_bufferStream << force;

  QList<PlayerData> playerDataList;

  if (m_game) {
    const QList<Player *> &players = m_game->getPlayers();

    QList<Player *>::const_iterator it;
    for (it = players.constBegin(); it != players.constEnd(); ++it) {
      Player *player = *it;
      PlayerData playerData;
      playerData.position = player->pos();
      playerData.velocity = player->getVelocity();
      playerData.loadedPowerup = player->getLoadedPower();
      playerData.score = player->getScore();
      playerData.balance = player->getBalance();
      playerData.windMod = player->getWindMod();
      playerData.earthMod = player->getEarthMod();

      playerDataList.append(playerData);
    }
  } else {
    qDebug() << "Parser::serverSync: Warning: Game does not exist!";
  }
  m_bufferStream << playerDataList;
  sendBuffer();

  return m_buffer;
}

void Parser::serverSyncReceived(QDataStream &input) {
  bool force;
  input >> force;
  //    Player player("Peder", QPixmap());
  QList<PlayerData> playerDataList;
  input >> playerDataList;

  if (m_game) {
    QList<Player *> &players = m_game->getPlayers();

    QList<Player *>::iterator it;
    QList<PlayerData>::const_iterator dataIterator = playerDataList.begin();
    for (it = players.begin(); it != players.end(); ++it) {
      assert(dataIterator != playerDataList.end());
      Player *player = *it;
      const PlayerData &data = *dataIterator;
      PlayerData::syncData(data, player, m_game->getDelay(),
                           m_game->getGravity(), m_game);

      ++dataIterator;
    }
  } else {
    qDebug() << "Parser::serverSync: Warning: Game does not exist!";
  }
  //    qDebug() << playerDataList.back().position << " "<<
  //    playerDataList.back().velocity << " " <<
  //    playerDataList.back().loadedPowerup;
}

/*
    //Not used.
  */
QByteArray Parser::serverGameEnd(/**/) {
  prepareBuffer(commands::COMMAND_SV_GAMEEND);
  sendBuffer();
  return m_buffer;
}

/*
    //Not used.
  */
QByteArray Parser::serverPlayerDC(quint8 playerNumber) {
  prepareBuffer(commands::COMMAND_SV_PLAYERDC);
  m_bufferStream << playerNumber;
  sendBuffer();
  return m_buffer;
}

/*!
    Test message.
  */
QByteArray Parser::serverTestMessage(const QString &message) {
  prepareBuffer(commands::COMMAND_SV_TEST);
  m_bufferStream << message;
  sendBuffer();
  return m_buffer;
}

/*!
    Interpret commands from server to client.
*/
// TODO, fix name according to the other function.
void Parser::interpretData(QDataStream &input) {
  using namespace commands;
  qint8 command;
  input >> command;
  qint32 timeStamp;
  input >> timeStamp;

  if (m_game) {
    m_game->setDelay(timeStamp);
    qDebug() << "Delay: " << m_game->getDelay();
  }

  switch (command) {
  /*        case COMMAND_SV_BROADCAST:
          {
              serverBroadcastReceived(input);
              break;
          }*/
  case COMMAND_SV_ACCEPTJOIN: {
    serverAcceptJoinReceived(input);
    break;
  }
  case COMMAND_SV_CLIENTINFO: // Server info to player on join
  {
    serverClientInfoReceived(input);
    break;
  }
  case COMMAND_SV_LOADIN: // Game info when starting
  {
    serverLoadInReceived(input);
    break;
  }
  case COMMAND_SV_START: // Game info when starting
  {
    serverStartReceived(input);
    break;
  } /*
   case COMMAND_SV_SHUTDOWN:
   {
       serverShutdownReceived(input);
       break;
   }
   case COMMAND_SV_FULL:
   {
       serverIsFullReceived(input);
       break;
   }*/
  case COMMAND_SV_PLAYERJOINED: {
    serverPlayerJoinedReceived(input);
    break;
  } /*
   case COMMAND_SV_INFOCHANGED:
   {
       //TODO, implement
       break;
   }*/
  case COMMAND_SV_PLAYERMOVE: // Server info to player on join
  {
    serverPlayerMoveReceived(input);
    break;
  }
  case COMMAND_SV_PLAYERKILLRESPAWN: {
    serverPlayerKillRespawnReceived(input);
    break;
  }
  case COMMAND_SV_SYNC: // Syncronize server and client
  {
    //            qDebug("Sync received");
    serverSyncReceived(input);
    break;
  }
  /*        case COMMAND_SV_GAMEEND:
          {
              serverGameEndReceived(input);
              break;
          }
          case COMMAND_SV_PLAYERDC:
          {
              serverPlayerDCReceived(input);
              break;
          }*/
  case COMMAND_SV_TEST: {
    QString bla;
    input >> bla;
    qDebug() << "Test data from server:\n";
    qDebug() << bla;
    break;
  }

  default: {
    // std::cout << "Warning: Unknown command received." << std::endl;
    break;
  }
  }
}

/*!
    Interpret commands from client to server
*/
void Parser::interpretClientData(QDataStream &input, quint32 id) {

  using namespace commands;
  qint8 command;

  input >> command;

  switch (command) {

  case COMMAND_CL_RESERVED: {
    qDebug() << "Warning: received command 0";
  }
  case COMMAND_CL_REQJOIN: {
    playerReqJoinReceived(input, id);
    break;
  }
  /*        case COMMAND_CL_DROP:
          {
              playerLeaveReveived(input, id);
              break;
          }*/
  case COMMAND_CL_JUMP: {
    playerMoveReceived(input, id);
    break;
  }
  /*        case COMMAND_CL_WANTSYNC:
          {
              playerWantSyncReceived(input, id);
              break;
          }*/
  case COMMAND_CL_READY: {
    playerReadyReceived(input, id);
    break;
  }

  case COMMAND_CL_TEST: {
    QString bla;
    input >> bla;
    qDebug() << "Test data from client:\n";
    qDebug() << bla;
    break;
  }

  default: {
    // std::cout << "Warning: Unknown command received:\n";
    //            //std::cout << command << std::endl;
    break;
  }
  }
  // std::cout << input.device()->bytesAvailable() << std::endl;
}

void Parser::serverBroadcastReceived(QDataStream &) {
  /*
    --
    */
  // send message to GUI
}

void Parser::serverShutdownReceived(QDataStream &) {
  // etc
}

void Parser::serverIsFullReceived(QDataStream &) {
  // Send message to GUI
}

void Parser::serverGameEndReceived(QDataStream &) {
  // input >> gameData
  // Send message
}

void Parser::serverPlayerDCReceived(QDataStream &input) {
  quint8 playerNumber;
  input >> playerNumber;
  // Send message to Game
}
