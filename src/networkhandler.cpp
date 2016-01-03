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

#include "networkhandler.h"

#include <QMessageBox>

#include "client.h"
#include "game.h"
#include "localclient.h"
#include "makeunique.h"

/*!
        The networkHandler constructor
*/
NetworkHandler::NetworkHandler(/*QObject *parent */)
    //	: QThread(parent)
    : m_isClientsInOrder(true),
      m_parser(true),
      m_seed(0),
      m_syncTimer(0),
      m_readyClients(1) {
  // address = QHostAddress("127.0.0.1");
  m_port = constants::DEFAULTPORT;
  //        m_udpserver = 0;
  //	read = 0;
  //	numClients = 0;

  m_clients.push_back(Utils::make_unique<LocalClient>(0));
  m_clients.back()->setName("****");
  connect(&m_parser, SIGNAL(serverJoinRequest(quint8, const QString &)), this,
          SLOT(joinRequest(quint8, const QString &)));
  connect(&m_parser,
          SIGNAL(playerMoveNeedsSending(const quint8, const QLine &)), this,
          SLOT(clientMove(const quint8, const QLine &)));
}

void NetworkHandler::checkReady() {
#ifndef QT_NO_DEBUG

  qDebug() << "Checking clients";

  std::size_t counter = 0;
  for (std::size_t i = 0; i < m_clients.size(); ++i) {
    if (m_clients[i]->getIsReady()) {
      qDebug() << "Client " << i << " is ready";
      ++counter;
    }
  }
#endif

  if (m_readyClients >= m_clients.size()) {
    qDebug("All clients ready, starting..");
#ifndef QT_NO_DEBUG
    assert(counter == m_readyClients);
#endif
    begin();
  }
}

/*!
  Start listening for clients and generate a seed for the game that is to be
  hosted.
  */
bool NetworkHandler::startListening(quint16 port) {

  m_port = port;
  // TODO, listen for ipv6 adresses as well.
  if (!listen(QHostAddress::Any, m_port)) {
    QMessageBox::information(0, "ninjajump",
                             tr("Error: %1.").arg(this->errorString()));
    // TODO something more should happen here.
    return false;
  }
  m_seed = time(0);
  srand(m_seed);
  //    m_udpserver = new UDPServer(this);
  //    m_udpserver->startListening(m_port);

  qDebug() << serverAddress();

  return true;
}

/*!
    The networkHandlers main loop, takes care of connecting clients.
*/
void NetworkHandler::run() {
  quint32 id;
  while (
      hasPendingConnections()) // TODO, change this to a signal-based approach.
  {
    current = nextPendingConnection();
    id = getSocketID(current);

    // Assuming connection isn't already open.
    m_clients.push_back(Utils::make_unique<Client>(id, current));

    connect(m_clients.back().get(), SIGNAL(dataReady(quint8, QTcpSocket *)),
            this, SLOT(processIncomingData(quint8, QTcpSocket *)));

    // Workaround, check socket for data, since we received it before the client
    // object is made.
    m_clients.back()->sendDataServer();
  }
}

/*!
    Method to send data to all connected clients
*/
void NetworkHandler::sendToAll(const QByteArray &data) {

  for (std::size_t i = 1; i < m_clients.size(); i++) {
    if (m_clients[i]->type() == AbstractClient::NETWORKCLIENT) {
      qint64 bytes = m_clients[i]->getSocket()->write(data);
      m_clients[i]->getSocket()->flush();
      qDebug() << "Server sendt: " << bytes << " bytes to client: " << i;
    }
  }
}

/*!
  Send data to all clients except the sender.
  */
void NetworkHandler::sendToAllExceptOne(quint8 id, const QByteArray &data) {
  // does not send back to itself.
  for (std::size_t i = 1; i < m_clients.size(); i++) {
    if (m_clients[i]->getId() != id) {
      if (m_clients[i]->type() == AbstractClient::NETWORKCLIENT) {
        qint64 bytes = m_clients[i]->getSocket()->write(data);
        m_clients[i]->getSocket()->flush();
        qDebug() << "Server sendt: " << bytes << " bytes to client: " << i;
      }
      // else *
    }
  }
}

/*!
  Method to send data to a single client
*/
void NetworkHandler::sendToSingle(quint8 playerId, const QByteArray &data) {
  if (m_clients[playerId]->type() == AbstractClient::NETWORKCLIENT) {
    m_clients[playerId]->getSocket()->write(data);
  }
}

/*!
  Add players to the game object.
  */
void NetworkHandler::addPlayers(Game *game) {
  assert(game != 0);
  m_parser.setGame(game);
  for (auto &client : m_clients) {
    game->addPlayer(client->getName());
  }
  connect(game, SIGNAL(wantSync()), this, SLOT(sync()));
  connect(game, SIGNAL(playerKilled(quint8, quint8, const QPointF &)), this,
          SLOT(playerKillRespawn(quint8, quint8, const QPointF &)));
}

void NetworkHandler::setPlayerName(const QString &name) {
  m_clients.back()->setName(name);
}

/*!
  Sends the incoming data to the parser.
  */
void NetworkHandler::processIncomingData(quint8 id, QTcpSocket *socket) {
  QDataStream stream(socket);
  stream.setVersion(constants::DATASTREAM_VERSION);
  qDebug() << "Received data " << socket->bytesAvailable();

  m_parser.interpretClientData(stream, id);
  qDebug() << "Data left after parsing: " << socket->bytesAvailable();
}

void NetworkHandler::clientMove(const quint8 playerNumber,
                                const QLine &movement) {
  sendToAllExceptOne(playerNumber,
                     m_parser.serverPlayerMove(playerNumber, movement));
}

bool NetworkHandler::joinRequest(quint8 playerId, const QString &name) {
  if (playerId >= m_clients.size()) {
    // Shouldn't ever get this far.
    m_clients[playerId]->getSocket()->disconnectFromHost();
    return false;
  }
  // Could add some checking here at some point if the protocol changes.
  // Tell the player the player number.
  // Maybe we should add some nice message later, also need to send a list with
  // player names.

  m_clients[playerId]->setName(name);
  QList<QString> players;
  QList<QHostAddress> ips;
  for (auto &client : m_clients) {
    players.append(client->getName());
    if (client->type() == AbstractClient::NETWORKCLIENT) {
      ips.append(client->getSocket()->peerAddress());
    } else {
      ips.append(QHostAddress());
    }
  }

  sendToSingle(playerId, m_parser.serverAcceptJoin(playerId, m_seed));
  sendToSingle(playerId, m_parser.serverClientInfo(players, ips));
  sendToAllExceptOne(
      playerId, m_parser.serverPlayerJoined(
                    name, m_clients[playerId]->getSocket()->peerAddress()));
  QString address = m_clients[playerId]->getSocket()->peerAddress().toString();

  emit clientJoin(name, address);
  return true;
}

/*!
    method used for finding a QTcpSockets ID
*/
int NetworkHandler::getSocketID(QTcpSocket *) {

  // Check if the socket is already in the list.
  quint32 id = m_clients.size();
  if (!m_isClientsInOrder) {
    id = 0;
    bool finished = false;
    qint32 timesTraversed = 0;
    while (!finished) {
      for (auto &client : m_clients) {
        if (id == client->getId()) {
          ++id;
          timesTraversed = 0;
        }
      }

      ++timesTraversed;

      // If we have gone through the list twice and not incremented, return.
      if (timesTraversed > 1 ||
          static_cast<std::size_t>(id) >= m_clients.size()) {
        finished = true;
      }
    }
  }

  return id;
}

/*!
  Local player movement. This should probably not be done this way.
  */
void NetworkHandler::localMove(const QLine &movement) {
  sendToAllExceptOne(0, m_parser.serverPlayerMove(0, movement));
}

void NetworkHandler::startGame() {
  qDebug() << m_parser.getGame()->getMap();
  connect(&m_parser, SIGNAL(readyPlayer(quint8)), this,
          SLOT(readyPlayer(quint8)));
  sendToAll(m_parser.serverLoadIn(m_parser.getGame()->getMap()));

  // Wait 3 seconds, then start the game.
  // TODO: Visual indication.
  // QTimer::singleShot(3000, this, SLOT(begin()));
}

void NetworkHandler::begin() {
  sendToAll(m_parser.serverStart());
  m_parser.getGame()->begin();
  m_syncTimer = new QTimer(this);
  m_syncTimer->start(200);
  connect(m_syncTimer, SIGNAL(timeout()), this, SLOT(sync()));
}

/*!
  Send a sync message to all clients.
  */
void NetworkHandler::sync() { sendToAll(m_parser.serverSync()); }
/*!
  Send a message that says that someone got killed to everyone.
  */
void NetworkHandler::playerKillRespawn(quint8 killedPlayerNumber,
                                       quint8 killerPlayerNumber,
                                       const QPointF &newPos) {
  sendToAll(m_parser.serverPlayerKillRespawn(killedPlayerNumber,
                                             killerPlayerNumber, newPos));
  // sendToAll(m_parser.serverSync());
}
