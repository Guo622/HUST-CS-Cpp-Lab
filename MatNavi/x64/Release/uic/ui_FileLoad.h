/********************************************************************************
** Form generated from reading UI file 'FileLoad.ui'
**
** Created by: Qt User Interface Compiler version 5.12.11
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILELOAD_H
#define UI_FILELOAD_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FileLoad
{
public:
    QLabel *labelWind;
    QLabel *labelStop;
    QLabel *labelLine;
    QPushButton *pushButtonStop;
    QPushButton *pushButtonLine;
    QPushButton *pushButtonDone;
    QPushButton *pushButtonQuit;
    QTextEdit *textEditStop;
    QTextEdit *textEditLine;
    QLabel *labelHits;
    QLabel *labelOrg;
    QTextEdit *textEditOrg;
    QPushButton *pushButtonOrg;

    void setupUi(QWidget *FileLoad)
    {
        if (FileLoad->objectName().isEmpty())
            FileLoad->setObjectName(QString::fromUtf8("FileLoad"));
        FileLoad->resize(592, 292);
        FileLoad->setMaximumSize(QSize(635, 292));
        labelWind = new QLabel(FileLoad);
        labelWind->setObjectName(QString::fromUtf8("labelWind"));
        labelWind->setGeometry(QRect(110, 20, 371, 41));
        QFont font;
        font.setPointSize(18);
        labelWind->setFont(font);
        labelWind->setAlignment(Qt::AlignCenter);
        labelStop = new QLabel(FileLoad);
        labelStop->setObjectName(QString::fromUtf8("labelStop"));
        labelStop->setGeometry(QRect(20, 90, 111, 31));
        QFont font1;
        font1.setPointSize(14);
        labelStop->setFont(font1);
        labelStop->setAlignment(Qt::AlignCenter);
        labelLine = new QLabel(FileLoad);
        labelLine->setObjectName(QString::fromUtf8("labelLine"));
        labelLine->setGeometry(QRect(20, 140, 111, 31));
        labelLine->setFont(font1);
        labelLine->setAlignment(Qt::AlignCenter);
        pushButtonStop = new QPushButton(FileLoad);
        pushButtonStop->setObjectName(QString::fromUtf8("pushButtonStop"));
        pushButtonStop->setGeometry(QRect(480, 90, 81, 31));
        pushButtonLine = new QPushButton(FileLoad);
        pushButtonLine->setObjectName(QString::fromUtf8("pushButtonLine"));
        pushButtonLine->setGeometry(QRect(480, 140, 81, 31));
        pushButtonDone = new QPushButton(FileLoad);
        pushButtonDone->setObjectName(QString::fromUtf8("pushButtonDone"));
        pushButtonDone->setGeometry(QRect(140, 240, 91, 31));
        QFont font2;
        font2.setPointSize(12);
        pushButtonDone->setFont(font2);
        pushButtonQuit = new QPushButton(FileLoad);
        pushButtonQuit->setObjectName(QString::fromUtf8("pushButtonQuit"));
        pushButtonQuit->setGeometry(QRect(360, 240, 91, 31));
        pushButtonQuit->setFont(font2);
        textEditStop = new QTextEdit(FileLoad);
        textEditStop->setObjectName(QString::fromUtf8("textEditStop"));
        textEditStop->setGeometry(QRect(140, 90, 321, 31));
        textEditLine = new QTextEdit(FileLoad);
        textEditLine->setObjectName(QString::fromUtf8("textEditLine"));
        textEditLine->setGeometry(QRect(140, 140, 321, 31));
        labelHits = new QLabel(FileLoad);
        labelHits->setObjectName(QString::fromUtf8("labelHits"));
        labelHits->setGeometry(QRect(10, 270, 551, 20));
        labelOrg = new QLabel(FileLoad);
        labelOrg->setObjectName(QString::fromUtf8("labelOrg"));
        labelOrg->setGeometry(QRect(20, 190, 111, 31));
        labelOrg->setFont(font1);
        labelOrg->setAlignment(Qt::AlignCenter);
        textEditOrg = new QTextEdit(FileLoad);
        textEditOrg->setObjectName(QString::fromUtf8("textEditOrg"));
        textEditOrg->setGeometry(QRect(140, 190, 321, 31));
        pushButtonOrg = new QPushButton(FileLoad);
        pushButtonOrg->setObjectName(QString::fromUtf8("pushButtonOrg"));
        pushButtonOrg->setGeometry(QRect(480, 190, 81, 31));

        retranslateUi(FileLoad);
        QObject::connect(pushButtonDone, SIGNAL(clicked()), FileLoad, SLOT(checkFile()));
        QObject::connect(pushButtonLine, SIGNAL(clicked()), FileLoad, SLOT(inputLine()));
        QObject::connect(pushButtonQuit, SIGNAL(clicked()), FileLoad, SLOT(close()));
        QObject::connect(pushButtonStop, SIGNAL(clicked()), FileLoad, SLOT(inputStop()));
        QObject::connect(pushButtonOrg, SIGNAL(clicked()), FileLoad, SLOT(inputOrg()));

        QMetaObject::connectSlotsByName(FileLoad);
    } // setupUi

    void retranslateUi(QWidget *FileLoad)
    {
        FileLoad->setWindowTitle(QApplication::translate("FileLoad", "FileLoad", nullptr));
        labelWind->setText(QApplication::translate("FileLoad", "\347\253\231\347\202\271\345\217\212\350\267\257\347\272\277\346\226\207\344\273\266\345\212\240\350\275\275", nullptr));
        labelStop->setText(QApplication::translate("FileLoad", "\347\253\231\347\202\271\346\226\207\344\273\266", nullptr));
        labelLine->setText(QApplication::translate("FileLoad", "\347\272\277\350\267\257\346\226\207\344\273\266", nullptr));
        pushButtonStop->setText(QApplication::translate("FileLoad", "\346\265\217\350\247\210...", nullptr));
        pushButtonLine->setText(QApplication::translate("FileLoad", "\346\265\217\350\247\210...", nullptr));
        pushButtonDone->setText(QApplication::translate("FileLoad", "\347\241\256\345\256\232", nullptr));
        pushButtonQuit->setText(QApplication::translate("FileLoad", "\351\200\200\345\207\272", nullptr));
        labelHits->setText(QString());
        labelOrg->setText(QApplication::translate("FileLoad", "\345\234\260\347\202\271\346\226\207\344\273\266", nullptr));
        pushButtonOrg->setText(QApplication::translate("FileLoad", "\346\265\217\350\247\210...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FileLoad: public Ui_FileLoad {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILELOAD_H
