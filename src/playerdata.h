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

#ifndef PLAYERDATA_H
#define PLAYERDATA_H

#include <QPointF>

class Player;
class Game;
class QDataStream;

/*!
  Data structure for sending player data over the network. (used for syncing.)
  */
struct PlayerData {
  //    PlayerData();
  //    PlayerData(Player* player);
  QPointF position;
  QPointF velocity;
  quint16 loadedPowerup;
  quint16 score;
  double balance;
  double earthMod;
  double windMod;
  static void syncData(const PlayerData &data, Player *player, int latency,
                       qreal gravity, Game *game);
  //    QString m_name;
};

QDataStream &operator<<(QDataStream &stream, const PlayerData &playerData);
QDataStream &operator>>(QDataStream &stream, PlayerData &playerData);

#endif // PLAYERDATA_H
