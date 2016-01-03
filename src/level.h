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

#ifndef LEVEL_H
#define LEVEL_H

#include <QLineF>
#include <QString>
#include <QPointF>

#include <vector>

/*!
  Class to store platform info used in the level loader
  */
class Platform2 {
public:
  Platform2();
  QLineF line;
  float width;
  void setPoint1(QPointF point1);
  void setPoint2(QPointF point2);
};

/*!
  Class for storing level info
  */
class Level {
public:
  Level();
  void loadLevel(QString levelName);
  void removeEnd(QByteArray *line);
  QString name;
  int width, height;
  const std::vector<Platform2> &getPlatforms();

private:
  std::vector<Platform2> platforms;
};

#endif
