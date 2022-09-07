/********************************************************************************
** Form generated from reading UI file 'ViewTip.ui'
**
** Created by: Qt User Interface Compiler version 5.12.11
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIEWTIP_H
#define UI_VIEWTIP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_ViewTip
{
public:
    QLabel *labelMsg;

    void setupUi(QDialog *ViewTip)
    {
        if (ViewTip->objectName().isEmpty())
            ViewTip->setObjectName(QString::fromUtf8("ViewTip"));
        ViewTip->resize(191, 30);
        labelMsg = new QLabel(ViewTip);
        labelMsg->setObjectName(QString::fromUtf8("labelMsg"));
        labelMsg->setGeometry(QRect(5, 5, 181, 20));
        labelMsg->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));

        retranslateUi(ViewTip);

        QMetaObject::connectSlotsByName(ViewTip);
    } // setupUi

    void retranslateUi(QDialog *ViewTip)
    {
        ViewTip->setWindowTitle(QApplication::translate("ViewTip", "ViewTip", nullptr));
        labelMsg->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ViewTip: public Ui_ViewTip {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIEWTIP_H
