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

#ifndef ABSTRACTCLIENT_H
#define ABSTRACTCLIENT_H
#include <QObject>

class QTcpSocket;

/*!
  Base class for network clients in the networkHandler.
  */
class AbstractClient : public QObject {
  Q_OBJECT

public:
  enum clientType {
    NONE,
    LOCALCLIENT,
    NETWORKCLIENT,
    AI // Not implemented.
  };

  AbstractClient(quint8 id, QObject *parent = 0);
  virtual int type();
  virtual quint8 getId();
  virtual bool getIsDropped();
  virtual QTcpSocket *getSocket();
  virtual const QString &getName() const;
  virtual void setName(const QString &name);
  virtual bool getIsReady();
  virtual void setIsReady(bool ready);
public slots:
  virtual void sendDataServer();
signals:
  void dataReady(quint8 id, QTcpSocket *socket);
protected slots:
protected:
  quint8 m_id;

private:
  QString m_name;
};

#endif // ABSTRACTCLIENT_H
