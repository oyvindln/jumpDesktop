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

#ifndef JUMPMAINWINDOW_H
#define JUMPMAINWINDOW_H

#include <QMainWindow>

#include <memory>

#include "optionsmenu.h"
#include "hostmenu.h"
#include "joinmenu.h"
#include "networkhandler.h"

class Game;
class MainMenu;
class NetworkClient;

/*!
    Main window
*/
class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  MainWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
  ~MainWindow();
  void loadSettings();
  void saveSettings();
public slots:
  void startGame();
  void startServer();
  void joinServer(const QString &mapName, NetworkClient &);

protected:
  void closeEvent(QCloseEvent *event);

private:
  std::unique_ptr<Game> m_game;
  // This is a raw pointer as we need to set the main window as parent.
  MainMenu *m_mainMenu;
  OptionsMenu m_optionsMenu;
  JoinMenu m_joinMenu;
  NetworkHandler m_networkHandler;
  HostMenu m_hostMenu;
};

#endif
