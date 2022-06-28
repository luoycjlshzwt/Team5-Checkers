/********************************************************************************
** Form generated from reading UI file 'wait.ui'
**
** Created by: Qt User Interface Compiler version 6.2.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WAIT_H
#define UI_WAIT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Wait
{
public:
    QLabel *ques;
    QPushButton *EXIT;
    QPushButton *YES;
    QLineEdit *name1;
    QLineEdit *name2;
    QLineEdit *name3;
    QLineEdit *name4;
    QLineEdit *name5;
    QLineEdit *name6;
    QLabel *situ1;
    QLabel *situ2;
    QLabel *situ3;
    QLabel *situ4;
    QLabel *situ5;
    QLabel *situ6;
    QLabel *room;
    QLabel *roomnum;

    void setupUi(QWidget *Wait)
    {
        if (Wait->objectName().isEmpty())
            Wait->setObjectName(QString::fromUtf8("Wait"));
        Wait->resize(581, 345);
        Wait->setStyleSheet(QString::fromUtf8("background-color: rgb(176, 224, 230);"));
        ques = new QLabel(Wait);
        ques->setObjectName(QString::fromUtf8("ques"));
        ques->setGeometry(QRect(60, 70, 221, 41));
        QFont font;
        font.setPointSize(20);
        font.setBold(true);
        ques->setFont(font);
        EXIT = new QPushButton(Wait);
        EXIT->setObjectName(QString::fromUtf8("EXIT"));
        EXIT->setGeometry(QRect(440, 50, 80, 31));
        QFont font1;
        font1.setPointSize(12);
        font1.setBold(true);
        EXIT->setFont(font1);
        EXIT->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        YES = new QPushButton(Wait);
        YES->setObjectName(QString::fromUtf8("YES"));
        YES->setGeometry(QRect(310, 50, 80, 31));
        YES->setFont(font1);
        YES->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 255);\n"
"background-color: rgb(255, 255, 255);"));
        name1 = new QLineEdit(Wait);
        name1->setObjectName(QString::fromUtf8("name1"));
        name1->setGeometry(QRect(50, 140, 141, 31));
        QFont font2;
        font2.setPointSize(10);
        name1->setFont(font2);
        name1->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        name2 = new QLineEdit(Wait);
        name2->setObjectName(QString::fromUtf8("name2"));
        name2->setGeometry(QRect(50, 200, 141, 31));
        name2->setFont(font2);
        name2->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        name3 = new QLineEdit(Wait);
        name3->setObjectName(QString::fromUtf8("name3"));
        name3->setGeometry(QRect(50, 260, 141, 31));
        name3->setFont(font2);
        name3->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        name4 = new QLineEdit(Wait);
        name4->setObjectName(QString::fromUtf8("name4"));
        name4->setGeometry(QRect(300, 140, 151, 31));
        name4->setFont(font2);
        name4->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        name5 = new QLineEdit(Wait);
        name5->setObjectName(QString::fromUtf8("name5"));
        name5->setGeometry(QRect(300, 200, 151, 31));
        name5->setFont(font2);
        name5->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        name6 = new QLineEdit(Wait);
        name6->setObjectName(QString::fromUtf8("name6"));
        name6->setGeometry(QRect(300, 260, 151, 31));
        name6->setFont(font2);
        name6->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        situ1 = new QLabel(Wait);
        situ1->setObjectName(QString::fromUtf8("situ1"));
        situ1->setGeometry(QRect(210, 145, 54, 21));
        QFont font3;
        font3.setPointSize(11);
        situ1->setFont(font3);
        situ2 = new QLabel(Wait);
        situ2->setObjectName(QString::fromUtf8("situ2"));
        situ2->setGeometry(QRect(210, 206, 54, 20));
        situ2->setFont(font3);
        situ3 = new QLabel(Wait);
        situ3->setObjectName(QString::fromUtf8("situ3"));
        situ3->setGeometry(QRect(210, 265, 54, 21));
        situ3->setFont(font3);
        situ4 = new QLabel(Wait);
        situ4->setObjectName(QString::fromUtf8("situ4"));
        situ4->setGeometry(QRect(470, 146, 54, 20));
        situ4->setFont(font3);
        situ5 = new QLabel(Wait);
        situ5->setObjectName(QString::fromUtf8("situ5"));
        situ5->setGeometry(QRect(470, 205, 54, 21));
        situ5->setFont(font3);
        situ6 = new QLabel(Wait);
        situ6->setObjectName(QString::fromUtf8("situ6"));
        situ6->setGeometry(QRect(470, 266, 54, 20));
        situ6->setFont(font3);
        room = new QLabel(Wait);
        room->setObjectName(QString::fromUtf8("room"));
        room->setGeometry(QRect(50, 40, 54, 21));
        QFont font4;
        font4.setPointSize(12);
        font4.setBold(true);
        room->setFont(font4);
        roomnum = new QLabel(Wait);
        roomnum->setObjectName(QString::fromUtf8("roomnum"));
        roomnum->setGeometry(QRect(130, 40, 54, 21));
        roomnum->setFont(font3);

        retranslateUi(Wait);

        QMetaObject::connectSlotsByName(Wait);
    } // setupUi

    void retranslateUi(QWidget *Wait)
    {
        Wait->setWindowTitle(QCoreApplication::translate("Wait", "Form", nullptr));

        ques->setText(QCoreApplication::translate("Wait", "Are you ready ?", nullptr));
        EXIT->setText(QCoreApplication::translate("Wait", "QUIT", nullptr));
        YES->setText(QCoreApplication::translate("Wait", "READY", nullptr));
        situ1->setText(QCoreApplication::translate("Wait", "waiting", nullptr));
        situ2->setText(QCoreApplication::translate("Wait", "waiting", nullptr));
        situ3->setText(QCoreApplication::translate("Wait", "waiting", nullptr));
        situ4->setText(QCoreApplication::translate("Wait", "waiting", nullptr));
        situ5->setText(QCoreApplication::translate("Wait", "waiting", nullptr));
        situ6->setText(QCoreApplication::translate("Wait", "waiting", nullptr));
        room->setText(QCoreApplication::translate("Wait", "ROOM", nullptr));

    } // retranslateUi

};

namespace Ui {
    class Wait: public Ui_Wait {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WAIT_H

