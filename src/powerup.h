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

#ifndef POWERUP_H
#define POWERUP_H

#include "gameobject.h"

/*!
  Class to store powerup data.
  */
class PowerUp : public GameObject {
public:
  PowerUp(const QPixmap &pixmap, int type, int id, QGraphicsItem *parent = 0);
  ~PowerUp();
  int type();
  int getType();
  int getID();

  bool operator==(const PowerUp &p);

private:
  int m_type;
  int m_id;
};

#endif // POWERUP_H
