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

#include "abstractclient.h"

AbstractClient::AbstractClient(quint8 id, QObject *parent)
    : QObject(parent), m_id(id) {}

int AbstractClient::type() { return AbstractClient::NONE; }

bool AbstractClient::getIsDropped() { return false; }

QTcpSocket *AbstractClient::getSocket() { return 0; }

void AbstractClient::sendDataServer() {
  // std::cout << "Send data function from abstract client called, something
  // went wrong.." << std::endl;
  // emit dataReady(255, 0);
}

const QString &AbstractClient::getName() const { return m_name; }

void AbstractClient::setName(const QString &name) { m_name = name; }

quint8 AbstractClient::getId() { return m_id; }

bool AbstractClient::getIsReady() { return true; }

void AbstractClient::setIsReady(bool) {
  // return;
}
