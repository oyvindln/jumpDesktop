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

#include "powerup.h"

PowerUp::PowerUp(const QPixmap &pixmap, int type, int id, QGraphicsItem *parent)
    : GameObject(pixmap, parent) {
  m_type = type;
  m_id = id;
}

PowerUp::~PowerUp() {}

int PowerUp::type() { return GameObject::IT_POWERUP; }

int PowerUp::getType() { return m_type; }
int PowerUp::getID() { return m_id; }

/*!
  Checks if the ids of both powerups is the same.
  */
bool PowerUp::operator==(const PowerUp &p) {
  if (m_id == p.m_id) {
    return true;
  }
  return false;
}
