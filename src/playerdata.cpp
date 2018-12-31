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

#include "playerdata.h"

#include <cmath>

#include "constants.h"
#include "dummybox.h"
#include "player.h"
#include "game.h"
#include "ingameview.h"

/*!
  Syncronize and interpolate/extrapolate.
  (Currently only changes things if the difference is too high.)
  */
void PlayerData::syncData(const PlayerData &data, Player *player, int latency,
                          qreal gravity, Game *game) {
  if (/*player->isMoving()*/ true) {
    qreal delta = latency / 10000.0;
    QPointF newPos;

    QPointF newVelocity = data.velocity;
    newVelocity.ry(); // += gravity * delta * 0.1;
    newPos.rx() = data.position.x() + (data.velocity.x() * delta);
    newPos.ry() = data.position.y() + (data.velocity.y() * delta) +
                  (0.5 * (gravity * delta));

#ifndef QT_NO_DEBUG
    if (game->getGraphicsView()->getPlayerNumber() == 1 &&
        player->getPlayerNumber() == 1) {
      game->m_playerPos->setPos(player->pos());
      game->m_predPos->setPos(newPos);
      game->m_serverPos->setPos(data.position);
    }
#endif

    /*        qDebug() << Q_FUNC_INFO << " newPos " << newPos << " data.position
       " << data.position;
            qDebug() << Q_FUNC_INFO << " player.position " << player->pos();
            qDebug() << Q_FUNC_INFO << " playerpos - newpos " << player->pos() -
       newPos;
            qDebug() << Q_FUNC_INFO << " delta " << delta << "  " << latency;
            qDebug() << Q_FUNC_INFO << " player.velocity " <<
       player->getVelocity() << " new vel " << newVelocity << " Server vel " <<
       data.velocity;*/

    if (player->isMoving()) {
      if ((std::fabs(data.position.x() - player->pos().x()) > 20) ||
          std::fabs(data.position.y() - player->pos().y() > 20)) {
        player->setPos(data.position);
        //            qDebug() << "PlayerData:: offset:" <<
        //            player->getMovementNeeded();
      } else /* if( (std::fabs(newPos.x() - player->pos().x()) >
             constants::Sync_Pos_Max_Threshold) ||
             std::fabs(newPos.y() - player->pos().y() >
             constants::Sync_Pos_Max_Threshold))*/
      {
        player->setMovementNeeded(newPos - player->pos(), 0.5);
        qDebug() << "PlayerData:: offset:" << player->getMovementNeeded();
      }

      if ((std::fabs(newVelocity.x() - player->getVelocity().x()) >
           constants::Sync_Velocity_Max_Threshold) ||
          std::fabs(newVelocity.y() - player->getVelocity().y() >
               constants::Sync_Velocity_Max_Threshold)) {
        player->setVelocity(newVelocity);
      }
    } else {
      player->setMovementNeeded(data.position - player->pos(), 0.5);
    }

    if (std::fabs(data.balance - player->getBalance() >
             constants::Sync_Balance_Max_Threshold)) {
      player->setBalance(data.balance);
    }
  } else {
    if ((std::fabs(data.position.x() - player->pos().x()) >
         constants::Sync_Pos_Max_Threshold) ||
        std::fabs(data.position.y() - player->pos().y() >
             constants::Sync_Pos_Max_Threshold)) {
      player->setMovementNeeded(player->pos() - data.position, 1.000);
    }
  }
  player->setPower(data.loadedPowerup);
  player->setScore(data.score);

  player->setWindMod(data.windMod);
  player->setEarthMod(data.earthMod);
}

/*!
  Stream operators for putting a playerData object into and from a datastream.
  */
QDataStream &operator<<(QDataStream &stream, const PlayerData &playerData) {
  stream << playerData.position;
  stream << playerData.velocity;
  // stream << player.getScore();
  stream << playerData.loadedPowerup;
  stream << playerData.score;
  stream << playerData.balance;
  stream << playerData.windMod;
  stream << playerData.earthMod;
  return stream;
}

QDataStream &operator>>(QDataStream &stream, PlayerData &playerData) {
  stream >> playerData.position;
  stream >> playerData.velocity;
  stream >> playerData.loadedPowerup;
  stream >> playerData.score;
  stream >> playerData.balance;
  stream >> playerData.windMod;
  stream >> playerData.earthMod;

  return stream;
}
