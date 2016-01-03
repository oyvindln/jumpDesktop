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

#include "mainwindow.h"

#include <QApplication>
#include <QCloseEvent>
#include <QSettings>

#include "game.h"
#include "ingameview.h"
#include "makeunique.h"
#include "mainmenu.h"
#include "networkclient.h"

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags), m_mainMenu(new MainMenu(this)),
      m_networkHandler(), m_hostMenu(m_networkHandler)

{
  // setWindowTitle(tr("Ninja Jump"));
  QApplication::setApplicationName(tr("Ninja jump"));

  setCentralWidget(m_mainMenu);
  m_mainMenu->show();

  connect(m_mainMenu, SIGNAL(joinPressed()), &m_joinMenu, SLOT(show()));
  //    connect(m_joinMenu->refreshButton, SIGNAL(clicked()), this,
  //    SLOT(joinServer()));
  connect(&m_joinMenu, SIGNAL(gameStarting(const QString &, NetworkClient &)),
          this, SLOT(joinServer(const QString &, NetworkClient &)));
  // connect(m_joinMenu, SIGNAL(joinOkPressed()), this, SLOT(startGame()));
  connect(m_mainMenu, SIGNAL(hostPressed()), &m_hostMenu, SLOT(show()));
  connect(m_mainMenu, SIGNAL(optionsPressed()), &m_optionsMenu, SLOT(show()));
  connect(m_mainMenu, SIGNAL(singlePressed()), this, SLOT(startGame()));
  connect(&m_hostMenu, SIGNAL(starting()), this, SLOT(startServer()));

  setMinimumWidth(640);
  setMinimumHeight(480);
  setMaximumSize(640, 480);
  //	loadSettings();
  //
}

MainWindow::~MainWindow() {}

/*!
  Starts a single player game. Not really implemented, just used for testing.
  */
void MainWindow::startGame() {

  m_game = Utils::make_unique<Game>(
      4, 1, m_optionsMenu.updateRateLineEdit->text().toInt());
  m_game->addPlayer("Ola");
  m_game->start(22);
  m_game->begin();
  //    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

  m_game->getGraphicsView()->setParent(this);
  m_mainMenu->hide();
  setCentralWidget(m_game->getGraphicsView());
}

/*!
  Hosts a server.
  */
void MainWindow::startServer() {
  m_hostMenu.hide();

  m_game = Utils::make_unique<Game>(
      4, 1, m_optionsMenu.updateRateLineEdit->text().toInt());
  m_game->setMap(m_hostMenu.getMapName());

  // NetworkHandler::getInstance()->getParser().setGame(m_game);
  m_networkHandler.addPlayers(m_game.get());
  m_networkHandler.startGame();
  m_game->start(m_networkHandler.getSeed());
  IngameView *ingameView = m_game->getGraphicsView();
  ingameView->setParent(this);
  connect(ingameView, SIGNAL(localMove(QLine)), &m_networkHandler,
          SLOT(localMove(QLine)));
  setCentralWidget(m_game->getGraphicsView());
}

/*!
  Starts the game which is joined.
  */
void MainWindow::joinServer(const QString &mapName, NetworkClient &client) {
  qDebug() << Q_FUNC_INFO << mapName;
  m_joinMenu.hide();
  m_game = Utils::make_unique<Game>(
      4, 1, m_optionsMenu.updateRateLineEdit->text().toInt());
  m_game->setMap(mapName);
  client.setGame(m_game.get());
  client.addPlayers();
  m_game->start(client.getSeed());
  IngameView *ingameView = m_game->getGraphicsView();
  ingameView->setParent(this);
  connect(ingameView, SIGNAL(localMove(QLine)), &client,
          SLOT(localPlayerMove(QLine)));
  setCentralWidget(m_game->getGraphicsView());
  client.sendReady();
}

/*!
  Loads settings from disk.
  */
void MainWindow::loadSettings() {
  QSettings settings("ninjajump", "ninjajump");
  /* m_optionsMenu->soundEffectsCheckbox->setChecked(
                  settings.value("sound/useSoundFX", true).toBool());
  m_optionsMenu->musicCheckBox->setChecked(
                  settings.value("sound/useMusic", false).toBool());
  m_optionsMenu->openGLCheckBox->setChecked(
                  settings.value("graphics/useOpenGL", false).toBool());
  m_optionsMenu->antiAliasingCheckBox->setChecked(
                  settings.value("graphics/useAntiAliasing", false).toBool());
  */
  m_optionsMenu.goreSlider->setValue(
      settings.value("gameplay/gore", 0).toInt());
}

/*!
  Saves settings to disk.
  */
void MainWindow::saveSettings() {
  QSettings settings("ninjajump", "ninjajump");
  /*   settings.setValue("sound/useSoundFX",
                       m_optionsMenu->soundEffectsCheckbox->isChecked());
     settings.setValue("sound/useMusic",
                       m_optionsMenu->musicCheckBox->isChecked());
     settings.setValue("graphics/useOpenGL",
                       m_optionsMenu->openGLCheckBox->isChecked());
     settings.setValue("graphics/useAntiAliasing",
                       m_optionsMenu->antiAliasingCheckBox->isChecked()); */
  settings.setValue("gameplay/gore", m_optionsMenu.goreSlider->value());
}

void MainWindow::closeEvent(QCloseEvent *event) {
  // Disabled, don't want mess in the registry yet.
  //  saveSettings();
  event->accept();
}
