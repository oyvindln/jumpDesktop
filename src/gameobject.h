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

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QGraphicsPixmapItem>

/*!
        Base class for all game objects.
*/
class GameObject : public QGraphicsPixmapItem {
public:
  enum itemType {
    IT_NONE = QGraphicsItem::UserType + 1,
    IT_PLATFORM,
    IT_POWERUP,
    IT_PLAYER
    // IT_TRAP?
  };

  enum powerType { PT_POWER, PT_SPEED, PT_SHIELD, PT_NUMBER };

  GameObject(const QPixmap &pixmap, QGraphicsItem *parent = 0);
  ~GameObject();

  virtual int type();

private:
};

#endif
