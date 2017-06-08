/********************************************************************************
** Form generated from reading UI file 'statementshow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STATEMENTSHOW_H
#define UI_STATEMENTSHOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_statementshow
{
public:
    QWidget *centralwidget;
    QTableView *tableView;
    QTableView *tableView_2;
    QPushButton *pushButtonAll;
    QPushButton *pushButtonOne;
    QLabel *label;
    QLineEdit *lineEdit;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonDay;
    QPushButton *pushButtonWeek;
    QPushButton *pushButtonMonth;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *statementshow)
    {
        if (statementshow->objectName().isEmpty())
            statementshow->setObjectName(QStringLiteral("statementshow"));
        statementshow->resize(1300, 430);
        statementshow->setMinimumSize(QSize(1300, 430));
        statementshow->setMaximumSize(QSize(1300, 430));
        centralwidget = new QWidget(statementshow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        tableView = new QTableView(centralwidget);
        tableView->setObjectName(QStringLiteral("tableView"));
        tableView->setGeometry(QRect(20, 100, 811, 261));
        tableView_2 = new QTableView(centralwidget);
        tableView_2->setObjectName(QStringLiteral("tableView_2"));
        tableView_2->setGeometry(QRect(890, 100, 341, 261));
        pushButtonAll = new QPushButton(centralwidget);
        pushButtonAll->setObjectName(QStringLiteral("pushButtonAll"));
        pushButtonAll->setGeometry(QRect(630, 30, 207, 47));
        pushButtonAll->setStyleSheet(QLatin1String("font: 20pt \"Cambria\";\n"
"color: rgb(85, 0, 255)"));
        pushButtonOne = new QPushButton(centralwidget);
        pushButtonOne->setObjectName(QStringLiteral("pushButtonOne"));
        pushButtonOne->setGeometry(QRect(500, 30, 93, 47));
        pushButtonOne->setStyleSheet(QLatin1String("font: 20pt \"Cambria\";\n"
"color: rgb(85, 0, 255)"));
        label = new QLabel(centralwidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 30, 132, 38));
        label->setStyleSheet(QLatin1String("font: 20pt \"Cambria\";\n"
"color: rgb(85, 0, 255)"));
        lineEdit = new QLineEdit(centralwidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(160, 30, 307, 44));
        lineEdit->setStyleSheet(QLatin1String("font: 20pt \"Cambria\";\n"
""));
        widget = new QWidget(centralwidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(891, 30, 340, 49));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pushButtonDay = new QPushButton(widget);
        pushButtonDay->setObjectName(QStringLiteral("pushButtonDay"));
        pushButtonDay->setStyleSheet(QLatin1String("font: 20pt \"Cambria\";\n"
"color: rgb(85, 0, 255)"));

        horizontalLayout->addWidget(pushButtonDay);

        pushButtonWeek = new QPushButton(widget);
        pushButtonWeek->setObjectName(QStringLiteral("pushButtonWeek"));
        pushButtonWeek->setStyleSheet(QLatin1String("font: 20pt \"Cambria\";\n"
"color: rgb(85, 0, 255)"));

        horizontalLayout->addWidget(pushButtonWeek);

        pushButtonMonth = new QPushButton(widget);
        pushButtonMonth->setObjectName(QStringLiteral("pushButtonMonth"));
        pushButtonMonth->setStyleSheet(QLatin1String("font: 20pt \"Cambria\";\n"
"color: rgb(85, 0, 255)"));

        horizontalLayout->addWidget(pushButtonMonth);

        statementshow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(statementshow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 1300, 26));
        statementshow->setMenuBar(menubar);
        statusbar = new QStatusBar(statementshow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        statementshow->setStatusBar(statusbar);

        retranslateUi(statementshow);

        QMetaObject::connectSlotsByName(statementshow);
    } // setupUi

    void retranslateUi(QMainWindow *statementshow)
    {
        statementshow->setWindowTitle(QApplication::translate("statementshow", "MainWindow", 0));
        pushButtonAll->setText(QApplication::translate("statementshow", "\346\237\245\347\234\213\346\211\200\346\234\211\346\210\277\351\227\264", 0));
        pushButtonOne->setText(QApplication::translate("statementshow", "\346\237\245\347\234\213", 0));
        label->setText(QApplication::translate("statementshow", "\346\210\277\351\227\264\345\217\267\357\274\232", 0));
        pushButtonDay->setText(QApplication::translate("statementshow", "\346\227\245\346\212\245\350\241\250", 0));
        pushButtonWeek->setText(QApplication::translate("statementshow", "\345\221\250\346\212\245\350\241\250", 0));
        pushButtonMonth->setText(QApplication::translate("statementshow", "\346\234\210\346\212\245\350\241\250", 0));
    } // retranslateUi

};

namespace Ui {
    class statementshow: public Ui_statementshow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STATEMENTSHOW_H
