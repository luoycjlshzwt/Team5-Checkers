/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 6.2.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QPushButton *QUIT;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(832, 427);
        QUIT = new QPushButton(Widget);
        QUIT->setObjectName(QString::fromUtf8("QUIT"));
        QUIT->setGeometry(QRect(600, 50, 60, 30));
        QFont font;
        font.setPointSize(11);
        font.setBold(true);
        QUIT->setFont(font);
        QUIT->setStyleSheet(QString::fromUtf8("color: rgb(255, 85, 0);\n"
                                              "background-color: rgb(255, 255, 255);"));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Form", nullptr));
        QUIT->setText(QCoreApplication::translate("Widget", "QUIT", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
