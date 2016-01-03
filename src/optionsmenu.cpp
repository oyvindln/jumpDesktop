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

#include "optionsmenu.h"

#include <QIntValidator>

OptionsMenu::OptionsMenu(QWidget *parent) : QWidget(parent) {
  setupUi(this);

  QValidator *validator = new QIntValidator(0, 1000, this);
  updateRateLineEdit->setValidator(validator);
// Set the frequency in ms that the timer that calls the game update is called.
// On n8x0 we want more updates, to get a better framerate,
// on desktop, the framerate can get too high, so using 3 ms based on some
// testing.
#ifdef Q_WS_HILDON
  updateRateLineEdit->setText(QString::number(1));
#else
  updateRateLineEdit->setText(QString::number(10));
#endif
}
