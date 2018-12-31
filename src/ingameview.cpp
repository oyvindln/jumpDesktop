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

#include "ingameview.h"

#include <cassert>
#include <cmath>

#include <QMouseEvent>
#include <QPainter>
#include <QGraphicsScene>
#include <QLineF>
#include <QPaintEngine>

#include "constants.h"

/*!
  IngameView constructor.
  NOTE Might be safer to use a pointer to the player list.
  */
IngameView::IngameView(QGraphicsScene *scene, int playerNumber,
                       QList<Player *> &players, QWidget *parent)
    : QGraphicsView(scene, parent), m_playerNumber(playerNumber),
      m_players(players) {

  // QGLFormat format;
  // format.defaultFormat();
  // format.setSampleBuffers(true);

  /*    setViewport(new QGLWidget());
      QGLWidget* widget = static_cast<QGLWidget*>(viewport());
      qDebug() << widget->doubleBuffer();
      qDebug() << widget->format().directRendering();
      qDebug() << widget->format().sampleBuffers();*/
  // Make sure there are no scrollbars
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  //    setInteractive(false);

  // setMaximumSize(640, 480);
  // setMinimumSize(640, 480);
  setFixedSize(640, 480);

  // setRenderHint(QPainter::Antialiasing,true);
  //    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

  //#if QT_VERSION >= 0x040500 //FIXME temporary workaround for graphical
  //glitches
  //    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
  //#endif

  // Load the pixmaps for the energy bar.
  m_energyBarCenterPixmap.load(constants::FILENAME_Energybar_Center);
  m_yinyangPixmap.load(constants::FILENAME_Energybar_Yinyang);
  m_energyBarPowerPixmap.load(constants::FILENAME_Energybar_BarPower);

  setPoints();

  /*
  m_svgrenderer = new QSvgRenderer(constants::FILENAME_ENERGYBAR, this);

  //Set the rect for the energy bar, currently using half the screen width, and
  scales accordingly.
  qreal aspect = m_svgrenderer->boundsOnElement("bar").height() /
  m_svgrenderer->boundsOnElement("bar").width();
  m_energyBarRect.setRect(width() / 4,
                          0,
                          width() / 2,
                          static_cast<int>((width() / 2) * aspect));

  //Set the rect for the yinyang symbol.
  m_middleRect.setRect(m_energyBarRect.x() + (m_energyBarRect.width() / 2) -
  (m_energyBarRect.height() / 2),
                       m_energyBarRect.y(),
                       m_energyBarRect.height(),
                       m_energyBarRect.height());
  //Set rect for the sun, make it a bit larger than the yinyang.
  //NOTE: scales using addition, if the screen size changes alot this could be a
  problem.
  m_sunRect.setCoords(m_middleRect.x() - constants::ENERGYBAR_SUN_OFFSET ,
                      m_middleRect.y() - constants::ENERGYBAR_SUN_OFFSET,
                      m_middleRect.bottomRight().x() +
  constants::ENERGYBAR_SUN_OFFSET,
                      m_middleRect.bottomRight().y() +
  constants::ENERGYBAR_SUN_OFFSET);

  m_energyBarUpdateRect.setTopRight(m_energyBarRect.topLeft());
  m_energyBarUpdateRect.setBottomRight(m_energyBarRect.bottomRight());
  //dy1 not needed, since it's already at the top of the screen.
  m_energyBarUpdateRect.adjust(-((m_middleRect.width() / 2) + 1),
                               0,
                               (m_middleRect.width() / 2) + 1,
                               (m_sunRect.height() - m_energyBarRect.height()) /
  2);
*/

  m_frameRate.append("0");
  m_scores.resize(m_players.size());
  updateScore();
}

IngameView::~IngameView() {}

QRectF IngameView::mapRectToScene(const QRect &rect) {
  QRectF mappedRect(mapToScene(rect.topLeft()), mapToScene(rect.bottomRight()));
  return mappedRect;
}

void IngameView::updateScore() {
  for (int i = 0; i < m_players.size(); ++i) {
    Player *player = m_players[i];

    m_scores[i] =
        player->getPlayerName() + ": " + QString::number(player->getScore());
  }
}

/*!
  Overridden function to draw score and balance bar.
  NOTE Should have been done using paintEvent instead, but couldn't get that to
  work. ~Øyvind
*/
void IngameView::drawForeground(QPainter *painter, const QRectF &) {
// Temporary, used for checking backend support for opacity.
#ifndef QT_NO_DEBUG
  static int first = 0;
  if (first == 0) {
    first++;

    if (painter->paintEngine()->hasFeature(QPaintEngine::ConstantOpacity)) {
      qDebug("Opacity supported by backend.");
    } else {
      qDebug("Opacity not supported by backend.");
    }
  }
#endif

  // Render player scores.
  // NOTE, will allways draw, but usually has to anyway.

  // Turn of matrix transformations, so we don't have to translate back again.
  painter->setWorldMatrixEnabled(false);

  int xPos = constants::Playername_Offset_X;
  QVector<QString>::const_iterator it;
  for (it = m_scores.begin(); it != m_scores.end(); ++it) {
    painter->drawText(QPoint(xPos, constants::Playername_Offset_Y), *it);
    xPos += constants::Playername_Spacing;
  }
  //    qDebug() << painter->worldMatrixEnabled();
  painter->drawText(QPoint(width() - 50, constants::Playername_Offset_Y),
                    m_frameRate);

  // Render energy bar.
  /*
        //Svg version, too slow.
        m_svgrenderer->render(painter, "bar",
                              mapRectToScene(m_energyBarRect));

        //Render sun with opacity.
        painter->setOpacity((100.0-std::fabs(m_players[m_playerNumber]->balance)*2)/100.0);
        m_svgrenderer->render(painter, "barPower",
        mapRectToScene(m_sunRect));
        painter->setOpacity(1);

        //Render yinyang symbol with offset.
        qreal offsetScale = m_energyBarRect.width() / 100.0;
        int offset = m_players[m_playerNumber]->balance * offsetScale;
        //Using points here, since qrect doesn't have a ctor that takes another
     qrect.
        QPoint topLeft(m_middleRect.topLeft());
        QPoint bottomRight(m_middleRect.bottomRight());
        topLeft.rx() += offset;
        bottomRight.rx() += offset;
        m_svgrenderer->render(painter, "YinYang",
                              QRectF(mapToScene(topLeft),
                                     mapToScene(bottomRight)));
  */

  // Pixmap version
  painter->drawPixmap(m_energyBarCenterDrawPos, m_energyBarCenterPixmap);

  painter->setOpacity(
      (100.0 - std::fabs(m_players[m_playerNumber]->getBalance()) * 2) / 100.0);
  //      painter->setWorldMatrixEnabled(false);

  // If opacity is != 1.0,
  // painter seems to use transformations even if they are disabled, where
  // opacity is not supported.
  if (painter->paintEngine()->hasFeature(QPaintEngine::ConstantOpacity)) {
    painter->drawPixmap(m_energyBarPowerDrawPos, m_energyBarPowerPixmap);
  } else {
    painter->drawPixmap(mapToScene(m_energyBarPowerDrawPos),
                        m_energyBarPowerPixmap);
  }

  painter->setOpacity(1.0);

  qreal offsetScale = m_energyBarCenterPixmap.width() / 100.0;
  int offset = m_players[m_playerNumber]->getBalance() * offsetScale;
  QPoint yinYangPos = m_yinyangDrawPos;
  yinYangPos.rx() += offset;
  painter->drawPixmap(yinYangPos, m_yinyangPixmap);
}

/*!
  Overidden function to set movement from mouse presses.
  */
void IngameView::mousePressEvent(QMouseEvent *event) {
  if (event->buttons() & Qt::LeftButton) {
    QLine moveVector;
    assert(m_playerNumber < m_players.size());
    moveVector.setPoints(mapFromScene(m_players[m_playerNumber]->pos()),
                         QPoint(event->x(), event->y()));
    // TODO: make sure move won't be too powerful, maybe normalize vector?
    emit localMove(moveVector);
  }

  event->accept();
}

/*!
  Reimplementation of wheelEvent to prevent scrolling with the wheel.
  */
void IngameView::wheelEvent(QWheelEvent *event) { event->accept(); }

void IngameView::focusInEvent(QFocusEvent *event) { event->accept(); }

void IngameView::resizeEvent(QResizeEvent *event) {
  setPoints();
  QGraphicsView::resizeEvent(event);
}

void IngameView::setPoints() {
  int middle = this->width() / 2;
  // Set the points where the energy bar is drawn.
  m_energyBarCenterDrawPos.setX(middle - (m_energyBarCenterPixmap.width() / 2));
  m_energyBarCenterDrawPos.setY(0);

  m_yinyangDrawPos.setX(middle - (m_yinyangPixmap.width() / 2));
  m_yinyangDrawPos.setY(0);

  m_energyBarPowerDrawPos.setX(middle - (m_energyBarPowerPixmap.width() / 2));
  m_energyBarPowerDrawPos.setY(-constants::ENERGYBAR_SUN_OFFSET);

  // Set the rectangle where the scene needs to be redrawn.
  m_energyBarUpdateRect.setTopLeft(m_energyBarCenterDrawPos -
                                   QPoint(m_yinyangPixmap.width(), 0));
  m_energyBarUpdateRect.setBottomRight(
      m_energyBarCenterDrawPos +
      QPoint(m_energyBarCenterPixmap.width() + (m_yinyangPixmap.width()),
             m_energyBarPowerPixmap.height() + 50));
}
