/********************************************************************************
** Form generated from reading UI file 'joinmenu.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_JOINMENU_H
#define UI_JOINMENU_H

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
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_JoinMenu {
public:
  QGridLayout *gridLayout_2;
  QGridLayout *gridLayout;
  QLabel *label_2;
  QLineEdit *serverip;
  QLabel *connectToIpLabel;
  QLineEdit *nickNameLineEdit;
  QLabel *nickNameLabel;
  QListWidget *serverListWidget;
  QLineEdit *portLineEdit;
  QPushButton *joinOk;
  QPushButton *refreshButton;
  QPushButton *joinCancel;

  void setupUi(QWidget *JoinMenu) {
    if (JoinMenu->objectName().isEmpty())
      JoinMenu->setObjectName(QStringLiteral("JoinMenu"));
    JoinMenu->resize(460, 410);
    gridLayout_2 = new QGridLayout(JoinMenu);
    gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
    gridLayout = new QGridLayout();
    gridLayout->setObjectName(QStringLiteral("gridLayout"));
    label_2 = new QLabel(JoinMenu);
    label_2->setObjectName(QStringLiteral("label_2"));

    gridLayout->addWidget(label_2, 0, 0, 1, 1);

    serverip = new QLineEdit(JoinMenu);
    serverip->setObjectName(QStringLiteral("serverip"));
    serverip->setMaxLength(32767);

    gridLayout->addWidget(serverip, 4, 1, 1, 1);

    connectToIpLabel = new QLabel(JoinMenu);
    connectToIpLabel->setObjectName(QStringLiteral("connectToIpLabel"));

    gridLayout->addWidget(connectToIpLabel, 4, 0, 1, 1);

    nickNameLineEdit = new QLineEdit(JoinMenu);
    nickNameLineEdit->setObjectName(QStringLiteral("nickNameLineEdit"));

    gridLayout->addWidget(nickNameLineEdit, 5, 1, 1, 2);

    nickNameLabel = new QLabel(JoinMenu);
    nickNameLabel->setObjectName(QStringLiteral("nickNameLabel"));

    gridLayout->addWidget(nickNameLabel, 5, 0, 1, 1);

    serverListWidget = new QListWidget(JoinMenu);
    serverListWidget->setObjectName(QStringLiteral("serverListWidget"));

    gridLayout->addWidget(serverListWidget, 1, 0, 1, 3);

    portLineEdit = new QLineEdit(JoinMenu);
    portLineEdit->setObjectName(QStringLiteral("portLineEdit"));

    gridLayout->addWidget(portLineEdit, 4, 2, 1, 1);

    gridLayout_2->addLayout(gridLayout, 0, 0, 1, 2);

    joinOk = new QPushButton(JoinMenu);
    joinOk->setObjectName(QStringLiteral("joinOk"));

    gridLayout_2->addWidget(joinOk, 3, 0, 1, 1);

    refreshButton = new QPushButton(JoinMenu);
    refreshButton->setObjectName(QStringLiteral("refreshButton"));

    gridLayout_2->addWidget(refreshButton, 3, 1, 1, 1);

    joinCancel = new QPushButton(JoinMenu);
    joinCancel->setObjectName(QStringLiteral("joinCancel"));

    gridLayout_2->addWidget(joinCancel, 4, 1, 1, 1);

    QWidget::setTabOrder(serverListWidget, serverip);
    QWidget::setTabOrder(serverip, joinOk);
    QWidget::setTabOrder(joinOk, refreshButton);
    QWidget::setTabOrder(refreshButton, joinCancel);

    retranslateUi(JoinMenu);
    QObject::connect(joinCancel, SIGNAL(clicked()), JoinMenu, SLOT(close()));

    QMetaObject::connectSlotsByName(JoinMenu);
  } // setupUi

  void retranslateUi(QWidget *JoinMenu) {
    JoinMenu->setWindowTitle(
        QApplication::translate("JoinMenu", "Join game", 0));
    label_2->setText(QApplication::translate("JoinMenu", "Server info:", 0));
    serverip->setText(QApplication::translate("JoinMenu", "192.168.0.199", 0));
    connectToIpLabel->setText(
        QApplication::translate("JoinMenu", "Connect to IP/Port:", 0));
    nickNameLineEdit->setText(
        QApplication::translate("JoinMenu", "Unnamed", 0));
    nickNameLabel->setText(
        QApplication::translate("JoinMenu", "Player name", 0));
    joinOk->setText(QApplication::translate("JoinMenu", "Connect", 0));
    refreshButton->setText(QApplication::translate("JoinMenu", "Refresh", 0));
    joinCancel->setText(QApplication::translate("JoinMenu", "Cancel", 0));
  } // retranslateUi
};

namespace Ui {
class JoinMenu : public Ui_JoinMenu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_JOINMENU_H
