#include "responsesdashboardentry.h"
#include "dbaccess/nutdb.h"

namespace Nutmeg{

responsesDashboardEntry::responsesDashboardEntry(const QSqlRecord &record)
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
Key responsesDashboardEntry::getTaskId() const { return m_TaskId; }
void responsesDashboardEntry::slotSetTaskId(Key v)
{
    Nutdb::UpdateField("responsesDashboardComplete", "TaskId", m_TaskId, QString::number(v));
    m_TaskId = v;
}

// --- TaskClassName ---
const QString& responsesDashboardEntry::getTaskClassName() const { return m_TaskClassName; }
void responsesDashboardEntry::slotSetTaskClassName(const QString& v)
{
    m_TaskClassName = v;
    Nutdb::UpdateField("responsesDashboardComplete", "TaskClassName", m_TaskId, v);
}

// --- AttorneyDocketNumber ---
const QString& responsesDashboardEntry::getAttorneyDocketNumber() const { return m_AttorneyDocketNumber; }
void responsesDashboardEntry::slotSetAttorneyDocketNumber(const QString& v)
{
    m_AttorneyDocketNumber = v;
    Nutdb::UpdateField("responsesDashboardComplete", "AttorneyDocketNumber", m_TaskId, v);
}

// --- TaskName ---
const QString& responsesDashboardEntry::getTaskName() const { return m_TaskName; }
void responsesDashboardEntry::slotSetTaskName(const QString& v)
{
    m_TaskName = v;
    Nutdb::UpdateField("responsesDashboardComplete", "TaskName", m_TaskId, v);
}

// --- Title ---
const QString& responsesDashboardEntry::getTitle() const { return m_Title; }
void responsesDashboardEntry::slotSetTitle(const QString& v)
{
    m_Title = v;
    Nutdb::UpdateField("responsesDashboardComplete", "Title", m_TaskId, v);
}

// --- TriggerDate ---
QDate responsesDashboardEntry::getTriggerDate() const { return m_TriggerDate; }
void responsesDashboardEntry::slotSetTriggerDate(const QDate& v)
{
    m_TriggerDate = v;
    Nutdb::UpdateField("responsesDashboardComplete", "TriggerDate", m_TaskId, v.toString("yyyy-MM-dd"));
}

// --- NextDeadline ---
QDate responsesDashboardEntry::getNextDeadline() const { return m_NextDeadline; }
void responsesDashboardEntry::slotSetNextDeadline(const QDate& v)
{
    m_NextDeadline = v;
    Nutdb::UpdateField("responsesDashboardComplete", "NextDeadline", m_TaskId, v.toString("yyyy-MM-dd"));
}

// --- SoftDeadline ---
QDate responsesDashboardEntry::getSoftDeadline() const { return m_SoftDeadline; }
void responsesDashboardEntry::slotSetSoftDeadline(const QDate& v)
{
    m_SoftDeadline = v;
    Nutdb::UpdateField("responsesDashboardComplete", "SoftDeadline", m_TaskId, v.toString("yyyy-MM-dd"));
}

// --- HardDeadline ---
QDate responsesDashboardEntry::getHardDeadline() const { return m_HardDeadline; }
void responsesDashboardEntry::slotSetHardDeadline(const QDate& v)
{
    m_HardDeadline = v;
    Nutdb::UpdateField("responsesDashboardComplete", "HardDeadline", m_TaskId, v.toString("yyyy-MM-dd"));
}

// --- client_EntityId ---
Key responsesDashboardEntry::getClientEntityId() const { return m_ClientEntityId; }
void responsesDashboardEntry::slotSetClientEntityId(Key v)
{
    m_ClientEntityId = v;
    Nutdb::UpdateField("responsesDashboardComplete", "ClientEntityId", m_TaskId, QString::number(v));
}

// --- client_EntityName ---
const QString& responsesDashboardEntry::getClientEntityName() const { return m_ClientEntityName; }
void responsesDashboardEntry::slotSetClientEntityName(const QString& v)
{
    m_ClientEntityName = v;
    Nutdb::UpdateField("responsesDashboardComplete", "ClientEntityName", m_TaskId, v);
}

// --- paralegal_EntityName ---
const QString& responsesDashboardEntry::getParalegalEntityName() const { return m_ParalegalEntityName; }
void responsesDashboardEntry::slotSetParalegalEntityName(const QString& v)
{
    m_ParalegalEntityName = v;
    Nutdb::UpdateField("responsesDashboardComplete", "ParalegalEntityName", m_TaskId, v);
}

// --- workAttorney_EntityName ---
const QString& responsesDashboardEntry::getWorkAttorneyEntityName() const { return m_WorkAttorneyEntityName; }
void responsesDashboardEntry::slotSetWorkAttorneyEntityName(const QString& v)
{
    m_WorkAttorneyEntityName = v;
    Nutdb::UpdateField("responsesDashboardComplete", "WorkAttorneyEntityName", m_TaskId, v);
}

// --- WithParalegal ---
bool responsesDashboardEntry::getWithParalegal() const { return m_WithParalegal; }
void responsesDashboardEntry::slotSetWithParalegal(bool v)
{
    m_WithParalegal = v;
    Nutdb::UpdateField("responsesDashboardComplete", "WithParalegal", m_TaskId, v ? "1" : "0");
}

// --- NeedsExaminerInterview ---
bool responsesDashboardEntry::getNeedsExaminerInterview() const { return m_NeedsExaminerInterview; }
void responsesDashboardEntry::slotSetNeedsExaminerInterview(bool v)
{
    m_NeedsExaminerInterview = v;
    Nutdb::UpdateField("responsesDashboardComplete", "NeedsExaminerInterview", m_TaskId, v ? "1" : "0");
}

// --- ExaminerInterviewScheduled ---
bool responsesDashboardEntry::getExaminerInterviewScheduled() const { return m_ExaminerInterviewScheduled; }
void responsesDashboardEntry::slotSetExaminerInterviewScheduled(bool v)
{
    m_ExaminerInterviewScheduled = v;
    Nutdb::UpdateField("responsesDashboardComplete", "ExaminerInterviewScheduled", m_TaskId, v ? "1" : "0");
}

} //namespace Nutmeg
