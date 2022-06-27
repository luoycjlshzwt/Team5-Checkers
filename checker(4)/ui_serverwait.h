/********************************************************************************
** Form generated from reading UI file 'serverwait.ui'
**
** Created by: Qt User Interface Compiler version 6.2.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVERWAIT_H
#define UI_SERVERWAIT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_serverwait
{
public:
    QLineEdit *name1;
    QLineEdit *name2;
    QLineEdit *name3;
    QLabel *waiting;
    QLabel *situ1;
    QLabel *situ2;
    QLabel *situ3;
    QLineEdit *name4;
    QLineEdit *name5;
    QLineEdit *name6;
    QLabel *situ4;
    QLabel *situ5;
    QLabel *situ6;
    QLabel *room;
    QLabel *roomnum;

    void setupUi(QWidget *serverwait)
    {
        if (serverwait->objectName().isEmpty())
            serverwait->setObjectName(QString::fromUtf8("serverwait"));
        serverwait->resize(581, 345);
        serverwait->setStyleSheet(QString::fromUtf8("background-color: rgb(176, 224, 230);"));
        name1 = new QLineEdit(serverwait);
        name1->setObjectName(QString::fromUtf8("name1"));
        name1->setGeometry(QRect(52, 102, 131, 31));
        QFont font;
        font.setPointSize(10);
        name1->setFont(font);
        name1->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        name2 = new QLineEdit(serverwait);
        name2->setObjectName(QString::fromUtf8("name2"));
        name2->setGeometry(QRect(52, 172, 131, 31));
        name2->setFont(font);
        name2->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        name3 = new QLineEdit(serverwait);
        name3->setObjectName(QString::fromUtf8("name3"));
        name3->setGeometry(QRect(52, 242, 131, 31));
        name3->setFont(font);
        name3->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        waiting = new QLabel(serverwait);
        waiting->setObjectName(QString::fromUtf8("waiting"));
        waiting->setGeometry(QRect(230, 30, 191, 51));
        QFont font1;
        font1.setPointSize(26);
        font1.setBold(true);
        waiting->setFont(font1);
        situ1 = new QLabel(serverwait);
        situ1->setObjectName(QString::fromUtf8("situ1"));
        situ1->setGeometry(QRect(200, 106, 54, 20));
        QFont font2;
        font2.setPointSize(11);
        situ1->setFont(font2);
        situ2 = new QLabel(serverwait);
        situ2->setObjectName(QString::fromUtf8("situ2"));
        situ2->setGeometry(QRect(200, 176, 54, 20));
        situ2->setFont(font2);
        situ3 = new QLabel(serverwait);
        situ3->setObjectName(QString::fromUtf8("situ3"));
        situ3->setGeometry(QRect(200, 246, 54, 20));
        situ3->setFont(font2);
        name4 = new QLineEdit(serverwait);
        name4->setObjectName(QString::fromUtf8("name4"));
        name4->setGeometry(QRect(312, 102, 131, 31));
        name4->setFont(font);
        name4->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        name5 = new QLineEdit(serverwait);
        name5->setObjectName(QString::fromUtf8("name5"));
        name5->setGeometry(QRect(312, 172, 131, 31));
        name5->setFont(font);
        name5->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        name6 = new QLineEdit(serverwait);
        name6->setObjectName(QString::fromUtf8("name6"));
        name6->setGeometry(QRect(312, 242, 131, 31));
        name6->setFont(font);
        name6->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        situ4 = new QLabel(serverwait);
        situ4->setObjectName(QString::fromUtf8("situ4"));
        situ4->setGeometry(QRect(460, 106, 54, 20));
        situ4->setFont(font2);
        situ5 = new QLabel(serverwait);
        situ5->setObjectName(QString::fromUtf8("situ5"));
        situ5->setGeometry(QRect(460, 176, 54, 20));
        situ5->setFont(font2);
        situ6 = new QLabel(serverwait);
        situ6->setObjectName(QString::fromUtf8("situ6"));
        situ6->setGeometry(QRect(460, 246, 54, 20));
        situ6->setFont(font2);
        room = new QLabel(serverwait);
        room->setObjectName(QString::fromUtf8("room"));
        room->setGeometry(QRect(50, 40, 54, 21));
        QFont font3;
        font3.setPointSize(12);
        font3.setBold(true);
        room->setFont(font3);
        roomnum = new QLabel(serverwait);
        roomnum->setObjectName(QString::fromUtf8("roomnum"));
        roomnum->setGeometry(QRect(130, 40, 54, 21));
        roomnum->setFont(font2);

        retranslateUi(serverwait);

        QMetaObject::connectSlotsByName(serverwait);
    } // setupUi

    void retranslateUi(QWidget *serverwait)
    {
        serverwait->setWindowTitle(QCoreApplication::translate("serverwait", "Form", nullptr));
        waiting->setText(QCoreApplication::translate("serverwait", "waiting...", nullptr));
        situ1->setText(QCoreApplication::translate("serverwait", "waiting", nullptr));
        situ2->setText(QCoreApplication::translate("serverwait", "waiting", nullptr));
        situ3->setText(QCoreApplication::translate("serverwait", "waiting", nullptr));
        situ4->setText(QCoreApplication::translate("serverwait", "waiting", nullptr));
        situ5->setText(QCoreApplication::translate("serverwait", "waiting", nullptr));
        situ6->setText(QCoreApplication::translate("serverwait", "waiting", nullptr));
        room->setText(QCoreApplication::translate("serverwait", "ROOM", nullptr));
    } // retranslateUi

};

namespace Ui {
    class serverwait: public Ui_serverwait {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVERWAIT_H
