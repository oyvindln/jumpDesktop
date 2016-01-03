/*
 *   NinjaJump
 *
 *   Copyright (C) 2009 oyvindln
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

#include "udpserver.h"

UDPServer::UDPServer(QObject *parent) : QObject(parent) {
  socket = new QUdpSocket(this);
}

UDPServer::~UDPServer() {}

bool UDPServer::startListening(quint16 port) {
  socket->bind(QHostAddress::LocalHost, port);
  connect(socket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
  return true;
}

void UDPServer::readPendingDatagrams() {
  while (socket->hasPendingDatagrams()) {
    QByteArray datagram;
    datagram.resize(socket->pendingDatagramSize());
    QHostAddress sender;
    quint16 senderPort;
    socket->readDatagram(datagram.data(), datagram.size(), &sender,
                         &senderPort);

    qDebug() << Q_FUNC_INFO << "Datagram received: " << datagram
             << " size: " << datagram.size() << sender;
  }
}

void UDPServer::processDatagram(QByteArray) {}
