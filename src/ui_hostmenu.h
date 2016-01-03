/********************************************************************************
** Form generated from reading UI file 'hostmenu.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HOSTMENU_H
#define UI_HOSTMENU_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HostMenu {
public:
  QGridLayout *gridLayout;
  QPushButton *mapSelectButton;
  QPushButton *startGameButton;
  QListWidget *clientListWidget;
  QPushButton *hostButton;
  QPushButton *backButton;
  QLabel *infoLabel;
  QGridLayout *gridLayout_2;
  QLabel *playerNameLabel;
  QLineEdit *playerNameLineEdit;
  QLabel *portLabel;
  QLineEdit *portLineEdit;
  QLabel *maxScore;
  QSpinBox *livesSpinBox;
  QLabel *serverIPLabel;
  QTextEdit *serverIpsTextEdit;

  void setupUi(QWidget *HostMenu) {
    if (HostMenu->objectName().isEmpty())
      HostMenu->setObjectName(QStringLiteral("HostMenu"));
    HostMenu->resize(460, 410);
    gridLayout = new QGridLayout(HostMenu);
    gridLayout->setObjectName(QStringLiteral("gridLayout"));
    mapSelectButton = new QPushButton(HostMenu);
    mapSelectButton->setObjectName(QStringLiteral("mapSelectButton"));

    gridLayout->addWidget(mapSelectButton, 0, 0, 1, 2);

    startGameButton = new QPushButton(HostMenu);
    startGameButton->setObjectName(QStringLiteral("startGameButton"));
    startGameButton->setEnabled(false);

    gridLayout->addWidget(startGameButton, 0, 2, 1, 2);

    clientListWidget = new QListWidget(HostMenu);
    clientListWidget->setObjectName(QStringLiteral("clientListWidget"));

    gridLayout->addWidget(clientListWidget, 11, 0, 1, 4);

    hostButton = new QPushButton(HostMenu);
    hostButton->setObjectName(QStringLiteral("hostButton"));

    gridLayout->addWidget(hostButton, 12, 0, 1, 2);

    backButton = new QPushButton(HostMenu);
    backButton->setObjectName(QStringLiteral("backButton"));

    gridLayout->addWidget(backButton, 12, 2, 1, 2);

    infoLabel = new QLabel(HostMenu);
    infoLabel->setObjectName(QStringLiteral("infoLabel"));

    gridLayout->addWidget(infoLabel, 8, 0, 1, 1);

    gridLayout_2 = new QGridLayout();
    gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
    playerNameLabel = new QLabel(HostMenu);
    playerNameLabel->setObjectName(QStringLiteral("playerNameLabel"));

    gridLayout_2->addWidget(playerNameLabel, 2, 0, 1, 1);

    playerNameLineEdit = new QLineEdit(HostMenu);
    playerNameLineEdit->setObjectName(QStringLiteral("playerNameLineEdit"));

    gridLayout_2->addWidget(playerNameLineEdit, 2, 1, 1, 1);

    portLabel = new QLabel(HostMenu);
    portLabel->setObjectName(QStringLiteral("portLabel"));

    gridLayout_2->addWidget(portLabel, 1, 0, 1, 1);

    portLineEdit = new QLineEdit(HostMenu);
    portLineEdit->setObjectName(QStringLiteral("portLineEdit"));
    QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(
        portLineEdit->sizePolicy().hasHeightForWidth());
    portLineEdit->setSizePolicy(sizePolicy);

    gridLayout_2->addWidget(portLineEdit, 1, 1, 1, 1);

    maxScore = new QLabel(HostMenu);
    maxScore->setObjectName(QStringLiteral("maxScore"));
    maxScore->setEnabled(false);

    gridLayout_2->addWidget(maxScore, 0, 0, 1, 1);

    livesSpinBox = new QSpinBox(HostMenu);
    livesSpinBox->setObjectName(QStringLiteral("livesSpinBox"));
    livesSpinBox->setEnabled(false);

    gridLayout_2->addWidget(livesSpinBox, 0, 1, 1, 1);

    gridLayout->addLayout(gridLayout_2, 5, 0, 1, 4);

    serverIPLabel = new QLabel(HostMenu);
    serverIPLabel->setObjectName(QStringLiteral("serverIPLabel"));

    gridLayout->addWidget(serverIPLabel, 6, 0, 1, 1);

    serverIpsTextEdit = new QTextEdit(HostMenu);
    serverIpsTextEdit->setObjectName(QStringLiteral("serverIpsTextEdit"));
    serverIpsTextEdit->setReadOnly(true);

    gridLayout->addWidget(serverIpsTextEdit, 7, 0, 1, 4);

    retranslateUi(HostMenu);
    QObject::connect(backButton, SIGNAL(clicked()), HostMenu, SLOT(hide()));

    QMetaObject::connectSlotsByName(HostMenu);
  } // setupUi

  void retranslateUi(QWidget *HostMenu) {
    HostMenu->setWindowTitle(
        QApplication::translate("HostMenu", "Host game", 0));
    mapSelectButton->setText(
        QApplication::translate("HostMenu", "Select map..", 0));
    startGameButton->setText(
        QApplication::translate("HostMenu", "Start Game", 0));
    hostButton->setText(QApplication::translate("HostMenu", "Host", 0));
    backButton->setText(QApplication::translate("HostMenu", "Back", 0));
    infoLabel->setText(QApplication::translate("HostMenu", "Players:", 0));
    playerNameLabel->setText(
        QApplication::translate("HostMenu", "Player Name", 0));
    playerNameLineEdit->setText(
        QApplication::translate("HostMenu", "Unnamed", 0));
    portLabel->setText(QApplication::translate("HostMenu", "Port", 0));
    portLineEdit->setText(QString());
    maxScore->setText(QApplication::translate("HostMenu", "Max score", 0));
    serverIPLabel->setText(
        QApplication::translate("HostMenu", "Server Ip(s)", 0));
  } // retranslateUi
};

namespace Ui {
class HostMenu : public Ui_HostMenu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HOSTMENU_H
