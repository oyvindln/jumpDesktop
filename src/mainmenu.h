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

#ifndef JUMPMAINMENU_H
#define JUMPMAINMENU_H

#include <QWidget>

#include "ui_mainmenu.h"

/*!
    Main menu.
*/
class MainMenu : public QWidget, public Ui::MainMenu {
  Q_OBJECT
public:
  MainMenu(QWidget *parent = 0);

private slots:
  void hostButtonClick();
  void joinButtonClick();
  void optionsButtonClick();
  void singleButtonClick();
  // // 	void exitButton();
signals:
  void hostPressed();
  void joinPressed();
  void optionsPressed();
  void singlePressed();
  //  void optionsPressed();
  //  void exitPressed();
};

#endif
