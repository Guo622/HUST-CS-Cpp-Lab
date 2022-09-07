/********************************************************************************
** Form generated from reading UI file 'MatNavi.ui'
**
** Created by: Qt User Interface Compiler version 5.12.11
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MATNAVI_H
#define UI_MATNAVI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MatNaviClass
{
public:
    QAction *action_minChange;
    QAction *action_searchLoc;
    QAction *action_open;
    QAction *action_exit;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QGraphicsView *graphicsView;
    QMenuBar *menuBar;
    QMenu *menu_Q;
    QMenu *menu_F;

    void setupUi(QMainWindow *MatNaviClass)
    {
        if (MatNaviClass->objectName().isEmpty())
            MatNaviClass->setObjectName(QString::fromUtf8("MatNaviClass"));
        MatNaviClass->resize(1700, 854);
        QFont font;
        font.setPointSize(9);
        MatNaviClass->setFont(font);
        MatNaviClass->setStyleSheet(QString::fromUtf8(""));
        action_minChange = new QAction(MatNaviClass);
        action_minChange->setObjectName(QString::fromUtf8("action_minChange"));
        action_minChange->setCheckable(true);
        QFont font1;
        font1.setPointSize(12);
        action_minChange->setFont(font1);
        action_searchLoc = new QAction(MatNaviClass);
        action_searchLoc->setObjectName(QString::fromUtf8("action_searchLoc"));
        action_searchLoc->setCheckable(true);
        action_searchLoc->setFont(font1);
        action_open = new QAction(MatNaviClass);
        action_open->setObjectName(QString::fromUtf8("action_open"));
        action_open->setCheckable(true);
        action_open->setFont(font1);
        action_exit = new QAction(MatNaviClass);
        action_exit->setObjectName(QString::fromUtf8("action_exit"));
        action_exit->setCheckable(true);
        action_exit->setFont(font1);
        centralWidget = new QWidget(MatNaviClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        centralWidget->setStyleSheet(QString::fromUtf8(""));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        graphicsView = new QGraphicsView(centralWidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setFont(font1);
        graphicsView->setMouseTracking(true);
        graphicsView->setAutoFillBackground(true);
        graphicsView->setStyleSheet(QString::fromUtf8("background-image: url(:/MatNavi/hustmap.bmp);"));

        gridLayout->addWidget(graphicsView, 0, 0, 1, 1);

        MatNaviClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MatNaviClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1700, 22));
        menuBar->setFont(font1);
        menu_Q = new QMenu(menuBar);
        menu_Q->setObjectName(QString::fromUtf8("menu_Q"));
        menu_F = new QMenu(menuBar);
        menu_F->setObjectName(QString::fromUtf8("menu_F"));
        MatNaviClass->setMenuBar(menuBar);

        menuBar->addAction(menu_F->menuAction());
        menuBar->addAction(menu_Q->menuAction());
        menu_Q->addAction(action_minChange);
        menu_Q->addAction(action_searchLoc);
        menu_F->addAction(action_open);
        menu_F->addAction(action_exit);

        retranslateUi(MatNaviClass);
        QObject::connect(action_minChange, SIGNAL(triggered(bool)), MatNaviClass, SLOT(minChange()));
        QObject::connect(action_exit, SIGNAL(triggered(bool)), MatNaviClass, SLOT(closewnd()));
        QObject::connect(action_open, SIGNAL(triggered(bool)), MatNaviClass, SLOT(loadmap()));
        QObject::connect(action_searchLoc, SIGNAL(triggered(bool)), MatNaviClass, SLOT(searchLoc()));

        QMetaObject::connectSlotsByName(MatNaviClass);
    } // setupUi

    void retranslateUi(QMainWindow *MatNaviClass)
    {
        MatNaviClass->setWindowTitle(QApplication::translate("MatNaviClass", "MatNavi", nullptr));
        action_minChange->setText(QApplication::translate("MatNaviClass", "\346\234\200\345\260\221\350\275\254\344\271\230", nullptr));
        action_searchLoc->setText(QApplication::translate("MatNaviClass", "\346\220\234\347\264\242\345\234\260\347\202\271", nullptr));
        action_open->setText(QApplication::translate("MatNaviClass", "\350\257\273\345\205\245\345\234\260\345\233\276", nullptr));
        action_exit->setText(QApplication::translate("MatNaviClass", "\351\200\200\345\207\272", nullptr));
        menu_Q->setTitle(QApplication::translate("MatNaviClass", "\346\237\245\350\257\242(Q)", nullptr));
        menu_F->setTitle(QApplication::translate("MatNaviClass", "\346\226\207\344\273\266(F)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MatNaviClass: public Ui_MatNaviClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MATNAVI_H
