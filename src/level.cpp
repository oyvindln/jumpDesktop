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

#include "level.h"

#include <QFile>
#include <QtDebug>

Platform2::Platform2() {
  line.setP1(QPointF(0, 0));
  line.setP2(QPointF(0, 0));
  width = 1;
}

void Platform2::setPoint1(QPointF point1) { line.setP1(point1); }

void Platform2::setPoint2(QPointF point2) { line.setP2(point2); }

Level::Level() : name("untitled"), width(100), height(100) {}

void Level::removeEnd(QByteArray *line) {
  int i = line->indexOf(10, 0);
  line->truncate(i);
}

void Level::loadLevel(QString levelName) {
  QFile file(levelName);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    //            std::cout << file.errorString().toStdString() << std::endl;
    qDebug() << "error" << file.errorString() << levelName;
    return;
  }

  QByteArray line = file.readLine();

  removeEnd(&line);
  QString str(line);
  name = str;

  line = file.readLine();
  removeEnd(&line);

  // set height and width to level
  int i = line.indexOf("|", 0);
  QByteArray temp;
  temp = line.left(i);
  width = temp.toInt();
  temp = line.right(line.size() - (i + 1));
  height = temp.toInt();

  // load platforms, might want to fix because end of file might be later than
  // expected.
  while (!file.atEnd()) {
    Platform2 platform;
    QByteArray temp;
    QPointF point;

    line = file.readLine();

    removeEnd(&line);

    i = line.indexOf("|", 0);
    temp = line.left(i);
    point.setX(temp.toInt());

    temp = line.right(line.size() - (i + 1));
    point.setY(temp.toInt());
    platform.setPoint1(point);

    line = file.readLine();
    removeEnd(&line);

    i = line.indexOf("|", 0);
    temp = line.left(i);
    point.setX(temp.toInt());

    temp = line.right(line.size() - (i + 1));
    point.setY(temp.toInt());
    platform.setPoint2(point);

    line = file.readLine();
    removeEnd(&line);

    i = line.indexOf("|", 0);
    platform.width = line.toFloat();

    platforms.push_back(platform);
  }
}

const std::vector<Platform2> &Level::getPlatforms() { return platforms; }
