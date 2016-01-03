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

#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <QUdpSocket>
#include <QHostAddress>
#include <QDebug>

#include "constants.h"

class UDPServer : public QObject {
  Q_DISABLE_COPY(UDPServer)
  Q_OBJECT
public:
  UDPServer(QObject *parent);
  ~UDPServer();

  // bool connect(QHostAddress address, quint16 port = constants::DEFAULTPORT);
  bool startListening(quint16 port = constants::DEFAULTPORT);

  // public slots:

  // signals:

private:
  QList<QUdpSocket *> clients;

  QUdpSocket *socket;

private slots:
  void processDatagram(QByteArray datagram);
  void readPendingDatagrams();
};

#endif // UDPSERVER_H
