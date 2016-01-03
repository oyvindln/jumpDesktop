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

#ifndef REPLAY_H
#define REPLAY_H

/*!
  Struct to store replay data.
  */
struct Replay {
  int time;
  int player;
  int x;
  int y;
  int action;
  Replay(int time, int player, int x, int y, int action) {
    this->time = time;
    this->player = player;
    this->x = x;
    this->y = y;
    this->action = action;
  }
};

#endif // REPLAY_H
