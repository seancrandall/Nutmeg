#include "websocketserver.h"

#include <QWebSocketServer>
#include <QWebSocket>
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QDateTime>
#include <QDebug>
#include "objects/matter.h"
#include "objects/appointment.h"
#include "objects/copyrightmatter.h"
#include "objects/deadline.h"
#include "objects/document.h"
#include "objects/enterprise.h"
#include "objects/entity.h"
#include "objects/filing.h"
#include "objects/filingsdashboardentry.h"
#include "objects/flag.h"
#include <QSqlQuery>
#include <QSqlRecord>

namespace Nutmeg {

WebSocketServer::WebSocketServer(quint16 port, QObject *parent)
    : QObject(parent)
    , m_server(new QWebSocketServer(QStringLiteral("NutmegBroker"),
                                    QWebSocketServer::NonSecureMode,
                                    this))
{
    const bool ok = m_server->listen(QHostAddress::Any, port);
    if (!ok) {
        qWarning() << "WebSocketServer failed to listen on port" << port
                   << ":" << m_server->errorString();
    } else {
        qInfo() << "WebSocketServer listening on" << m_server->serverAddress()
                << ":" << m_server->serverPort();
    }

    connect(m_server, &QWebSocketServer::newConnection,
            this, &WebSocketServer::onNewConnection);

    // Register built-in actions on the router
    m_router.registerAction(QStringLiteral("ping"), ActionSpec{
        /*fields*/ {},
        /*handler*/ [](const QJsonObject &){
            DispatchResult r; r.ok = true;
            r.result = QJsonObject{
                {"pong", true},
                {"now", QDateTime::currentDateTimeUtc().toString(Qt::ISODate)}
            };
            return r;
        }
    });

    m_router.registerAction(QStringLiteral("info.get"), ActionSpec{
        /*fields*/ {},
        /*handler*/ [this](const QJsonObject &){
            DispatchResult r; r.ok = true;
            r.result = QJsonObject{
                {"service", QStringLiteral("NutmegBroker")},
                {"apiVersion", protocolVersion()}
            };
            return r;
        }
    });

    // appointment.get: by id
    m_router.registerAction(QStringLiteral("appointment.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{
            FieldSpec{QStringLiteral("id"), QJsonValue::Double, true}
        },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            const AppointmentData a = Nutdb::GetAppointment(id);
            DispatchResult r;
            if (a.AppointmentId == 0) {
                r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Appointment not found");
                return r;
            }
            const Key assoc = Nutdb::GetAppointmentObject(id);
            const QString typeStr = Nutdb::GetAppointmentTypeString(id);
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(a.AppointmentId)},
                {"appointmentTime", a.AppointmentTime.toUTC().toString(Qt::ISODate)},
                {"fkAppointmentType", static_cast<double>(a.fkAppointmentType)},
                {"complete", a.Complete},
                {"needsAgenda", Nutdb::GetFlag(id, QStringLiteral("NeedsAgenda"))},
                {"agendaSent", Nutdb::GetFlag(id, QStringLiteral("AgendaSent"))},
                {"confirmed", Nutdb::GetFlag(id, QStringLiteral("Confirmed"))},
                {"associatedObject", static_cast<double>(assoc)},
                {"typeString", typeStr}
            };
            return r;
        }
    });

    // appointment.update: update fields for an appointment
    m_router.registerAction(QStringLiteral("appointment.update"), ActionSpec{
        /*fields*/ QList<FieldSpec>{
            FieldSpec{QStringLiteral("id"), QJsonValue::Double, true},
            FieldSpec{QStringLiteral("appointmentTime"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("fkAppointmentType"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("complete"), QJsonValue::Bool, false},
            FieldSpec{QStringLiteral("needsAgenda"), QJsonValue::Bool, false},
            FieldSpec{QStringLiteral("agendaSent"), QJsonValue::Bool, false},
            FieldSpec{QStringLiteral("confirmed"), QJsonValue::Bool, false}
        },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            Appointment appt{id};
            DispatchResult r;
            if (appt.getId() == 0) {
                r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Appointment not found");
                return r;
            }

            if (payload.contains(QStringLiteral("appointmentTime"))) {
                const QString s = payload.value(QStringLiteral("appointmentTime")).toString();
                const QDateTime dt = QDateTime::fromString(s, Qt::ISODate);
                if (!dt.isValid()) {
                    r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("Invalid appointmentTime ISO string");
                    return r;
                }
                appt.SetAppointmentTime(dt);
            }
            if (payload.contains(QStringLiteral("fkAppointmentType"))) {
                appt.SetfkAppointmentType(static_cast<Key>(payload.value(QStringLiteral("fkAppointmentType")).toDouble()));
            }
            if (payload.contains(QStringLiteral("complete"))) {
                appt.setComplete(payload.value(QStringLiteral("complete")).toBool());
            }
            if (payload.contains(QStringLiteral("needsAgenda"))) {
                appt.setNeedsAgenda(payload.value(QStringLiteral("needsAgenda")).toBool());
            }
            if (payload.contains(QStringLiteral("agendaSent"))) {
                appt.setAgendaSent(payload.value(QStringLiteral("agendaSent")).toBool());
            }
            if (payload.contains(QStringLiteral("confirmed"))) {
                appt.setConfirmed(payload.value(QStringLiteral("confirmed")).toBool());
            }

            const AppointmentData a = Nutdb::GetAppointment(id);
            const Key assoc = Nutdb::GetAppointmentObject(id);
            const QString typeStr = Nutdb::GetAppointmentTypeString(id);
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(a.AppointmentId)},
                {"appointmentTime", a.AppointmentTime.toUTC().toString(Qt::ISODate)},
                {"fkAppointmentType", static_cast<double>(a.fkAppointmentType)},
                {"complete", a.Complete},
                {"needsAgenda", Nutdb::GetFlag(id, QStringLiteral("NeedsAgenda"))},
                {"agendaSent", Nutdb::GetFlag(id, QStringLiteral("AgendaSent"))},
                {"confirmed", Nutdb::GetFlag(id, QStringLiteral("Confirmed"))},
                {"associatedObject", static_cast<double>(assoc)},
                {"typeString", typeStr}
            };
            return r;
        }
    });

    // copyrightMatter.get: by id
    m_router.registerAction(QStringLiteral("copyrightMatter.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{
            FieldSpec{QStringLiteral("id"), QJsonValue::Double, true}
        },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            const CopyrightMatterData m = Nutdb::GetCopyrightMatter(id);
            DispatchResult r;
            if (m.CopyrightMatterId == 0) {
                r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("CopyrightMatter not found");
                return r;
            }
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(m.CopyrightMatterId)},
                {"fkAuthor", static_cast<double>(m.fkAuthor)},
                {"created", m.Created.toString(Qt::ISODate)},
                {"filed", m.Filed.toString(Qt::ISODate)},
                {"registered", m.Registered.toString(Qt::ISODate)},
                {"serialNumber", m.SerialNumber},
                {"registrationNumber", m.RegistrationNumber},
                {"fkDeposit", static_cast<double>(m.fkDeposit)},
                {"fkWorkType", static_cast<double>(m.fkWorkType)},
                {"isRegistered", Nutdb::GetFlag(id, QStringLiteral("Registered"))}
            };
            return r;
        }
    });

    // copyrightMatter.update: updates provided fields
    m_router.registerAction(QStringLiteral("copyrightMatter.update"), ActionSpec{
        /*fields*/ QList<FieldSpec>{
            FieldSpec{QStringLiteral("id"), QJsonValue::Double, true},
            FieldSpec{QStringLiteral("fkAuthor"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("created"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("filed"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("registered"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("serialNumber"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("registrationNumber"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("fkDeposit"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("fkWorkType"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("isRegistered"), QJsonValue::Bool, false}
        },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            CopyrightMatter cm{id};
            DispatchResult r;
            if (cm.getId() == 0) {
                r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("CopyrightMatter not found");
                return r;
            }

            if (payload.contains("fkAuthor")) cm.slotSetfkAuthor(static_cast<Key>(payload.value("fkAuthor").toDouble()));

            auto parseDate = [](const QJsonObject &obj, const char *key, QDate &out, QString &err) -> bool {
                if (!obj.contains(key)) return true;
                const QString s = obj.value(QString::fromUtf8(key)).toString();
                const QDate d = QDate::fromString(s, Qt::ISODate);
                if (!d.isValid()) { err = QStringLiteral("Invalid date for '%1'").arg(QString::fromUtf8(key)); return false; }
                out = d; return true;
            };

            QString perr;
            QDate d;
            if (!parseDate(payload, "created", d, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; }
            if (payload.contains("created")) cm.slotSetCreated(d);

            if (!parseDate(payload, "filed", d, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; }
            if (payload.contains("filed")) cm.slotSetFiled(d);

            if (!parseDate(payload, "registered", d, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; }
            if (payload.contains("registered")) cm.slotSetRegistered(d);

            if (payload.contains("serialNumber")) cm.slotSetSerialNumber(payload.value("serialNumber").toString());
            if (payload.contains("registrationNumber")) cm.slotSetRegistrationNumber(payload.value("registrationNumber").toString());
            if (payload.contains("fkDeposit")) cm.slotSetfkDeposit(static_cast<Key>(payload.value("fkDeposit").toDouble()));
            if (payload.contains("fkWorkType")) cm.slotSetfkWorkType(static_cast<Key>(payload.value("fkWorkType").toDouble()));
            if (payload.contains("isRegistered")) cm.slotSetIsRegistered(payload.value("isRegistered").toBool());

            const CopyrightMatterData m = Nutdb::GetCopyrightMatter(id);
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(m.CopyrightMatterId)},
                {"fkAuthor", static_cast<double>(m.fkAuthor)},
                {"created", m.Created.toString(Qt::ISODate)},
                {"filed", m.Filed.toString(Qt::ISODate)},
                {"registered", m.Registered.toString(Qt::ISODate)},
                {"serialNumber", m.SerialNumber},
                {"registrationNumber", m.RegistrationNumber},
                {"fkDeposit", static_cast<double>(m.fkDeposit)},
                {"fkWorkType", static_cast<double>(m.fkWorkType)},
                {"isRegistered", Nutdb::GetFlag(id, QStringLiteral("Registered"))}
            };
            return r;
        }
    });

    // deadline.get: by id
    m_router.registerAction(QStringLiteral("deadline.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{
            FieldSpec{QStringLiteral("id"), QJsonValue::Double, true}
        },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            const DeadlineData d = Nutdb::GetDeadline(id);
            DispatchResult r;
            if (d.DeadlineId == 0) {
                r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Deadline not found");
                return r;
            }
            // Compute color from NextDeadline for convenience
            const QColor c = Deadline::getDateColor(d.NextDeadline);
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(d.DeadlineId)},
                {"triggerDate", d.TriggerDate.toString(Qt::ISODate)},
                {"softDeadline", d.SoftDeadline.toString(Qt::ISODate)},
                {"hardDeadline", d.HardDeadline.toString(Qt::ISODate)},
                {"nextDeadline", d.NextDeadline.toString(Qt::ISODate)},
                {"color", c.name(QColor::HexRgb)}
            };
            return r;
        }
    });

    // deadline.update: update any of the date fields
    m_router.registerAction(QStringLiteral("deadline.update"), ActionSpec{
        /*fields*/ QList<FieldSpec>{
            FieldSpec{QStringLiteral("id"), QJsonValue::Double, true},
            FieldSpec{QStringLiteral("triggerDate"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("softDeadline"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("hardDeadline"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("nextDeadline"), QJsonValue::String, false}
        },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            DispatchResult r;
            Deadline dl{id};
            if (dl.getId() == 0) {
                r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Deadline not found");
                return r;
            }

            auto parseDate = [](const QJsonObject &obj, const char *key, QDate &out, QString &err) -> bool {
                if (!obj.contains(key)) return true;
                const QString s = obj.value(QString::fromUtf8(key)).toString();
                const QDate d = QDate::fromString(s, Qt::ISODate);
                if (!d.isValid()) { err = QStringLiteral("Invalid date for '%1'").arg(QString::fromUtf8(key)); return false; }
                out = d; return true;
            };

            QString perr; QDate dt;
            if (!parseDate(payload, "triggerDate", dt, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; }
            if (payload.contains("triggerDate")) dl.SetTriggerDate(dt);

            if (!parseDate(payload, "softDeadline", dt, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; }
            if (payload.contains("softDeadline")) dl.SetSoftDeadline(dt);

            if (!parseDate(payload, "hardDeadline", dt, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; }
            if (payload.contains("hardDeadline")) dl.SetHardDeadline(dt);

            if (!parseDate(payload, "nextDeadline", dt, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; }
            if (payload.contains("nextDeadline")) dl.SetNextDeadline(dt);

            const DeadlineData d = Nutdb::GetDeadline(id);
            const QColor c = Deadline::getDateColor(d.NextDeadline);
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(d.DeadlineId)},
                {"triggerDate", d.TriggerDate.toString(Qt::ISODate)},
                {"softDeadline", d.SoftDeadline.toString(Qt::ISODate)},
                {"hardDeadline", d.HardDeadline.toString(Qt::ISODate)},
                {"nextDeadline", d.NextDeadline.toString(Qt::ISODate)},
                {"color", c.name(QColor::HexRgb)}
            };
            return r;
        }
    });

    // document.get: by id
    m_router.registerAction(QStringLiteral("document.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{
            FieldSpec{QStringLiteral("id"), QJsonValue::Double, true}
        },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            const DocumentData d = Nutdb::GetDocument(id);
            DispatchResult r;
            if (d.DocumentId == 0) {
                r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Document not found");
                return r;
            }
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(d.DocumentId)},
                {"fullyQualifiedPath", d.FullyQualifiedPath},
                {"url", d.URL},
                {"filename", d.Filename},
                {"extension", d.Extension},
                {"title", d.Title}
            };
            return r;
        }
    });

    // document.update: update fields on a document
    m_router.registerAction(QStringLiteral("document.update"), ActionSpec{
        /*fields*/ QList<FieldSpec>{
            FieldSpec{QStringLiteral("id"), QJsonValue::Double, true},
            FieldSpec{QStringLiteral("fullyQualifiedPath"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("url"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("filename"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("extension"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("title"), QJsonValue::String, false}
        },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            Document doc{id};
            DispatchResult r;
            if (doc.getId() == 0) {
                r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Document not found");
                return r;
            }

            if (payload.contains("fullyQualifiedPath")) doc.slotSetFullyQualifiedPath(payload.value("fullyQualifiedPath").toString());
            if (payload.contains("url")) doc.slotSetURL(payload.value("url").toString());
            if (payload.contains("filename"))
                Nutdb::UpdateField(QStringLiteral("document"), QStringLiteral("Filename"), id, payload.value("filename").toString());
            if (payload.contains("extension"))
                Nutdb::UpdateField(QStringLiteral("document"), QStringLiteral("Extension"), id, payload.value("extension").toString());
            if (payload.contains("title"))
                Nutdb::UpdateField(QStringLiteral("document"), QStringLiteral("Title"), id, payload.value("title").toString());

            const DocumentData d = Nutdb::GetDocument(id);
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(d.DocumentId)},
                {"fullyQualifiedPath", d.FullyQualifiedPath},
                {"url", d.URL},
                {"filename", d.Filename},
                {"extension", d.Extension},
                {"title", d.Title}
            };
            return r;
        }
    });

    // enterprise.get: by id
    m_router.registerAction(QStringLiteral("enterprise.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("id"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            const EnterpriseData e = Nutdb::GetEnterprise(id);
            DispatchResult r;
            if (e.EnterpriseId == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Enterprise not found"); return r; }
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(e.EnterpriseId)},
                {"enterpriseName", e.EnterpriseName},
                {"fkBusinessOrganizationType", static_cast<double>(e.fkBusinessOrganizationType)},
                {"fkBusinessJurisdiction", static_cast<double>(e.fkBusinessJurisdiction)},
                {"fkStateOfIncorporation", static_cast<double>(e.fkStateOfIncorporation)},
                {"fkMainContact", static_cast<double>(e.fkMainContact)},
                {"oldOrganizationId", static_cast<double>(e.OldOrganizationId)}
            };
            return r;
        }
    });

    // enterprise.update: update selected fields
    m_router.registerAction(QStringLiteral("enterprise.update"), ActionSpec{
        /*fields*/ QList<FieldSpec>{
            FieldSpec{QStringLiteral("id"), QJsonValue::Double, true},
            FieldSpec{QStringLiteral("enterpriseName"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("fkBusinessJurisdiction"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("fkStateOfIncorporation"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("fkMainContact"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("oldOrganizationId"), QJsonValue::Double, false}
        },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            Enterprise ent{id};
            DispatchResult r;
            if (ent.getId() == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Enterprise not found"); return r; }

            if (payload.contains("enterpriseName")) ent.slotSetEnterpriseName(payload.value("enterpriseName").toString());
            if (payload.contains("fkBusinessJurisdiction")) ent.slotSetfkBusinessJurisdiction(static_cast<Key>(payload.value("fkBusinessJurisdiction").toDouble()));
            if (payload.contains("fkStateOfIncorporation")) ent.slotSetfkStateOfIncorporation(static_cast<Key>(payload.value("fkStateOfIncorporation").toDouble()));
            if (payload.contains("fkMainContact")) ent.slotSetfkMainContact(static_cast<Key>(payload.value("fkMainContact").toDouble()));
            if (payload.contains("oldOrganizationId")) ent.slotSetOldOrganizationId(static_cast<Key>(payload.value("oldOrganizationId").toDouble()));

            const EnterpriseData e = Nutdb::GetEnterprise(id);
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(e.EnterpriseId)},
                {"enterpriseName", e.EnterpriseName},
                {"fkBusinessOrganizationType", static_cast<double>(e.fkBusinessOrganizationType)},
                {"fkBusinessJurisdiction", static_cast<double>(e.fkBusinessJurisdiction)},
                {"fkStateOfIncorporation", static_cast<double>(e.fkStateOfIncorporation)},
                {"fkMainContact", static_cast<double>(e.fkMainContact)},
                {"oldOrganizationId", static_cast<double>(e.OldOrganizationId)}
            };
            return r;
        }
    });

    // entity.get: by id
    m_router.registerAction(QStringLiteral("entity.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("id"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            const EntityData e = Nutdb::GetEntity(id);
            DispatchResult r;
            if (e.EntityId == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Entity not found"); return r; }
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(e.EntityId)},
                {"entityName", e.EntityName},
                {"fullLegalName", e.FullLegalName},
                {"primaryAddress", e.PrimaryAddress},
                {"secondaryAddress", e.SecondaryAddress},
                {"website", e.Website},
                {"primaryPhone", e.PrimaryPhone},
                {"secondaryPhone", e.SecondaryPhone},
                {"faxNumber", e.FaxNumber},
                {"primaryEmail", e.PrimaryEmail},
                {"secondaryEmail", e.SecondaryEmail},
                {"fkState", static_cast<double>(e.fkState)},
                {"fkJurisdiction", static_cast<double>(e.fkJurisdiction)},
                {"docketPrefix", e.DocketPrefix}
            };
            return r;
        }
    });

    // entity.update: selected fields
    m_router.registerAction(QStringLiteral("entity.update"), ActionSpec{
        /*fields*/ QList<FieldSpec>{
            FieldSpec{QStringLiteral("id"), QJsonValue::Double, true},
            FieldSpec{QStringLiteral("entityName"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("fullLegalName"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("primaryAddress"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("secondaryAddress"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("website"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("primaryPhone"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("secondaryPhone"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("faxNumber"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("primaryEmail"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("secondaryEmail"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("fkJurisdiction"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("fkState"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("docketPrefix"), QJsonValue::String, false}
        },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            Entity ent{id};
            DispatchResult r;
            if (ent.getId() == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Entity not found"); return r; }

            if (payload.contains("entityName")) ent.slotSetEntityName(payload.value("entityName").toString());
            if (payload.contains("fullLegalName")) ent.slotSetFullLegalName(payload.value("fullLegalName").toString());
            if (payload.contains("primaryAddress")) ent.slotSetPrimaryAddress(payload.value("primaryAddress").toString());
            if (payload.contains("secondaryAddress")) ent.slotSetSecondaryAddress(payload.value("secondaryAddress").toString());
            if (payload.contains("website")) ent.slotSetWebsite(payload.value("website").toString());
            if (payload.contains("primaryPhone")) ent.slotSetPrimaryPhone(payload.value("primaryPhone").toString());
            if (payload.contains("secondaryPhone")) ent.slotSetSecondaryPhone(payload.value("secondaryPhone").toString());
            if (payload.contains("faxNumber")) ent.slotSetFaxNumber(payload.value("faxNumber").toString());
            if (payload.contains("primaryEmail")) ent.slotSetPrimaryEmail(payload.value("primaryEmail").toString());
            if (payload.contains("secondaryEmail")) ent.slotSetSecondaryEmail(payload.value("secondaryEmail").toString());
            if (payload.contains("fkJurisdiction")) ent.slotSetfkJurisdiction(static_cast<Key>(payload.value("fkJurisdiction").toDouble()));
            if (payload.contains("fkState")) Nutdb::UpdateField(QStringLiteral("entity"), QStringLiteral("fkState"), id, QString::number(static_cast<quint64>(payload.value("fkState").toDouble())));
            if (payload.contains("docketPrefix")) ent.slotSetDocketPrefix(payload.value("docketPrefix").toString());

            const EntityData e = Nutdb::GetEntity(id);
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(e.EntityId)},
                {"entityName", e.EntityName},
                {"fullLegalName", e.FullLegalName},
                {"primaryAddress", e.PrimaryAddress},
                {"secondaryAddress", e.SecondaryAddress},
                {"website", e.Website},
                {"primaryPhone", e.PrimaryPhone},
                {"secondaryPhone", e.SecondaryPhone},
                {"faxNumber", e.FaxNumber},
                {"primaryEmail", e.PrimaryEmail},
                {"secondaryEmail", e.SecondaryEmail},
                {"fkState", static_cast<double>(e.fkState)},
                {"fkJurisdiction", static_cast<double>(e.fkJurisdiction)},
                {"docketPrefix", e.DocketPrefix}
            };
            return r;
        }
    });

    // filing.get: by id
    m_router.registerAction(QStringLiteral("filing.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("id"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            const FilingData f = Nutdb::GetFiling(id);
            DispatchResult r;
            if (f.FilingId == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Filing not found"); return r; }
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(f.FilingId)},
                {"fkFilingStatus", static_cast<double>(f.fkFilingStatus)},
                {"fkAsFiledDocument", static_cast<double>(f.fkAsFiledDocument)}
            };
            return r;
        }
    });

    // filing.update: update fields
    m_router.registerAction(QStringLiteral("filing.update"), ActionSpec{
        /*fields*/ QList<FieldSpec>{
            FieldSpec{QStringLiteral("id"), QJsonValue::Double, true},
            FieldSpec{QStringLiteral("fkFilingStatus"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("fkAsFiledDocument"), QJsonValue::Double, false}
        },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            Filing filing{id};
            DispatchResult r;
            if (filing.getId() == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Filing not found"); return r; }

            if (payload.contains("fkFilingStatus")) filing.setfkFilingStatus(static_cast<Key>(payload.value("fkFilingStatus").toDouble()));
            if (payload.contains("fkAsFiledDocument")) filing.setfkAsFiledDocument(static_cast<Key>(payload.value("fkAsFiledDocument").toDouble()));

            const FilingData f = Nutdb::GetFiling(id);
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(f.FilingId)},
                {"fkFilingStatus", static_cast<double>(f.fkFilingStatus)},
                {"fkAsFiledDocument", static_cast<double>(f.fkAsFiledDocument)}
            };
            return r;
        }
    });

    // filingsDashboardEntry.get: by TaskId (read-only view)
    m_router.registerAction(QStringLiteral("filingsDashboardEntry.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("taskId"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){
            const Key taskId = static_cast<Key>(payload.value(QStringLiteral("taskId")).toDouble());
            QSqlQuery q(QSqlDatabase::database());
            q.prepare(QStringLiteral("SELECT * FROM filingsDashboardComplete WHERE TaskId = :id"));
            q.bindValue(":id", QVariant::fromValue(taskId));
            DispatchResult r;
            if (!q.exec() || !q.next()) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("filingsDashboardEntry not found"); return r; }
            const QSqlRecord rec = q.record();
            r.ok = true;
            r.result = QJsonObject{
                {"taskId", static_cast<double>(rec.field("TaskId").value().toUInt())},
                {"taskClassName", rec.field("TaskClassName").value().toString()},
                {"attorneyDocketNumber", rec.field("AttorneyDocketNumber").value().toString()},
                {"taskName", rec.field("TaskName").value().toString()},
                {"title", rec.field("Title").value().toString()},
                {"triggerDate", rec.field("TriggerDate").value().toDate().toString(Qt::ISODate)},
                {"nextDeadline", rec.field("NextDeadline").value().toDate().toString(Qt::ISODate)},
                {"softDeadline", rec.field("SoftDeadline").value().toDate().toString(Qt::ISODate)},
                {"hardDeadline", rec.field("HardDeadline").value().toDate().toString(Qt::ISODate)},
                {"clientEntityId", static_cast<double>(rec.field("ClientEntityId").value().toUInt())},
                {"clientEntityName", rec.field("ClientEntityName").value().toString()},
                {"paralegalEntityName", rec.field("ParalegalEntityName").value().toString()},
                {"workAttorneyEntityName", rec.field("WorkAttorneyEntityName").value().toString()},
                {"withParalegal", rec.field("WithParalegal").value().toBool()},
                {"needsExaminerInterview", rec.field("NeedsExaminerInterview").value().toBool()},
                {"examinerInterviewScheduled", rec.field("ExaminerInterviewScheduled").value().toBool()}
            };
            return r;
        }
    });

    // flag.get: read a boolean flag by object and camelCase
    m_router.registerAction(QStringLiteral("flag.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{
            FieldSpec{QStringLiteral("objectId"), QJsonValue::Double, true},
            FieldSpec{QStringLiteral("camelCase"), QJsonValue::String, true}
        },
        /*handler*/ [](const QJsonObject &payload){
            const Key objectId = static_cast<Key>(payload.value(QStringLiteral("objectId")).toDouble());
            const QString camel = payload.value(QStringLiteral("camelCase")).toString();
            DispatchResult r; r.ok = true;
            const bool value = Nutdb::GetFlag(objectId, camel);
            r.result = QJsonObject{
                {"objectId", static_cast<double>(objectId)},
                {"camelCase", camel},
                {"value", value}
            };
            return r;
        }
    });

    // flag.set: set/clear a boolean flag by object and camelCase
    m_router.registerAction(QStringLiteral("flag.set"), ActionSpec{
        /*fields*/ QList<FieldSpec>{
            FieldSpec{QStringLiteral("objectId"), QJsonValue::Double, true},
            FieldSpec{QStringLiteral("camelCase"), QJsonValue::String, true},
            FieldSpec{QStringLiteral("value"), QJsonValue::Bool, true}
        },
        /*handler*/ [](const QJsonObject &payload){
            const Key objectId = static_cast<Key>(payload.value(QStringLiteral("objectId")).toDouble());
            const QString camel = payload.value(QStringLiteral("camelCase")).toString();
            const bool value = payload.value(QStringLiteral("value")).toBool();
            DispatchResult r;
            if (camel.isEmpty()) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("camelCase is required"); return r; }
            if (value) Nutdb::SetFlag(objectId, camel); else Nutdb::ClearFlag(objectId, camel);
            r.ok = true;
            r.result = QJsonObject{
                {"objectId", static_cast<double>(objectId)},
                {"camelCase", camel},
                {"value", value}
            };
            return r;
        }
    });

    // matter.get: by id
    m_router.registerAction(QStringLiteral("matter.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{
            FieldSpec{QStringLiteral("id"), QJsonValue::Double, true}
        },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            const MatterData m = Nutdb::GetMatter(id);
            DispatchResult r;
            if (m.MatterId == 0) {
                r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Matter not found");
                return r;
            }
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(m.MatterId)},
                {"fkParent", static_cast<double>(m.fkParent)},
                {"attorneyDocketNumber", m.AttorneyDocketNumber},
                {"clientDocketNumber", m.ClientDocketNumber},
                {"title", m.Title},
                {"fkClient", static_cast<double>(m.fkClient)},
                {"fkAssigningFirm", static_cast<double>(m.fkAssigningFirm)},
                {"fkDefaultWorkAttorney", static_cast<double>(m.fkDefaultWorkAttorney)},
                {"fkDefaultParalegal", static_cast<double>(m.fkDefaultParalegal)},
                {"fkKeyDocument", static_cast<double>(m.fkKeyDocument)},
                {"fkMatterJurisdiction", static_cast<double>(m.fkMatterJurisdiction)},
                {"oldMatterId", static_cast<double>(m.OldMatterId)}
            };
            return r;
        }
    });

    // matter.update: updates provided fields; returns updated record
    m_router.registerAction(QStringLiteral("matter.update"), ActionSpec{
        /*fields*/ QList<FieldSpec>{
            FieldSpec{QStringLiteral("id"), QJsonValue::Double, true},
            FieldSpec{QStringLiteral("fkParent"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("attorneyDocketNumber"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("clientDocketNumber"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("title"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("fkClient"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("fkAssigningFirm"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("fkDefaultWorkAttorney"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("fkDefaultParalegal"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("fkKeyDocument"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("fkMatterJurisdiction"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("oldMatterId"), QJsonValue::Double, false}
        },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            Matter matter{id};
            DispatchResult r;
            if (matter.getId() == 0) {
                r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Matter not found");
                return r;
            }

            // Apply provided fields with explicit type checks
            if (payload.contains("fkParent") && payload.value("fkParent").isDouble())
                matter.SetfkParent(static_cast<Key>(payload.value("fkParent").toDouble()));
            if (payload.contains("attorneyDocketNumber") && payload.value("attorneyDocketNumber").isString())
                matter.SetAttorneyDocketNumber(payload.value("attorneyDocketNumber").toString());
            if (payload.contains("clientDocketNumber") && payload.value("clientDocketNumber").isString())
                matter.SetClientDocketNumber(payload.value("clientDocketNumber").toString());
            if (payload.contains("title") && payload.value("title").isString())
                matter.SetTitle(payload.value("title").toString());
            if (payload.contains("fkClient") && payload.value("fkClient").isDouble())
                matter.SetfkClient(static_cast<Key>(payload.value("fkClient").toDouble()));
            if (payload.contains("fkAssigningFirm") && payload.value("fkAssigningFirm").isDouble())
                matter.SetfkAssigningFirm(static_cast<Key>(payload.value("fkAssigningFirm").toDouble()));
            if (payload.contains("fkDefaultWorkAttorney") && payload.value("fkDefaultWorkAttorney").isDouble())
                matter.SetfkDefaultWorkAttorney(static_cast<Key>(payload.value("fkDefaultWorkAttorney").toDouble()));
            if (payload.contains("fkDefaultParalegal") && payload.value("fkDefaultParalegal").isDouble())
                matter.SetfkDefaultParalegal(static_cast<Key>(payload.value("fkDefaultParalegal").toDouble()));
            if (payload.contains("fkKeyDocument") && payload.value("fkKeyDocument").isDouble())
                matter.SetfkKeyDocument(static_cast<Key>(payload.value("fkKeyDocument").toDouble()));
            if (payload.contains("fkMatterJurisdiction") && payload.value("fkMatterJurisdiction").isDouble())
                matter.SetfkMatterJurisdiction(static_cast<Key>(payload.value("fkMatterJurisdiction").toDouble()));
            if (payload.contains("oldMatterId") && payload.value("oldMatterId").isDouble())
                matter.SetOldId(static_cast<Key>(payload.value("oldMatterId").toDouble()));

            // Return fresh record
            const MatterData m = Nutdb::GetMatter(id);
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(m.MatterId)},
                {"fkParent", static_cast<double>(m.fkParent)},
                {"attorneyDocketNumber", m.AttorneyDocketNumber},
                {"clientDocketNumber", m.ClientDocketNumber},
                {"title", m.Title},
                {"fkClient", static_cast<double>(m.fkClient)},
                {"fkAssigningFirm", static_cast<double>(m.fkAssigningFirm)},
                {"fkDefaultWorkAttorney", static_cast<double>(m.fkDefaultWorkAttorney)},
                {"fkDefaultParalegal", static_cast<double>(m.fkDefaultParalegal)},
                {"fkKeyDocument", static_cast<double>(m.fkKeyDocument)},
                {"fkMatterJurisdiction", static_cast<double>(m.fkMatterJurisdiction)},
                {"oldMatterId", static_cast<double>(m.OldMatterId)}
            };
            return r;
        }
    });
}

WebSocketServer::~WebSocketServer()
{
    for (QWebSocket *sock : m_clients) {
        sock->close();
        sock->deleteLater();
    }
}

bool WebSocketServer::isListening() const
{
    return m_server->isListening();
}

quint16 WebSocketServer::port() const
{
    return static_cast<quint16>(m_server->serverPort());
}

void WebSocketServer::onNewConnection()
{
    QWebSocket *socket = m_server->nextPendingConnection();
    if (!socket)
        return;

    m_clients << socket;

    connect(socket, &QWebSocket::textMessageReceived,
            this, &WebSocketServer::processTextMessage);
    connect(socket, &QWebSocket::disconnected,
            this, &WebSocketServer::socketDisconnected);

    emit clientConnected(socket->peerAddress().toString());
    qInfo() << "WebSocket client connected:" << socket->peerAddress().toString();

    // Protocol v0: send server.hello event
    QJsonObject helloPayload{
        {"service", QStringLiteral("NutmegBroker")},
        {"apiVersion", protocolVersion()}
    };
    sendEvent(socket, QStringLiteral("server.hello"), helloPayload);
}

void WebSocketServer::processTextMessage(const QString &message)
{
    QWebSocket *socket = qobject_cast<QWebSocket*>(sender());
    if (!socket)
        return;

    const QString trimmed = message.trimmed();
    if (trimmed.compare(QStringLiteral("ping"), Qt::CaseInsensitive) == 0) {
        socket->sendTextMessage(QStringLiteral("pong"));
        return;
    }

    // Expect JSON envelope
    QJsonParseError perr{};
    const QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8(), &perr);
    if (perr.error != QJsonParseError::NoError || !doc.isObject()) {
        QJsonObject req; // empty
        sendResponse(socket, makeErr(req, QStringLiteral("EBADREQ"), QStringLiteral("Invalid JSON")));
        return;
    }
    const QJsonObject req = doc.object();
    const QString type = req.value(QStringLiteral("type")).toString();
    if (!(type.compare(QStringLiteral("req"), Qt::CaseInsensitive) == 0)) {
        sendResponse(socket, makeErr(req, QStringLiteral("EBADREQ"), QStringLiteral("Expected type 'req'")));
        return;
    }

    handleAction(socket, req);
}

void WebSocketServer::socketDisconnected()
{
    QWebSocket *socket = qobject_cast<QWebSocket*>(sender());
    if (!socket)
        return;

    m_clients.removeAll(socket);
    emit clientDisconnected();
    qInfo() << "WebSocket client disconnected";
    socket->deleteLater();
}

} // namespace Nutmeg

// ===== Protocol helpers & dispatch =====

namespace Nutmeg {

void WebSocketServer::sendEvent(QWebSocket *socket, const QString &event, const QJsonObject &payload)
{
    QJsonObject evt{
        {"type", QStringLiteral("event")},
        {"event", event},
        {"payload", payload}
    };
    socket->sendTextMessage(QString::fromUtf8(QJsonDocument(evt).toJson(QJsonDocument::Compact)));
}

void WebSocketServer::sendResponse(QWebSocket *socket, const QJsonObject &res)
{
    socket->sendTextMessage(QString::fromUtf8(QJsonDocument(res).toJson(QJsonDocument::Compact)));
}

QJsonObject WebSocketServer::makeOk(const QJsonObject &req, const QJsonObject &result)
{
    QJsonObject res{
        {"type", QStringLiteral("res")},
        {"ok", true},
        {"result", result}
    };
    if (req.contains("id")) res.insert("id", req.value("id"));
    return res;
}

QJsonObject WebSocketServer::makeErr(const QJsonObject &req, const QString &code, const QString &message, const QJsonObject &details)
{
    QJsonObject err{
        {"code", code},
        {"message", message}
    };
    if (!details.isEmpty()) err.insert("details", details);

    QJsonObject res{
        {"type", QStringLiteral("res")},
        {"ok", false},
        {"error", err}
    };
    if (req.contains("id")) res.insert("id", req.value("id"));
    return res;
}

void WebSocketServer::handleAction(QWebSocket *socket, const QJsonObject &req)
{
    const QString action = req.value(QStringLiteral("action")).toString();
    const QJsonObject payload = req.value(QStringLiteral("payload")).toObject();

    if (action.isEmpty()) {
        sendResponse(socket, makeErr(req, QStringLiteral("EBADREQ"), QStringLiteral("Missing action")));
        return;
    }

    // Dispatch via router with validation
    const DispatchResult r = m_router.dispatch(action, payload);
    if (r.ok) {
        sendResponse(socket, makeOk(req, r.result));
    } else {
        sendResponse(socket, makeErr(req, r.errorCode, r.errorMessage, r.errorDetails));
    }
}

} // namespace Nutmeg
