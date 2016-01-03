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

#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include "abstractclient.h"

class QTcpSocket;

/*!
  Network client class for network clients in the NetworkHandler.
  */

class Client : public AbstractClient // TODO fix name
{
  Q_OBJECT
public:
  Client(quint8 id, QTcpSocket *socket, QObject *parent = 0);
  int type();
  QTcpSocket *getSocket();
  //    quint8 getId();
  bool getIsDropped();
  bool getIsReady();
  void setIsReady(bool ready);
public slots:
  //    void sendData(const QByteArray& data);
  virtual void sendDataServer();
signals:
  void dataReady(quint8 id, QTcpSocket *socket);
protected slots:
private:
  QTcpSocket *m_socket;
  bool m_isDropped;
  quint16 m_lastBlockSize;
  bool m_isReady;
};

inline QTcpSocket *Client::getSocket() { return m_socket; }

#endif // CLIENT_H
