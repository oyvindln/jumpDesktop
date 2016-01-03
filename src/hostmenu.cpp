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

#include "hostmenu.h"
#include "ui_hostmenu.h"

#include <QFileDialog>
#include <QListWidgetItem>
#include <QNetworkInterface>

#include "game.h"
#include "networkhandler.h"

HostMenu::HostMenu(NetworkHandler &networkHandler, QWidget *parent)
    : QWidget(parent), m_ui(new Ui::HostMenu), m_mapName("big.txt"),
      m_networkHandler(&networkHandler) {
  m_parent = parent;
  m_ui->setupUi(this);
  connect(m_ui->startGameButton, SIGNAL(clicked()), this, SLOT(startClicked()));
  connect(m_ui->hostButton, SIGNAL(clicked()), this, SLOT(startListening()));
  connect(m_ui->mapSelectButton, SIGNAL(clicked()), this,
          SLOT(selectMapClicked()));
  m_listenTimer = 0;
  // Make sure the port line edit only contains integers inside the valid port
  // range
  // NOTE Should we go up to 65535 here?
  QValidator *validator = new QIntValidator(1025, 49150, this);
  m_ui->portLineEdit->setValidator(validator);
  m_ui->portLineEdit->setText(QString::number(constants::DEFAULTPORT));
  // NOTE didn't find a way to do this in the designer.
  m_ui->infoLabel->hide();
  QList<QHostAddress> adresses = QNetworkInterface::allAddresses();
  QString adressString;
  // Show local adresses (omitting localhost (127.0.0.1 etc))
  // TODO, make this look a bit better
  for (QList<QHostAddress>::iterator it = adresses.begin();
       it != adresses.end(); ++it) {
    QHostAddress &address = *it;
    if (address != QHostAddress::LocalHost &&
        address != QHostAddress::LocalHostIPv6) {
      adressString.append(address.toString());
      adressString.append("\n");
    }
  }
  m_ui->serverIpsTextEdit->setText(adressString);
}

HostMenu::~HostMenu() {
  //    delete m_game;
  delete m_ui;
  //    delete m_listenTimer;
}

/*!
  Start Game.
  */
void HostMenu::startClicked() {
  if (m_listenTimer != 0) {
    m_listenTimer->stop();
  }
  // NetworkHandler::getInstance()->startGame();
  emit starting();
}

/*!
  Start listening for connections, and show info about server.
  TODO, use signals for this instead.
  */
void HostMenu::startListening() {
  m_ui->startGameButton->setEnabled(true);
  connect(m_networkHandler, SIGNAL(clientJoin(QString, QString)), this,
          SLOT(addClientToView(QString, QString)));
  m_networkHandler->setPlayerName(m_ui->playerNameLineEdit->text());
  m_networkHandler->startListening(m_ui->portLineEdit->text().toInt());
  m_listenTimer = new QTimer(this);
  connect(m_listenTimer, SIGNAL(timeout()), m_networkHandler, SLOT(run()));
  m_listenTimer->start(100);
  m_ui->portLineEdit->hide();
  m_ui->portLabel->hide();
  m_ui->hostButton->setEnabled(false);
  m_ui->infoLabel->show();
  // FIXME IP address isn't fetched properly.
  m_ui->infoLabel->setText(tr("Now hosting on port: ") +
                           QString::number(m_networkHandler->serverPort()));
}

/*!
  Add name and ip for a connected client to the list view.
  */
void HostMenu::addClientToView(const QString &name, const QString &ip) {
  // TODO, keep a list of these so they can be removed if someone disconnects.
  new QListWidgetItem(name + "\t" + ip, m_ui->clientListWidget);
}

/*!
    Selects which map the game should load.
  */
void HostMenu::selectMapClicked() {
  QString newMapName =
      QFileDialog::getOpenFileName(this, tr("Open level file"), "./Levels",
                                   tr("Level files (*.lev);;All files (*.*)"));
  QDir dir;
  if (!newMapName.isEmpty()) {
    m_mapName = dir.relativeFilePath(newMapName);
  }
}

void HostMenu::changeEvent(QEvent *e) {
  switch (e->type()) {
  case QEvent::LanguageChange:
    m_ui->retranslateUi(this);
    break;
  default:
    break;
  }
}
