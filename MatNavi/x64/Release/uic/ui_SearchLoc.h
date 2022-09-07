/********************************************************************************
** Form generated from reading UI file 'SearchLoc.ui'
**
** Created by: Qt User Interface Compiler version 5.12.11
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEARCHLOC_H
#define UI_SEARCHLOC_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SearchLoc
{
public:

    void setupUi(QWidget *SearchLoc)
    {
        if (SearchLoc->objectName().isEmpty())
            SearchLoc->setObjectName(QString::fromUtf8("SearchLoc"));
        SearchLoc->resize(400, 300);

        retranslateUi(SearchLoc);

        QMetaObject::connectSlotsByName(SearchLoc);
    } // setupUi

    void retranslateUi(QWidget *SearchLoc)
    {
        SearchLoc->setWindowTitle(QApplication::translate("SearchLoc", "SearchLoc", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SearchLoc: public Ui_SearchLoc {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEARCHLOC_H
