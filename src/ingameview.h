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

#ifndef JUMPINGAMEVIEW_H
#define JUMPINGAMEVIEW_H

#include <QGraphicsView>

#include "player.h"

class QMouseEvent;
class QPainter;
class QGraphicsScene;
class QLineF;

/*!
    Class for handling the input and in-game GUI.
*/
class IngameView : public QGraphicsView {
  Q_OBJECT
public:
  IngameView(QGraphicsScene *scene, int playerNumber, QList<Player *> &players,
             QWidget *parent = 0);

  ~IngameView();
  int getPlayerNumber();
  QRectF mapRectToScene(const QRect &rect);
  QRectF getEnergyBarSceneRect();

  void setFrameRate(qreal frameRate);
  void updateScore();
signals:
  void localMove(const QLine &moveVector);

protected:
  virtual void drawForeground(QPainter *painter, const QRectF &rect);
  void mousePressEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);
  void focusInEvent(QFocusEvent *event);
  void resizeEvent(QResizeEvent *event);

  void setPoints();

private:
  int m_playerNumber;

  QPixmap m_energyBarCenterPixmap;
  QPoint m_energyBarCenterDrawPos;
  QPixmap m_yinyangPixmap;
  QPoint m_yinyangDrawPos;
  QPixmap m_energyBarPowerPixmap;
  QPoint m_energyBarPowerDrawPos;
  // NOTE Reference, so will segfault if the refered object for some reason is
  // destroyed.
  const QList<Player *> &m_players;
  QRect m_energyBarUpdateRect;
  QString m_frameRate;
  QVector<QString> m_scores;
};

inline int IngameView::getPlayerNumber() { return m_playerNumber; }

inline QRectF IngameView::getEnergyBarSceneRect() {
  return mapRectToScene(m_energyBarUpdateRect);
}

inline void IngameView::setFrameRate(qreal frameRate) {
  m_frameRate = QString::number(frameRate);
}
#endif
