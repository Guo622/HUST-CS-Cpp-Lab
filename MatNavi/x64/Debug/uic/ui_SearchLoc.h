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
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SearchLoc
{
public:
    QPushButton *pushButtonDone;
    QPushButton *pushButtonQuit;
    QLabel *labelStart;
    QLabel *labelEnd;
    QLineEdit *lineEditStart;
    QLineEdit *lineEditEnd;
    QLabel *labelHit;

    void setupUi(QWidget *SearchLoc)
    {
        if (SearchLoc->objectName().isEmpty())
            SearchLoc->setObjectName(QString::fromUtf8("SearchLoc"));
        SearchLoc->resize(513, 288);
        pushButtonDone = new QPushButton(SearchLoc);
        pushButtonDone->setObjectName(QString::fromUtf8("pushButtonDone"));
        pushButtonDone->setGeometry(QRect(130, 200, 91, 31));
        QFont font;
        font.setPointSize(12);
        pushButtonDone->setFont(font);
        pushButtonQuit = new QPushButton(SearchLoc);
        pushButtonQuit->setObjectName(QString::fromUtf8("pushButtonQuit"));
        pushButtonQuit->setGeometry(QRect(330, 200, 91, 31));
        pushButtonQuit->setFont(font);
        labelStart = new QLabel(SearchLoc);
        labelStart->setObjectName(QString::fromUtf8("labelStart"));
        labelStart->setGeometry(QRect(20, 80, 111, 31));
        QFont font1;
        font1.setPointSize(14);
        labelStart->setFont(font1);
        labelStart->setAlignment(Qt::AlignCenter);
        labelEnd = new QLabel(SearchLoc);
        labelEnd->setObjectName(QString::fromUtf8("labelEnd"));
        labelEnd->setGeometry(QRect(20, 130, 111, 31));
        labelEnd->setFont(font1);
        labelEnd->setAlignment(Qt::AlignCenter);
        lineEditStart = new QLineEdit(SearchLoc);
        lineEditStart->setObjectName(QString::fromUtf8("lineEditStart"));
        lineEditStart->setGeometry(QRect(150, 80, 251, 27));
        lineEditEnd = new QLineEdit(SearchLoc);
        lineEditEnd->setObjectName(QString::fromUtf8("lineEditEnd"));
        lineEditEnd->setGeometry(QRect(150, 130, 251, 27));
        labelHit = new QLabel(SearchLoc);
        labelHit->setObjectName(QString::fromUtf8("labelHit"));
        labelHit->setGeometry(QRect(40, 250, 461, 20));

        retranslateUi(SearchLoc);
        QObject::connect(pushButtonQuit, SIGNAL(clicked()), SearchLoc, SLOT(close()));

        QMetaObject::connectSlotsByName(SearchLoc);
    } // setupUi

    void retranslateUi(QWidget *SearchLoc)
    {
        SearchLoc->setWindowTitle(QApplication::translate("SearchLoc", "SearchLoc", nullptr));
        pushButtonDone->setText(QApplication::translate("SearchLoc", "\347\241\256\345\256\232", nullptr));
        pushButtonQuit->setText(QApplication::translate("SearchLoc", "\351\200\200\345\207\272", nullptr));
        labelStart->setText(QApplication::translate("SearchLoc", "\350\276\223\345\205\245\350\265\267\347\202\271", nullptr));
        labelEnd->setText(QApplication::translate("SearchLoc", "\350\276\223\345\205\245\347\273\210\347\202\271", nullptr));
        labelHit->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class SearchLoc: public Ui_SearchLoc {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEARCHLOC_H
