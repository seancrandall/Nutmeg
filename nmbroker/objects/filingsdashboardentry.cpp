#include "filingsdashboardentry.h"
#include "dbaccess/nutdb.h"

namespace Nutmeg{

filingsDashboardEntry::filingsDashboardEntry(const QSqlRecord &record)
{
    m_TaskId = record.value("TaskId").toUInt();
    m_TaskClassName = record.value("TaskClassName").toString();
    m_AttorneyDocketNumber = record.value("AttorneyDocketNumber").toString();
    m_TaskName = record.value("TaskName").toString();
    m_Title = record.value("Title").toString();
    m_TriggerDate = record.value("TriggerDate").toDate();
    m_NextDeadline = record.value("NextDeadline").toDate();
    m_SoftDeadline = record.value("SoftDeadline").toDate();
    m_HardDeadline = record.value("HardDeadline").toDate();
    m_ClientEntityId = record.value("ClientEntityId").toUInt();
    m_ClientEntityName = record.value("ClientEntityName").toString();
    m_ParalegalEntityName = record.value("ParalegalEntityName").toString();
    m_WorkAttorneyEntityName = record.value("WorkAttorneyEntityName").toString();
    m_WithParalegal = record.value("WithParalegal").toBool();
    m_NeedsExaminerInterview = record.value("NeedsExaminerInterview").toBool();
    m_ExaminerInterviewScheduled = record.value("ExaminerInterviewScheduled").toBool();
}

// --- TaskId ---
Key filingsDashboardEntry::getTaskId() const { return m_TaskId; }
void filingsDashboardEntry::slotSetTaskId(Key v)
{
    Nutdb::UpdateField("filingsDashboardComplete", "TaskId", m_TaskId, QString::number(v));
    m_TaskId = v;
}

// --- TaskClassName ---
const QString& filingsDashboardEntry::getTaskClassName() const { return m_TaskClassName; }
void filingsDashboardEntry::slotSetTaskClassName(const QString& v)
{
    m_TaskClassName = v;
    Nutdb::UpdateField("filingsDashboardComplete", "TaskClassName", m_TaskId, v);
}

// --- AttorneyDocketNumber ---
const QString& filingsDashboardEntry::getAttorneyDocketNumber() const { return m_AttorneyDocketNumber; }
void filingsDashboardEntry::slotSetAttorneyDocketNumber(const QString& v)
{
    m_AttorneyDocketNumber = v;
    Nutdb::UpdateField("filingsDashboardComplete", "AttorneyDocketNumber", m_TaskId, v);
}

// --- TaskName ---
const QString& filingsDashboardEntry::getTaskName() const { return m_TaskName; }
void filingsDashboardEntry::slotSetTaskName(const QString& v)
{
    m_TaskName = v;
    Nutdb::UpdateField("filingsDashboardComplete", "TaskName", m_TaskId, v);
}

// --- Title ---
const QString& filingsDashboardEntry::getTitle() const { return m_Title; }
void filingsDashboardEntry::slotSetTitle(const QString& v)
{
    m_Title = v;
    Nutdb::UpdateField("filingsDashboardComplete", "Title", m_TaskId, v);
}

// --- TriggerDate ---
QDate filingsDashboardEntry::getTriggerDate() const { return m_TriggerDate; }
void filingsDashboardEntry::slotSetTriggerDate(const QDate& v)
{
    m_TriggerDate = v;
    Nutdb::UpdateField("filingsDashboardComplete", "TriggerDate", m_TaskId, v.toString("yyyy-MM-dd"));
}

// --- NextDeadline ---
QDate filingsDashboardEntry::getNextDeadline() const { return m_NextDeadline; }
void filingsDashboardEntry::slotSetNextDeadline(const QDate& v)
{
    m_NextDeadline = v;
    Nutdb::UpdateField("filingsDashboardComplete", "NextDeadline", m_TaskId, v.toString("yyyy-MM-dd"));
}

// --- SoftDeadline ---
QDate filingsDashboardEntry::getSoftDeadline() const { return m_SoftDeadline; }
void filingsDashboardEntry::slotSetSoftDeadline(const QDate& v)
{
    m_SoftDeadline = v;
    Nutdb::UpdateField("filingsDashboardComplete", "SoftDeadline", m_TaskId, v.toString("yyyy-MM-dd"));
}

// --- HardDeadline ---
QDate filingsDashboardEntry::getHardDeadline() const { return m_HardDeadline; }
void filingsDashboardEntry::slotSetHardDeadline(const QDate& v)
{
    m_HardDeadline = v;
    Nutdb::UpdateField("filingsDashboardComplete", "HardDeadline", m_TaskId, v.toString("yyyy-MM-dd"));
}

// --- ClientEntityId ---
Key filingsDashboardEntry::getClientEntityId() const { return m_ClientEntityId; }
void filingsDashboardEntry::slotSetClientEntityId(Key v)
{
    m_ClientEntityId = v;
    Nutdb::UpdateField("filingsDashboardComplete", "ClientEntityId", m_TaskId, QString::number(v));
}

// --- ClientEntityName ---
const QString& filingsDashboardEntry::getClientEntityName() const { return m_ClientEntityName; }
void filingsDashboardEntry::slotSetClientEntityName(const QString& v)
{
    m_ClientEntityName = v;
    Nutdb::UpdateField("filingsDashboardComplete", "ClientEntityName", m_TaskId, v);
}

// --- ParalegalEntityName ---
const QString& filingsDashboardEntry::getParalegalEntityName() const { return m_ParalegalEntityName; }
void filingsDashboardEntry::slotSetParalegalEntityName(const QString& v)
{
    m_ParalegalEntityName = v;
    Nutdb::UpdateField("filingsDashboardComplete", "ParalegalEntityName", m_TaskId, v);
}

// --- WorkAttorneyEntityName ---
const QString& filingsDashboardEntry::getWorkAttorneyEntityName() const { return m_WorkAttorneyEntityName; }
void filingsDashboardEntry::slotSetWorkAttorneyEntityName(const QString& v)
{
    m_WorkAttorneyEntityName = v;
    Nutdb::UpdateField("filingsDashboardComplete", "WorkAttorneyEntityName", m_TaskId, v);
}

// --- WithParalegal ---
bool filingsDashboardEntry::getWithParalegal() const { return m_WithParalegal; }
void filingsDashboardEntry::slotSetWithParalegal(bool v)
{
    m_WithParalegal = v;
    Nutdb::UpdateField("filingsDashboardComplete", "WithParalegal", m_TaskId, v ? "1" : "0");
}

// --- NeedsExaminerInterview ---
bool filingsDashboardEntry::getNeedsExaminerInterview() const { return m_NeedsExaminerInterview; }
void filingsDashboardEntry::slotSetNeedsExaminerInterview(bool v)
{
    m_NeedsExaminerInterview = v;
    Nutdb::UpdateField("filingsDashboardComplete", "NeedsExaminerInterview", m_TaskId, v ? "1" : "0");
}

// --- ExaminerInterviewScheduled ---
bool filingsDashboardEntry::getExaminerInterviewScheduled() const { return m_ExaminerInterviewScheduled; }
void filingsDashboardEntry::slotSetExaminerInterviewScheduled(bool v)
{
    m_ExaminerInterviewScheduled = v;
    Nutdb::UpdateField("filingsDashboardComplete", "ExaminerInterviewScheduled", m_TaskId, v ? "1" : "0");
}

} //namespace Nutmeg
