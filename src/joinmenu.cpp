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

#include "joinmenu.h"

#include <QString>

#include <cassert>

#include "constants.h"
#include "networkclient.h"

JoinMenu::JoinMenu(QWidget *parent) : QWidget(parent) {
  setupUi(this);
  connect(joinOk, SIGNAL(clicked()), this, SLOT(joinOkButtonClick()));

  // Makes sure the value in the field is a valid port.
  QValidator *validator = new QIntValidator(1025, 49150, this);
  portLineEdit->setValidator(validator);
  portLineEdit->setText(QString::number(constants::DEFAULTPORT));
}

JoinMenu::~JoinMenu() {}

/*!
  Connects to the entered IP, and sets the Nickname from the textbox.
  */
void JoinMenu::joinOkButtonClick() {
  m_client = new NetworkClient(this);
  connect(m_client, SIGNAL(gameStarting(const QString &, NetworkClient &)),
          this, SIGNAL(gameStarting(const QString &, NetworkClient &)));
  connect(m_client, SIGNAL(clientInfo(QList<QString>, QList<QHostAddress>)),
          this, SLOT(clientInfo(QList<QString>, QList<QHostAddress>)));
  connect(m_client, SIGNAL(playerJoined(QString, QHostAddress)), this,
          SLOT(clientJoined(QString, QHostAddress)));
  m_client->setNickName(nickNameLineEdit->text());
  m_client->connectToServer(serverip->text(), portLineEdit->text().toInt());
}
/*!
  Adds client info to the list widget when connected to the server.
  */
void JoinMenu::clientInfo(const QList<QString> &names,
                          const QList<QHostAddress> &ips) {
  assert(names.size() == ips.size());
  for (int i = 0; i < names.size(); ++i) {
    clientJoined(names[i], ips[i]);
  }
}

/*!
  Adds client info to the list widget.
  TODO make this look better
  */
void JoinMenu::clientJoined(const QString &name, const QHostAddress &ip) {
  new QListWidgetItem(name + "\t" + ip.toString(), serverListWidget);
}
