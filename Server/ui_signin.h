/********************************************************************************
** Form generated from reading UI file 'signin.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIGNIN_H
#define UI_SIGNIN_H

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
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_signIn
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLabel *label_2;
    QVBoxLayout *verticalLayout_2;
    QLineEdit *Username;
    QLineEdit *Password;
    QHBoxLayout *horizontalLayout;
    QPushButton *signIn_2;
    QPushButton *Enroll;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *signIn)
    {
        if (signIn->objectName().isEmpty())
            signIn->setObjectName(QStringLiteral("signIn"));
        signIn->resize(640, 480);
        centralwidget = new QWidget(signIn);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        verticalLayout_4 = new QVBoxLayout(centralwidget);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(centralwidget);
        label->setObjectName(QStringLiteral("label"));
        label->setStyleSheet(QLatin1String("font: 20pt \"Cambria\";\n"
"color: rgb(85, 0, 255)"));

        verticalLayout->addWidget(label);

        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setStyleSheet(QLatin1String("font: 20pt \"Cambria\";\n"
"color: rgb(85, 0, 255)"));

        verticalLayout->addWidget(label_2);


        horizontalLayout_2->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        Username = new QLineEdit(centralwidget);
        Username->setObjectName(QStringLiteral("Username"));
        Username->setStyleSheet(QLatin1String("font: 20pt \"Cambria\";\n"
""));

        verticalLayout_2->addWidget(Username);

        Password = new QLineEdit(centralwidget);
        Password->setObjectName(QStringLiteral("Password"));
        Password->setStyleSheet(QLatin1String("font: 20pt \"Cambria\";\n"
""));
        Password->setEchoMode(QLineEdit::Password);

        verticalLayout_2->addWidget(Password);


        horizontalLayout_2->addLayout(verticalLayout_2);


        verticalLayout_3->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        signIn_2 = new QPushButton(centralwidget);
        signIn_2->setObjectName(QStringLiteral("signIn_2"));
        signIn_2->setStyleSheet(QLatin1String("font: 20pt \"Cambria\";\n"
"color: rgb(85, 0, 255)"));

        horizontalLayout->addWidget(signIn_2);

        Enroll = new QPushButton(centralwidget);
        Enroll->setObjectName(QStringLiteral("Enroll"));
        Enroll->setStyleSheet(QLatin1String("font: 20pt \"Cambria\";\n"
"color: rgb(85, 0, 255)"));

        horizontalLayout->addWidget(Enroll);


        verticalLayout_3->addLayout(horizontalLayout);


        verticalLayout_4->addLayout(verticalLayout_3);

        signIn->setCentralWidget(centralwidget);
        menubar = new QMenuBar(signIn);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 640, 26));
        signIn->setMenuBar(menubar);
        statusbar = new QStatusBar(signIn);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        signIn->setStatusBar(statusbar);

        retranslateUi(signIn);

        QMetaObject::connectSlotsByName(signIn);
    } // setupUi

    void retranslateUi(QMainWindow *signIn)
    {
        signIn->setWindowTitle(QApplication::translate("signIn", "MainWindow", 0));
        label->setText(QApplication::translate("signIn", " \347\224\250\346\210\267\345\220\215:", 0));
        label_2->setText(QApplication::translate("signIn", "\345\257\206\347\240\201:", 0));
        Password->setText(QString());
        signIn_2->setText(QApplication::translate("signIn", "\347\231\273\345\205\245", 0));
        Enroll->setText(QApplication::translate("signIn", "\346\263\250\345\206\214", 0));
    } // retranslateUi

};

namespace Ui {
    class signIn: public Ui_signIn {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIGNIN_H
