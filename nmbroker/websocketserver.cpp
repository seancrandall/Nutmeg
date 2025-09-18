#include "websocketserver.h"

#include <QWebSocketServer>
#include <QWebSocket>
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
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
#include "objects/flagclass.h"
#include "objects/object.h"
#include "objects/person.h"
#include "objects/response.h"
#include "objects/tag.h"
#include "objects/task.h"
#include "objects/trademarkmatter.h"
#include "objects/patentmatter.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

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

    // appointmentType.get: by id (read-only fetch)
    m_router.registerAction(QStringLiteral("appointmentType.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("id"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            QSqlQuery q(QSqlDatabase::database());
            q.prepare(QStringLiteral("SELECT AppointmentTypeId, AppointmentTypeName FROM appointmentType WHERE AppointmentTypeId = :id"));
            q.bindValue(":id", QVariant::fromValue(id));
            DispatchResult r;
            if (!q.exec() || !q.next()) {
                r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("AppointmentType not found");
                return r;
            }
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(q.value(0).toUInt())},
                {"appointmentTypeName", q.value(1).toString()}
            };
            return r;
        }
    });

    // appointmentType.list: list all rows (read-only)
    m_router.registerAction(QStringLiteral("appointmentType.list"), ActionSpec{
        /*fields*/ QList<FieldSpec>{},
        /*handler*/ [](const QJsonObject &){
            QSqlQuery q(QSqlDatabase::database());
            DispatchResult r; r.ok = true;
            QJsonArray items;
            if (q.exec(QStringLiteral("SELECT AppointmentTypeId, AppointmentTypeName FROM appointmentType ORDER BY AppointmentTypeName"))) {
                while (q.next()) {
                    items.append(QJsonObject{
                        {"id", static_cast<double>(q.value(0).toUInt())},
                        {"appointmentTypeName", q.value(1).toString()}
                    });
                }
            }
            r.result = QJsonObject{{"items", items}};
            return r;
        }
    });

    // appointmentType.create: insert a new row (write)
    m_router.registerAction(QStringLiteral("appointmentType.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("appointmentTypeName"), QJsonValue::String, true} },
        /*handler*/ [](const QJsonObject &payload){
            const QString name = payload.value(QStringLiteral("appointmentTypeName")).toString();
            DispatchResult r;
            if (name.trimmed().isEmpty()) {
                r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("appointmentTypeName is required");
                return r;
            }
            QSqlQuery q(QSqlDatabase::database());
            q.prepare(QStringLiteral("INSERT INTO appointmentType (AppointmentTypeName) VALUES (:name)"));
            q.bindValue(":name", name);
            if (!q.exec()) {
                r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = q.lastError().text();
                return r;
            }
            const QVariant newIdVar = q.lastInsertId();
            Key newId = 0;
            if (newIdVar.isValid()) newId = newIdVar.toUInt();
            // Fallback: select last row by name if driver didn't return lastInsertId
            if (newId == 0) {
                QSqlQuery q2(QSqlDatabase::database());
                q2.prepare(QStringLiteral("SELECT AppointmentTypeId FROM appointmentType WHERE AppointmentTypeName = :name ORDER BY AppointmentTypeId DESC LIMIT 1"));
                q2.bindValue(":name", name);
                if (q2.exec() && q2.next()) newId = q2.value(0).toUInt();
            }
            // Return created row
            QSqlQuery qr(QSqlDatabase::database());
            qr.prepare(QStringLiteral("SELECT AppointmentTypeId, AppointmentTypeName FROM appointmentType WHERE AppointmentTypeId = :id"));
            qr.bindValue(":id", QVariant::fromValue(newId));
            if (!qr.exec() || !qr.next()) {
                r.ok = true; // created, but failed to re-read
                r.result = QJsonObject{{"id", static_cast<double>(newId)}, {"appointmentTypeName", name}};
                return r;
            }
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(qr.value(0).toUInt())},
                {"appointmentTypeName", qr.value(1).toString()}
            };
            return r;
        }
    });

    // appointmentType.update: update a row (write)
    m_router.registerAction(QStringLiteral("appointmentType.update"), ActionSpec{
        /*fields*/ QList<FieldSpec>{
            FieldSpec{QStringLiteral("id"), QJsonValue::Double, true},
            FieldSpec{QStringLiteral("appointmentTypeName"), QJsonValue::String, false}
        },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            bool any = false;
            QSqlQuery q(QSqlDatabase::database());
            if (payload.contains(QStringLiteral("appointmentTypeName"))) {
                any = true;
                QSqlQuery qu(QSqlDatabase::database());
                qu.prepare(QStringLiteral("UPDATE appointmentType SET AppointmentTypeName = :name WHERE AppointmentTypeId = :id"));
                qu.bindValue(":name", payload.value(QStringLiteral("appointmentTypeName")).toString());
                qu.bindValue(":id", QVariant::fromValue(id));
                if (!qu.exec()) {
                    DispatchResult r; r.ok = false; r.errorCode = QStringLiteral("EUPDATE"); r.errorMessage = qu.lastError().text(); return r;
                }
            }
            DispatchResult r;
            if (!any) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("No updatable fields provided"); return r; }
            // Return updated row
            QSqlQuery qr(QSqlDatabase::database());
            qr.prepare(QStringLiteral("SELECT AppointmentTypeId, AppointmentTypeName FROM appointmentType WHERE AppointmentTypeId = :id"));
            qr.bindValue(":id", QVariant::fromValue(id));
            if (!qr.exec() || !qr.next()) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("AppointmentType not found"); return r; }
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(qr.value(0).toUInt())},
                {"appointmentTypeName", qr.value(1).toString()}
            };
            return r;
        }
    });

    // appointmentType.delete: delete a row (write)
    m_router.registerAction(QStringLiteral("appointmentType.delete"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("id"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            QSqlQuery q(QSqlDatabase::database());
            q.prepare(QStringLiteral("DELETE FROM appointmentType WHERE AppointmentTypeId = :id"));
            q.bindValue(":id", QVariant::fromValue(id));
            DispatchResult r;
            if (!q.exec()) { r.ok = false; r.errorCode = QStringLiteral("EDELETE"); r.errorMessage = q.lastError().text(); return r; }
            r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(id)}, {"deleted", true}}; return r;
        }
    });

    // appointmentObject.get (read-only via viewAppointmentObjects)
    m_router.registerAction(QStringLiteral("appointmentObject.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("appointmentId"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){
            const Key apptId = static_cast<Key>(payload.value(QStringLiteral("appointmentId")).toDouble());
            QSqlQuery q(QSqlDatabase::database());
            q.prepare(QStringLiteral("SELECT AppointmentId, ObjectId, AppointmentTypeName FROM viewAppointmentObjects WHERE AppointmentId = :id"));
            q.bindValue(":id", QVariant::fromValue(apptId));
            DispatchResult r;
            if (!q.exec() || !q.next()) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Appointment mapping not found"); return r; }
            const QSqlRecord rec = q.record();
            r.ok = true;
            r.result = QJsonObject{
                {"appointmentId", static_cast<double>(rec.field("AppointmentId").value().toUInt())},
                {"objectId", static_cast<double>(rec.field("ObjectId").value().toUInt())},
                {"appointmentTypeName", rec.field("AppointmentTypeName").isValid() ? rec.field("AppointmentTypeName").value().toString() : QString()}
            };
            return r;
        }
    });

    // appointmentObject.listForObject (read-only): list appointments for an object
    m_router.registerAction(QStringLiteral("appointmentObject.listForObject"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("objectId"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){
            const Key objectId = static_cast<Key>(payload.value(QStringLiteral("objectId")).toDouble());
            QSqlQuery q(QSqlDatabase::database());
            q.prepare(QStringLiteral("SELECT AppointmentId, ObjectId, AppointmentTypeName FROM viewAppointmentObjects WHERE ObjectId = :oid ORDER BY AppointmentId"));
            q.bindValue(":oid", QVariant::fromValue(objectId));
            DispatchResult r; r.ok = true;
            QJsonArray items;
            if (q.exec()) {
                while (q.next()) {
                    items.append(QJsonObject{
                        {"appointmentId", static_cast<double>(q.value(0).toUInt())},
                        {"objectId", static_cast<double>(q.value(1).toUInt())},
                        {"appointmentTypeName", q.value(2).toString()}
                    });
                }
            }
            r.result = QJsonObject{{"objectId", static_cast<double>(objectId)}, {"items", items}};
            return r;
        }
    });

    // client.get (read-only via viewClients), tolerant to differing id column names
    m_router.registerAction(QStringLiteral("client.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("id"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            auto packRecord = [](const QSqlRecord &rec){
                QJsonObject obj;
                for (int i = 0; i < rec.count(); ++i) {
                    const QString name = rec.fieldName(i);
                    const QVariant v = rec.value(i);
                    switch (v.typeId()) {
                        case QMetaType::Bool: obj.insert(name, v.toBool()); break;
                        case QMetaType::Int:
                        case QMetaType::UInt:
                        case QMetaType::LongLong:
                        case QMetaType::ULongLong:
                        case QMetaType::Double: obj.insert(name, v.toDouble()); break;
                        case QMetaType::QDate: obj.insert(name, v.toDate().toString(Qt::ISODate)); break;
                        case QMetaType::QDateTime: obj.insert(name, v.toDateTime().toString(Qt::ISODate)); break;
                        default: obj.insert(name, v.toString()); break;
                    }
                }
                return obj;
            };

            DispatchResult r;
            QSqlQuery q(QSqlDatabase::database());
            q.prepare(QStringLiteral("SELECT * FROM viewClients WHERE ClientEntityId = :id LIMIT 1"));
            q.bindValue(":id", QVariant::fromValue(id));
            if (q.exec() && q.next()) {
                r.ok = true; r.result = packRecord(q.record()); return r;
            }

            QSqlQuery q2(QSqlDatabase::database());
            q2.prepare(QStringLiteral("SELECT * FROM viewClients WHERE EntityId = :id LIMIT 1"));
            q2.bindValue(":id", QVariant::fromValue(id));
            if (q2.exec() && q2.next()) {
                r.ok = true; r.result = packRecord(q2.record()); return r;
            }

            r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Client not found");
            return r;
        }
    });

    // client.list (read-only via viewClients)
    m_router.registerAction(QStringLiteral("client.list"), ActionSpec{
        /*fields*/ QList<FieldSpec>{},
        /*handler*/ [](const QJsonObject &){
            QSqlQuery q(QSqlDatabase::database());
            // Order by second column if it's a name, otherwise by first
            bool ok = q.exec(QStringLiteral("SELECT * FROM viewClients"));
            DispatchResult r; r.ok = true;
            QJsonArray items;
            if (ok) {
                while (q.next()) {
                    const QSqlRecord rec = q.record();
                    QJsonObject obj;
                    for (int i = 0; i < rec.count(); ++i) {
                        const QString name = rec.fieldName(i);
                        const QVariant v = rec.value(i);
                        switch (v.typeId()) {
                            case QMetaType::Bool: obj.insert(name, v.toBool()); break;
                            case QMetaType::Int:
                            case QMetaType::UInt:
                            case QMetaType::LongLong:
                            case QMetaType::ULongLong:
                            case QMetaType::Double: obj.insert(name, v.toDouble()); break;
                            case QMetaType::QDate: obj.insert(name, v.toDate().toString(Qt::ISODate)); break;
                            case QMetaType::QDateTime: obj.insert(name, v.toDateTime().toString(Qt::ISODate)); break;
                            default: obj.insert(name, v.toString()); break;
                        }
                    }
                    items.append(obj);
                }
            }
            r.result = QJsonObject{{"items", items}};
            return r;
        }
    });

    // contractingFirm.get (read-only via viewContractingFirms)
    m_router.registerAction(QStringLiteral("contractingFirm.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("id"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            auto packRecord = [](const QSqlRecord &rec){
                QJsonObject obj;
                for (int i = 0; i < rec.count(); ++i) {
                    const QString name = rec.fieldName(i);
                    const QVariant v = rec.value(i);
                    switch (v.typeId()) {
                        case QMetaType::Bool: obj.insert(name, v.toBool()); break;
                        case QMetaType::Int:
                        case QMetaType::UInt:
                        case QMetaType::LongLong:
                        case QMetaType::ULongLong:
                        case QMetaType::Double: obj.insert(name, v.toDouble()); break;
                        case QMetaType::QDate: obj.insert(name, v.toDate().toString(Qt::ISODate)); break;
                        case QMetaType::QDateTime: obj.insert(name, v.toDateTime().toString(Qt::ISODate)); break;
                        default: obj.insert(name, v.toString()); break;
                    }
                }
                return obj;
            };

            DispatchResult r;
            QSqlQuery q(QSqlDatabase::database());
            q.prepare(QStringLiteral("SELECT * FROM viewContractingFirms WHERE ContractingFirmEntityId = :id LIMIT 1"));
            q.bindValue(":id", QVariant::fromValue(id));
            if (q.exec() && q.next()) { r.ok = true; r.result = packRecord(q.record()); return r; }

            QSqlQuery q2(QSqlDatabase::database());
            q2.prepare(QStringLiteral("SELECT * FROM viewContractingFirms WHERE EntityId = :id LIMIT 1"));
            q2.bindValue(":id", QVariant::fromValue(id));
            if (q2.exec() && q2.next()) { r.ok = true; r.result = packRecord(q2.record()); return r; }

            r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Contracting firm not found");
            return r;
        }
    });

    // contractingFirm.list (read-only via viewContractingFirms)
    m_router.registerAction(QStringLiteral("contractingFirm.list"), ActionSpec{
        /*fields*/ QList<FieldSpec>{},
        /*handler*/ [](const QJsonObject &){
            QSqlQuery q(QSqlDatabase::database());
            bool ok = q.exec(QStringLiteral("SELECT * FROM viewContractingFirms"));
            DispatchResult r; r.ok = true;
            QJsonArray items;
            if (ok) {
                while (q.next()) {
                    const QSqlRecord rec = q.record();
                    QJsonObject obj;
                    for (int i = 0; i < rec.count(); ++i) {
                        const QString name = rec.fieldName(i);
                        const QVariant v = rec.value(i);
                        switch (v.typeId()) {
                            case QMetaType::Bool: obj.insert(name, v.toBool()); break;
                            case QMetaType::Int:
                            case QMetaType::UInt:
                            case QMetaType::LongLong:
                            case QMetaType::ULongLong:
                            case QMetaType::Double: obj.insert(name, v.toDouble()); break;
                            case QMetaType::QDate: obj.insert(name, v.toDate().toString(Qt::ISODate)); break;
                            case QMetaType::QDateTime: obj.insert(name, v.toDateTime().toString(Qt::ISODate)); break;
                            default: obj.insert(name, v.toString()); break;
                        }
                    }
                    items.append(obj);
                }
            }
            r.result = QJsonObject{{"items", items}};
            return r;
        }
    });

    // copyrightFilingType.get (read-only via viewCopyrightFilingTypes)
    m_router.registerAction(QStringLiteral("copyrightFilingType.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("id"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            auto packRecord = [](const QSqlRecord &rec){
                QJsonObject obj;
                for (int i = 0; i < rec.count(); ++i) {
                    const QString name = rec.fieldName(i);
                    const QVariant v = rec.value(i);
                    switch (v.typeId()) {
                        case QMetaType::Bool: obj.insert(name, v.toBool()); break;
                        case QMetaType::Int:
                        case QMetaType::UInt:
                        case QMetaType::LongLong:
                        case QMetaType::ULongLong:
                        case QMetaType::Double: obj.insert(name, v.toDouble()); break;
                        case QMetaType::QDate: obj.insert(name, v.toDate().toString(Qt::ISODate)); break;
                        case QMetaType::QDateTime: obj.insert(name, v.toDateTime().toString(Qt::ISODate)); break;
                        default: obj.insert(name, v.toString()); break;
                    }
                }
                return obj;
            };

            const QStringList idColumns{
                QStringLiteral("CopyrightFilingTypeId"),
                QStringLiteral("FilingTypeId"),
                QStringLiteral("TaskTypeId"),
                QStringLiteral("Id")
            };

            DispatchResult r;
            for (const QString &col : idColumns) {
                QSqlQuery q(QSqlDatabase::database());
                q.prepare(QStringLiteral("SELECT * FROM viewCopyrightFilingTypes WHERE ") + col + QStringLiteral(" = :id LIMIT 1"));
                q.bindValue(":id", QVariant::fromValue(id));
                if (q.exec() && q.next()) { r.ok = true; r.result = packRecord(q.record()); return r; }
            }
            r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("CopyrightFilingType not found");
            return r;
        }
    });

    // copyrightFilingType.list (read-only)
    m_router.registerAction(QStringLiteral("copyrightFilingType.list"), ActionSpec{
        /*fields*/ QList<FieldSpec>{},
        /*handler*/ [](const QJsonObject &){
            QSqlQuery q(QSqlDatabase::database());
            bool ok = q.exec(QStringLiteral("SELECT * FROM viewCopyrightFilingTypes"));
            DispatchResult r; r.ok = true;
            QJsonArray items;
            if (ok) {
                while (q.next()) {
                    const QSqlRecord rec = q.record();
                    QJsonObject obj;
                    for (int i = 0; i < rec.count(); ++i) {
                        const QString name = rec.fieldName(i);
                        const QVariant v = rec.value(i);
                        switch (v.typeId()) {
                            case QMetaType::Bool: obj.insert(name, v.toBool()); break;
                            case QMetaType::Int:
                            case QMetaType::UInt:
                            case QMetaType::LongLong:
                            case QMetaType::ULongLong:
                            case QMetaType::Double: obj.insert(name, v.toDouble()); break;
                            case QMetaType::QDate: obj.insert(name, v.toDate().toString(Qt::ISODate)); break;
                            case QMetaType::QDateTime: obj.insert(name, v.toDateTime().toString(Qt::ISODate)); break;
                            default: obj.insert(name, v.toString()); break;
                        }
                    }
                    items.append(obj);
                }
            }
            r.result = QJsonObject{{"items", items}};
            return r;
        }
    });

    // copyrightTaskType.get (read-only via viewCopyrightTaskTypes)
    m_router.registerAction(QStringLiteral("copyrightTaskType.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("id"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            auto packRecord = [](const QSqlRecord &rec){
                QJsonObject obj;
                for (int i = 0; i < rec.count(); ++i) {
                    const QString name = rec.fieldName(i);
                    const QVariant v = rec.value(i);
                    switch (v.typeId()) {
                        case QMetaType::Bool: obj.insert(name, v.toBool()); break;
                        case QMetaType::Int:
                        case QMetaType::UInt:
                        case QMetaType::LongLong:
                        case QMetaType::ULongLong:
                        case QMetaType::Double: obj.insert(name, v.toDouble()); break;
                        case QMetaType::QDate: obj.insert(name, v.toDate().toString(Qt::ISODate)); break;
                        case QMetaType::QDateTime: obj.insert(name, v.toDateTime().toString(Qt::ISODate)); break;
                        default: obj.insert(name, v.toString()); break;
                    }
                }
                return obj;
            };

            const QStringList idColumns{
                QStringLiteral("CopyrightTaskTypeId"),
                QStringLiteral("TaskTypeId"),
                QStringLiteral("Id")
            };

            DispatchResult r;
            for (const QString &col : idColumns) {
                QSqlQuery q(QSqlDatabase::database());
                q.prepare(QStringLiteral("SELECT * FROM viewCopyrightTaskTypes WHERE ") + col + QStringLiteral(" = :id LIMIT 1"));
                q.bindValue(":id", QVariant::fromValue(id));
                if (q.exec() && q.next()) { r.ok = true; r.result = packRecord(q.record()); return r; }
            }
            r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("CopyrightTaskType not found");
            return r;
        }
    });

    // copyrightTaskType.list (read-only)
    m_router.registerAction(QStringLiteral("copyrightTaskType.list"), ActionSpec{
        /*fields*/ QList<FieldSpec>{},
        /*handler*/ [](const QJsonObject &){
            QSqlQuery q(QSqlDatabase::database());
            bool ok = q.exec(QStringLiteral("SELECT * FROM viewCopyrightTaskTypes"));
            DispatchResult r; r.ok = true;
            QJsonArray items;
            if (ok) {
                while (q.next()) {
                    const QSqlRecord rec = q.record();
                    QJsonObject obj;
                    for (int i = 0; i < rec.count(); ++i) {
                        const QString name = rec.fieldName(i);
                        const QVariant v = rec.value(i);
                        switch (v.typeId()) {
                            case QMetaType::Bool: obj.insert(name, v.toBool()); break;
                            case QMetaType::Int:
                            case QMetaType::UInt:
                            case QMetaType::LongLong:
                            case QMetaType::ULongLong:
                            case QMetaType::Double: obj.insert(name, v.toDouble()); break;
                            case QMetaType::QDate: obj.insert(name, v.toDate().toString(Qt::ISODate)); break;
                            case QMetaType::QDateTime: obj.insert(name, v.toDateTime().toString(Qt::ISODate)); break;
                            default: obj.insert(name, v.toString()); break;
                        }
                    }
                    items.append(obj);
                }
            }
            r.result = QJsonObject{{"items", items}};
            return r;
        }
    });

    // entityView.get (read-only via viewEntities)
    m_router.registerAction(QStringLiteral("entityView.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("id"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            auto packRecord = [](const QSqlRecord &rec){
                QJsonObject obj;
                for (int i = 0; i < rec.count(); ++i) {
                    const QString name = rec.fieldName(i);
                    const QVariant v = rec.value(i);
                    switch (v.typeId()) {
                        case QMetaType::Bool: obj.insert(name, v.toBool()); break;
                        case QMetaType::Int:
                        case QMetaType::UInt:
                        case QMetaType::LongLong:
                        case QMetaType::ULongLong:
                        case QMetaType::Double: obj.insert(name, v.toDouble()); break;
                        case QMetaType::QDate: obj.insert(name, v.toDate().toString(Qt::ISODate)); break;
                        case QMetaType::QDateTime: obj.insert(name, v.toDateTime().toString(Qt::ISODate)); break;
                        default: obj.insert(name, v.toString()); break;
                    }
                }
                return obj;
            };

            const QStringList idColumns{
                QStringLiteral("EntityId"),
                QStringLiteral("Id")
            };

            DispatchResult r;
            for (const QString &col : idColumns) {
                QSqlQuery q(QSqlDatabase::database());
                q.prepare(QStringLiteral("SELECT * FROM viewEntities WHERE ") + col + QStringLiteral(" = :id LIMIT 1"));
                q.bindValue(":id", QVariant::fromValue(id));
                if (q.exec() && q.next()) { r.ok = true; r.result = packRecord(q.record()); return r; }
            }
            r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Entity not found in view");
            return r;
        }
    });

    // entityView.list (read-only via viewEntities)
    m_router.registerAction(QStringLiteral("entityView.list"), ActionSpec{
        /*fields*/ QList<FieldSpec>{},
        /*handler*/ [](const QJsonObject &){
            QSqlQuery q(QSqlDatabase::database());
            bool ok = q.exec(QStringLiteral("SELECT * FROM viewEntities"));
            DispatchResult r; r.ok = true;
            QJsonArray items;
            if (ok) {
                while (q.next()) {
                    const QSqlRecord rec = q.record();
                    QJsonObject obj;
                    for (int i = 0; i < rec.count(); ++i) {
                        const QString name = rec.fieldName(i);
                        const QVariant v = rec.value(i);
                        switch (v.typeId()) {
                            case QMetaType::Bool: obj.insert(name, v.toBool()); break;
                            case QMetaType::Int:
                            case QMetaType::UInt:
                            case QMetaType::LongLong:
                            case QMetaType::ULongLong:
                            case QMetaType::Double: obj.insert(name, v.toDouble()); break;
                            case QMetaType::QDate: obj.insert(name, v.toDate().toString(Qt::ISODate)); break;
                            case QMetaType::QDateTime: obj.insert(name, v.toDateTime().toString(Qt::ISODate)); break;
                            default: obj.insert(name, v.toString()); break;
                        }
                    }
                    items.append(obj);
                }
            }
            r.result = QJsonObject{{"items", items}};
            return r;
        }
    });

    // filingView.get (read-only via viewFilings)
    m_router.registerAction(QStringLiteral("filingView.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("id"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            auto packRecord = [](const QSqlRecord &rec){
                QJsonObject obj;
                for (int i = 0; i < rec.count(); ++i) {
                    const QString name = rec.fieldName(i);
                    const QVariant v = rec.value(i);
                    switch (v.typeId()) {
                        case QMetaType::Bool: obj.insert(name, v.toBool()); break;
                        case QMetaType::Int:
                        case QMetaType::UInt:
                        case QMetaType::LongLong:
                        case QMetaType::ULongLong:
                        case QMetaType::Double: obj.insert(name, v.toDouble()); break;
                        case QMetaType::QDate: obj.insert(name, v.toDate().toString(Qt::ISODate)); break;
                        case QMetaType::QDateTime: obj.insert(name, v.toDateTime().toString(Qt::ISODate)); break;
                        default: obj.insert(name, v.toString()); break;
                    }
                }
                return obj;
            };

            const QStringList idColumns{
                QStringLiteral("FilingId"),
                QStringLiteral("PatentFilingId"),
                QStringLiteral("TrademarkFilingId"),
                QStringLiteral("Id")
            };

            DispatchResult r;
            for (const QString &col : idColumns) {
                QSqlQuery q(QSqlDatabase::database());
                q.prepare(QStringLiteral("SELECT * FROM viewFilings WHERE ") + col + QStringLiteral(" = :id LIMIT 1"));
                q.bindValue(":id", QVariant::fromValue(id));
                if (q.exec() && q.next()) { r.ok = true; r.result = packRecord(q.record()); return r; }
            }
            r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Filing not found in view");
            return r;
        }
    });

    // filingView.list (read-only via viewFilings)
    m_router.registerAction(QStringLiteral("filingView.list"), ActionSpec{
        /*fields*/ QList<FieldSpec>{},
        /*handler*/ [](const QJsonObject &){
            QSqlQuery q(QSqlDatabase::database());
            bool ok = q.exec(QStringLiteral("SELECT * FROM viewFilings"));
            DispatchResult r; r.ok = true;
            QJsonArray items;
            if (ok) {
                while (q.next()) {
                    const QSqlRecord rec = q.record();
                    QJsonObject obj;
                    for (int i = 0; i < rec.count(); ++i) {
                        const QString name = rec.fieldName(i);
                        const QVariant v = rec.value(i);
                        switch (v.typeId()) {
                            case QMetaType::Bool: obj.insert(name, v.toBool()); break;
                            case QMetaType::Int:
                            case QMetaType::UInt:
                            case QMetaType::LongLong:
                            case QMetaType::ULongLong:
                            case QMetaType::Double: obj.insert(name, v.toDouble()); break;
                            case QMetaType::QDate: obj.insert(name, v.toDate().toString(Qt::ISODate)); break;
                            case QMetaType::QDateTime: obj.insert(name, v.toDateTime().toString(Qt::ISODate)); break;
                            default: obj.insert(name, v.toString()); break;
                        }
                    }
                    items.append(obj);
                }
            }
            r.result = QJsonObject{{"items", items}};
            return r;
        }
    });

    // filingIncompleteView.get (read-only via viewFilingsIncomplete)
    m_router.registerAction(QStringLiteral("filingIncompleteView.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("id"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            auto packRecord = [](const QSqlRecord &rec){
                QJsonObject obj;
                for (int i = 0; i < rec.count(); ++i) {
                    const QString name = rec.fieldName(i);
                    const QVariant v = rec.value(i);
                    switch (v.typeId()) {
                        case QMetaType::Bool: obj.insert(name, v.toBool()); break;
                        case QMetaType::Int:
                        case QMetaType::UInt:
                        case QMetaType::LongLong:
                        case QMetaType::ULongLong:
                        case QMetaType::Double: obj.insert(name, v.toDouble()); break;
                        case QMetaType::QDate: obj.insert(name, v.toDate().toString(Qt::ISODate)); break;
                        case QMetaType::QDateTime: obj.insert(name, v.toDateTime().toString(Qt::ISODate)); break;
                        default: obj.insert(name, v.toString()); break;
                    }
                }
                return obj;
            };

            const QStringList idColumns{
                QStringLiteral("FilingId"),
                QStringLiteral("PatentFilingId"),
                QStringLiteral("TrademarkFilingId"),
                QStringLiteral("Id")
            };

            DispatchResult r;
            for (const QString &col : idColumns) {
                QSqlQuery q(QSqlDatabase::database());
                q.prepare(QStringLiteral("SELECT * FROM viewFilingsIncomplete WHERE ") + col + QStringLiteral(" = :id LIMIT 1"));
                q.bindValue(":id", QVariant::fromValue(id));
                if (q.exec() && q.next()) { r.ok = true; r.result = packRecord(q.record()); return r; }
            }
            r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Filing (incomplete) not found");
            return r;
        }
    });

    // filingIncompleteView.list (read-only via viewFilingsIncomplete)
    m_router.registerAction(QStringLiteral("filingIncompleteView.list"), ActionSpec{
        /*fields*/ QList<FieldSpec>{},
        /*handler*/ [](const QJsonObject &){
            QSqlQuery q(QSqlDatabase::database());
            bool ok = q.exec(QStringLiteral("SELECT * FROM viewFilingsIncomplete"));
            DispatchResult r; r.ok = true;
            QJsonArray items;
            if (ok) {
                while (q.next()) {
                    const QSqlRecord rec = q.record();
                    QJsonObject obj;
                    for (int i = 0; i < rec.count(); ++i) {
                        const QString name = rec.fieldName(i);
                        const QVariant v = rec.value(i);
                        switch (v.typeId()) {
                            case QMetaType::Bool: obj.insert(name, v.toBool()); break;
                            case QMetaType::Int:
                            case QMetaType::UInt:
                            case QMetaType::LongLong:
                            case QMetaType::ULongLong:
                            case QMetaType::Double: obj.insert(name, v.toDouble()); break;
                            case QMetaType::QDate: obj.insert(name, v.toDate().toString(Qt::ISODate)); break;
                            case QMetaType::QDateTime: obj.insert(name, v.toDateTime().toString(Qt::ISODate)); break;
                            default: obj.insert(name, v.toString()); break;
                        }
                    }
                    items.append(obj);
                }
            }
            r.result = QJsonObject{{"items", items}};
            return r;
        }
    });

    // filingTaskType.get (read-only via viewFilingTaskTypes)
    m_router.registerAction(QStringLiteral("filingTaskType.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("id"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            auto packRecord = [](const QSqlRecord &rec){
                QJsonObject obj;
                for (int i = 0; i < rec.count(); ++i) {
                    const QString name = rec.fieldName(i);
                    const QVariant v = rec.value(i);
                    switch (v.typeId()) {
                        case QMetaType::Bool: obj.insert(name, v.toBool()); break;
                        case QMetaType::Int:
                        case QMetaType::UInt:
                        case QMetaType::LongLong:
                        case QMetaType::ULongLong:
                        case QMetaType::Double: obj.insert(name, v.toDouble()); break;
                        case QMetaType::QDate: obj.insert(name, v.toDate().toString(Qt::ISODate)); break;
                        case QMetaType::QDateTime: obj.insert(name, v.toDateTime().toString(Qt::ISODate)); break;
                        default: obj.insert(name, v.toString()); break;
                    }
                }
                return obj;
            };

            const QStringList idColumns{
                QStringLiteral("FilingTaskTypeId"),
                QStringLiteral("TaskTypeId"),
                QStringLiteral("Id")
            };

            DispatchResult r;
            for (const QString &col : idColumns) {
                QSqlQuery q(QSqlDatabase::database());
                q.prepare(QStringLiteral("SELECT * FROM viewFilingTaskTypes WHERE ") + col + QStringLiteral(" = :id LIMIT 1"));
                q.bindValue(":id", QVariant::fromValue(id));
                if (q.exec() && q.next()) { r.ok = true; r.result = packRecord(q.record()); return r; }
            }
            r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("FilingTaskType not found");
            return r;
        }
    });

    // filingTaskType.list (read-only)
    m_router.registerAction(QStringLiteral("filingTaskType.list"), ActionSpec{
        /*fields*/ QList<FieldSpec>{},
        /*handler*/ [](const QJsonObject &){
            QSqlQuery q(QSqlDatabase::database());
            bool ok = q.exec(QStringLiteral("SELECT * FROM viewFilingTaskTypes"));
            DispatchResult r; r.ok = true;
            QJsonArray items;
            if (ok) {
                while (q.next()) {
                    const QSqlRecord rec = q.record();
                    QJsonObject obj;
                    for (int i = 0; i < rec.count(); ++i) {
                        const QString name = rec.fieldName(i);
                        const QVariant v = rec.value(i);
                        switch (v.typeId()) {
                            case QMetaType::Bool: obj.insert(name, v.toBool()); break;
                            case QMetaType::Int:
                            case QMetaType::UInt:
                            case QMetaType::LongLong:
                            case QMetaType::ULongLong:
                            case QMetaType::Double: obj.insert(name, v.toDouble()); break;
                            case QMetaType::QDate: obj.insert(name, v.toDate().toString(Qt::ISODate)); break;
                            case QMetaType::QDateTime: obj.insert(name, v.toDateTime().toString(Qt::ISODate)); break;
                            default: obj.insert(name, v.toString()); break;
                        }
                    }
                    items.append(obj);
                }
            }
            r.result = QJsonObject{{"items", items}};
            return r;
        }
    });

    // filingType.get (read-only via viewFilingTypes)
    m_router.registerAction(QStringLiteral("filingType.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("id"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            auto packRecord = [](const QSqlRecord &rec){
                QJsonObject obj;
                for (int i = 0; i < rec.count(); ++i) {
                    const QString name = rec.fieldName(i);
                    const QVariant v = rec.value(i);
                    switch (v.typeId()) {
                        case QMetaType::Bool: obj.insert(name, v.toBool()); break;
                        case QMetaType::Int:
                        case QMetaType::UInt:
                        case QMetaType::LongLong:
                        case QMetaType::ULongLong:
                        case QMetaType::Double: obj.insert(name, v.toDouble()); break;
                        case QMetaType::QDate: obj.insert(name, v.toDate().toString(Qt::ISODate)); break;
                        case QMetaType::QDateTime: obj.insert(name, v.toDateTime().toString(Qt::ISODate)); break;
                        default: obj.insert(name, v.toString()); break;
                    }
                }
                return obj;
            };

            const QStringList idColumns{
                QStringLiteral("FilingTypeId"),
                QStringLiteral("TaskTypeId"),
                QStringLiteral("Id")
            };

            DispatchResult r;
            for (const QString &col : idColumns) {
                QSqlQuery q(QSqlDatabase::database());
                q.prepare(QStringLiteral("SELECT * FROM viewFilingTypes WHERE ") + col + QStringLiteral(" = :id LIMIT 1"));
                q.bindValue(":id", QVariant::fromValue(id));
                if (q.exec() && q.next()) { r.ok = true; r.result = packRecord(q.record()); return r; }
            }
            r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("FilingType not found");
            return r;
        }
    });

    // filingType.list (read-only)
    m_router.registerAction(QStringLiteral("filingType.list"), ActionSpec{
        /*fields*/ QList<FieldSpec>{},
        /*handler*/ [](const QJsonObject &){
            QSqlQuery q(QSqlDatabase::database());
            bool ok = q.exec(QStringLiteral("SELECT * FROM viewFilingTypes"));
            DispatchResult r; r.ok = true;
            QJsonArray items;
            if (ok) {
                while (q.next()) {
                    const QSqlRecord rec = q.record();
                    QJsonObject obj;
                    for (int i = 0; i < rec.count(); ++i) {
                        const QString name = rec.fieldName(i);
                        const QVariant v = rec.value(i);
                        switch (v.typeId()) {
                            case QMetaType::Bool: obj.insert(name, v.toBool()); break;
                            case QMetaType::Int:
                            case QMetaType::UInt:
                            case QMetaType::LongLong:
                            case QMetaType::ULongLong:
                            case QMetaType::Double: obj.insert(name, v.toDouble()); break;
                            case QMetaType::QDate: obj.insert(name, v.toDate().toString(Qt::ISODate)); break;
                            case QMetaType::QDateTime: obj.insert(name, v.toDateTime().toString(Qt::ISODate)); break;
                            default: obj.insert(name, v.toString()); break;
                        }
                    }
                    items.append(obj);
                }
            }
            r.result = QJsonObject{{"items", items}};
            return r;
        }
    });

    // inventor.get (read-only via viewInventors)
    m_router.registerAction(QStringLiteral("inventor.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("id"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            auto packRecord = [](const QSqlRecord &rec){
                QJsonObject obj;
                for (int i = 0; i < rec.count(); ++i) {
                    const QString name = rec.fieldName(i);
                    const QVariant v = rec.value(i);
                    switch (v.typeId()) {
                        case QMetaType::Bool: obj.insert(name, v.toBool()); break;
                        case QMetaType::Int:
                        case QMetaType::UInt:
                        case QMetaType::LongLong:
                        case QMetaType::ULongLong:
                        case QMetaType::Double: obj.insert(name, v.toDouble()); break;
                        case QMetaType::QDate: obj.insert(name, v.toDate().toString(Qt::ISODate)); break;
                        case QMetaType::QDateTime: obj.insert(name, v.toDateTime().toString(Qt::ISODate)); break;
                        default: obj.insert(name, v.toString()); break;
                    }
                }
                return obj;
            };

            const QStringList idColumns{
                QStringLiteral("PersonId"),
                QStringLiteral("fkPerson"),
                QStringLiteral("InventorId"),
                QStringLiteral("Id")
            };

            DispatchResult r;
            for (const QString &col : idColumns) {
                QSqlQuery q(QSqlDatabase::database());
                q.prepare(QStringLiteral("SELECT * FROM viewInventors WHERE ") + col + QStringLiteral(" = :id LIMIT 1"));
                q.bindValue(":id", QVariant::fromValue(id));
                if (q.exec() && q.next()) { r.ok = true; r.result = packRecord(q.record()); return r; }
            }
            r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Inventor not found");
            return r;
        }
    });

    // inventor.list (read-only)
    m_router.registerAction(QStringLiteral("inventor.list"), ActionSpec{
        /*fields*/ QList<FieldSpec>{},
        /*handler*/ [](const QJsonObject &){
            QSqlQuery q(QSqlDatabase::database());
            bool ok = q.exec(QStringLiteral("SELECT * FROM viewInventors"));
            DispatchResult r; r.ok = true;
            QJsonArray items;
            if (ok) {
                while (q.next()) {
                    const QSqlRecord rec = q.record();
                    QJsonObject obj;
                    for (int i = 0; i < rec.count(); ++i) {
                        const QString name = rec.fieldName(i);
                        const QVariant v = rec.value(i);
                        switch (v.typeId()) {
                            case QMetaType::Bool: obj.insert(name, v.toBool()); break;
                            case QMetaType::Int:
                            case QMetaType::UInt:
                            case QMetaType::LongLong:
                            case QMetaType::ULongLong:
                            case QMetaType::Double: obj.insert(name, v.toDouble()); break;
                            case QMetaType::QDate: obj.insert(name, v.toDate().toString(Qt::ISODate)); break;
                            case QMetaType::QDateTime: obj.insert(name, v.toDateTime().toString(Qt::ISODate)); break;
                            default: obj.insert(name, v.toString()); break;
                        }
                    }
                    items.append(obj);
                }
            }
            r.result = QJsonObject{{"items", items}};
            return r;
        }
    });

    // jurisdictionView.get (read-only via viewJurisdictions)
    m_router.registerAction(QStringLiteral("jurisdictionView.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("id"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            auto packRecord = [](const QSqlRecord &rec){
                QJsonObject obj;
                for (int i = 0; i < rec.count(); ++i) {
                    const QString name = rec.fieldName(i);
                    const QVariant v = rec.value(i);
                    switch (v.typeId()) {
                        case QMetaType::Bool: obj.insert(name, v.toBool()); break;
                        case QMetaType::Int:
                        case QMetaType::UInt:
                        case QMetaType::LongLong:
                        case QMetaType::ULongLong:
                        case QMetaType::Double: obj.insert(name, v.toDouble()); break;
                        case QMetaType::QDate: obj.insert(name, v.toDate().toString(Qt::ISODate)); break;
                        case QMetaType::QDateTime: obj.insert(name, v.toDateTime().toString(Qt::ISODate)); break;
                        default: obj.insert(name, v.toString()); break;
                    }
                }
                return obj;
            };

            const QStringList idColumns{
                QStringLiteral("JurisdictionId"),
                QStringLiteral("Id")
            };

            DispatchResult r;
            for (const QString &col : idColumns) {
                QSqlQuery q(QSqlDatabase::database());
                q.prepare(QStringLiteral("SELECT * FROM viewJurisdictions WHERE ") + col + QStringLiteral(" = :id LIMIT 1"));
                q.bindValue(":id", QVariant::fromValue(id));
                if (q.exec() && q.next()) { r.ok = true; r.result = packRecord(q.record()); return r; }
            }
            r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Jurisdiction not found");
            return r;
        }
    });

    // jurisdictionView.list (read-only)
    m_router.registerAction(QStringLiteral("jurisdictionView.list"), ActionSpec{
        /*fields*/ QList<FieldSpec>{},
        /*handler*/ [](const QJsonObject &){
            QSqlQuery q(QSqlDatabase::database());
            bool ok = q.exec(QStringLiteral("SELECT * FROM viewJurisdictions"));
            DispatchResult r; r.ok = true;
            QJsonArray items;
            if (ok) {
                while (q.next()) {
                    const QSqlRecord rec = q.record();
                    QJsonObject obj;
                    for (int i = 0; i < rec.count(); ++i) {
                        const QString name = rec.fieldName(i);
                        const QVariant v = rec.value(i);
                        switch (v.typeId()) {
                            case QMetaType::Bool: obj.insert(name, v.toBool()); break;
                            case QMetaType::Int:
                            case QMetaType::UInt:
                            case QMetaType::LongLong:
                            case QMetaType::ULongLong:
                            case QMetaType::Double: obj.insert(name, v.toDouble()); break;
                            case QMetaType::QDate: obj.insert(name, v.toDate().toString(Qt::ISODate)); break;
                            case QMetaType::QDateTime: obj.insert(name, v.toDateTime().toString(Qt::ISODate)); break;
                            default: obj.insert(name, v.toString()); break;
                        }
                    }
                    items.append(obj);
                }
            }
            r.result = QJsonObject{{"items", items}};
            return r;
        }
    });

    // matterView.get (read-only via viewMatters)
    m_router.registerAction(QStringLiteral("matterView.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("id"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            auto packRecord = [](const QSqlRecord &rec){
                QJsonObject obj;
                for (int i = 0; i < rec.count(); ++i) {
                    const QString name = rec.fieldName(i);
                    const QVariant v = rec.value(i);
                    switch (v.typeId()) {
                        case QMetaType::Bool: obj.insert(name, v.toBool()); break;
                        case QMetaType::Int:
                        case QMetaType::UInt:
                        case QMetaType::LongLong:
                        case QMetaType::ULongLong:
                        case QMetaType::Double: obj.insert(name, v.toDouble()); break;
                        case QMetaType::QDate: obj.insert(name, v.toDate().toString(Qt::ISODate)); break;
                        case QMetaType::QDateTime: obj.insert(name, v.toDateTime().toString(Qt::ISODate)); break;
                        default: obj.insert(name, v.toString()); break;
                    }
                }
                return obj;
            };

            const QStringList idColumns{
                QStringLiteral("MatterId"),
                QStringLiteral("PatentMatterId"),
                QStringLiteral("TrademarkMatterId"),
                QStringLiteral("Id")
            };

            DispatchResult r;
            for (const QString &col : idColumns) {
                QSqlQuery q(QSqlDatabase::database());
                q.prepare(QStringLiteral("SELECT * FROM viewMatters WHERE ") + col + QStringLiteral(" = :id LIMIT 1"));
                q.bindValue(":id", QVariant::fromValue(id));
                if (q.exec() && q.next()) { r.ok = true; r.result = packRecord(q.record()); return r; }
            }
            r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Matter not found in view");
            return r;
        }
    });

    // matterView.list (read-only via viewMatters)
    m_router.registerAction(QStringLiteral("matterView.list"), ActionSpec{
        /*fields*/ QList<FieldSpec>{},
        /*handler*/ [](const QJsonObject &){
            QSqlQuery q(QSqlDatabase::database());
            bool ok = q.exec(QStringLiteral("SELECT * FROM viewMatters"));
            DispatchResult r; r.ok = true;
            QJsonArray items;
            if (ok) {
                while (q.next()) {
                    const QSqlRecord rec = q.record();
                    QJsonObject obj;
                    for (int i = 0; i < rec.count(); ++i) {
                        const QString name = rec.fieldName(i);
                        const QVariant v = rec.value(i);
                        switch (v.typeId()) {
                            case QMetaType::Bool: obj.insert(name, v.toBool()); break;
                            case QMetaType::Int:
                            case QMetaType::UInt:
                            case QMetaType::LongLong:
                            case QMetaType::ULongLong:
                            case QMetaType::Double: obj.insert(name, v.toDouble()); break;
                            case QMetaType::QDate: obj.insert(name, v.toDate().toString(Qt::ISODate)); break;
                            case QMetaType::QDateTime: obj.insert(name, v.toDateTime().toString(Qt::ISODate)); break;
                            default: obj.insert(name, v.toString()); break;
                        }
                    }
                    items.append(obj);
                }
            }
            r.result = QJsonObject{{"items", items}};
            return r;
        }
    });

    // objectFlagView.get (read-only via viewObjectFlags) by objectId+camelCase or id
    m_router.registerAction(QStringLiteral("objectFlagView.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{
            FieldSpec{QStringLiteral("id"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("objectId"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("camelCase"), QJsonValue::String, false}
        },
        /*handler*/ [](const QJsonObject &payload){
            auto packRecord = [](const QSqlRecord &rec){
                QJsonObject obj;
                for (int i = 0; i < rec.count(); ++i) {
                    const QString name = rec.fieldName(i);
                    const QVariant v = rec.value(i);
                    switch (v.typeId()) {
                        case QMetaType::Bool: obj.insert(name, v.toBool()); break;
                        case QMetaType::Int:
                        case QMetaType::UInt:
                        case QMetaType::LongLong:
                        case QMetaType::ULongLong:
                        case QMetaType::Double: obj.insert(name, v.toDouble()); break;
                        case QMetaType::QDate: obj.insert(name, v.toDate().toString(Qt::ISODate)); break;
                        case QMetaType::QDateTime: obj.insert(name, v.toDateTime().toString(Qt::ISODate)); break;
                        default: obj.insert(name, v.toString()); break;
                    }
                }
                return obj;
            };

            DispatchResult r;
            const bool hasId = payload.contains(QStringLiteral("id"));
            const bool hasObj = payload.contains(QStringLiteral("objectId"));
            const bool hasCC = payload.contains(QStringLiteral("camelCase"));
            if (!hasId && !(hasObj && hasCC)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("Provide 'id' or objectId+camelCase"); return r; }

            if (hasId) {
                const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
                const QStringList idCols{ QStringLiteral("ObjectFlagId"), QStringLiteral("Id") };
                for (const QString &col : idCols) {
                    QSqlQuery q(QSqlDatabase::database());
                    q.prepare(QStringLiteral("SELECT * FROM viewObjectFlags WHERE ") + col + QStringLiteral(" = :id LIMIT 1"));
                    q.bindValue(":id", QVariant::fromValue(id));
                    if (q.exec() && q.next()) { r.ok = true; r.result = packRecord(q.record()); return r; }
                }
            } else {
                const Key objectId = static_cast<Key>(payload.value(QStringLiteral("objectId")).toDouble());
                const QString cc = payload.value(QStringLiteral("camelCase")).toString();
                QSqlQuery q(QSqlDatabase::database());
                q.prepare(QStringLiteral("SELECT * FROM viewObjectFlags WHERE ObjectId = :oid AND CamelCase = :cc LIMIT 1"));
                q.bindValue(":oid", QVariant::fromValue(objectId));
                q.bindValue(":cc", cc);
                if (q.exec() && q.next()) { r.ok = true; r.result = packRecord(q.record()); return r; }
            }
            r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Object flag not found");
            return r;
        }
    });

    // objectFlagView.listForObject (read-only)
    m_router.registerAction(QStringLiteral("objectFlagView.listForObject"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("objectId"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){
            const Key objectId = static_cast<Key>(payload.value(QStringLiteral("objectId")).toDouble());
            QSqlQuery q(QSqlDatabase::database());
            q.prepare(QStringLiteral("SELECT * FROM viewObjectFlags WHERE ObjectId = :oid ORDER BY CamelCase"));
            q.bindValue(":oid", QVariant::fromValue(objectId));
            DispatchResult r; r.ok = true;
            QJsonArray items;
            if (q.exec()) {
                while (q.next()) {
                    const QSqlRecord rec = q.record();
                    QJsonObject obj;
                    for (int i = 0; i < rec.count(); ++i) {
                        const QString name = rec.fieldName(i);
                        const QVariant v = rec.value(i);
                        switch (v.typeId()) {
                            case QMetaType::Bool: obj.insert(name, v.toBool()); break;
                            case QMetaType::Int:
                            case QMetaType::UInt:
                            case QMetaType::LongLong:
                            case QMetaType::ULongLong:
                            case QMetaType::Double: obj.insert(name, v.toDouble()); break;
                            case QMetaType::QDate: obj.insert(name, v.toDate().toString(Qt::ISODate)); break;
                            case QMetaType::QDateTime: obj.insert(name, v.toDateTime().toString(Qt::ISODate)); break;
                            default: obj.insert(name, v.toString()); break;
                        }
                    }
                    items.append(obj);
                }
            }
            r.result = QJsonObject{{"objectId", static_cast<double>(objectId)}, {"items", items}};
            return r;
        }
    });

    // objectType.get: by id (read-only fetch)
    m_router.registerAction(QStringLiteral("objectType.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("id"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            QSqlQuery q(QSqlDatabase::database());
            q.prepare(QStringLiteral("SELECT ObjectTypeId, TypeName FROM objectType WHERE ObjectTypeId = :id"));
            q.bindValue(":id", QVariant::fromValue(id));
            DispatchResult r;
            if (!q.exec() || !q.next()) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("ObjectType not found"); return r; }
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(q.value(0).toUInt())},
                {"objectTypeName", q.value(1).toString()}
            };
            return r;
        }
    });

    // objectType.list: list all rows (read-only)
    m_router.registerAction(QStringLiteral("objectType.list"), ActionSpec{
        /*fields*/ QList<FieldSpec>{},
        /*handler*/ [](const QJsonObject &){
            QSqlQuery q(QSqlDatabase::database());
            DispatchResult r; r.ok = true;
            QJsonArray items;
            if (q.exec(QStringLiteral("SELECT ObjectTypeId, TypeName FROM objectType ORDER BY TypeName"))) {
                while (q.next()) {
                    items.append(QJsonObject{
                        {"id", static_cast<double>(q.value(0).toUInt())},
                        {"objectTypeName", q.value(1).toString()}
                    });
                }
            }
            r.result = QJsonObject{{"items", items}};
            return r;
        }
    });

    // objectType.update: update TypeName by id
    m_router.registerAction(QStringLiteral("objectType.update"), ActionSpec{
        /*fields*/ QList<FieldSpec>{
            FieldSpec{QStringLiteral("id"), QJsonValue::Double, true},
            FieldSpec{QStringLiteral("objectTypeName"), QJsonValue::String, false}
        },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            bool any = false;
            if (payload.contains(QStringLiteral("objectTypeName"))) {
                any = true;
                QSqlQuery q(QSqlDatabase::database());
                q.prepare(QStringLiteral("UPDATE objectType SET TypeName = :name WHERE ObjectTypeId = :id"));
                q.bindValue(":name", payload.value(QStringLiteral("objectTypeName")).toString());
                q.bindValue(":id", QVariant::fromValue(id));
                if (!q.exec()) { DispatchResult r; r.ok = false; r.errorCode = QStringLiteral("EUPDATE"); r.errorMessage = q.lastError().text(); return r; }
            }
            DispatchResult r;
            if (!any) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("No updatable fields provided"); return r; }
            QSqlQuery qr(QSqlDatabase::database());
            qr.prepare(QStringLiteral("SELECT ObjectTypeId, TypeName FROM objectType WHERE ObjectTypeId = :id"));
            qr.bindValue(":id", QVariant::fromValue(id));
            if (!qr.exec() || !qr.next()) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("ObjectType not found"); return r; }
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(qr.value(0).toUInt())},
                {"objectTypeName", qr.value(1).toString()}
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

    // flagClass.get: by id or camelCase (at least one required)
    m_router.registerAction(QStringLiteral("flagClass.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{
            FieldSpec{QStringLiteral("id"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("camelCase"), QJsonValue::String, false}
        },
        /*handler*/ [](const QJsonObject &payload){
            DispatchResult r;
            FlagClassData fc;
            const bool hasId = payload.contains(QStringLiteral("id"));
            const bool hasCamel = payload.contains(QStringLiteral("camelCase"));
            if (!hasId && !hasCamel) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("Provide 'id' or 'camelCase'"); return r; }

            if (hasId) {
                const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
                fc = Nutdb::GetFlagClass(id);
            } else {
                fc = Nutdb::GetFlagClass(payload.value(QStringLiteral("camelCase")).toString());
            }
            if (fc.FlagClassId == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("FlagClass not found"); return r; }
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(fc.FlagClassId)},
                {"camelCase", fc.CamelCase},
                {"label", fc.Label},
                {"description", fc.Description}
            };
            return r;
        }
    });

    // flagClass.update: by id; updates camelCase/label/description
    m_router.registerAction(QStringLiteral("flagClass.update"), ActionSpec{
        /*fields*/ QList<FieldSpec>{
            FieldSpec{QStringLiteral("id"), QJsonValue::Double, true},
            FieldSpec{QStringLiteral("camelCase"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("label"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("description"), QJsonValue::String, false}
        },
        /*handler*/ [](const QJsonObject &payload){
            DispatchResult r;
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            FlagClass fc(id);
            // We can't directly check existence from FlagClass; validate using Nutdb
            const FlagClassData cur = Nutdb::GetFlagClass(id);
            if (cur.FlagClassId == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("FlagClass not found"); return r; }

            if (payload.contains(QStringLiteral("camelCase"))) fc.setCamelCase(payload.value(QStringLiteral("camelCase")).toString());
            if (payload.contains(QStringLiteral("label"))) fc.setLabel(payload.value(QStringLiteral("label")).toString());
            if (payload.contains(QStringLiteral("description"))) fc.setDescription(payload.value(QStringLiteral("description")).toString());

            const FlagClassData out = Nutdb::GetFlagClass(id);
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(out.FlagClassId)},
                {"camelCase", out.CamelCase},
                {"label", out.Label},
                {"description", out.Description}
            };
            return r;
        }
    });

    // object.get: base object info with related lists
    m_router.registerAction(QStringLiteral("object.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("id"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            const QSqlRecord rec = Nutdb::GetRecord(QStringLiteral("object"), id);
            DispatchResult r;
            if (!rec.isEmpty() && rec.field(0).isNull()) { /* fall through */ }
            if (rec.isEmpty() || rec.field(QStringLiteral("ObjectId")).isNull()) {
                r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Object not found"); return r; }

            Object obj{id};
            // Flags
            QJsonArray flagsArr;
            for (const auto &fd : obj.getObjectFlags()) {
                flagsArr.append(QJsonObject{
                    {"flagClassId", static_cast<double>(fd.FlagClassId)},
                    {"camelCase", fd.CamelCase},
                    {"label", fd.Label},
                    {"description", fd.Description},
                    {"value", fd.FlagValue}
                });
            }
            // Tags
            QJsonArray tagsArr;
            for (const auto &tg : obj.getObjectTags()) {
                tagsArr.append(QJsonObject{{"tagId", static_cast<double>(tg.TagId)}, {"tagText", tg.TagText}});
            }
            // Docs and appointments
            QJsonArray docsArr; for (Key k : obj.getObjectDocuments()) docsArr.append(static_cast<double>(k));
            QJsonArray apptArr; for (Key k : obj.getObjectAppointments()) apptArr.append(static_cast<double>(k));

            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(obj.getId())},
                {"fkObjectType", static_cast<double>(obj.getfkObjectType())},
                {"objectType", obj.getObjectType()},
                {"flags", flagsArr},
                {"tags", tagsArr},
                {"documents", docsArr},
                {"appointments", apptArr}
            };
            return r;
        }
    });

    // object.update: change object type
    m_router.registerAction(QStringLiteral("object.update"), ActionSpec{
        /*fields*/ QList<FieldSpec>{
            FieldSpec{QStringLiteral("id"), QJsonValue::Double, true},
            FieldSpec{QStringLiteral("fkObjectType"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("objectType"), QJsonValue::String, false}
        },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            Object obj{id};
            DispatchResult r;
            if (obj.getId() == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Object not found"); return r; }
            if (payload.contains("fkObjectType")) obj.setfkObjectType(static_cast<ObjectType>(payload.value("fkObjectType").toDouble()));
            if (payload.contains("objectType")) obj.ChangeObjectType(payload.value("objectType").toString());

            // respond with current state
            QJsonArray flagsArr; for (const auto &fd : obj.getObjectFlags()) flagsArr.append(QJsonObject{{"flagClassId", static_cast<double>(fd.FlagClassId)}, {"camelCase", fd.CamelCase}, {"label", fd.Label}, {"description", fd.Description}, {"value", fd.FlagValue}});
            QJsonArray tagsArr; for (const auto &tg : obj.getObjectTags()) tagsArr.append(QJsonObject{{"tagId", static_cast<double>(tg.TagId)}, {"tagText", tg.TagText}});
            QJsonArray docsArr; for (Key k : obj.getObjectDocuments()) docsArr.append(static_cast<double>(k));
            QJsonArray apptArr; for (Key k : obj.getObjectAppointments()) apptArr.append(static_cast<double>(k));
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(obj.getId())},
                {"fkObjectType", static_cast<double>(obj.getfkObjectType())},
                {"objectType", obj.getObjectType()},
                {"flags", flagsArr},
                {"tags", tagsArr},
                {"documents", docsArr},
                {"appointments", apptArr}
            };
            return r;
        }
    });

    // person.get: person-specific fields
    m_router.registerAction(QStringLiteral("person.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("id"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            const PersonData p = Nutdb::GetPerson(id);
            DispatchResult r;
            if (p.PersonId == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Person not found"); return r; }
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(p.PersonId)},
                {"firstName", p.FirstName},
                {"lastName", p.LastName},
                {"fkResidence", static_cast<double>(p.fkResidence)},
                {"fkCitizenship", static_cast<double>(p.fkCitizenship)},
                {"oldId", static_cast<double>(p.OldId)}
            };
            return r;
        }
    });

    // person.update: update person-specific fields
    m_router.registerAction(QStringLiteral("person.update"), ActionSpec{
        /*fields*/ QList<FieldSpec>{
            FieldSpec{QStringLiteral("id"), QJsonValue::Double, true},
            FieldSpec{QStringLiteral("firstName"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("lastName"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("fkResidence"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("fkCitizenship"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("oldId"), QJsonValue::Double, false}
        },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            Person person{id};
            DispatchResult r;
            if (person.getId() == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Person not found"); return r; }
            if (payload.contains("firstName")) person.slotSetFirstName(payload.value("firstName").toString());
            if (payload.contains("lastName")) person.slotSetLastName(payload.value("lastName").toString());
            if (payload.contains("fkResidence")) person.slotSetfkResidence(static_cast<Key>(payload.value("fkResidence").toDouble()));
            if (payload.contains("fkCitizenship")) person.slotSetfkCitizenship(static_cast<Key>(payload.value("fkCitizenship").toDouble()));
            if (payload.contains("oldId")) person.slotSetOldId(static_cast<Key>(payload.value("oldId").toDouble()));

            const PersonData p = Nutdb::GetPerson(id);
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(p.PersonId)},
                {"firstName", p.FirstName},
                {"lastName", p.LastName},
                {"fkResidence", static_cast<double>(p.fkResidence)},
                {"fkCitizenship", static_cast<double>(p.fkCitizenship)},
                {"oldId", static_cast<double>(p.OldId)}
            };
            return r;
        }
    });

    // response.get
    m_router.registerAction(QStringLiteral("response.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("id"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            const ResponseData rdat = Nutdb::GetResponse(id);
            DispatchResult r;
            if (rdat.ResponseId == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Response not found"); return r; }
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(rdat.ResponseId)},
                {"fkClientOfficeHours", static_cast<double>(rdat.fkClientOfficeHours)},
                {"fkExaminerInterview", static_cast<double>(rdat.fkExaminerInterview)},
                {"mailingDate", rdat.MailingDate.toString(Qt::ISODate)},
                {"dateFiled", rdat.DateFiled.toString(Qt::ISODate)},
                {"fkResponseAsFiled", static_cast<double>(rdat.fkResponseAsFiled)},
                {"fkActionDocument", static_cast<double>(rdat.fkActionDocument)}
            };
            return r;
        }
    });

    // response.update
    m_router.registerAction(QStringLiteral("response.update"), ActionSpec{
        /*fields*/ QList<FieldSpec>{
            FieldSpec{QStringLiteral("id"), QJsonValue::Double, true},
            FieldSpec{QStringLiteral("fkClientOfficeHours"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("fkExaminerInterview"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("mailingDate"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("dateFiled"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("fkResponseAsFiled"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("fkActionDocument"), QJsonValue::Double, false}
        },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            Response resp{id};
            DispatchResult r;
            if (resp.getId() == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Response not found"); return r; }

            if (payload.contains("fkClientOfficeHours")) resp.slotSetfkClientOfficeHours(static_cast<Key>(payload.value("fkClientOfficeHours").toDouble()));
            if (payload.contains("fkExaminerInterview")) resp.slotSetfkExaminerInterview(static_cast<Key>(payload.value("fkExaminerInterview").toDouble()));
            auto parseDate = [](const QJsonObject &obj, const char *key, QDate &out, QString &err) -> bool {
                if (!obj.contains(key)) return true;
                const QString s = obj.value(QString::fromUtf8(key)).toString();
                const QDate d = QDate::fromString(s, Qt::ISODate);
                if (!d.isValid()) { err = QStringLiteral("Invalid date for '%1'").arg(QString::fromUtf8(key)); return false; }
                out = d; return true;
            };
            QString perr; QDate dt;
            if (!parseDate(payload, "mailingDate", dt, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; }
            if (payload.contains("mailingDate")) resp.slotSetMailingDate(dt);
            if (!parseDate(payload, "dateFiled", dt, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; }
            if (payload.contains("dateFiled")) resp.slotSetDateFiled(dt);
            if (payload.contains("fkResponseAsFiled")) resp.slotSetfkResponseAsFiled(static_cast<Key>(payload.value("fkResponseAsFiled").toDouble()));
            if (payload.contains("fkActionDocument")) resp.slotSetfkActionDocument(static_cast<Key>(payload.value("fkActionDocument").toDouble()));

            const ResponseData rdat = Nutdb::GetResponse(id);
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(rdat.ResponseId)},
                {"fkClientOfficeHours", static_cast<double>(rdat.fkClientOfficeHours)},
                {"fkExaminerInterview", static_cast<double>(rdat.fkExaminerInterview)},
                {"mailingDate", rdat.MailingDate.toString(Qt::ISODate)},
                {"dateFiled", rdat.DateFiled.toString(Qt::ISODate)},
                {"fkResponseAsFiled", static_cast<double>(rdat.fkResponseAsFiled)},
                {"fkActionDocument", static_cast<double>(rdat.fkActionDocument)}
            };
            return r;
        }
    });

    // tag.search: search tags by prefix (simple)
    m_router.registerAction(QStringLiteral("tag.search"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("q"), QJsonValue::String, true} },
        /*handler*/ [](const QJsonObject &payload){
            const QString q = payload.value(QStringLiteral("q")).toString();
            DispatchResult r; r.ok = true;
            QJsonArray items;
            // Use the view model via Nutdb; fallback simple LIKE
            QSqlQuery qq(QSqlDatabase::database());
            qq.prepare(QStringLiteral("SELECT TagId, TagText FROM tag WHERE TagText LIKE :q ORDER BY TagText LIMIT 50"));
            qq.bindValue(":q", QString("%1%").arg(q));
            if (qq.exec()) {
                while (qq.next()) {
                    items.append(QJsonObject{{"tagId", static_cast<double>(qq.value(0).toUInt())}, {"tagText", qq.value(1).toString()}});
                }
            }
            r.result = QJsonObject{{"items", items}};
            return r;
        }
    });

    // object.tag: add/remove a tag on an object
    m_router.registerAction(QStringLiteral("object.tag"), ActionSpec{
        /*fields*/ QList<FieldSpec>{
            FieldSpec{QStringLiteral("objectId"), QJsonValue::Double, true},
            FieldSpec{QStringLiteral("tagText"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("removeTagText"), QJsonValue::String, false}
        },
        /*handler*/ [](const QJsonObject &payload){
            const Key objectId = static_cast<Key>(payload.value(QStringLiteral("objectId")).toDouble());
            DispatchResult r; r.ok = true;
            if (payload.contains("tagText")) Nutdb::TagObject(objectId, payload.value("tagText").toString());
            if (payload.contains("removeTagText")) Nutdb::ClearTag(objectId, payload.value("removeTagText").toString());
            // Return updated tags
            QJsonArray tagsArr; for (const auto &tg : Nutdb::GetObjectTags(objectId)) tagsArr.append(QJsonObject{{"tagId", static_cast<double>(tg.TagId)}, {"tagText", tg.TagText}});
            r.result = QJsonObject{{"objectId", static_cast<double>(objectId)}, {"tags", tagsArr}};
            return r;
        }
    });

    // task.get
    m_router.registerAction(QStringLiteral("task.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("id"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            const TaskData t = Nutdb::GetTask(id);
            DispatchResult r;
            if (t.TaskId == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Task not found"); return r; }
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(t.TaskId)},
                {"fkMatter", static_cast<double>(t.fkMatter)},
                {"dateAssigned", t.DateAssigned.toString(Qt::ISODate)},
                {"fkDeadline", static_cast<double>(t.fkDeadline)},
                {"expectedFee", t.ExpectedFee},
                {"feePercent", t.FeePercent},
                {"approvalRequested", t.ApprovalRequested.toString(Qt::ISODate)},
                {"approvalReceived", t.ApprovalReceived.toString(Qt::ISODate)},
                {"complete", t.Complete},
                {"fkWorkAttorney", static_cast<double>(t.fkWorkAttorney)},
                {"fkParalegal", static_cast<double>(t.fkParalegal)},
                {"fkAuthorizationDocument", static_cast<double>(t.fkAuthorizationDocument)},
                {"oldTaskId", static_cast<double>(t.OldTaskId)},
                {"fkTaskType", static_cast<double>(t.fkTaskType)},
                {"taskTypeString", Task{id}.getTaskTypeString()}
            };
            return r;
        }
    });

    // task.update
    m_router.registerAction(QStringLiteral("task.update"), ActionSpec{
        /*fields*/ QList<FieldSpec>{
            FieldSpec{QStringLiteral("id"), QJsonValue::Double, true},
            FieldSpec{QStringLiteral("fkMatter"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("dateAssigned"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("fkDeadline"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("expectedFee"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("feePercent"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("approvalRequested"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("approvalReceived"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("complete"), QJsonValue::Bool, false},
            FieldSpec{QStringLiteral("fkWorkAttorney"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("fkParalegal"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("fkAuthorizationDocument"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("oldTaskId"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("fkTaskType"), QJsonValue::Double, false}
        },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            Task task{id};
            DispatchResult r;
            if (task.getId() == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Task not found"); return r; }
            auto parseDate = [](const QJsonObject &obj, const char *key, QDate &out, QString &err) -> bool {
                if (!obj.contains(key)) return true;
                const QString s = obj.value(QString::fromUtf8(key)).toString();
                const QDate d = QDate::fromString(s, Qt::ISODate);
                if (!d.isValid()) { err = QStringLiteral("Invalid date for '%1'").arg(QString::fromUtf8(key)); return false; }
                out = d; return true;
            };
            QString perr; QDate dt;
            if (payload.contains("fkMatter")) task.SetfkMatter(static_cast<Key>(payload.value("fkMatter").toDouble()));
            if (!parseDate(payload, "dateAssigned", dt, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; }
            if (payload.contains("dateAssigned")) task.SetDateAssigned(dt);
            if (payload.contains("fkDeadline")) task.SetfkDeadline(static_cast<Key>(payload.value("fkDeadline").toDouble()));
            if (payload.contains("expectedFee")) task.SetExpectedFee(static_cast<float>(payload.value("expectedFee").toDouble()));
            if (payload.contains("feePercent")) task.SetFeePercent(static_cast<float>(payload.value("feePercent").toDouble()));
            if (!parseDate(payload, "approvalRequested", dt, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; }
            if (payload.contains("approvalRequested")) task.SetApprovalRequested(dt);
            if (!parseDate(payload, "approvalReceived", dt, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; }
            if (payload.contains("approvalReceived")) task.SetApprovalReceived(dt);
            if (payload.contains("complete")) task.SetComplete(payload.value("complete").toBool());
            if (payload.contains("fkWorkAttorney")) task.SetfkWorkAttorney(static_cast<Key>(payload.value("fkWorkAttorney").toDouble()));
            if (payload.contains("fkParalegal")) task.SetfkParalegal(static_cast<Key>(payload.value("fkParalegal").toDouble()));
            if (payload.contains("fkAuthorizationDocument")) task.SetfkAuthorizationDocument(static_cast<Key>(payload.value("fkAuthorizationDocument").toDouble()));
            if (payload.contains("oldTaskId")) task.SetOldTaskId(static_cast<Key>(payload.value("oldTaskId").toDouble()));
            if (payload.contains("fkTaskType")) task.SetfkTaskType(static_cast<Key>(payload.value("fkTaskType").toDouble()));

            const TaskData t = Nutdb::GetTask(id);
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(t.TaskId)},
                {"fkMatter", static_cast<double>(t.fkMatter)},
                {"dateAssigned", t.DateAssigned.toString(Qt::ISODate)},
                {"fkDeadline", static_cast<double>(t.fkDeadline)},
                {"expectedFee", t.ExpectedFee},
                {"feePercent", t.FeePercent},
                {"approvalRequested", t.ApprovalRequested.toString(Qt::ISODate)},
                {"approvalReceived", t.ApprovalReceived.toString(Qt::ISODate)},
                {"complete", t.Complete},
                {"fkWorkAttorney", static_cast<double>(t.fkWorkAttorney)},
                {"fkParalegal", static_cast<double>(t.fkParalegal)},
                {"fkAuthorizationDocument", static_cast<double>(t.fkAuthorizationDocument)},
                {"oldTaskId", static_cast<double>(t.OldTaskId)},
                {"fkTaskType", static_cast<double>(t.fkTaskType)},
                {"taskTypeString", Task{id}.getTaskTypeString()}
            };
            return r;
        }
    });

    // trademarkMatter.get
    m_router.registerAction(QStringLiteral("trademarkMatter.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("id"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            const TrademarkMatterData m = Nutdb::GetTrademarkMatter(id);
            DispatchResult r;
            if (m.TrademarkMatterId == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("TrademarkMatter not found"); return r; }
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(m.TrademarkMatterId)},
                {"firstUseInCommerce", m.FirstUseInCommerce.toString(Qt::ISODate)},
                {"internationalClass", m.InternationalClass},
                {"fkStatus", static_cast<double>(m.fkStatus)},
                {"serialNumber", m.SerialNumber},
                {"registrationNumber", m.RegistrationNumber},
                {"publicationDate", m.PublicationDate.toString(Qt::ISODate)},
                {"windowOpens", m.WindowOpens.toString(Qt::ISODate)},
                {"nofeeWindowCloses", m.NofeeWindowCloses.toString(Qt::ISODate)},
                {"finalWindowCloses", m.FinalWindowCloses.toString(Qt::ISODate)},
                {"fkTrademarkExaminer", static_cast<double>(m.fkTrademarkExaminer)},
                {"fkFilingBasis", static_cast<double>(m.fkFilingBasis)},
                {"fkTrademarkJurisdiction", static_cast<double>(m.fkTrademarkJurisdiction)},
                {"fkSpecimen", static_cast<double>(m.fkSpecimen)},
                {"fkEvidenceOfUse", static_cast<double>(m.fkEvidenceOfUse)},
                {"mark", m.Mark},
                {"goodsServices", m.GoodsServices}
            };
            return r;
        }
    });

    // trademarkMatter.update
    m_router.registerAction(QStringLiteral("trademarkMatter.update"), ActionSpec{
        /*fields*/ QList<FieldSpec>{
            FieldSpec{QStringLiteral("id"), QJsonValue::Double, true},
            FieldSpec{QStringLiteral("firstUseInCommerce"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("internationalClass"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("fkStatus"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("serialNumber"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("registrationNumber"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("publicationDate"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("windowOpens"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("nofeeWindowCloses"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("finalWindowCloses"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("fkTrademarkExaminer"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("fkFilingBasis"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("fkTrademarkJurisdiction"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("fkSpecimen"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("fkEvidenceOfUse"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("mark"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("goodsServices"), QJsonValue::String, false}
        },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            TrademarkMatter tm{id};
            DispatchResult r;
            if (tm.getId() == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("TrademarkMatter not found"); return r; }

            auto parseDate = [](const QJsonObject &obj, const char *key, QDate &out, QString &err) -> bool {
                if (!obj.contains(key)) return true;
                const QString s = obj.value(QString::fromUtf8(key)).toString();
                const QDate d = QDate::fromString(s, Qt::ISODate);
                if (!d.isValid()) { err = QStringLiteral("Invalid date for '%1'").arg(QString::fromUtf8(key)); return false; }
                out = d; return true;
            };
            QString perr; QDate dt;
            if (!parseDate(payload, "firstUseInCommerce", dt, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; }
            if (payload.contains("firstUseInCommerce")) tm.slotSetFirstUseInCommerce(dt);
            if (payload.contains("internationalClass")) tm.slotSetInternationalClass(static_cast<int>(payload.value("internationalClass").toDouble()));
            if (payload.contains("fkStatus")) tm.slotSetfkStatus(static_cast<Key>(payload.value("fkStatus").toDouble()));
            if (payload.contains("serialNumber")) tm.slotSetSerialNumber(payload.value("serialNumber").toString());
            if (payload.contains("registrationNumber")) tm.slotSetRegistrationNumber(payload.value("registrationNumber").toString());
            if (!parseDate(payload, "publicationDate", dt, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; }
            if (payload.contains("publicationDate")) tm.slotSetPublicationDate(dt);
            if (!parseDate(payload, "windowOpens", dt, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; }
            if (payload.contains("windowOpens")) tm.slotSetWindowOpens(dt);
            if (!parseDate(payload, "nofeeWindowCloses", dt, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; }
            if (payload.contains("nofeeWindowCloses")) tm.slotSetNofeeWindowCloses(dt);
            if (!parseDate(payload, "finalWindowCloses", dt, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; }
            if (payload.contains("finalWindowCloses")) tm.slotSetFinalWindowCloses(dt);
            if (payload.contains("fkTrademarkExaminer")) tm.slotSetfkTrademarkExaminer(static_cast<Key>(payload.value("fkTrademarkExaminer").toDouble()));
            if (payload.contains("fkFilingBasis")) tm.slotSetfkFilingBasis(static_cast<Key>(payload.value("fkFilingBasis").toDouble()));
            if (payload.contains("fkTrademarkJurisdiction")) tm.slotSetfkTrademarkJurisdiction(static_cast<Key>(payload.value("fkTrademarkJurisdiction").toDouble()));
            if (payload.contains("fkSpecimen")) tm.slotSetfkSpecimen(static_cast<Key>(payload.value("fkSpecimen").toDouble()));
            if (payload.contains("fkEvidenceOfUse")) tm.slotSetfkEvidenceOfUse(static_cast<Key>(payload.value("fkEvidenceOfUse").toDouble()));
            if (payload.contains("mark")) tm.slotSetMark(payload.value("mark").toString());
            if (payload.contains("goodsServices")) tm.slotSetGoodsServices(payload.value("goodsServices").toString());

            const TrademarkMatterData m = Nutdb::GetTrademarkMatter(id);
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(m.TrademarkMatterId)},
                {"firstUseInCommerce", m.FirstUseInCommerce.toString(Qt::ISODate)},
                {"internationalClass", m.InternationalClass},
                {"fkStatus", static_cast<double>(m.fkStatus)},
                {"serialNumber", m.SerialNumber},
                {"registrationNumber", m.RegistrationNumber},
                {"publicationDate", m.PublicationDate.toString(Qt::ISODate)},
                {"windowOpens", m.WindowOpens.toString(Qt::ISODate)},
                {"nofeeWindowCloses", m.NofeeWindowCloses.toString(Qt::ISODate)},
                {"finalWindowCloses", m.FinalWindowCloses.toString(Qt::ISODate)},
                {"fkTrademarkExaminer", static_cast<double>(m.fkTrademarkExaminer)},
                {"fkFilingBasis", static_cast<double>(m.fkFilingBasis)},
                {"fkTrademarkJurisdiction", static_cast<double>(m.fkTrademarkJurisdiction)},
                {"fkSpecimen", static_cast<double>(m.fkSpecimen)},
                {"fkEvidenceOfUse", static_cast<double>(m.fkEvidenceOfUse)},
                {"mark", m.Mark},
                {"goodsServices", m.GoodsServices}
            };
            return r;
        }
    });

    // patentMatter.get
    m_router.registerAction(QStringLiteral("patentMatter.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("id"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            const PatentMatterData m = Nutdb::GetPatentMatter(id);
            DispatchResult r;
            if (m.PatentMatterId == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("PatentMatter not found"); return r; }
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(m.PatentMatterId)},
                {"filingDate", m.FilingDate.toString(Qt::ISODate)},
                {"applicationSerialNumber", m.ApplicationSerialNumber},
                {"confirmationNumber", m.ConfirmationNumber},
                {"artUnit", m.ArtUnit},
                {"patentNumber", m.PatentNumber},
                {"fkExaminer", static_cast<double>(m.fkExaminer)},
                {"fkFirstInventor", static_cast<double>(m.fkFirstInventor)},
                {"fkSupervisoryExaminer", static_cast<double>(m.fkSupervisoryExaminer)},
                {"fkApplicant", static_cast<double>(m.fkApplicant)},
                {"barDate", m.BarDate.toString(Qt::ISODate)},
                {"criticalDate", m.CriticalDate.toString(Qt::ISODate)},
                {"dateIssued", m.DateIssued.toString(Qt::ISODate)},
                {"fkSpecification", static_cast<double>(m.fkSpecification)},
                {"fkDrawings", static_cast<double>(m.fkDrawings)},
                {"fkAsFiledClaims", static_cast<double>(m.fkAsFiledClaims)}
            };
            return r;
        }
    });

    // patentMatter.update
    m_router.registerAction(QStringLiteral("patentMatter.update"), ActionSpec{
        /*fields*/ QList<FieldSpec>{
            FieldSpec{QStringLiteral("id"), QJsonValue::Double, true},
            FieldSpec{QStringLiteral("filingDate"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("applicationSerialNumber"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("confirmationNumber"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("artUnit"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("patentNumber"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("fkExaminer"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("fkFirstInventor"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("fkSupervisoryExaminer"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("fkApplicant"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("barDate"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("criticalDate"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("dateIssued"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("fkSpecification"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("fkDrawings"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("fkAsFiledClaims"), QJsonValue::Double, false}
        },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            DispatchResult r;
            const PatentMatterData cur = Nutdb::GetPatentMatter(id);
            if (cur.PatentMatterId == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("PatentMatter not found"); return r; }

            auto parseDate = [](const QJsonObject &obj, const char *key, QDate &out, QString &err) -> bool {
                if (!obj.contains(key)) return true;
                const QString s = obj.value(QString::fromUtf8(key)).toString();
                const QDate d = QDate::fromString(s, Qt::ISODate);
                if (!d.isValid()) { err = QStringLiteral("Invalid date for '%1'").arg(QString::fromUtf8(key)); return false; }
                out = d; return true;
            };
            QString perr; QDate dt;
            if (!parseDate(payload, "filingDate", dt, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; }
            if (payload.contains("filingDate")) Nutdb::UpdateField(QStringLiteral("patentMatter"), QStringLiteral("FilingDate"), id, dt.toString(Qt::ISODate));
            if (payload.contains("applicationSerialNumber")) Nutdb::UpdateField(QStringLiteral("patentMatter"), QStringLiteral("ApplicationSerialNumber"), id, payload.value("applicationSerialNumber").toString());
            if (payload.contains("confirmationNumber")) Nutdb::UpdateField(QStringLiteral("patentMatter"), QStringLiteral("ConfirmationNumber"), id, payload.value("confirmationNumber").toString());
            if (payload.contains("artUnit")) Nutdb::UpdateField(QStringLiteral("patentMatter"), QStringLiteral("ArtUnit"), id, payload.value("artUnit").toString());
            if (payload.contains("patentNumber")) Nutdb::UpdateField(QStringLiteral("patentMatter"), QStringLiteral("PatentNumber"), id, payload.value("patentNumber").toString());
            if (payload.contains("fkExaminer")) Nutdb::UpdateField(QStringLiteral("patentMatter"), QStringLiteral("fkExaminer"), id, QString::number(static_cast<quint64>(payload.value("fkExaminer").toDouble())));
            if (payload.contains("fkFirstInventor")) Nutdb::UpdateField(QStringLiteral("patentMatter"), QStringLiteral("fkFirstInventor"), id, QString::number(static_cast<quint64>(payload.value("fkFirstInventor").toDouble())));
            if (payload.contains("fkSupervisoryExaminer")) Nutdb::UpdateField(QStringLiteral("patentMatter"), QStringLiteral("fkSupervisoryExaminer"), id, QString::number(static_cast<quint64>(payload.value("fkSupervisoryExaminer").toDouble())));
            if (payload.contains("fkApplicant")) Nutdb::UpdateField(QStringLiteral("patentMatter"), QStringLiteral("fkApplicant"), id, QString::number(static_cast<quint64>(payload.value("fkApplicant").toDouble())));
            if (!parseDate(payload, "barDate", dt, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; }
            if (payload.contains("barDate")) Nutdb::UpdateField(QStringLiteral("patentMatter"), QStringLiteral("BarDate"), id, dt.toString(Qt::ISODate));
            if (!parseDate(payload, "criticalDate", dt, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; }
            if (payload.contains("criticalDate")) Nutdb::UpdateField(QStringLiteral("patentMatter"), QStringLiteral("CriticalDate"), id, dt.toString(Qt::ISODate));
            if (!parseDate(payload, "dateIssued", dt, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; }
            if (payload.contains("dateIssued")) Nutdb::UpdateField(QStringLiteral("patentMatter"), QStringLiteral("DateIssued"), id, dt.toString(Qt::ISODate));
            if (payload.contains("fkSpecification")) Nutdb::UpdateField(QStringLiteral("patentMatter"), QStringLiteral("fkSpecification"), id, QString::number(static_cast<quint64>(payload.value("fkSpecification").toDouble())));
            if (payload.contains("fkDrawings")) Nutdb::UpdateField(QStringLiteral("patentMatter"), QStringLiteral("fkDrawings"), id, QString::number(static_cast<quint64>(payload.value("fkDrawings").toDouble())));
            if (payload.contains("fkAsFiledClaims")) Nutdb::UpdateField(QStringLiteral("patentMatter"), QStringLiteral("fkAsFiledClaims"), id, QString::number(static_cast<quint64>(payload.value("fkAsFiledClaims").toDouble())));

            const PatentMatterData m = Nutdb::GetPatentMatter(id);
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(m.PatentMatterId)},
                {"filingDate", m.FilingDate.toString(Qt::ISODate)},
                {"applicationSerialNumber", m.ApplicationSerialNumber},
                {"confirmationNumber", m.ConfirmationNumber},
                {"artUnit", m.ArtUnit},
                {"patentNumber", m.PatentNumber},
                {"fkExaminer", static_cast<double>(m.fkExaminer)},
                {"fkFirstInventor", static_cast<double>(m.fkFirstInventor)},
                {"fkSupervisoryExaminer", static_cast<double>(m.fkSupervisoryExaminer)},
                {"fkApplicant", static_cast<double>(m.fkApplicant)},
                {"barDate", m.BarDate.toString(Qt::ISODate)},
                {"criticalDate", m.CriticalDate.toString(Qt::ISODate)},
                {"dateIssued", m.DateIssued.toString(Qt::ISODate)},
                {"fkSpecification", static_cast<double>(m.fkSpecification)},
                {"fkDrawings", static_cast<double>(m.fkDrawings)},
                {"fkAsFiledClaims", static_cast<double>(m.fkAsFiledClaims)}
            };
            return r;
        }
    });

    // responsesDashboardEntry.get (read-only)
    m_router.registerAction(QStringLiteral("responsesDashboardEntry.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("taskId"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){
            const Key taskId = static_cast<Key>(payload.value(QStringLiteral("taskId")).toDouble());
            QSqlQuery q(QSqlDatabase::database());
            q.prepare(QStringLiteral("SELECT * FROM responsesDashboardComplete WHERE TaskId = :id"));
            q.bindValue(":id", QVariant::fromValue(taskId));
            DispatchResult r;
            if (!q.exec() || !q.next()) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("responsesDashboardEntry not found"); return r; }
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
                {"examinerInterviewScheduled", rec.field("ExaminerInterviewScheduled").value().toBool()}
            };
            return r;
        }
    });

    // responsesDashboardEntry.list (read-only)
    m_router.registerAction(QStringLiteral("responsesDashboardEntry.list"), ActionSpec{
        /*fields*/ QList<FieldSpec>{
            FieldSpec{QStringLiteral("limit"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("offset"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("clientEntityId"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("withParalegal"), QJsonValue::Bool, false}
        },
        /*handler*/ [](const QJsonObject &payload){
            int limit = payload.contains("limit") ? qBound(1, payload.value("limit").toInt(), 500) : 200;
            int offset = payload.contains("offset") ? qMax(0, payload.value("offset").toInt()) : 0;

            QString sql = QStringLiteral("SELECT * FROM responsesDashboardComplete");
            QStringList where;
            if (payload.contains("clientEntityId")) where << QStringLiteral("ClientEntityId = :clientEntityId");
            if (payload.contains("withParalegal")) where << QStringLiteral("WithParalegal = :withParalegal");
            if (!where.isEmpty()) sql += QStringLiteral(" WHERE ") + where.join(QStringLiteral(" AND "));
            sql += QStringLiteral(" ORDER BY NextDeadline ASC, TaskId ASC LIMIT :limit OFFSET :offset");

            QSqlQuery q(QSqlDatabase::database());
            q.prepare(sql);
            if (payload.contains("clientEntityId")) q.bindValue(":clientEntityId", payload.value("clientEntityId").toInt());
            if (payload.contains("withParalegal")) q.bindValue(":withParalegal", payload.value("withParalegal").toBool());
            q.bindValue(":limit", limit);
            q.bindValue(":offset", offset);

            DispatchResult r; r.ok = true;
            QJsonArray items;
            if (q.exec()) {
                while (q.next()) {
                    const QSqlRecord rec = q.record();
                    items.append(QJsonObject{
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
                        {"examinerInterviewScheduled", rec.field("ExaminerInterviewScheduled").value().toBool()}
                    });
                }
            }
            r.result = QJsonObject{{"items", items}, {"limit", limit}, {"offset", offset}};
            return r;
        }
    });

    // tag.get: by id or text
    m_router.registerAction(QStringLiteral("tag.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{
            FieldSpec{QStringLiteral("id"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("text"), QJsonValue::String, false}
        },
        /*handler*/ [](const QJsonObject &payload){
            DispatchResult r;
            const bool hasId = payload.contains(QStringLiteral("id"));
            const bool hasText = payload.contains(QStringLiteral("text"));
            if (!hasId && !hasText) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("Provide 'id' or 'text'"); return r; }
            TagData td;
            if (hasId) td = Nutdb::GetTag(static_cast<Key>(payload.value(QStringLiteral("id")).toDouble()));
            else {
                const Key id = Nutdb::GetTagId(payload.value(QStringLiteral("text")).toString());
                td = Nutdb::GetTag(id);
            }
            if (td.TagId == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Tag not found"); return r; }
            r.ok = true;
            r.result = QJsonObject{{"tagId", static_cast<double>(td.TagId)}, {"tagText", td.TagText}};
            return r;
        }
    });

    // taskClass.get: by id
    m_router.registerAction(QStringLiteral("taskClass.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("id"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            QSqlQuery q(QSqlDatabase::database());
            q.prepare(QStringLiteral("SELECT TaskClassId, TaskClassName FROM viewTaskClass WHERE TaskClassId = :id"));
            q.bindValue(":id", QVariant::fromValue(id));
            DispatchResult r;
            if (!q.exec() || !q.next()) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("TaskClass not found"); return r; }
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(q.value(0).toUInt())},
                {"taskClassName", q.value(1).toString()}
            };
            return r;
        }
    });

    // taskType.get: by id
    m_router.registerAction(QStringLiteral("taskType.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("id"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            QSqlQuery q(QSqlDatabase::database());
            q.prepare(QStringLiteral("SELECT TaskTypeId, fkTaskClass, TaskName FROM taskType WHERE TaskTypeId = :id"));
            q.bindValue(":id", QVariant::fromValue(id));
            DispatchResult r;
            if (!q.exec() || !q.next()) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("TaskType not found"); return r; }
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(q.value(0).toUInt())},
                {"fkTaskClass", static_cast<double>(q.value(1).toUInt())},
                {"taskName", q.value(2).toString()}
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
