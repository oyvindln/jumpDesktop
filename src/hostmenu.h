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

#ifndef HOSTMENU_H
#define HOSTMENU_H

#include <QtWidgets/QWidget>
#include <QTimer>
#include "parser.h"

class NetworkHandler;

namespace Ui {
class HostMenu;
}

/*!
  The menu used when hosting a game.
    TODO: Make the menu files consistant.
    TODO: Make it possible to stop and restart hosting.
  */
class HostMenu : public QWidget {
  Q_OBJECT
  Q_DISABLE_COPY(HostMenu)
public:
  explicit HostMenu(NetworkHandler &networkHandler, QWidget *parent = 0);
  virtual ~HostMenu();
  const QString &getMapName();

public slots:

signals:
  // Signal to tell the game object that the game is starting.
  void starting();

protected:
  virtual void changeEvent(QEvent *e);
protected slots:
  void startClicked();
  void startListening();
  void addClientToView(const QString &name, const QString &ip);
  void selectMapClicked();

private:
  Ui::HostMenu *m_ui;
  QWidget *m_parent;
  QTimer *m_listenTimer;
  QString m_mapName;
  NetworkHandler *m_networkHandler;
};

inline const QString &HostMenu::getMapName() { return m_mapName; }

#endif // HOSTMENU_H
