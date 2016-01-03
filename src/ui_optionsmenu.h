/********************************************************************************
** Form generated from reading UI file 'optionsmenu.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPTIONSMENU_H
#define UI_OPTIONSMENU_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OptionsMenu {
public:
  QGridLayout *gridLayout;
  QLabel *updateRateLabel;
  QLineEdit *updateRateLineEdit;
  QLabel *goreLabel;
  QSlider *goreSlider;
  QPushButton *backButton;

  void setupUi(QWidget *OptionsMenu) {
    if (OptionsMenu->objectName().isEmpty())
      OptionsMenu->setObjectName(QStringLiteral("OptionsMenu"));
    OptionsMenu->resize(460, 410);
    gridLayout = new QGridLayout(OptionsMenu);
    gridLayout->setObjectName(QStringLiteral("gridLayout"));
    updateRateLabel = new QLabel(OptionsMenu);
    updateRateLabel->setObjectName(QStringLiteral("updateRateLabel"));

    gridLayout->addWidget(updateRateLabel, 0, 0, 1, 1);

    updateRateLineEdit = new QLineEdit(OptionsMenu);
    updateRateLineEdit->setObjectName(QStringLiteral("updateRateLineEdit"));

    gridLayout->addWidget(updateRateLineEdit, 0, 1, 1, 1);

    goreLabel = new QLabel(OptionsMenu);
    goreLabel->setObjectName(QStringLiteral("goreLabel"));
    goreLabel->setEnabled(false);

    gridLayout->addWidget(goreLabel, 1, 0, 1, 1);

    goreSlider = new QSlider(OptionsMenu);
    goreSlider->setObjectName(QStringLiteral("goreSlider"));
    goreSlider->setEnabled(false);
    goreSlider->setMaximum(5);
    goreSlider->setOrientation(Qt::Horizontal);
    goreSlider->setInvertedAppearance(false);
    goreSlider->setInvertedControls(false);

    gridLayout->addWidget(goreSlider, 1, 1, 1, 1);

    backButton = new QPushButton(OptionsMenu);
    backButton->setObjectName(QStringLiteral("backButton"));
    backButton->setEnabled(true);

    gridLayout->addWidget(backButton, 2, 0, 1, 2);

    QWidget::setTabOrder(goreSlider, backButton);

    retranslateUi(OptionsMenu);
    QObject::connect(backButton, SIGNAL(clicked()), OptionsMenu, SLOT(close()));

    QMetaObject::connectSlotsByName(OptionsMenu);
  } // setupUi

  void retranslateUi(QWidget *OptionsMenu) {
    OptionsMenu->setWindowTitle(
        QApplication::translate("OptionsMenu", "Options", 0));
    updateRateLabel->setText(
        QApplication::translate("OptionsMenu", "Update Rate", 0));
    updateRateLineEdit->setText(QApplication::translate("OptionsMenu", "0", 0));
    goreLabel->setText(
        QApplication::translate("OptionsMenu", "Gore level:", 0));
    backButton->setText(QApplication::translate("OptionsMenu", "Back", 0));
  } // retranslateUi
};

namespace Ui {
class OptionsMenu : public Ui_OptionsMenu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPTIONSMENU_H
