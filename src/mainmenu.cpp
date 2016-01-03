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

#include <QApplication>
#include "mainmenu.h"

MainMenu::MainMenu(QWidget *parent) : QWidget(parent) {
  setupUi(this); // Set up the Ui from the Ui file.
  // Make the exit button exit the program when clicked.
  // Link to close instead of qApp->exit to
  connect(exitButton, SIGNAL(clicked()), parent, SLOT(close()));
  connect(hostButton, SIGNAL(clicked()), this, SLOT(hostButtonClick()));
  connect(joinButton, SIGNAL(clicked()), this, SLOT(joinButtonClick()));
  connect(optionsButton, SIGNAL(clicked()), this, SLOT(optionsButtonClick()));
  connect(singlePlayerButton, SIGNAL(clicked()), this,
          SLOT(singleButtonClick()));
}

void MainMenu::hostButtonClick() { emit hostPressed(); }

void MainMenu::joinButtonClick() { emit joinPressed(); }

void MainMenu::optionsButtonClick() { emit optionsPressed(); }

void MainMenu::singleButtonClick() { emit singlePressed(); }
