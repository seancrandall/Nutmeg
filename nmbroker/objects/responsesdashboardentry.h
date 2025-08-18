#pragma once
/**
 * responsesDashboardEntry
 * Auto-generated data holder for rows from view `responsesDashboardComplete`.
 *
 * Each field from the view has a getter, a setter (named slotSetX), and a
 * Visual C++-style property via the `Property(get,set)` macro declared in
 * your property.h.
 *
 * Types:
 *  - Key: 32-bit unsigned id (adjust typedef below if you already define Key elsewhere)
 *  - QString for text
 *  - QDate for DATE columns
 *  - bool for 0/1 flags
 */

#include "nutmeg.h"
#include "property.h"
#include <QString>
#include <QDate>
#include <cstdint>

namespace Nutmeg{

class responsesDashboardEntry {
public:
    responsesDashboardEntry() = default;
    ~responsesDashboardEntry() = default;
    responsesDashboardEntry(const responsesDashboardEntry&) = default;
    responsesDashboardEntry& operator=(const responsesDashboardEntry&) = default;
    responsesDashboardEntry(responsesDashboardEntry&&) noexcept = default;
    responsesDashboardEntry& operator=(responsesDashboardEntry&&) noexcept = default;

    // --- TaskId ---
    Key getTaskId() const;
    void slotSetTaskId(Key v);
    Property(getTaskId, slotSetTaskId) Key TaskId;

    // --- TaskClassName ---
    const QString& getTaskClassName() const;
    void slotSetTaskClassName(const QString& v);
    Property(getTaskClassName, slotSetTaskClassName) QString TaskClassName;

    // --- AttorneyDocketNumber ---
    const QString& getAttorneyDocketNumber() const;
    void slotSetAttorneyDocketNumber(const QString& v);
    Property(getAttorneyDocketNumber, slotSetAttorneyDocketNumber) QString AttorneyDocketNumber;

    // --- TaskName ---
    const QString& getTaskName() const;
    void slotSetTaskName(const QString& v);
    Property(getTaskName, slotSetTaskName) QString TaskName;

    // --- Title ---
    const QString& getTitle() const;
    void slotSetTitle(const QString& v);
    Property(getTitle, slotSetTitle) QString Title;

    // --- TriggerDate ---
    QDate getTriggerDate() const;
    void slotSetTriggerDate(const QDate& v);
    Property(getTriggerDate, slotSetTriggerDate) QDate TriggerDate;

    // --- NextDeadline ---
    QDate getNextDeadline() const;
    void slotSetNextDeadline(const QDate& v);
    Property(getNextDeadline, slotSetNextDeadline) QDate NextDeadline;

    // --- SoftDeadline ---
    QDate getSoftDeadline() const;
    void slotSetSoftDeadline(const QDate& v);
    Property(getSoftDeadline, slotSetSoftDeadline) QDate SoftDeadline;

    // --- HardDeadline ---
    QDate getHardDeadline() const;
    void slotSetHardDeadline(const QDate& v);
    Property(getHardDeadline, slotSetHardDeadline) QDate HardDeadline;

    // --- ClientEntityId ---
    Key getClientEntityId() const;
    void slotSetClientEntityId(Key v);
    Property(getClientEntityId, slotSetClientEntityId) Key ClientEntityId;

    // --- ClientEntityName ---
    const QString& getClientEntityName() const;
    void slotSetClientEntityName(const QString& v);
    Property(getClientEntityName, slotSetClientEntityName) QString ClientEntityName;

    // --- ParalegalEntityName ---
    const QString& getParalegalEntityName() const;
    void slotSetParalegalEntityName(const QString& v);
    Property(getParalegalEntityName, slotSetParalegalEntityName) QString ParalegalEntityName;

    // --- WorkAttorneyEntityName ---
    const QString& getWorkAttorneyEntityName() const;
    void slotSetWorkAttorneyEntityName(const QString& v);
    Property(getWorkAttorneyEntityName, slotSetWorkAttorneyEntityName) QString WorkAttorneyEntityName;

    // --- WithParalegal ---
    bool getWithParalegal() const;
    void slotSetWithParalegal(bool v);
    Property(getWithParalegal, slotSetWithParalegal) bool WithParalegal;

    // --- NeedsExaminerInterview ---
    bool getNeedsExaminerInterview() const;
    void slotSetNeedsExaminerInterview(bool v);
    Property(getNeedsExaminerInterview, slotSetNeedsExaminerInterview) bool NeedsExaminerInterview;

    // --- ExaminerInterviewScheduled ---
    bool getExaminerInterviewScheduled() const;
    void slotSetExaminerInterviewScheduled(bool v);
    Property(getExaminerInterviewScheduled, slotSetExaminerInterviewScheduled) bool ExaminerInterviewScheduled;

private:
    Key m_TaskId {0};
    QString m_TaskClassName;
    QString m_AttorneyDocketNumber;
    QString m_TaskName;
    QString m_Title;
    QDate m_TriggerDate;
    QDate m_NextDeadline;
    QDate m_SoftDeadline;
    QDate m_HardDeadline;
    Key m_ClientEntityId {0};
    QString m_ClientEntityName;
    QString m_ParalegalEntityName;
    QString m_WorkAttorneyEntityName;
    bool m_WithParalegal {false};
    bool m_NeedsExaminerInterview {false};
    bool m_ExaminerInterviewScheduled {false};
};

} //namespace Nutmeg
