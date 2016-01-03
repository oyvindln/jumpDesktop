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

#ifndef NETWORKHANDLER_H
#define NETWORKHANDLER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>

#include <cassert>
#include <memory>
#include <vector>

#include "constants.h"
#include "parser.h"
#include "abstractclient.h"

/*!
    Class to handle all networking stuff on the server side.
    Singleton class.
*/
class NetworkHandler : public QTcpServer

{
  Q_OBJECT
  Q_DISABLE_COPY(NetworkHandler)
public:
  NetworkHandler(/* QObject *parent = 0 */);
  bool startListening(quint16 port = constants::DEFAULTPORT);

  void sendToAll(const QByteArray &data);
  void sendToAllExceptOne(quint8 id, const QByteArray &data);
  void sendToSingle(quint8 id, const QByteArray &data);
  void addPlayers(Game *game);
  void setPlayerName(const QString &name);

  qint64 getSeed() const;

signals:
  void clientJoin(const QString &name, const QString &ip);

protected:
  //  void incomingConnection(int socketDescriptor);

public slots:
  void run();
  void processIncomingData(quint8 id, QTcpSocket *socket);

  void localMove(const QLine &movement);
  void startGame();
  void begin();
  void clientMove(const quint8 playerNumber, const QLine &movement);

  bool joinRequest(const quint8 playerId,
                   const QString &name = "Name not set.");
  void sync();
  void playerKillRespawn(quint8 killedPlayerNumber, quint8 killerPlayerNumber,
                         const QPointF &newPos);

private:
  void checkReady();

private slots:
  void readyPlayer(quint8 id);

private:
  int getSocketID(QTcpSocket *socket);
  std::vector<std::unique_ptr<AbstractClient>> m_clients;
  QHostAddress address;
  QTcpSocket host;
  quint16 m_port;
  QTcpSocket *current;
  char buffer[512];
  bool m_isClientsInOrder;
  Parser m_parser;
  qint64 m_seed;

  QTimer *m_syncTimer;

  //    UDPServer* m_udpserver;

  std::size_t m_readyClients;
};

inline qint64 NetworkHandler::getSeed() const { return m_seed; }

inline void NetworkHandler::readyPlayer(quint8 id) {
  assert(id < m_clients.size());
  assert(!(m_clients[id]->getIsReady()));

  m_clients[id]->setIsReady(true);
  ++m_readyClients;
  qDebug() << "Player " << id << " is now ready";

  checkReady();
}
#endif
