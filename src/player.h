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

#ifndef PLAYER_H
#define PLAYER_H

#include <QPointF>

#include "gameobject.h"

/*!
    Class to store player info, and to move the players.
*/
class Player
    // May have to make a base class for all game items.
    : public GameObject {
public:
  Player(const QString &name, quint16 playerNumber, const QPixmap &pixmap,
         QGraphicsItem *parent = 0);
  ~Player();
  void setVelocity(qreal x, qreal y);
  void setVelocity(const QPointF &velocity);
  void addVelocity(qreal x, qreal y);
  //    bool collidesWithWall(qreal move, qreal wall);
  QPointF adjust(qreal delta);
  void setMoving(bool isMoving);
  bool isMoving() const;
  void reverseMovementX();
  void reverseMovementY();
  void stop();
  void addPower(quint16 power);
  quint16 getPower() const;
  void setPower(quint16 power);
  void setScore(quint16 score);
  void addScore(quint16 score = 1);
  void reduceTime(int time);
  qreal getBalance() const;
  void setBalance(qreal balance);
  qreal getWindMod() const;
  void setWindMod(qreal windMod);
  qreal getEarthMod() const;
  void setEarthMod(qreal earthMod);
  void addBalance(qreal balance);
  quint16 getLoadedPower() const;
  quint16 getScore() const;
  QPointF getVelocity() const;

  // Determine the item type
  virtual int type() const;
  quint16 getPlayerNumber() const;
  const QString &getPlayerName();

  void setMovementNeeded(const QPointF &movement, qreal time);
  const QPointF &getMovementNeeded() const;
  void setWantResync();
  bool getWantResync() const;

private:
  qreal m_velocityX;
  qreal m_velocityY;

  QString m_name;
  qreal m_balance;
  quint16 m_loadedPower;
  int m_powerup_timer;

  QPointF m_movementNeeded;
  QPointF m_movementNeededPerSec;
  qreal m_movementNeededTime;
  qreal m_movementNeededTimeLeft;
  bool m_wantResync;

  bool m_isCharging;
  bool m_isMoving;

  short m_lives;
  qreal m_wind_mod, m_earth_mod;
  quint16 m_powerups;
  quint16 m_playerNumber;

  quint16 m_score;

  // Prevent copying
  Player(const Player &);
  Player &operator=(const Player &);
};

// Inline functions has to be in the header.
inline void Player::setVelocity(qreal x, qreal y) {
  m_velocityX = x;
  m_velocityY = y;
}

inline void Player::setVelocity(const QPointF &velocity) {
  m_velocityX = velocity.x();
  m_velocityY = velocity.y();
}

inline void Player::addVelocity(qreal x, qreal y) {
  m_velocityX += x;
  m_velocityY += y;
}

inline void Player::setMoving(bool isMoving) { m_isMoving = isMoving; }

inline bool Player::isMoving() const { return m_isMoving; }

inline void Player::reverseMovementX() { m_velocityX = -m_velocityX * 0.90; }

inline void Player::reverseMovementY() { m_velocityY = -m_velocityY * 0.90; }

inline QPointF Player::getVelocity() const {
  return QPointF(m_velocityX, m_velocityY);
}

inline int Player::type() const { return GameObject::IT_PLAYER; }

inline quint16 Player::getPlayerNumber() const { return m_playerNumber; }

inline void Player::setPower(quint16 power) { m_loadedPower = power; }

inline void Player::setScore(quint16 score) { m_score = score; }

inline void Player::addScore(quint16 score) { m_score += score; }

inline qreal Player::getWindMod() const { return m_wind_mod; }

inline void Player::setWindMod(qreal windMod) { m_wind_mod = windMod; }

inline qreal Player::getEarthMod() const { return m_earth_mod; }

inline void Player::setEarthMod(qreal earthMod) { m_earth_mod = earthMod; }

inline quint16 Player::getPower() const { return m_loadedPower; }

inline qreal Player::getBalance() const { return m_balance; }

inline void Player::addBalance(qreal balance) { m_balance += balance; }

inline void Player::setBalance(qreal balance) { m_balance = balance; }

inline void Player::setMovementNeeded(const QPointF &movement, qreal time) {
  m_movementNeeded = movement;
  m_movementNeededTime = time;
  m_movementNeededTimeLeft = time;
  m_movementNeededPerSec = m_movementNeeded / time;
}

inline const QPointF &Player::getMovementNeeded() const {
  return m_movementNeeded;
}

inline void Player::setWantResync() { m_wantResync = true; }

inline bool Player::getWantResync() const { return m_wantResync; }

// QDataStream& operator<<(QDataStream& out, const Player& player);
// QDataStream& operator>>(QDataStream& in, Player& player);

#endif
