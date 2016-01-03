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

#ifndef JOINMENU_H
#define JOINMENU_H

#include <QWidget>
#include <QListWidgetItem>
#include <QHostAddress>
#include "ui_joinmenu.h"

class NetworkClient;

/*!
    The menu used for joining a game.
*/
class JoinMenu : public QWidget, public Ui::JoinMenu {
  Q_OBJECT
public:
  JoinMenu(QWidget *parent = 0);

  ~JoinMenu();

private slots:
  void joinOkButtonClick();

  void clientInfo(const QList<QString> &names, const QList<QHostAddress> &ips);
  void clientJoined(const QString &name, const QHostAddress &ip);
signals:
  void joinOkPressed(const QString &text);
  void gameStarting(const QString &mapName, NetworkClient &client);

private:
  NetworkClient *m_client;
};

#endif
