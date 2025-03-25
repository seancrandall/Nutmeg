#include "appointmentpanel.h"

namespace Nutmeg{

Nutmeg::AppointmentPanel::AppointmentPanel(std::shared_ptr<Appointment> appt, QWidget *parent)
    : Frame(parent)
    , mAppointment(appt)
{
    mIsValid = true;
    Initialize();
}

AppointmentPanel::AppointmentPanel(Key apptid, QWidget *parent)
    : Frame(parent)
    , mAppointment(std::make_shared<Appointment>(apptid))
{
    mIsValid = true;
    Initialize();
}

AppointmentPanel::AppointmentPanel(QWidget *parent)
    : Frame(parent)
{
    mIsValid = false;
    Initialize();
}

QDateTime AppointmentPanel::getAppointment()
{
    if(mIsValid)
        return mAppointment->AppointmentTime;
    else return QDateTime();
}

void AppointmentPanel::setAppointment(QDateTime newAppointment)
{
    mAppointment->AppointmentTime = newAppointment;
}

bool AppointmentPanel::getValid()
{
    return mIsValid;
}

void AppointmentPanel::setValid(bool newval)
{
    mIsValid = newval;
}

void AppointmentPanel::setAppointmentObject(std::shared_ptr<Appointment> newobject)
{
    mAppointment = newobject;
}

std::shared_ptr<Appointment> AppointmentPanel::getAppointmentObject()
{
    if(mIsValid)
        return mAppointment;
    else
        return nullptr;
}

void AppointmentPanel::validate()
{
    mIsValid = true;
    cId->setEnabled(true);
    cDateTime->setEnabled(true);
    cAppointmentType->setEnabled(true);
    plus->hide();
    Scatter();
}

void AppointmentPanel::invalidate()
{
    valid = false;
    cId->setEnabled(false);
    cDateTime->setEnabled(false);
    cAppointmentType->setEnabled(false);
    plus->show();
}

void AppointmentPanel::Scatter()
{
    //If not a valid object, nothing to do
    if(!mIsValid) return;

    cId->id = mAppointment->AppointmentId;
    cDateTime->dateTime = mAppointment->AppointmentTime;
    cAppointmentType->key = mAppointment->fkAppointmentType;
}

void AppointmentPanel::Gather()
{
    //If not a valid object, nothign to do
    if(!mIsValid) return;

    mAppointment->holdAppointmentTime(cDateTime->dateTime);
    mAppointment->holdfkAppointmentType(cAppointmentType->key);
    mAppointment->commit();
}

void AppointmentPanel::Initialize()
{
    //typeModel = new appointmentTypeModel(this);
    if(!gAppointmentTypeModel)
        gAppointmentTypeModel = std::make_unique<appointmentTypeModel>();

    //Setup Widgets
    cId = new LineDisplayId();
    cDateTime = new DateTimeEdit(mAppointment->AppointmentTime);
    cAppointmentType = new ComboBox();
    cAppointmentType->setModel(gAppointmentTypeModel.get());
    plus = new PlusButton();

    lId = new LabeledWidgetLeft("Appointment ID", cId);
    lDateTime = new LabeledWidgetLeft("Appointment Time", cDateTime);
    lAppointmentType = new LabeledWidgetLeft("Appointment Type", cAppointmentType);

    grid = new QGridLayout(this);
    grid->addLayout(lId, 0, 0);
    grid->addLayout(lAppointmentType, 0, 1);
    grid->addLayout(lDateTime, 1, 0);
    grid->addWidget(plus, 1, 1);

    setLayout(grid);

    //If invalid, disable controls
    if(mIsValid)
        validate();
    else
        invalidate();

    Scatter();
}

void AppointmentPanel::ConnectSignalsAndSlots()
{
    QObject::connect(plus,      &QPushButton::clicked,
                     this,      &Nutmeg::AppointmentPanel::slotCreateNewAppointment);
}

void AppointmentPanel::slotCreateNewAppointment()
{
    Key newapp;
    newapp = Nutdb::InsertAppointment(QDateTime::currentDateTime());

    if(newapp)
    {
        validate();
        emit signalAppointmentCreated(newapp);
    }
}


} //namespace Nutmeg
