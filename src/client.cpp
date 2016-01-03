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

#include "client.h"

#include <QTcpSocket>

#include "constants.h"

Client::Client(quint8 id, QTcpSocket *socket, QObject *parent)
    : AbstractClient(id, parent), m_lastBlockSize(0), m_isReady(false) {
  m_socket = socket;
  connect(socket, SIGNAL(readyRead()), this, SLOT(sendDataServer()));
}

/*!
  Send data from buffer to parser.
  */
void Client::sendDataServer() {
  // quint32 id = m_id;
  ////std::cout << " data, id: " << id  << " " << m_id << std::endl;
  QDataStream stream(m_socket);
  stream.setVersion(constants::DATASTREAM_VERSION);
  if (m_lastBlockSize == 0) {
    if (m_socket->bytesAvailable() < (int)sizeof(quint16)) {
      ////std::cout << "Less than 2 bytes available" << std::endl;
      return;
    }
    stream >> m_lastBlockSize;
  }
  if (m_socket->bytesAvailable() < m_lastBlockSize) {
    // std::cout << "Haven't received all bytes yet. received:" <<
    // m_socket->bytesAvailable() << " expected: " << m_lastBlockSize <<
    // std::endl;
    return;
  }
  // std::cout << "Data sendt to parser" << m_lastBlockSize;
  // std::cout << " " << m_socket->bytesAvailable() << std::endl;
  m_lastBlockSize = 0;

  emit dataReady(m_id, m_socket);

  // Make sure we pick up all the available data.
  // NOTE this approach could potentially create a stack overflow if someone
  // floods the server.
  if (m_socket->bytesAvailable() > 2) {
    sendDataServer();
  }
}

bool Client::getIsReady() { return m_isReady; }

void Client::setIsReady(bool isReady) { m_isReady = isReady; }

int Client::type() { return AbstractClient::NETWORKCLIENT; }

bool Client::getIsDropped() { return m_isDropped; }
