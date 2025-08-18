#include "responsesDashboardEntry.h"

// --- TaskId ---
Key responsesDashboardEntry::getTaskId() const { return m_TaskId; }
void responsesDashboardEntry::slotSetTaskId(Key v) { m_TaskId = v; }

// --- TaskClassName ---
const QString& responsesDashboardEntry::getTaskClassName() const { return m_TaskClassName; }
void responsesDashboardEntry::slotSetTaskClassName(const QString& v) { m_TaskClassName = v; }

// --- AttorneyDocketNumber ---
const QString& responsesDashboardEntry::getAttorneyDocketNumber() const { return m_AttorneyDocketNumber; }
void responsesDashboardEntry::slotSetAttorneyDocketNumber(const QString& v) { m_AttorneyDocketNumber = v; }

// --- TaskName ---
const QString& responsesDashboardEntry::getTaskName() const { return m_TaskName; }
void responsesDashboardEntry::slotSetTaskName(const QString& v) { m_TaskName = v; }

// --- Title ---
const QString& responsesDashboardEntry::getTitle() const { return m_Title; }
void responsesDashboardEntry::slotSetTitle(const QString& v) { m_Title = v; }

// --- TriggerDate ---
QDate responsesDashboardEntry::getTriggerDate() const { return m_TriggerDate; }
void responsesDashboardEntry::slotSetTriggerDate(const QDate& v) { m_TriggerDate = v; }

// --- NextDeadline ---
QDate responsesDashboardEntry::getNextDeadline() const { return m_NextDeadline; }
void responsesDashboardEntry::slotSetNextDeadline(const QDate& v) { m_NextDeadline = v; }

// --- SoftDeadline ---
QDate responsesDashboardEntry::getSoftDeadline() const { return m_SoftDeadline; }
void responsesDashboardEntry::slotSetSoftDeadline(const QDate& v) { m_SoftDeadline = v; }

// --- HardDeadline ---
QDate responsesDashboardEntry::getHardDeadline() const { return m_HardDeadline; }
void responsesDashboardEntry::slotSetHardDeadline(const QDate& v) { m_HardDeadline = v; }

// --- client_EntityId ---
Key responsesDashboardEntry::getclient_EntityId() const { return m_client_EntityId; }
void responsesDashboardEntry::slotSetclient_EntityId(Key v) { m_client_EntityId = v; }

// --- client_EntityName ---
const QString& responsesDashboardEntry::getclient_EntityName() const { return m_client_EntityName; }
void responsesDashboardEntry::slotSetclient_EntityName(const QString& v) { m_client_EntityName = v; }

// --- paralegal_EntityName ---
const QString& responsesDashboardEntry::getparalegal_EntityName() const { return m_paralegal_EntityName; }
void responsesDashboardEntry::slotSetparalegal_EntityName(const QString& v) { m_paralegal_EntityName = v; }

// --- workAttorney_EntityName ---
const QString& responsesDashboardEntry::getworkAttorney_EntityName() const { return m_workAttorney_EntityName; }
void responsesDashboardEntry::slotSetworkAttorney_EntityName(const QString& v) { m_workAttorney_EntityName = v; }

// --- WithParalegal ---
bool responsesDashboardEntry::getWithParalegal() const { return m_WithParalegal; }
void responsesDashboardEntry::slotSetWithParalegal(bool v) { m_WithParalegal = v; }

// --- NeedsExaminerInterview ---
bool responsesDashboardEntry::getNeedsExaminerInterview() const { return m_NeedsExaminerInterview; }
void responsesDashboardEntry::slotSetNeedsExaminerInterview(bool v) { m_NeedsExaminerInterview = v; }

// --- ExaminerInterviewScheduled ---
bool responsesDashboardEntry::getExaminerInterviewScheduled() const { return m_ExaminerInterviewScheduled; }
void responsesDashboardEntry::slotSetExaminerInterviewScheduled(bool v) { m_ExaminerInterviewScheduled = v; }

