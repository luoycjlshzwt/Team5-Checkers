/********************************************************************************
** Form generated from reading UI file 'rank.ui'
**
** Created by: Qt User Interface Compiler version 6.2.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RANK_H
#define UI_RANK_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_Rank
{
public:

    void setupUi(QDialog *Rank)
    {
        if (Rank->objectName().isEmpty())
            Rank->setObjectName(QString::fromUtf8("Rank"));
        Rank->resize(274, 245);

        retranslateUi(Rank);

        QMetaObject::connectSlotsByName(Rank);
    } // setupUi

    void retranslateUi(QDialog *Rank)
    {
        Rank->setWindowTitle(QCoreApplication::translate("Rank", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Rank: public Ui_Rank {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RANK_H
