#include "comboeditadd.h"

namespace Nutmeg
{

ComboEditAdd::ComboEditAdd(QWidget *parent) : QWidget{parent}
{
    cLabel->text = QString();
    cLabel->hide();

    setupDisplay();

    connectSignalsAndSlots();
}

ComboEditAdd::ComboEditAdd(QString label, QWidget *parent) : QWidget(parent)
{
    cLabel->text = label;

    setupDisplay();

    connectSignalsAndSlots();
}

void ComboEditAdd::setupDisplay()
{
    delete layout;
    layout = new QHBoxLayout();
    layout->addStretch();
    layout->addWidget(cLabel);
    layout->addWidget(cCombo);
    layout->addWidget(cEdit);
    layout->addWidget(cPlus);

    setLayout(layout);
}

void ComboEditAdd::connectSignalsAndSlots()
{
    QObject::connect(cCombo, &QComboBox::currentIndexChanged, this, &ComboEditAdd::slotChangeIndex,
                     Qt::UniqueConnection);

    QObject::connect(cEdit, &QPushButton::clicked, this, &ComboEditAdd::slotOpen, Qt::UniqueConnection);

    QObject::connect(cPlus, &QPushButton::clicked, this, &ComboEditAdd::slotNew, Qt::UniqueConnection);
}

void ComboEditAdd::slotChangeIndex() { emit signalSelected(cCombo->key); }

void ComboEditAdd::slotOpen() { emit signalEdit(cCombo->key); }

void ComboEditAdd::slotNew() { emit signalAddNew(); }

} // namespace Nutmeg
