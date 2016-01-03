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

#ifndef LOCALCLIENT_H
#define LOCALCLIENT_H

#include "abstractclient.h"

/*!
  Class for local client in the networkHandler.
  */
class LocalClient : public AbstractClient {
public:
  LocalClient(quint8 id, QObject *parent = 0);
  int type();
  /*signals:
      void dataReady(quint32 id, QTcpSocket* socket);
  protected slots:
      void sendDataServer();*/
};

inline int LocalClient::type() { return AbstractClient::LOCALCLIENT; }

#endif // LOCALCLIENT_H
