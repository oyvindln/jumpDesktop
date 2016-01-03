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

#include "networkclient.h"

#include <QMessageBox>
#include <QtDebug>
#include <QUdpSocket>

#include "constants.h"
#include "game.h"

NetworkClient::NetworkClient(QObject *parent)
    : QObject(parent), m_lastBlockSize(0), m_playerId(255), m_seed(0),
      m_nickName("Unnamed") {
  connect(&m_parser, SIGNAL(gameStarting(const QString &)), this,
          SLOT(sendGameStarting(QString)));
  connect(&m_parser, SIGNAL(clientInfo(QList<QString>, QList<QHostAddress>)),
          this, SIGNAL(clientInfo(QList<QString>, QList<QHostAddress>)));
  connect(&m_parser, SIGNAL(clientInfo(QList<QString>, QList<QHostAddress>)),
          this, SLOT(saveClientInfo(QList<QString>)));
  connect(&m_parser,
          SIGNAL(playerJoined(const QString &, const QHostAddress &)), this,
          SLOT(otherPlayerJoin(QString, QHostAddress)));
}

void NetworkClient::setGame(Game *game) {
  m_game = game;
  m_parser.setGame(game);
}

/*!
  Adds the players on the server to the game object.
  */
void NetworkClient::addPlayers() {
  for (auto &name : m_playerNames) {
    m_game->addPlayer(name);
  }
  m_playerNames.clear();
  m_game->setPlayer(m_playerId);
}

qint64 NetworkClient::getSeed() { return m_seed; }

/*!
  Connects to a server and connects the input to the parser.
  */
void NetworkClient::connectToServer(const QString &ip, quint16 port) {
  // TODO handle disconnect.
  m_socket.connectToHost(ip, port);
  connect(&m_socket, SIGNAL(readyRead()), this, SLOT(parse()));
  connect(&m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this,
          SLOT(handleError(QAbstractSocket::SocketError)));
  connect(&m_socket, SIGNAL(connected()), this, SLOT(sendRequestMessage()));
  connect(&m_parser, SIGNAL(clientSetPlayerId(quint8, qint64)), this,
          SLOT(setPlayerId(quint8, qint64)));

  m_udpSocket = new QUdpSocket(this);
  m_udpSocket->bind(QHostAddress::LocalHost, port);
  QByteArray datagram("TEST TEST");
  m_udpSocket->writeDatagram(datagram, QHostAddress(ip), port);
}

/*!
  Send received data to parser.
  */
void NetworkClient::parse() {
  QDataStream stream(&m_socket);
  stream.setVersion(constants::DATASTREAM_VERSION);
  if (m_lastBlockSize == 0) {
    if (m_socket.bytesAvailable() < (int)sizeof(quint16)) {
      qDebug() << "NetworkClient::Less than 2 bytes available.";
      return;
    }
    stream >> m_lastBlockSize;
  }
  if (m_socket.bytesAvailable() < m_lastBlockSize) {
    qDebug() << "NetworkClient::Haven't received all bytes yet. received:"
             << m_socket.bytesAvailable() << " expected: " << m_lastBlockSize;
    return;
  }

  m_parser.interpretData(stream);
  m_lastBlockSize = 0;

  if (m_socket.bytesAvailable() > 0) {
    qDebug() << "NetworkClient::Data left after parsing:"
             << m_socket.bytesAvailable();
    qDebug() << "NetworkClient::Parsing again.";
    parse();
  }
  qDebug() << "NetworkClient::Data left after parsing:"
           << m_socket.bytesAvailable();
}

/*!
  Pop up error message if something went wrong.
  */
void NetworkClient::handleError(QAbstractSocket::SocketError /* error */) {
  QMessageBox::information(0, "ninjajump",
                           tr("Error: %1.").arg(m_socket.errorString()));
}

void NetworkClient::saveClientInfo(const QList<QString> &names) {
  m_playerNames = names.toVector().toStdVector();
}

/*!
  Adds a player that joined after this client and sends a message
  to the widget to add it to the list widget.
  */
void NetworkClient::otherPlayerJoin(const QString &name,
                                    const QHostAddress &ip) {
  m_playerNames.push_back(name);
  emit playerJoined(name, ip);
}

void NetworkClient::sendMessage(const QByteArray &message) {

  qint64 bytesWritten;
  bytesWritten = m_socket.write(message);
  qDebug() << "NetworkClient:: " << bytesWritten << " bytes sendt to server.";
  if (bytesWritten < 0) {
    qDebug() << m_socket.errorString();
  }

  m_socket.flush();
}

/*!
  Send message to request info from server.
  */
void NetworkClient::sendRequestMessage() {
  sendMessage(m_parser.playerReqJoin(m_nickName));
  disconnect(&m_socket, SIGNAL(connected()), this, SLOT(sendRequestMessage()));
}

void NetworkClient::localPlayerMove(const QLine &movement) {
  sendMessage(m_parser.playerMove(m_playerId, movement));
}

void NetworkClient::setPlayerId(quint8 playerId, qint64 seed) {
  m_playerId = playerId;
  m_seed = seed;
}

void NetworkClient::setNickName(const QString &name) { m_nickName = name; }

void NetworkClient::sendReady() { sendMessage(m_parser.playerReady()); }

void NetworkClient::udpReceiveData() {}

void NetworkClient::udpParseData(QByteArray) {}

void NetworkClient::sendGameStarting(const QString &mapName) {
  emit gameStarting(mapName, *this);
}
