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

#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <QTcpSocket>

#include <vector>

#include "parser.h"

class QUdpSocket;

/*!
  Network client class.
  Handles all network stuff on the client side.
  */
class NetworkClient : public QObject {
  Q_OBJECT
  Q_DISABLE_COPY(NetworkClient)
public:
  NetworkClient(QObject *parent = 0);

  void setGame(Game *game);
  void addPlayers();
  qint64 getSeed();
  Parser &gerParser();
public slots:
  void connectToServer(const QString &ip, quint16 port);
  void sendMessage(
      const QByteArray &message); // TODO make sure this doesn't deep copy
  void sendRequestMessage();
  void localPlayerMove(const QLine &movement);
  void setPlayerId(quint8 playerId, qint64);
  void setNickName(const QString &name);
  void sendReady();
signals:
  void gameStarting(const QString &mapName, NetworkClient &);
  void clientInfo(const QList<QString> &names, const QList<QHostAddress> &ips);
  void playerJoined(const QString &name, const QHostAddress &ip);
private slots:
  void parse();
  void handleError(QAbstractSocket::SocketError error);
  void saveClientInfo(const QList<QString> &names);
  void otherPlayerJoin(const QString &name, const QHostAddress &ip);

  void udpReceiveData();
  void udpParseData(QByteArray datagram);

  void sendGameStarting(const QString &mapName);

private:
  QTcpSocket m_socket;
  quint16 m_lastBlockSize;
  quint8 m_playerId;
  qint64 m_seed;
  Parser m_parser;
  Game *m_game;
  QString m_nickName;
  std::vector<QString> m_playerNames;

  QUdpSocket *m_udpSocket;
};
#endif // NETWORKCLIENT_H
