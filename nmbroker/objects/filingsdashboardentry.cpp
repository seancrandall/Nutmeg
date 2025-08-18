#include "filingsDashboardEntry.h"

// --- TaskId ---
Key filingsDashboardEntry::getTaskId() const { return m_TaskId; }
void filingsDashboardEntry::slotSetTaskId(Key v) { m_TaskId = v; }

// --- TaskClassName ---
const QString& filingsDashboardEntry::getTaskClassName() const { return m_TaskClassName; }
void filingsDashboardEntry::slotSetTaskClassName(const QString& v) { m_TaskClassName = v; }

// --- AttorneyDocketNumber ---
const QString& filingsDashboardEntry::getAttorneyDocketNumber() const { return m_AttorneyDocketNumber; }
void filingsDashboardEntry::slotSetAttorneyDocketNumber(const QString& v) { m_AttorneyDocketNumber = v; }

// --- TaskName ---
const QString& filingsDashboardEntry::getTaskName() const { return m_TaskName; }
void filingsDashboardEntry::slotSetTaskName(const QString& v) { m_TaskName = v; }

// --- Title ---
const QString& filingsDashboardEntry::getTitle() const { return m_Title; }
void filingsDashboardEntry::slotSetTitle(const QString& v) { m_Title = v; }

// --- TriggerDate ---
QDate filingsDashboardEntry::getTriggerDate() const { return m_TriggerDate; }
void filingsDashboardEntry::slotSetTriggerDate(const QDate& v) { m_TriggerDate = v; }

// --- NextDeadline ---
QDate filingsDashboardEntry::getNextDeadline() const { return m_NextDeadline; }
void filingsDashboardEntry::slotSetNextDeadline(const QDate& v) { m_NextDeadline = v; }

// --- SoftDeadline ---
QDate filingsDashboardEntry::getSoftDeadline() const { return m_SoftDeadline; }
void filingsDashboardEntry::slotSetSoftDeadline(const QDate& v) { m_SoftDeadline = v; }

// --- HardDeadline ---
QDate filingsDashboardEntry::getHardDeadline() const { return m_HardDeadline; }
void filingsDashboardEntry::slotSetHardDeadline(const QDate& v) { m_HardDeadline = v; }

// --- ClientEntityId ---
Key filingsDashboardEntry::getClientEntityId() const { return m_ClientEntityId; }
void filingsDashboardEntry::slotSetClientEntityId(Key v) { m_ClientEntityId = v; }

// --- ClientEntityName ---
const QString& filingsDashboardEntry::getClientEntityName() const { return m_ClientEntityName; }
void filingsDashboardEntry::slotSetClientEntityName(const QString& v) { m_ClientEntityName = v; }

// --- ParalegalEntityName ---
const QString& filingsDashboardEntry::getParalegalEntityName() const { return m_ParalegalEntityName; }
void filingsDashboardEntry::slotSetParalegalEntityName(const QString& v) { m_ParalegalEntityName = v; }

// --- WorkAttorneyEntityName ---
const QString& filingsDashboardEntry::getWorkAttorneyEntityName() const { return m_WorkAttorneyEntityName; }
void filingsDashboardEntry::slotSetWorkAttorneyEntityName(const QString& v) { m_WorkAttorneyEntityName = v; }

// --- WithParalegal ---
bool filingsDashboardEntry::getWithParalegal() const { return m_WithParalegal; }
void filingsDashboardEntry::slotSetWithParalegal(bool v) { m_WithParalegal = v; }

// --- NeedsExaminerInterview ---
bool filingsDashboardEntry::getNeedsExaminerInterview() const { return m_NeedsExaminerInterview; }
void filingsDashboardEntry::slotSetNeedsExaminerInterview(bool v) { m_NeedsExaminerInterview = v; }

// --- ExaminerInterviewScheduled ---
bool filingsDashboardEntry::getExaminerInterviewScheduled() const { return m_ExaminerInterviewScheduled; }
void filingsDashboardEntry::slotSetExaminerInterviewScheduled(bool v) { m_ExaminerInterviewScheduled = v; }

