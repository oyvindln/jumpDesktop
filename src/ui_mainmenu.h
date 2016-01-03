/********************************************************************************
** Form generated from reading UI file 'mainmenu.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINMENU_H
#define UI_MAINMENU_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainMenu {
public:
  QVBoxLayout *verticalLayout;
  QPushButton *hostButton;
  QPushButton *joinButton;
  QPushButton *singlePlayerButton;
  QPushButton *optionsButton;
  QPushButton *statsButton;
  QPushButton *exitButton;

  void setupUi(QWidget *MainMenu) {
    if (MainMenu->objectName().isEmpty())
      MainMenu->setObjectName(QStringLiteral("MainMenu"));
    MainMenu->resize(500, 400);
    verticalLayout = new QVBoxLayout(MainMenu);
    verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
    hostButton = new QPushButton(MainMenu);
    hostButton->setObjectName(QStringLiteral("hostButton"));

    verticalLayout->addWidget(hostButton);

    joinButton = new QPushButton(MainMenu);
    joinButton->setObjectName(QStringLiteral("joinButton"));

    verticalLayout->addWidget(joinButton);

    singlePlayerButton = new QPushButton(MainMenu);
    singlePlayerButton->setObjectName(QStringLiteral("singlePlayerButton"));

    verticalLayout->addWidget(singlePlayerButton);

    optionsButton = new QPushButton(MainMenu);
    optionsButton->setObjectName(QStringLiteral("optionsButton"));

    verticalLayout->addWidget(optionsButton);

    statsButton = new QPushButton(MainMenu);
    statsButton->setObjectName(QStringLiteral("statsButton"));

    verticalLayout->addWidget(statsButton);

    exitButton = new QPushButton(MainMenu);
    exitButton->setObjectName(QStringLiteral("exitButton"));

    verticalLayout->addWidget(exitButton);

    retranslateUi(MainMenu);
    QObject::connect(exitButton, SIGNAL(clicked()), MainMenu, SLOT(close()));

    QMetaObject::connectSlotsByName(MainMenu);
  } // setupUi

  void retranslateUi(QWidget *MainMenu) {
    MainMenu->setWindowTitle(QApplication::translate("MainMenu", "Form", 0));
    hostButton->setText(QApplication::translate("MainMenu", "Host Game", 0));
    joinButton->setText(QApplication::translate("MainMenu", "Join Game", 0));
    singlePlayerButton->setText(
        QApplication::translate("MainMenu", "Single player", 0));
    optionsButton->setText(QApplication::translate("MainMenu", "Options", 0));
    statsButton->setText(
        QApplication::translate("MainMenu", "Statistics/Highscore", 0));
    exitButton->setText(QApplication::translate("MainMenu", "Exit", 0));
  } // retranslateUi
};

namespace Ui {
class MainMenu : public Ui_MainMenu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINMENU_H
