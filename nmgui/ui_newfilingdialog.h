/********************************************************************************
** Form generated from reading UI file 'newfilingdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWFILINGDIALOG_H
#define UI_NEWFILINGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <widgets/combobox.h>
#include <widgets/dateedit.h>
#include <widgets/mattersearchbox.h>

QT_BEGIN_NAMESPACE

class Ui_NewFilingDialog
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLabel *labelTaskClass;
    Nutmeg::ComboBox *comboTaskClass;
    QVBoxLayout *verticalLayout_2;
    QLabel *labelTask;
    Nutmeg::ComboBox *comboTask;
    QVBoxLayout *verticalLayout_3;
    QLabel *labelMatter;
    Nutmeg::MatterSearchBox *comboMatter;
    QSpacerItem *verticalSpacer;
    QGroupBox *gbDates;
    QVBoxLayout *verticalLayout_8;
    QVBoxLayout *verticalLayout_4;
    QLabel *labelTriggerDate;
    Nutmeg::DateEdit *deTriggerDate;
    QVBoxLayout *verticalLayout_5;
    QLabel *labelSoftDeadline;
    Nutmeg::DateEdit *deSoftDeadline;
    QVBoxLayout *verticalLayout_6;
    QLabel *labelHardDeadline;
    Nutmeg::DateEdit *deHardDeadline;
    QVBoxLayout *verticalLayout_7;
    QLabel *labelNextDeadline;
    Nutmeg::DateEdit *deNextDeadline;
    QSpacerItem *horizontalSpacer;
    QGroupBox *gbFlags;
    QVBoxLayout *verticalLayout_9;
    QCheckBox *cbRequiresAuth;
    QCheckBox *cbAuthReceived;
    QSpacerItem *verticalSpacer_2;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *NewFilingDialog)
    {
        if (NewFilingDialog->objectName().isEmpty())
            NewFilingDialog->setObjectName("NewFilingDialog");
        NewFilingDialog->resize(517, 396);
        gridLayout = new QGridLayout(NewFilingDialog);
        gridLayout->setObjectName("gridLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        labelTaskClass = new QLabel(NewFilingDialog);
        labelTaskClass->setObjectName("labelTaskClass");

        verticalLayout->addWidget(labelTaskClass);

        comboTaskClass = new Nutmeg::ComboBox(NewFilingDialog);
        comboTaskClass->setObjectName("comboTaskClass");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(comboTaskClass->sizePolicy().hasHeightForWidth());
        comboTaskClass->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(comboTaskClass);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        labelTask = new QLabel(NewFilingDialog);
        labelTask->setObjectName("labelTask");

        verticalLayout_2->addWidget(labelTask);

        comboTask = new Nutmeg::ComboBox(NewFilingDialog);
        comboTask->setObjectName("comboTask");
        sizePolicy.setHeightForWidth(comboTask->sizePolicy().hasHeightForWidth());
        comboTask->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(comboTask);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        labelMatter = new QLabel(NewFilingDialog);
        labelMatter->setObjectName("labelMatter");

        verticalLayout_3->addWidget(labelMatter);

        comboMatter = new Nutmeg::MatterSearchBox(NewFilingDialog);
        comboMatter->setObjectName("comboMatter");
        sizePolicy.setHeightForWidth(comboMatter->sizePolicy().hasHeightForWidth());
        comboMatter->setSizePolicy(sizePolicy);

        verticalLayout_3->addWidget(comboMatter);


        horizontalLayout->addLayout(verticalLayout_3);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 3);

        verticalSpacer = new QSpacerItem(488, 10, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout->addItem(verticalSpacer, 1, 0, 1, 3);

        gbDates = new QGroupBox(NewFilingDialog);
        gbDates->setObjectName("gbDates");
        verticalLayout_8 = new QVBoxLayout(gbDates);
        verticalLayout_8->setObjectName("verticalLayout_8");
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName("verticalLayout_4");
        labelTriggerDate = new QLabel(gbDates);
        labelTriggerDate->setObjectName("labelTriggerDate");

        verticalLayout_4->addWidget(labelTriggerDate);

        deTriggerDate = new Nutmeg::DateEdit(gbDates);
        deTriggerDate->setObjectName("deTriggerDate");

        verticalLayout_4->addWidget(deTriggerDate);


        verticalLayout_8->addLayout(verticalLayout_4);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName("verticalLayout_5");
        labelSoftDeadline = new QLabel(gbDates);
        labelSoftDeadline->setObjectName("labelSoftDeadline");

        verticalLayout_5->addWidget(labelSoftDeadline);

        deSoftDeadline = new Nutmeg::DateEdit(gbDates);
        deSoftDeadline->setObjectName("deSoftDeadline");

        verticalLayout_5->addWidget(deSoftDeadline);


        verticalLayout_8->addLayout(verticalLayout_5);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName("verticalLayout_6");
        labelHardDeadline = new QLabel(gbDates);
        labelHardDeadline->setObjectName("labelHardDeadline");

        verticalLayout_6->addWidget(labelHardDeadline);

        deHardDeadline = new Nutmeg::DateEdit(gbDates);
        deHardDeadline->setObjectName("deHardDeadline");

        verticalLayout_6->addWidget(deHardDeadline);


        verticalLayout_8->addLayout(verticalLayout_6);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName("verticalLayout_7");
        labelNextDeadline = new QLabel(gbDates);
        labelNextDeadline->setObjectName("labelNextDeadline");

        verticalLayout_7->addWidget(labelNextDeadline);

        deNextDeadline = new Nutmeg::DateEdit(gbDates);
        deNextDeadline->setObjectName("deNextDeadline");

        verticalLayout_7->addWidget(deNextDeadline);


        verticalLayout_8->addLayout(verticalLayout_7);


        gridLayout->addWidget(gbDates, 2, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(181, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer, 2, 1, 1, 1);

        gbFlags = new QGroupBox(NewFilingDialog);
        gbFlags->setObjectName("gbFlags");
        verticalLayout_9 = new QVBoxLayout(gbFlags);
        verticalLayout_9->setObjectName("verticalLayout_9");
        cbRequiresAuth = new QCheckBox(gbFlags);
        cbRequiresAuth->setObjectName("cbRequiresAuth");

        verticalLayout_9->addWidget(cbRequiresAuth);

        cbAuthReceived = new QCheckBox(gbFlags);
        cbAuthReceived->setObjectName("cbAuthReceived");

        verticalLayout_9->addWidget(cbAuthReceived);

        verticalSpacer_2 = new QSpacerItem(20, 145, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_9->addItem(verticalSpacer_2);


        gridLayout->addWidget(gbFlags, 2, 2, 1, 1);

        buttonBox = new QDialogButtonBox(NewFilingDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        gridLayout->addWidget(buttonBox, 3, 0, 1, 3);


        retranslateUi(NewFilingDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, NewFilingDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, NewFilingDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(NewFilingDialog);
    } // setupUi

    void retranslateUi(QDialog *NewFilingDialog)
    {
        NewFilingDialog->setWindowTitle(QCoreApplication::translate("NewFilingDialog", "Dialog", nullptr));
        labelTaskClass->setText(QCoreApplication::translate("NewFilingDialog", "Task Class", nullptr));
        labelTask->setText(QCoreApplication::translate("NewFilingDialog", "Task", nullptr));
        labelMatter->setText(QCoreApplication::translate("NewFilingDialog", "Matter\n"
"(Can create new)", nullptr));
        gbDates->setTitle(QCoreApplication::translate("NewFilingDialog", "Dates", nullptr));
        labelTriggerDate->setText(QCoreApplication::translate("NewFilingDialog", "Trigger Date", nullptr));
        labelSoftDeadline->setText(QCoreApplication::translate("NewFilingDialog", "Soft Deadline", nullptr));
        labelHardDeadline->setText(QCoreApplication::translate("NewFilingDialog", "Hard Deadline", nullptr));
        labelNextDeadline->setText(QCoreApplication::translate("NewFilingDialog", "Next Deadline", nullptr));
        gbFlags->setTitle(QCoreApplication::translate("NewFilingDialog", "Flags", nullptr));
        cbRequiresAuth->setText(QCoreApplication::translate("NewFilingDialog", "Needs Authorization?", nullptr));
        cbAuthReceived->setText(QCoreApplication::translate("NewFilingDialog", "Authorization Received?", nullptr));
    } // retranslateUi

};

namespace Ui {
    class NewFilingDialog: public Ui_NewFilingDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWFILINGDIALOG_H
