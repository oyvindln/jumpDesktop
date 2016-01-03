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

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QDataStream>
#include <QString>

/*!
    Commands used in the application
   */
namespace commands {
const qint8 COMMAND_MASK = 0xFF;
//    const qint32 COMMAND_DETAILS = 0x00000000;
enum ServerCommand {
  // connect/lobby commands
  COMMAND_SV_BROADCAST = 0x00000000,
  COMMAND_SV_ACCEPTJOIN = 0x00000001,
  COMMAND_SV_CLIENTINFO = 0x00000002, // Server info to player on join
  COMMAND_SV_LOADIN, // Game info when starting
  COMMAND_SV_START,
  COMMAND_SV_SHUTDOWN,
  COMMAND_SV_FULL,
  // COMMAND_SV_CHAT,

  COMMAND_SV_PLAYERJOINED, // Another player joined
  COMMAND_SV_INFOCHANGED,

  // in-game commands
  COMMAND_SV_PLAYERMOVE,
  COMMAND_SV_PLAYERKILLRESPAWN,
  COMMAND_SV_SYNC,
  COMMAND_SV_GAMEEND,
  COMMAND_SV_PLAYERDC,
  COMMAND_SV_TEST,
  // COMMAND_SV_IG_FLOOD
  END_COMMAND_SV
};
enum ClientCommand {
  // Reserved, since 0 might indicate an error or reversed byte order etc.
  COMMAND_CL_RESERVED = 0x00000000,
  COMMAND_CL_REQJOIN,
  COMMAND_CL_DROP,
  COMMAND_CL_JUMP,
  // COMMAND_CL_CHAT,
  COMMAND_CL_WANTSYNC,
  COMMAND_CL_READY,
  COMMAND_CL_TEST,
  END_COMMAND_CL
};

//    const qint32 PLAYERSTATUS_PLAYERNUMBER = 0x0000000F;
//    const qint32 PLAYERSTATUS_POWERUP = 0x00000FF0;
}

/*!
    Constants used in the application
   */
namespace constants {
// Needs atleast 4.2 for QHostAddress, tho other things make us unable to go
// older than  Qt 4.3 anyway.
const int DATASTREAM_VERSION = QDataStream::Qt_4_3;
const quint16 DEFAULTPORT = 41112;
const int Playername_Offset_X = 10;
const int Playername_Offset_Y = 10;
const int Playername_Spacing = 100;

const QString FILENAME_Images_Prefix = "images/";
const QString FILENAME_Energybar_Center =
    FILENAME_Images_Prefix + "barcenter.png";
const QString FILENAME_Energybar_Yinyang =
    FILENAME_Images_Prefix + "yinyang.png";
const QString FILENAME_Energybar_BarPower =
    FILENAME_Images_Prefix + "barPower.png";
const QString FILENAME_Ninja = FILENAME_Images_Prefix + "ninja4.png";
const QString FILENAME_Platform = FILENAME_Images_Prefix + "platform3.png";
const QString FILENAME_Powerup_Sword =
    FILENAME_Images_Prefix + "powersword.png";
const QString FILENAME_Powerup_Speed = FILENAME_Images_Prefix + "speed.png";
const QString FILENAME_Powerup_Shield = FILENAME_Images_Prefix + "shield.png";
const QString FILENAME_Background = FILENAME_Images_Prefix + "background2.png";

const QString FILENAME_Dummy1 = FILENAME_Images_Prefix + "dummy1.png";

const QString FILENAME_Levels_Prefix = "Levels/";
const QString FILENAME_Default_Level = "big.lev";

const int ENERGYBAR_SUN_OFFSET = 7;

const qreal Sync_Pos_Max_Threshold = 2.0;
const qreal Sync_Velocity_Max_Threshold = 5.0;
const qreal Sync_Balance_Max_Threshold = 2.0;

const qreal Default_Gravity = 75.0;
const qreal Default_Speed_Vector_Length = 150.0;
}

#endif // CONSTANTS_H
