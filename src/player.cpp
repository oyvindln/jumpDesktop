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

#include "player.h"

#include <QPainterPath>

Player::Player(const QString &name, quint16 playerNumber, const QPixmap &pixmap,
               QGraphicsItem *parent)
    : GameObject(pixmap, parent), m_velocityX(0), m_velocityY(0), m_name(name),
      m_isCharging(false), m_isMoving(true) {
  m_balance = 0;
  m_loadedPower = 0;
  m_score = 0;
  m_playerNumber = playerNumber;
  m_wind_mod = 1.0;
  m_earth_mod = 1.0;
  m_powerup_timer = 0;
  m_movementNeededTime = 0;
  m_movementNeededTimeLeft = 0;

  setCacheMode(QGraphicsItem::DeviceCoordinateCache);
}

Player::~Player() {}

/*!
  Adjusts the player position according to the server sync info.
  */
QPointF Player::adjust(qreal delta) {
  QPointF adjustMovement;
  if (m_movementNeededTimeLeft > 0.0) {
    adjustMovement = m_movementNeededPerSec * delta;
    m_movementNeeded -= adjustMovement;
    m_movementNeededTimeLeft -= delta;
    //        moveBy(adjustMovement.x(), adjustMovement.y());
    // qDebug() << adjustMovement;
    return adjustMovement;
  } else {
    adjustMovement = QPointF(0, 0);
  }
  return adjustMovement;
}

/*!
  Stops the player.
  */
void Player::stop() {
  m_isMoving = false;
  m_velocityX = 0;
  m_velocityY = 0;
}

const QString &Player::getPlayerName() { return m_name; }

quint16 Player::getScore() const { return m_score; }

/*!
  Add powerup.
  */
void Player::addPower(quint16 power) {

  m_loadedPower += power;
  if (power == PT_POWER) {
    if (m_wind_mod <= 3.0) {
      m_wind_mod += 0.1;
    }
  } else if (power == PT_SHIELD) {
    if (m_earth_mod <= 3.0) {
      m_earth_mod += 0.1;
    }
  } else if (power == PT_SPEED) {
    m_loadedPower = PT_SPEED;
    m_powerup_timer = 1000; // for now..
  }
}

/*!
  Returns how well balanced the player is.
  */
quint16 Player::getLoadedPower() const { return (50.0 - abs(m_balance)); }

/*!
  Decrement powerup time.
  */
void Player::reduceTime(int time) {
  if (m_loadedPower != 0) {
    m_powerup_timer -= time;
    if (m_powerup_timer <= 0) {
      m_loadedPower = 0;
    }
  }
}
