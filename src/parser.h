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

#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QByteArray>
#include <QString>
#include <QHostAddress>
#include <QList>

class Game;

// QDataStream& operator<<(QDataStream& out, const Player& PlayerData);
// QDataStream& operator>>(QDataStream& in, Player& PlayerData);

/*!
  Class To Parse and make data for sending over the network.
  (Could possibly be split into client and server later, seing as it is growing
  quite large.
  */

class Parser : public QObject {
  Q_OBJECT
public:
  Parser(bool isServer = false, QObject *parent = 0); // TODO add options
  void setGame(Game *game);

  void interpretData(QDataStream &input);
  void interpretClientData(QDataStream &input, quint32 playerId);

  // TODO Probably forgotten some commands here.
  // Commands from player:
  // TODO, these functions needs to be reordered and renamed to something more
  // informative.
  QByteArray playerReqJoin(const QString &name);
  QByteArray playerLeave(/*quint8 playerNumber*/);
  QByteArray playerMove(quint8 playerNumber, const QLine &movement);
  QByteArray playerWantSync(/*quint8 playerNumber*/);
  QByteArray playerReady(/*quint8 playerNumber*/);
  QByteArray playerTestMessage(const QString &message);
  // Commands from server:
  QByteArray serverBroadcast();
  QByteArray serverAcceptJoin(/* data? */ quint8 playerNumber, qint64 seed);
  // Player names must be in the right order
  QByteArray serverClientInfo(
      const QList<QString> &names,
      const QList<QHostAddress> &ips /*, const GameData& gamedata*/);
  QByteArray serverLoadIn(const QString &mapName);
  QByteArray serverStart();
  QByteArray serverShutdown();
  QByteArray serverIsFull();
  QByteArray serverPlayerJoined(const QString &name, const QHostAddress &ip,
                                quint8 playerNumber = 0);
  // QByteArray serverInfoChanged(/* const GameData& gamedata*/);
  // In-game
  QByteArray serverPlayerMove(quint8 playerNumber, const QLine &movement);
  QByteArray serverPlayerKillRespawn(quint8 killedPlayerNumber,
                                     quint8 killerPlayerNumber,
                                     const QPointF &newPos);
  QByteArray serverSync(/*const QList<Player*>& players, */ bool force = false);
  QByteArray serverGameEnd(/* data? */);
  QByteArray serverPlayerDC(quint8 playerNumber /*, reason*/);
  QByteArray serverTestMessage(const QString &message);

  Game *getGame();

signals:
  void sendMessage(const QByteArray &message);
  // TODO, fix name, seed added later
  void clientSetPlayerId(quint8 playerId, qint64 seed);
  void gameStarting(const QString &mapName);
  void clientInfo(const QList<QString> &names, const QList<QHostAddress> &ips);
  void playerJoined(const QString &name, const QHostAddress &ip);
  void readyPlayer(quint8 playerId);
  //    void playerKillRespawn(quint8 killedPlayerNumber, quint8
  //    killerPlayerNumber, const QPointF& newPos);

  void serverJoinRequest(quint8 id, const QString &name);
  void playerMoveNeedsSending(const quint8 playerNumber, const QLine &movement);

protected:
  void prepareBuffer(qint8 command /*, qint16 length*/);
  void sendBuffer();
  // Command received from player.
  void playerReqJoinReceived(QDataStream &input, quint8 playerId);
  void playerLeaveReveived(QDataStream &input, quint8 playerId);
  void playerMoveReceived(QDataStream &input, quint8 playerId);
  void playerWantSyncReceived(QDataStream &input, quint8 playerId);
  void playerReadyReceived(QDataStream &input, quint8 playerId);
  void playerTestMessage(QDataStream &input, quint8 playerId);
  // Command received from server.
  void serverBroadcastReceived(QDataStream &input);
  void serverAcceptJoinReceived(QDataStream &input);
  void serverClientInfoReceived(QDataStream &input);
  void serverLoadInReceived(QDataStream &input);
  void serverStartReceived(QDataStream &input);
  void serverShutdownReceived(QDataStream &input);
  void serverIsFullReceived(QDataStream &input);
  void serverPlayerJoinedReceived(QDataStream &input);
  // void serverInfoChangedReceived(QDataStream& input);
  void serverPlayerMoveReceived(QDataStream &input);
  void serverPlayerKillRespawnReceived(QDataStream &input);
  void serverSyncReceived(QDataStream &input);
  void serverGameEndReceived(QDataStream &input);
  void serverPlayerDCReceived(QDataStream &input);

private:
  QByteArray m_buffer;
  QDataStream m_bufferStream;
  Game *m_game;
  bool m_isServer;
};

inline Game *Parser::getGame() { return m_game; }

#endif // PARSER_H
