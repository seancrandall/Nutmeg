/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

namespace Nutmeg {

class Ui_MainWindow
{
public:
    QAction *actionConnect;
    QAction *actionAddClientNaturalPerson;
    QAction *actionAddEnterprise;
    QAction *actionAddPerson;
    QAction *actionAddInventor;
    QAction *actionAddParalegal;
    QAction *actionAddPatentExaminer;
    QAction *actionAddTrademarkExaminer;
    QAction *actionAddWorkAttorney;
    QAction *actionNonfinal_Office_Action;
    QAction *actionFinal_Office_Action;
    QAction *actionOther_Patent;
    QAction *actionTrademark_Response;
    QAction *actionOther_Task;
    QAction *actionPatent_Matter;
    QAction *actionTrademark_Matter;
    QAction *actionCopyright_Matter;
    QAction *actionGeneral_Matter;
    QAction *actionRefreshData;
    QAction *actionPreferences;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QSplitter *splitter;
    QFrame *frameTop;
    QFrame *frameMiddle;
    QFrame *frameBottom;
    QFrame *frameMenuArea;
    QMenuBar *menubar;
    QMenu *menuB_ackend;
    QMenu *menuTables;
    QMenu *menuReports;
    QMenu *menuAdd_New;
    QMenu *menuPerson;
    QMenu *menuClient;
    QMenu *menuTask;
    QMenu *menuMatter;
    QMenu *menuView;
    QMenu *menu_Edit;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Nutmeg__MainWindow)
    {
        if (Nutmeg__MainWindow->objectName().isEmpty())
            Nutmeg__MainWindow->setObjectName("Nutmeg__MainWindow");
        Nutmeg__MainWindow->resize(800, 600);
        actionConnect = new QAction(Nutmeg__MainWindow);
        actionConnect->setObjectName("actionConnect");
        actionAddClientNaturalPerson = new QAction(Nutmeg__MainWindow);
        actionAddClientNaturalPerson->setObjectName("actionAddClientNaturalPerson");
        QIcon icon(QIcon::fromTheme(QString::fromUtf8("preferences-desktop-accessibility")));
        actionAddClientNaturalPerson->setIcon(icon);
        actionAddEnterprise = new QAction(Nutmeg__MainWindow);
        actionAddEnterprise->setObjectName("actionAddEnterprise");
        actionAddPerson = new QAction(Nutmeg__MainWindow);
        actionAddPerson->setObjectName("actionAddPerson");
        actionAddInventor = new QAction(Nutmeg__MainWindow);
        actionAddInventor->setObjectName("actionAddInventor");
        actionAddParalegal = new QAction(Nutmeg__MainWindow);
        actionAddParalegal->setObjectName("actionAddParalegal");
        actionAddPatentExaminer = new QAction(Nutmeg__MainWindow);
        actionAddPatentExaminer->setObjectName("actionAddPatentExaminer");
        actionAddTrademarkExaminer = new QAction(Nutmeg__MainWindow);
        actionAddTrademarkExaminer->setObjectName("actionAddTrademarkExaminer");
        actionAddWorkAttorney = new QAction(Nutmeg__MainWindow);
        actionAddWorkAttorney->setObjectName("actionAddWorkAttorney");
        actionNonfinal_Office_Action = new QAction(Nutmeg__MainWindow);
        actionNonfinal_Office_Action->setObjectName("actionNonfinal_Office_Action");
        actionFinal_Office_Action = new QAction(Nutmeg__MainWindow);
        actionFinal_Office_Action->setObjectName("actionFinal_Office_Action");
        actionOther_Patent = new QAction(Nutmeg__MainWindow);
        actionOther_Patent->setObjectName("actionOther_Patent");
        actionTrademark_Response = new QAction(Nutmeg__MainWindow);
        actionTrademark_Response->setObjectName("actionTrademark_Response");
        actionOther_Task = new QAction(Nutmeg__MainWindow);
        actionOther_Task->setObjectName("actionOther_Task");
        actionPatent_Matter = new QAction(Nutmeg__MainWindow);
        actionPatent_Matter->setObjectName("actionPatent_Matter");
        actionTrademark_Matter = new QAction(Nutmeg__MainWindow);
        actionTrademark_Matter->setObjectName("actionTrademark_Matter");
        actionCopyright_Matter = new QAction(Nutmeg__MainWindow);
        actionCopyright_Matter->setObjectName("actionCopyright_Matter");
        actionGeneral_Matter = new QAction(Nutmeg__MainWindow);
        actionGeneral_Matter->setObjectName("actionGeneral_Matter");
        actionRefreshData = new QAction(Nutmeg__MainWindow);
        actionRefreshData->setObjectName("actionRefreshData");
        QIcon icon1(QIcon::fromTheme(QString::fromUtf8("software-update-available")));
        actionRefreshData->setIcon(icon1);
        actionPreferences = new QAction(Nutmeg__MainWindow);
        actionPreferences->setObjectName("actionPreferences");
        centralwidget = new QWidget(Nutmeg__MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        splitter = new QSplitter(centralwidget);
        splitter->setObjectName("splitter");
        splitter->setOrientation(Qt::Orientation::Vertical);
        frameTop = new QFrame(splitter);
        frameTop->setObjectName("frameTop");
        frameTop->setFrameShape(QFrame::Shape::StyledPanel);
        frameTop->setFrameShadow(QFrame::Shadow::Raised);
        splitter->addWidget(frameTop);
        frameMiddle = new QFrame(splitter);
        frameMiddle->setObjectName("frameMiddle");
        frameMiddle->setFrameShape(QFrame::Shape::StyledPanel);
        frameMiddle->setFrameShadow(QFrame::Shadow::Raised);
        splitter->addWidget(frameMiddle);
        frameBottom = new QFrame(splitter);
        frameBottom->setObjectName("frameBottom");
        frameBottom->setFrameShape(QFrame::Shape::StyledPanel);
        frameBottom->setFrameShadow(QFrame::Shadow::Raised);
        splitter->addWidget(frameBottom);

        gridLayout->addWidget(splitter, 1, 0, 1, 1);

        frameMenuArea = new QFrame(centralwidget);
        frameMenuArea->setObjectName("frameMenuArea");
        frameMenuArea->setFrameShape(QFrame::Shape::StyledPanel);
        frameMenuArea->setFrameShadow(QFrame::Shadow::Raised);

        gridLayout->addWidget(frameMenuArea, 0, 0, 1, 1);

        Nutmeg__MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Nutmeg__MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 22));
        menuB_ackend = new QMenu(menubar);
        menuB_ackend->setObjectName("menuB_ackend");
        menuTables = new QMenu(menubar);
        menuTables->setObjectName("menuTables");
        menuReports = new QMenu(menubar);
        menuReports->setObjectName("menuReports");
        menuAdd_New = new QMenu(menubar);
        menuAdd_New->setObjectName("menuAdd_New");
        menuPerson = new QMenu(menuAdd_New);
        menuPerson->setObjectName("menuPerson");
        menuClient = new QMenu(menuAdd_New);
        menuClient->setObjectName("menuClient");
        menuTask = new QMenu(menuAdd_New);
        menuTask->setObjectName("menuTask");
        menuMatter = new QMenu(menuAdd_New);
        menuMatter->setObjectName("menuMatter");
        menuView = new QMenu(menubar);
        menuView->setObjectName("menuView");
        menu_Edit = new QMenu(menubar);
        menu_Edit->setObjectName("menu_Edit");
        Nutmeg__MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(Nutmeg__MainWindow);
        statusbar->setObjectName("statusbar");
        Nutmeg__MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuB_ackend->menuAction());
        menubar->addAction(menuTables->menuAction());
        menubar->addAction(menuReports->menuAction());
        menubar->addAction(menuAdd_New->menuAction());
        menubar->addAction(menuView->menuAction());
        menubar->addAction(menu_Edit->menuAction());
        menuB_ackend->addAction(actionConnect);
        menuAdd_New->addAction(menuTask->menuAction());
        menuAdd_New->addAction(menuMatter->menuAction());
        menuAdd_New->addAction(menuPerson->menuAction());
        menuAdd_New->addAction(menuClient->menuAction());
        menuPerson->addSeparator();
        menuPerson->addAction(actionAddPerson);
        menuPerson->addSeparator();
        menuPerson->addSeparator();
        menuPerson->addAction(actionAddInventor);
        menuPerson->addAction(actionAddParalegal);
        menuPerson->addAction(actionAddWorkAttorney);
        menuPerson->addSeparator();
        menuPerson->addAction(actionAddPatentExaminer);
        menuPerson->addAction(actionAddTrademarkExaminer);
        menuPerson->addSeparator();
        menuClient->addAction(actionAddClientNaturalPerson);
        menuClient->addAction(actionAddEnterprise);
        menuTask->addAction(actionNonfinal_Office_Action);
        menuTask->addAction(actionFinal_Office_Action);
        menuTask->addAction(actionOther_Patent);
        menuTask->addSeparator();
        menuTask->addAction(actionTrademark_Response);
        menuTask->addSeparator();
        menuTask->addAction(actionOther_Task);
        menuMatter->addAction(actionPatent_Matter);
        menuMatter->addAction(actionTrademark_Matter);
        menuMatter->addAction(actionCopyright_Matter);
        menuMatter->addAction(actionGeneral_Matter);
        menuView->addAction(actionRefreshData);
        menu_Edit->addAction(actionPreferences);

        retranslateUi(Nutmeg__MainWindow);

        QMetaObject::connectSlotsByName(Nutmeg__MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *Nutmeg__MainWindow)
    {
        Nutmeg__MainWindow->setWindowTitle(QCoreApplication::translate("Nutmeg::MainWindow", "MainWindow", nullptr));
        actionConnect->setText(QCoreApplication::translate("Nutmeg::MainWindow", "&Connect", nullptr));
        actionAddClientNaturalPerson->setText(QCoreApplication::translate("Nutmeg::MainWindow", "Natural Person", nullptr));
        actionAddEnterprise->setText(QCoreApplication::translate("Nutmeg::MainWindow", "Enterprise", nullptr));
        actionAddPerson->setText(QCoreApplication::translate("Nutmeg::MainWindow", "Person", nullptr));
        actionAddInventor->setText(QCoreApplication::translate("Nutmeg::MainWindow", "Inventor", nullptr));
        actionAddParalegal->setText(QCoreApplication::translate("Nutmeg::MainWindow", "Paralegal", nullptr));
        actionAddPatentExaminer->setText(QCoreApplication::translate("Nutmeg::MainWindow", "Patent Examiner", nullptr));
        actionAddTrademarkExaminer->setText(QCoreApplication::translate("Nutmeg::MainWindow", "Trademark Examiner", nullptr));
        actionAddWorkAttorney->setText(QCoreApplication::translate("Nutmeg::MainWindow", "Work Attorney", nullptr));
        actionNonfinal_Office_Action->setText(QCoreApplication::translate("Nutmeg::MainWindow", "Nonfinal Office Action", nullptr));
        actionFinal_Office_Action->setText(QCoreApplication::translate("Nutmeg::MainWindow", "Final Office Action", nullptr));
        actionOther_Patent->setText(QCoreApplication::translate("Nutmeg::MainWindow", "Other (Patent ...)", nullptr));
        actionTrademark_Response->setText(QCoreApplication::translate("Nutmeg::MainWindow", "Trademark Response", nullptr));
        actionOther_Task->setText(QCoreApplication::translate("Nutmeg::MainWindow", "Other Task ...", nullptr));
        actionPatent_Matter->setText(QCoreApplication::translate("Nutmeg::MainWindow", "Patent Matter", nullptr));
        actionTrademark_Matter->setText(QCoreApplication::translate("Nutmeg::MainWindow", "Trademark Matter", nullptr));
        actionCopyright_Matter->setText(QCoreApplication::translate("Nutmeg::MainWindow", "Copyright Matter", nullptr));
        actionGeneral_Matter->setText(QCoreApplication::translate("Nutmeg::MainWindow", "General Matter", nullptr));
        actionRefreshData->setText(QCoreApplication::translate("Nutmeg::MainWindow", "&Refresh Data", nullptr));
#if QT_CONFIG(shortcut)
        actionRefreshData->setShortcut(QCoreApplication::translate("Nutmeg::MainWindow", "Ctrl+R", nullptr));
#endif // QT_CONFIG(shortcut)
        actionPreferences->setText(QCoreApplication::translate("Nutmeg::MainWindow", "P&references", nullptr));
        menuB_ackend->setTitle(QCoreApplication::translate("Nutmeg::MainWindow", "&Backend", nullptr));
        menuTables->setTitle(QCoreApplication::translate("Nutmeg::MainWindow", "&Tables", nullptr));
        menuReports->setTitle(QCoreApplication::translate("Nutmeg::MainWindow", "&Reports", nullptr));
        menuAdd_New->setTitle(QCoreApplication::translate("Nutmeg::MainWindow", "&Add New", nullptr));
        menuPerson->setTitle(QCoreApplication::translate("Nutmeg::MainWindow", "Person", nullptr));
        menuClient->setTitle(QCoreApplication::translate("Nutmeg::MainWindow", "Client", nullptr));
        menuTask->setTitle(QCoreApplication::translate("Nutmeg::MainWindow", "Task", nullptr));
        menuMatter->setTitle(QCoreApplication::translate("Nutmeg::MainWindow", "Matter", nullptr));
        menuView->setTitle(QCoreApplication::translate("Nutmeg::MainWindow", "&View", nullptr));
        menu_Edit->setTitle(QCoreApplication::translate("Nutmeg::MainWindow", "&Edit", nullptr));
    } // retranslateUi

};

} // namespace Nutmeg

namespace Nutmeg {
namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui
} // namespace Nutmeg

#endif // UI_MAINWINDOW_H
