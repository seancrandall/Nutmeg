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
    : WebSocketServer(QHostAddress::Any, port, parent)
{
}

WebSocketServer::WebSocketServer(const QHostAddress &addr, quint16 port, QObject *parent)
    : QObject(parent)
    , m_server(new QWebSocketServer(QStringLiteral("NutmegBroker"),
                                    QWebSocketServer::NonSecureMode,
                                    this))
{
    const bool ok = m_server->listen(addr, port);
    if (!ok) {
        qWarning() << "WebSocketServer failed to listen on" << addr.toString() << ":" << port
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

    // objectTagView.get (read-only via viewObjectTags) by id or objectId+tagText/tagId
    m_router.registerAction(QStringLiteral("objectTagView.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{
            FieldSpec{QStringLiteral("id"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("objectId"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("tagId"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("tagText"), QJsonValue::String, false}
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
            const bool hasTagId = payload.contains(QStringLiteral("tagId"));
            const bool hasTagText = payload.contains(QStringLiteral("tagText"));
            if (!hasId && !hasObj) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("Provide 'id' or objectId + (tagId|tagText)"); return r; }

            if (hasId) {
                const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
                const QStringList idCols{ QStringLiteral("ObjectTagId"), QStringLiteral("Id") };
                for (const QString &col : idCols) {
                    QSqlQuery q(QSqlDatabase::database());
                    q.prepare(QStringLiteral("SELECT * FROM viewObjectTags WHERE ") + col + QStringLiteral(" = :id LIMIT 1"));
                    q.bindValue(":id", QVariant::fromValue(id));
                    if (q.exec() && q.next()) { r.ok = true; r.result = packRecord(q.record()); return r; }
                }
            } else {
                const Key objectId = static_cast<Key>(payload.value(QStringLiteral("objectId")).toDouble());
                if (hasTagId) {
                    QSqlQuery q(QSqlDatabase::database());
                    q.prepare(QStringLiteral("SELECT * FROM viewObjectTags WHERE ObjectId = :oid AND TagId = :tid LIMIT 1"));
                    q.bindValue(":oid", QVariant::fromValue(objectId));
                    q.bindValue(":tid", QVariant::fromValue(static_cast<Key>(payload.value(QStringLiteral("tagId")).toDouble())));
                    if (q.exec() && q.next()) { r.ok = true; r.result = packRecord(q.record()); return r; }
                }
                if (hasTagText) {
                    QSqlQuery q(QSqlDatabase::database());
                    q.prepare(QStringLiteral("SELECT * FROM viewObjectTags WHERE ObjectId = :oid AND TagText = :tt LIMIT 1"));
                    q.bindValue(":oid", QVariant::fromValue(objectId));
                    q.bindValue(":tt", payload.value(QStringLiteral("tagText")).toString());
                    if (q.exec() && q.next()) { r.ok = true; r.result = packRecord(q.record()); return r; }
                }
            }
            r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Object tag not found");
            return r;
        }
    });

    // objectTagView.listForObject (read-only)
    m_router.registerAction(QStringLiteral("objectTagView.listForObject"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("objectId"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){
            const Key objectId = static_cast<Key>(payload.value(QStringLiteral("objectId")).toDouble());
            QSqlQuery q(QSqlDatabase::database());
            q.prepare(QStringLiteral("SELECT * FROM viewObjectTags WHERE ObjectId = :oid ORDER BY TagText"));
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

    // paralegalView.get (read-only via viewParalegals)
    m_router.registerAction(QStringLiteral("paralegalView.get"), ActionSpec{
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
                QStringLiteral("ParalegalEntityId"),
                QStringLiteral("EntityId"),
                QStringLiteral("PersonId"),
                QStringLiteral("Id")
            };

            DispatchResult r;
            for (const QString &col : idColumns) {
                QSqlQuery q(QSqlDatabase::database());
                q.prepare(QStringLiteral("SELECT * FROM viewParalegals WHERE ") + col + QStringLiteral(" = :id LIMIT 1"));
                q.bindValue(":id", QVariant::fromValue(id));
                if (q.exec() && q.next()) { r.ok = true; r.result = packRecord(q.record()); return r; }
            }
            r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Paralegal not found");
            return r;
        }
    });

    // paralegalView.list (read-only)
    m_router.registerAction(QStringLiteral("paralegalView.list"), ActionSpec{
        /*fields*/ QList<FieldSpec>{},
        /*handler*/ [](const QJsonObject &){
            QSqlQuery q(QSqlDatabase::database());
            bool ok = q.exec(QStringLiteral("SELECT * FROM viewParalegals"));
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

    // patentExaminerView.get (read-only via viewPatentExaminers)
    m_router.registerAction(QStringLiteral("patentExaminerView.get"), ActionSpec{
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
                QStringLiteral("EntityId"),
                QStringLiteral("PatentExaminerEntityId"),
                QStringLiteral("ExaminerId"),
                QStringLiteral("Id")
            };

            DispatchResult r;
            for (const QString &col : idColumns) {
                QSqlQuery q(QSqlDatabase::database());
                q.prepare(QStringLiteral("SELECT * FROM viewPatentExaminers WHERE ") + col + QStringLiteral(" = :id LIMIT 1"));
                q.bindValue(":id", QVariant::fromValue(id));
                if (q.exec() && q.next()) { r.ok = true; r.result = packRecord(q.record()); return r; }
            }
            r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Patent examiner not found");
            return r;
        }
    });

    // patentExaminerView.list (read-only)
    m_router.registerAction(QStringLiteral("patentExaminerView.list"), ActionSpec{
        /*fields*/ QList<FieldSpec>{},
        /*handler*/ [](const QJsonObject &){
            QSqlQuery q(QSqlDatabase::database());
            bool ok = q.exec(QStringLiteral("SELECT * FROM viewPatentExaminers"));
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

    // patentMatterView.get (read-only via viewPatentMatters)
    m_router.registerAction(QStringLiteral("patentMatterView.get"), ActionSpec{
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
                QStringLiteral("PatentMatterId"),
                QStringLiteral("MatterId"),
                QStringLiteral("Id")
            };

            DispatchResult r;
            for (const QString &col : idColumns) {
                QSqlQuery q(QSqlDatabase::database());
                q.prepare(QStringLiteral("SELECT * FROM viewPatentMatters WHERE ") + col + QStringLiteral(" = :id LIMIT 1"));
                q.bindValue(":id", QVariant::fromValue(id));
                if (q.exec() && q.next()) { r.ok = true; r.result = packRecord(q.record()); return r; }
            }
            r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Patent matter not found in view");
            return r;
        }
    });

    // patentMatterView.list (read-only)
    m_router.registerAction(QStringLiteral("patentMatterView.list"), ActionSpec{
        /*fields*/ QList<FieldSpec>{},
        /*handler*/ [](const QJsonObject &){
            QSqlQuery q(QSqlDatabase::database());
            bool ok = q.exec(QStringLiteral("SELECT * FROM viewPatentMatters"));
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

    // peopleView.get (read-only via viewPeople)
    m_router.registerAction(QStringLiteral("peopleView.get"), ActionSpec{
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
                QStringLiteral("EntityId"),
                QStringLiteral("Id")
            };

            DispatchResult r;
            for (const QString &col : idColumns) {
                QSqlQuery q(QSqlDatabase::database());
                q.prepare(QStringLiteral("SELECT * FROM viewPeople WHERE ") + col + QStringLiteral(" = :id LIMIT 1"));
                q.bindValue(":id", QVariant::fromValue(id));
                if (q.exec() && q.next()) { r.ok = true; r.result = packRecord(q.record()); return r; }
            }
            r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Person not found in view");
            return r;
        }
    });

    // peopleView.list (read-only)
    m_router.registerAction(QStringLiteral("peopleView.list"), ActionSpec{
        /*fields*/ QList<FieldSpec>{},
        /*handler*/ [](const QJsonObject &){
            QSqlQuery q(QSqlDatabase::database());
            bool ok = q.exec(QStringLiteral("SELECT * FROM viewPeople"));
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

    // responseView.get (read-only via viewResponses)
    m_router.registerAction(QStringLiteral("responseView.get"), ActionSpec{
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
                QStringLiteral("ResponseId"),
                QStringLiteral("PatentResponseId"),
                QStringLiteral("TrademarkResponseId"),
                QStringLiteral("Id")
            };

            DispatchResult r;
            for (const QString &col : idColumns) {
                QSqlQuery q(QSqlDatabase::database());
                q.prepare(QStringLiteral("SELECT * FROM viewResponses WHERE ") + col + QStringLiteral(" = :id LIMIT 1"));
                q.bindValue(":id", QVariant::fromValue(id));
                if (q.exec() && q.next()) { r.ok = true; r.result = packRecord(q.record()); return r; }
            }
            r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Response not found in view");
            return r;
        }
    });

    // responseView.list (read-only)
    m_router.registerAction(QStringLiteral("responseView.list"), ActionSpec{
        /*fields*/ QList<FieldSpec>{},
        /*handler*/ [](const QJsonObject &){
            QSqlQuery q(QSqlDatabase::database());
            bool ok = q.exec(QStringLiteral("SELECT * FROM viewResponses"));
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

    // responsesIncompleteView.get (read-only via viewResponsesIncomplete)
    m_router.registerAction(QStringLiteral("responsesIncompleteView.get"), ActionSpec{
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
                QStringLiteral("ResponseId"),
                QStringLiteral("PatentResponseId"),
                QStringLiteral("TrademarkResponseId"),
                QStringLiteral("Id")
            };

            DispatchResult r;
            for (const QString &col : idColumns) {
                QSqlQuery q(QSqlDatabase::database());
                q.prepare(QStringLiteral("SELECT * FROM viewResponsesIncomplete WHERE ") + col + QStringLiteral(" = :id LIMIT 1"));
                q.bindValue(":id", QVariant::fromValue(id));
                if (q.exec() && q.next()) { r.ok = true; r.result = packRecord(q.record()); return r; }
            }
            r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Response (incomplete) not found");
            return r;
        }
    });

    // responsesIncompleteView.list (read-only)
    m_router.registerAction(QStringLiteral("responsesIncompleteView.list"), ActionSpec{
        /*fields*/ QList<FieldSpec>{},
        /*handler*/ [](const QJsonObject &){
            QSqlQuery q(QSqlDatabase::database());
            bool ok = q.exec(QStringLiteral("SELECT * FROM viewResponsesIncomplete"));
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

    // responseTaskTypeView.get (read-only via viewResponseTaskTypes)
    m_router.registerAction(QStringLiteral("responseTaskTypeView.get"), ActionSpec{
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
                QStringLiteral("ResponseTaskTypeId"),
                QStringLiteral("TaskTypeId"),
                QStringLiteral("Id")
            };

            DispatchResult r;
            for (const QString &col : idColumns) {
                QSqlQuery q(QSqlDatabase::database());
                q.prepare(QStringLiteral("SELECT * FROM viewResponseTaskTypes WHERE ") + col + QStringLiteral(" = :id LIMIT 1"));
                q.bindValue(":id", QVariant::fromValue(id));
                if (q.exec() && q.next()) { r.ok = true; r.result = packRecord(q.record()); return r; }
            }
            r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Response task type not found");
            return r;
        }
    });

    // responseTaskTypeView.list (read-only)
    m_router.registerAction(QStringLiteral("responseTaskTypeView.list"), ActionSpec{
        /*fields*/ QList<FieldSpec>{},
        /*handler*/ [](const QJsonObject &){
            QSqlQuery q(QSqlDatabase::database());
            bool ok = q.exec(QStringLiteral("SELECT * FROM viewResponseTaskTypes"));
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

    // taskView.get (read-only via viewTasks)
    m_router.registerAction(QStringLiteral("taskView.get"), ActionSpec{
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
                QStringLiteral("TaskId"),
                QStringLiteral("Id")
            };

            DispatchResult r;
            for (const QString &col : idColumns) {
                QSqlQuery q(QSqlDatabase::database());
                q.prepare(QStringLiteral("SELECT * FROM viewTasks WHERE ") + col + QStringLiteral(" = :id LIMIT 1"));
                q.bindValue(":id", QVariant::fromValue(id));
                if (q.exec() && q.next()) { r.ok = true; r.result = packRecord(q.record()); return r; }
            }
            r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Task not found in view");
            return r;
        }
    });

    // taskView.list (read-only)
    m_router.registerAction(QStringLiteral("taskView.list"), ActionSpec{
        /*fields*/ QList<FieldSpec>{},
        /*handler*/ [](const QJsonObject &){
            QSqlQuery q(QSqlDatabase::database());
            bool ok = q.exec(QStringLiteral("SELECT * FROM viewTasks"));
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

    // taskClass.list (read-only via viewTaskClass) — taskClass.get already exists
    m_router.registerAction(QStringLiteral("taskClass.list"), ActionSpec{
        /*fields*/ QList<FieldSpec>{},
        /*handler*/ [](const QJsonObject &){
            QSqlQuery q(QSqlDatabase::database());
            bool ok = q.exec(QStringLiteral("SELECT TaskClassId, TaskClassName FROM viewTaskClass ORDER BY TaskClassName"));
            DispatchResult r; r.ok = true;
            QJsonArray items;
            if (ok) {
                while (q.next()) {
                    items.append(QJsonObject{
                        {"id", static_cast<double>(q.value(0).toUInt())},
                        {"taskClassName", q.value(1).toString()}
                    });
                }
            }
            r.result = QJsonObject{{"items", items}};
            return r;
        }
    });

    // taskTypeView.get (read-only via viewTaskTypes)
    m_router.registerAction(QStringLiteral("taskTypeView.get"), ActionSpec{
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
                QStringLiteral("TaskTypeId"),
                QStringLiteral("Id")
            };

            DispatchResult r;
            for (const QString &col : idColumns) {
                QSqlQuery q(QSqlDatabase::database());
                q.prepare(QStringLiteral("SELECT * FROM viewTaskTypes WHERE ") + col + QStringLiteral(" = :id LIMIT 1"));
                q.bindValue(":id", QVariant::fromValue(id));
                if (q.exec() && q.next()) { r.ok = true; r.result = packRecord(q.record()); return r; }
            }
            r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Task type not found in view");
            return r;
        }
    });

    // taskTypeView.list (read-only)
    m_router.registerAction(QStringLiteral("taskTypeView.list"), ActionSpec{
        /*fields*/ QList<FieldSpec>{},
        /*handler*/ [](const QJsonObject &){
            QSqlQuery q(QSqlDatabase::database());
            bool ok = q.exec(QStringLiteral("SELECT * FROM viewTaskTypes"));
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

    // trademarkFilingTypeView.get (read-only via viewTrademarkFilingTypes)
    m_router.registerAction(QStringLiteral("trademarkFilingTypeView.get"), ActionSpec{
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
                QStringLiteral("TrademarkFilingTypeId"),
                QStringLiteral("FilingTypeId"),
                QStringLiteral("TaskTypeId"),
                QStringLiteral("Id")
            };

            DispatchResult r;
            for (const QString &col : idColumns) {
                QSqlQuery q(QSqlDatabase::database());
                q.prepare(QStringLiteral("SELECT * FROM viewTrademarkFilingTypes WHERE ") + col + QStringLiteral(" = :id LIMIT 1"));
                q.bindValue(":id", QVariant::fromValue(id));
                if (q.exec() && q.next()) { r.ok = true; r.result = packRecord(q.record()); return r; }
            }
            r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Trademark filing type not found");
            return r;
        }
    });

    // trademarkFilingTypeView.list (read-only)
    m_router.registerAction(QStringLiteral("trademarkFilingTypeView.list"), ActionSpec{
        /*fields*/ QList<FieldSpec>{},
        /*handler*/ [](const QJsonObject &){
            QSqlQuery q(QSqlDatabase::database());
            bool ok = q.exec(QStringLiteral("SELECT * FROM viewTrademarkFilingTypes"));
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

    // patentFilingTypeView.get (read-only via viewPatentFilingTypes)
    m_router.registerAction(QStringLiteral("patentFilingTypeView.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("id"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            auto packRecord = [](const QSqlRecord &rec){ QJsonObject obj; for (int i = 0; i < rec.count(); ++i) { const QString name = rec.fieldName(i); const QVariant v = rec.value(i); switch (v.typeId()) { case QMetaType::Bool: obj.insert(name, v.toBool()); break; case QMetaType::Int: case QMetaType::UInt: case QMetaType::LongLong: case QMetaType::ULongLong: case QMetaType::Double: obj.insert(name, v.toDouble()); break; case QMetaType::QDate: obj.insert(name, v.toDate().toString(Qt::ISODate)); break; case QMetaType::QDateTime: obj.insert(name, v.toDateTime().toString(Qt::ISODate)); break; default: obj.insert(name, v.toString()); break; } } return obj; };
            const QStringList idColumns{ QStringLiteral("PatentFilingTypeId"), QStringLiteral("FilingTypeId"), QStringLiteral("TaskTypeId"), QStringLiteral("Id") };
            DispatchResult r; for (const QString &col : idColumns) { QSqlQuery q(QSqlDatabase::database()); q.prepare(QStringLiteral("SELECT * FROM viewPatentFilingTypes WHERE ") + col + QStringLiteral(" = :id LIMIT 1")); q.bindValue(":id", QVariant::fromValue(id)); if (q.exec() && q.next()) { r.ok = true; r.result = packRecord(q.record()); return r; } }
            r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Patent filing type not found"); return r;
        }
    });

    // patentFilingTypeView.list (read-only)
    m_router.registerAction(QStringLiteral("patentFilingTypeView.list"), ActionSpec{
        /*fields*/ QList<FieldSpec>{},
        /*handler*/ [](const QJsonObject &){ QSqlQuery q(QSqlDatabase::database()); bool ok = q.exec(QStringLiteral("SELECT * FROM viewPatentFilingTypes")); DispatchResult r; r.ok = true; QJsonArray items; if (ok) { while (q.next()) { const QSqlRecord rec = q.record(); QJsonObject obj; for (int i = 0; i < rec.count(); ++i) { const QString name = rec.fieldName(i); const QVariant v = rec.value(i); switch (v.typeId()) { case QMetaType::Bool: obj.insert(name, v.toBool()); break; case QMetaType::Int: case QMetaType::UInt: case QMetaType::LongLong: case QMetaType::ULongLong: case QMetaType::Double: obj.insert(name, v.toDouble()); break; case QMetaType::QDate: obj.insert(name, v.toDate().toString(Qt::ISODate)); break; case QMetaType::QDateTime: obj.insert(name, v.toDateTime().toString(Qt::ISODate)); break; default: obj.insert(name, v.toString()); break; } } items.append(obj); } } r.result = QJsonObject{{"items", items}}; return r; }
    });

    // trademarkMatterView.get (read-only via viewTrademarkMatters)
    m_router.registerAction(QStringLiteral("trademarkMatterView.get"), ActionSpec{
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
                QStringLiteral("TrademarkMatterId"),
                QStringLiteral("MatterId"),
                QStringLiteral("Id")
            };

            DispatchResult r;
            for (const QString &col : idColumns) {
                QSqlQuery q(QSqlDatabase::database());
                q.prepare(QStringLiteral("SELECT * FROM viewTrademarkMatters WHERE ") + col + QStringLiteral(" = :id LIMIT 1"));
                q.bindValue(":id", QVariant::fromValue(id));
                if (q.exec() && q.next()) { r.ok = true; r.result = packRecord(q.record()); return r; }
            }
            r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Trademark matter not found in view");
            return r;
        }
    });

    // trademarkMatterView.list (read-only)
    m_router.registerAction(QStringLiteral("trademarkMatterView.list"), ActionSpec{
        /*fields*/ QList<FieldSpec>{},
        /*handler*/ [](const QJsonObject &){
            QSqlQuery q(QSqlDatabase::database());
            bool ok = q.exec(QStringLiteral("SELECT * FROM viewTrademarkMatters"));
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

    // trademarkTaskTypeView.get (read-only via viewTrademarkTaskTypes)
    m_router.registerAction(QStringLiteral("trademarkTaskTypeView.get"), ActionSpec{
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
                QStringLiteral("TrademarkTaskTypeId"),
                QStringLiteral("TaskTypeId"),
                QStringLiteral("Id")
            };

            DispatchResult r;
            for (const QString &col : idColumns) {
                QSqlQuery q(QSqlDatabase::database());
                q.prepare(QStringLiteral("SELECT * FROM viewTrademarkTaskTypes WHERE ") + col + QStringLiteral(" = :id LIMIT 1"));
                q.bindValue(":id", QVariant::fromValue(id));
                if (q.exec() && q.next()) { r.ok = true; r.result = packRecord(q.record()); return r; }
            }
            r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Trademark task type not found");
            return r;
        }
    });

    // trademarkTaskTypeView.list (read-only)
    m_router.registerAction(QStringLiteral("trademarkTaskTypeView.list"), ActionSpec{
        /*fields*/ QList<FieldSpec>{},
        /*handler*/ [](const QJsonObject &){
            QSqlQuery q(QSqlDatabase::database());
            bool ok = q.exec(QStringLiteral("SELECT * FROM viewTrademarkTaskTypes"));
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

    // upcomingAppointmentView.get (read-only via viewUpcomingAppointments)
    m_router.registerAction(QStringLiteral("upcomingAppointmentView.get"), ActionSpec{
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
                QStringLiteral("AppointmentId"),
                QStringLiteral("Id")
            };

            DispatchResult r;
            for (const QString &col : idColumns) {
                QSqlQuery q(QSqlDatabase::database());
                q.prepare(QStringLiteral("SELECT * FROM viewUpcomingAppointments WHERE ") + col + QStringLiteral(" = :id LIMIT 1"));
                q.bindValue(":id", QVariant::fromValue(id));
                if (q.exec() && q.next()) { r.ok = true; r.result = packRecord(q.record()); return r; }
            }
            r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Upcoming appointment not found");
            return r;
        }
    });

    // upcomingAppointmentView.list (read-only)
    m_router.registerAction(QStringLiteral("upcomingAppointmentView.list"), ActionSpec{
        /*fields*/ QList<FieldSpec>{},
        /*handler*/ [](const QJsonObject &){
            QSqlQuery q(QSqlDatabase::database());
            bool ok = q.exec(QStringLiteral("SELECT * FROM viewUpcomingAppointments"));
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

    // workAttorneyView.get (read-only via viewWorkAttorneys)
    m_router.registerAction(QStringLiteral("workAttorneyView.get"), ActionSpec{
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
                QStringLiteral("WorkAttorneyEntityId"),
                QStringLiteral("EntityId"),
                QStringLiteral("PersonId"),
                QStringLiteral("Id")
            };

            DispatchResult r;
            for (const QString &col : idColumns) {
                QSqlQuery q(QSqlDatabase::database());
                q.prepare(QStringLiteral("SELECT * FROM viewWorkAttorneys WHERE ") + col + QStringLiteral(" = :id LIMIT 1"));
                q.bindValue(":id", QVariant::fromValue(id));
                if (q.exec() && q.next()) { r.ok = true; r.result = packRecord(q.record()); return r; }
            }
            r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Work attorney not found");
            return r;
        }
    });

    // workAttorneyView.list (read-only)
    m_router.registerAction(QStringLiteral("workAttorneyView.list"), ActionSpec{
        /*fields*/ QList<FieldSpec>{},
        /*handler*/ [](const QJsonObject &){
            QSqlQuery q(QSqlDatabase::database());
            bool ok = q.exec(QStringLiteral("SELECT * FROM viewWorkAttorneys"));
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

    // objectType.create: insert new type
    m_router.registerAction(QStringLiteral("objectType.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("objectTypeName"), QJsonValue::String, true} },
        /*handler*/ [](const QJsonObject &payload){
            const QString name = payload.value(QStringLiteral("objectTypeName")).toString();
            DispatchResult r; if (name.trimmed().isEmpty()) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("objectTypeName is required"); return r; }
            QSqlQuery q(QSqlDatabase::database());
            q.prepare(QStringLiteral("INSERT INTO objectType (TypeName) VALUES (:name)"));
            q.bindValue(":name", name);
            if (!q.exec()) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = q.lastError().text(); return r; }
            const Key id = q.lastInsertId().toUInt();
            r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(id)}, {"objectTypeName", name}}; return r;
        }
    });

    // objectType.delete: delete a type
    m_router.registerAction(QStringLiteral("objectType.delete"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("id"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            QSqlQuery q(QSqlDatabase::database()); q.prepare(QStringLiteral("DELETE FROM objectType WHERE ObjectTypeId = :id")); q.bindValue(":id", QVariant::fromValue(id));
            DispatchResult r; if (!q.exec()) { r.ok = false; r.errorCode = QStringLiteral("EDELETE"); r.errorMessage = q.lastError().text(); return r; }
            r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(id)}, {"deleted", true}}; return r;
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
                {"needsAgenda", Nutdb::GetFlag(assoc, QStringLiteral("NeedsAgenda"))},
                {"agendaSent", Nutdb::GetFlag(assoc, QStringLiteral("AgendaSent"))},
                {"confirmed", Nutdb::GetFlag(assoc, QStringLiteral("Confirmed"))},
                {"associatedObject", static_cast<double>(assoc)},
                {"typeString", typeStr}
            };
            return r;
        }
    });

    // appointment.update: update using a single AppointmentData transaction + flags
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
            DispatchResult r;
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            Appointment appt{id};
            if (appt.getId() == 0) {
                r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Appointment not found");
                return r;
            }

            AppointmentData dat = Nutdb::GetAppointment(id);
            if (dat.AppointmentId == 0) {
                r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Appointment not found");
                return r;
            }

            // Merge incoming fields into dat
            if (payload.contains(QStringLiteral("appointmentTime"))) {
                const QString s = payload.value(QStringLiteral("appointmentTime")).toString();
                const QDateTime dt = QDateTime::fromString(s, Qt::ISODate);
                if (!dt.isValid()) {
                    r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("Invalid appointmentTime ISO string");
                    return r;
                }
                dat.AppointmentTime = dt;
            }
            if (payload.contains(QStringLiteral("fkAppointmentType"))) {
                dat.fkAppointmentType = static_cast<Key>(payload.value(QStringLiteral("fkAppointmentType")).toDouble());
            }
            if (payload.contains(QStringLiteral("complete"))) {
                dat.Complete = payload.value(QStringLiteral("complete")).toBool();
            }

            // Start a DB transaction to bundle changes
            QSqlDatabase db = QSqlDatabase::database();
            bool tx = db.isValid() ? db.transaction() : false;

            if (!appt.Update(dat)) {
                if (tx) db.rollback();
                r.ok = false; r.errorCode = QStringLiteral("EUPDATE"); r.errorMessage = QStringLiteral("Failed to update appointment");
                return r;
            }

            // Update flags against the associated object, if present
            const Key assoc = Nutdb::GetAppointmentObject(id);
            auto updFlag = [&](const char *name, bool value){
                if (!payload.contains(QLatin1String(name))) return true; // no change requested
                if (value) return Nutdb::SetFlag(assoc, QLatin1String(name)) != 0; else return Nutdb::ClearFlag(assoc, QLatin1String(name)) != 0;
            };
            bool ok = true;
            ok = ok && updFlag("needsAgenda", payload.value(QStringLiteral("needsAgenda")).toBool());
            ok = ok && updFlag("agendaSent", payload.value(QStringLiteral("agendaSent")).toBool());
            ok = ok && updFlag("confirmed", payload.value(QStringLiteral("confirmed")).toBool());
            if (!ok) {
                if (tx) db.rollback();
                r.ok = false; r.errorCode = QStringLiteral("EUPDATE"); r.errorMessage = QStringLiteral("Failed to update flags");
                return r;
            }

            if (tx) db.commit();

            // Return the updated row
            const AppointmentData a = Nutdb::GetAppointment(id);
            const QString typeStr = Nutdb::GetAppointmentTypeString(id);
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(a.AppointmentId)},
                {"appointmentTime", a.AppointmentTime.toUTC().toString(Qt::ISODate)},
                {"fkAppointmentType", static_cast<double>(a.fkAppointmentType)},
                {"complete", a.Complete},
                {"needsAgenda", Nutdb::GetFlag(assoc, QStringLiteral("NeedsAgenda"))},
                {"agendaSent", Nutdb::GetFlag(assoc, QStringLiteral("AgendaSent"))},
                {"confirmed", Nutdb::GetFlag(assoc, QStringLiteral("Confirmed"))},
                {"associatedObject", static_cast<double>(assoc)},
                {"typeString", typeStr}
            };
            return r;
        }
    });

    // Generic UpdateX endpoints for targeted field updates
    auto registerUpdateGeneric = [this](const QString &name, QJsonValue::Type vtype, std::function<QString(const QJsonValue&)> toString){
        m_router.registerAction(name, ActionSpec{
            QList<FieldSpec>{
                FieldSpec{QStringLiteral("table"), QJsonValue::String, true},
                FieldSpec{QStringLiteral("field"), QJsonValue::String, true},
                FieldSpec{QStringLiteral("id"), QJsonValue::Double, true},
                FieldSpec{QStringLiteral("value"), vtype, true}
            },
            [toString](const QJsonObject &payload){
                const QString table = payload.value(QStringLiteral("table")).toString();
                const QString field = payload.value(QStringLiteral("field")).toString();
                const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
                const QJsonValue v = payload.value(QStringLiteral("value"));
                DispatchResult r;
                if (table.isEmpty() || field.isEmpty() || id == 0) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("Missing/invalid table, field, or id"); return r; }
                const QString s = toString(v);
                if (!Nutdb::UpdateField(table, field, id, s)) { r.ok = false; r.errorCode = QStringLiteral("EUPDATE"); r.errorMessage = QStringLiteral("Update failed"); return r; }
                r.ok = true; r.result = QJsonObject{{"updated", true}}; return r;
            }
        });
    };

    registerUpdateGeneric(QStringLiteral("UpdateKey"), QJsonValue::Double, [](const QJsonValue &v){ return QString::number(static_cast<qulonglong>(v.toDouble())); });
    registerUpdateGeneric(QStringLiteral("UpdateInteger"), QJsonValue::Double, [](const QJsonValue &v){ return QString::number(static_cast<qlonglong>(v.toDouble())); });
    registerUpdateGeneric(QStringLiteral("UpdateFloat"), QJsonValue::Double, [](const QJsonValue &v){ return QString::number(v.toDouble()); });
    registerUpdateGeneric(QStringLiteral("UpdateString"), QJsonValue::String, [](const QJsonValue &v){ return v.toString(); });
    registerUpdateGeneric(QStringLiteral("UpdateBoolean"), QJsonValue::Bool, [](const QJsonValue &v){ return QString::number(v.toBool() ? 1 : 0); });

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

    // copyrightMatter.update: full-record update + flag in one transaction
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
            DispatchResult r;
            CopyrightMatter cm{id};
            if (cm.getId() == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("CopyrightMatter not found"); return r; }
            CopyrightMatterData dat = Nutdb::GetCopyrightMatter(id);
            if (dat.CopyrightMatterId == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("CopyrightMatter not found"); return r; }
            if (payload.contains("fkAuthor")) dat.fkAuthor = static_cast<Key>(payload.value("fkAuthor").toDouble());
            auto parseDate = [](const QJsonObject &obj, const char *key, QDate &out, QString &err) -> bool { if (!obj.contains(key)) return true; const QString s = obj.value(QString::fromUtf8(key)).toString(); const QDate d = QDate::fromString(s, Qt::ISODate); if (!d.isValid()) { err = QStringLiteral("Invalid date for '%1'").arg(QString::fromUtf8(key)); return false; } out = d; return true; };
            QString perr; QDate dt;
            if (!parseDate(payload, "created", dt, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; }
            if (payload.contains("created")) dat.Created = dt;
            if (!parseDate(payload, "filed", dt, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; }
            if (payload.contains("filed")) dat.Filed = dt;
            if (!parseDate(payload, "registered", dt, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; }
            if (payload.contains("registered")) dat.Registered = dt;
            if (payload.contains("serialNumber")) dat.SerialNumber = payload.value("serialNumber").toString();
            if (payload.contains("registrationNumber")) dat.RegistrationNumber = payload.value("registrationNumber").toString();
            if (payload.contains("fkDeposit")) dat.fkDeposit = static_cast<Key>(payload.value("fkDeposit").toDouble());
            if (payload.contains("fkWorkType")) dat.fkWorkType = static_cast<Key>(payload.value("fkWorkType").toDouble());

            QSqlDatabase db = QSqlDatabase::database();
            bool tx = db.isValid() ? db.transaction() : false;
            if (!cm.slotUpdate(dat)) { if (tx) db.rollback(); r.ok = false; r.errorCode = QStringLiteral("EUPDATE"); r.errorMessage = QStringLiteral("Failed to update copyrightMatter"); return r; }
            if (payload.contains("isRegistered")) {
                const bool v = payload.value("isRegistered").toBool();
                if (v) Nutdb::SetFlag(id, QStringLiteral("Registered")); else Nutdb::ClearFlag(id, QStringLiteral("Registered"));
            }
            if (tx) db.commit();

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

    // deadline.update: full-record update in a single transaction
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
            if (dl.getId() == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Deadline not found"); return r; }
            DeadlineData dat = Nutdb::GetDeadline(id);
            if (dat.DeadlineId == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Deadline not found"); return r; }
            auto parseDate = [](const QJsonObject &obj, const char *key, QDate &out, QString &err) -> bool {
                if (!obj.contains(key)) return true;
                const QString s = obj.value(QString::fromUtf8(key)).toString();
                const QDate d = QDate::fromString(s, Qt::ISODate);
                if (!d.isValid()) { err = QStringLiteral("Invalid date for '%1'").arg(QString::fromUtf8(key)); return false; }
                out = d; return true;
            };
            QString perr; QDate dt;
            if (!parseDate(payload, "triggerDate", dt, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; }
            if (payload.contains("triggerDate")) dat.TriggerDate = dt;
            if (!parseDate(payload, "softDeadline", dt, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; }
            if (payload.contains("softDeadline")) dat.SoftDeadline = dt;
            if (!parseDate(payload, "hardDeadline", dt, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; }
            if (payload.contains("hardDeadline")) dat.HardDeadline = dt;
            if (!parseDate(payload, "nextDeadline", dt, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; }
            if (payload.contains("nextDeadline")) dat.NextDeadline = dt;

            QSqlDatabase db = QSqlDatabase::database();
            bool tx = db.isValid() ? db.transaction() : false;
            if (!dl.Update(dat)) { if (tx) db.rollback(); r.ok = false; r.errorCode = QStringLiteral("EUPDATE"); r.errorMessage = QStringLiteral("Failed to update deadline"); return r; }
            if (tx) db.commit();

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

    // deadline.list (table, read-only)
    m_router.registerAction(QStringLiteral("deadline.list"), ActionSpec{
        /*fields*/ QList<FieldSpec>{},
        /*handler*/ [](const QJsonObject &){
            QSqlQuery q(QSqlDatabase::database());
            DispatchResult r; r.ok = true; QJsonArray items;
            if (q.exec(QStringLiteral("SELECT DeadlineId, TriggerDate, SoftDeadline, HardDeadline, NextDeadline FROM deadline ORDER BY NextDeadline ASC, DeadlineId ASC"))) {
                while (q.next()) {
                    items.append(QJsonObject{
                        {"id", static_cast<double>(q.value(0).toUInt())},
                        {"triggerDate", q.value(1).toDate().toString(Qt::ISODate)},
                        {"softDeadline", q.value(2).toDate().toString(Qt::ISODate)},
                        {"hardDeadline", q.value(3).toDate().toString(Qt::ISODate)},
                        {"nextDeadline", q.value(4).toDate().toString(Qt::ISODate)}
                    });
                }
            }
            r.result = QJsonObject{{"items", items}}; return r;
        }
    });

    // deadline.create (table insert)
    m_router.registerAction(QStringLiteral("deadline.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{
            FieldSpec{QStringLiteral("triggerDate"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("softDeadline"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("hardDeadline"), QJsonValue::String, false},
            FieldSpec{QStringLiteral("nextDeadline"), QJsonValue::String, false}
        },
        /*handler*/ [](const QJsonObject &payload){
            auto parseDateVar = [&](const char *k)->QVariant{ if (!payload.contains(QString::fromUtf8(k))) return QVariant(); QDate d = QDate::fromString(payload.value(QString::fromUtf8(k)).toString(), Qt::ISODate); return d.isValid() ? QVariant(d) : QVariant(); };
            QSqlQuery q(QSqlDatabase::database());
            q.prepare(QStringLiteral("INSERT INTO deadline (TriggerDate, SoftDeadline, HardDeadline, NextDeadline) VALUES (:trig, :soft, :hard, :next)"));
            q.bindValue(":trig", parseDateVar("triggerDate"));
            q.bindValue(":soft", parseDateVar("softDeadline"));
            q.bindValue(":hard", parseDateVar("hardDeadline"));
            q.bindValue(":next", parseDateVar("nextDeadline"));
            DispatchResult r; if (!q.exec()) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = q.lastError().text(); return r; }
            Key id = q.lastInsertId().toUInt();
            QSqlQuery qr(QSqlDatabase::database());
            qr.prepare(QStringLiteral("SELECT DeadlineId, TriggerDate, SoftDeadline, HardDeadline, NextDeadline FROM deadline WHERE DeadlineId = :id"));
            qr.bindValue(":id", QVariant::fromValue(id));
            if (!qr.exec() || !qr.next()) { r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(id)}}; return r; }
            r.ok = true; r.result = QJsonObject{
                {"id", static_cast<double>(qr.value(0).toUInt())},
                {"triggerDate", qr.value(1).toDate().toString(Qt::ISODate)},
                {"softDeadline", qr.value(2).toDate().toString(Qt::ISODate)},
                {"hardDeadline", qr.value(3).toDate().toString(Qt::ISODate)},
                {"nextDeadline", qr.value(4).toDate().toString(Qt::ISODate)}
            }; return r;
        }
    });

    // deadline.delete (table)
    m_router.registerAction(QStringLiteral("deadline.delete"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("id"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){ const Key id = static_cast<Key>(payload.value("id").toDouble()); QSqlQuery q(QSqlDatabase::database()); q.prepare(QStringLiteral("DELETE FROM deadline WHERE DeadlineId = :id")); q.bindValue(":id", QVariant::fromValue(id)); DispatchResult r; if (!q.exec()) { r.ok = false; r.errorCode = QStringLiteral("EDELETE"); r.errorMessage = q.lastError().text(); return r; } r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(id)}, {"deleted", true}}; return r; }
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

    // document.update: full-record update
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
            if (doc.getId() == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Document not found"); return r; }
            DocumentData dat = Nutdb::GetDocument(id);
            if (dat.DocumentId == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Document not found"); return r; }
            if (payload.contains("fullyQualifiedPath")) dat.FullyQualifiedPath = payload.value("fullyQualifiedPath").toString();
            if (payload.contains("url")) dat.URL = payload.value("url").toString();
            if (payload.contains("filename")) dat.Filename = payload.value("filename").toString();
            if (payload.contains("extension")) dat.Extension = payload.value("extension").toString();
            if (payload.contains("title")) dat.Title = payload.value("title").toString();

            QSqlDatabase db = QSqlDatabase::database();
            bool tx = db.isValid() ? db.transaction() : false;
            if (!doc.slotUpdate(dat)) { if (tx) db.rollback(); r.ok = false; r.errorCode = QStringLiteral("EUPDATE"); r.errorMessage = QStringLiteral("Failed to update document"); return r; }
            if (tx) db.commit();

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

    // enterprise.update: full-record update
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
            DispatchResult r;
            Enterprise ent{id};
            if (ent.getId() == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Enterprise not found"); return r; }
            EnterpriseData dat = Nutdb::GetEnterprise(id);
            if (dat.EnterpriseId == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Enterprise not found"); return r; }
            if (payload.contains("enterpriseName")) dat.EnterpriseName = payload.value("enterpriseName").toString();
            if (payload.contains("fkStateOfIncorporation")) dat.fkStateOfIncorporation = static_cast<Key>(payload.value("fkStateOfIncorporation").toDouble());
            if (payload.contains("fkMainContact")) dat.fkMainContact = static_cast<Key>(payload.value("fkMainContact").toDouble());
            if (payload.contains("oldOrganizationId")) dat.OldOrganizationId = static_cast<Key>(payload.value("oldOrganizationId").toDouble());

            QSqlDatabase db = QSqlDatabase::database();
            bool tx = db.isValid() ? db.transaction() : false;
            if (!ent.slotUpdate(dat)) { if (tx) db.rollback(); r.ok = false; r.errorCode = QStringLiteral("EUPDATE"); r.errorMessage = QStringLiteral("Failed to update enterprise"); return r; }
            // Persist fkBusinessJurisdiction when provided (not covered by UpdateEnterprise SP)
            if (payload.contains("fkBusinessJurisdiction")) {
                const Key v = static_cast<Key>(payload.value("fkBusinessJurisdiction").toDouble());
                if (!Nutdb::UpdateField(QStringLiteral("enterprise"), QStringLiteral("fkBusinessJurisdiction"), id, QString::number(static_cast<qulonglong>(v)))) {
                    if (tx) db.rollback();
                    r.ok = false; r.errorCode = QStringLiteral("EUPDATE"); r.errorMessage = QStringLiteral("Failed to update fkBusinessJurisdiction"); return r;
                }
            }
            if (tx) db.commit();

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

    // entity.update: full-record update
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
            EntityData dat = Nutdb::GetEntity(id);
            if (dat.EntityId == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Entity not found"); return r; }
            if (payload.contains("entityName")) dat.EntityName = payload.value("entityName").toString();
            if (payload.contains("fullLegalName")) dat.FullLegalName = payload.value("fullLegalName").toString();
            if (payload.contains("primaryAddress")) dat.PrimaryAddress = payload.value("primaryAddress").toString();
            if (payload.contains("secondaryAddress")) dat.SecondaryAddress = payload.value("secondaryAddress").toString();
            if (payload.contains("website")) dat.Website = payload.value("website").toString();
            if (payload.contains("primaryPhone")) dat.PrimaryPhone = payload.value("primaryPhone").toString();
            if (payload.contains("secondaryPhone")) dat.SecondaryPhone = payload.value("secondaryPhone").toString();
            if (payload.contains("faxNumber")) dat.FaxNumber = payload.value("faxNumber").toString();
            if (payload.contains("primaryEmail")) dat.PrimaryEmail = payload.value("primaryEmail").toString();
            if (payload.contains("secondaryEmail")) dat.SecondaryEmail = payload.value("secondaryEmail").toString();
            if (payload.contains("fkJurisdiction")) dat.fkJurisdiction = static_cast<Key>(payload.value("fkJurisdiction").toDouble());
            if (payload.contains("fkState")) dat.fkState = static_cast<Key>(payload.value("fkState").toDouble());
            if (payload.contains("docketPrefix")) dat.DocketPrefix = payload.value("docketPrefix").toString();

            QSqlDatabase db = QSqlDatabase::database();
            bool tx = db.isValid() ? db.transaction() : false;
            if (!ent.update(dat)) { if (tx) db.rollback(); r.ok = false; r.errorCode = QStringLiteral("EUPDATE"); r.errorMessage = QStringLiteral("Failed to update entity"); return r; }
            if (tx) db.commit();

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

    // filing.update: full-record update
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
            FilingData dat = Nutdb::GetFiling(id);
            if (dat.FilingId == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Filing not found"); return r; }
            if (payload.contains("fkFilingStatus")) dat.fkFilingStatus = static_cast<Key>(payload.value("fkFilingStatus").toDouble());
            if (payload.contains("fkAsFiledDocument")) dat.fkAsFiledDocument = static_cast<Key>(payload.value("fkAsFiledDocument").toDouble());

            QSqlDatabase db = QSqlDatabase::database();
            bool tx = db.isValid() ? db.transaction() : false;
            if (!filing.slotUpdate(dat)) { if (tx) db.rollback(); r.ok = false; r.errorCode = QStringLiteral("EUPDATE"); r.errorMessage = QStringLiteral("Failed to update filing"); return r; }
            if (tx) db.commit();

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
    // flagClass.list (table)
    m_router.registerAction(QStringLiteral("flagClass.list"), ActionSpec{
        /*fields*/ QList<FieldSpec>{},
        /*handler*/ [](const QJsonObject &){ QSqlQuery q(QSqlDatabase::database()); DispatchResult r; r.ok = true; QJsonArray items; if (q.exec(QStringLiteral("SELECT FlagClassId, CamelCase, Label, Description FROM flagClass ORDER BY CamelCase"))) { while (q.next()) { items.append(QJsonObject{{"id", static_cast<double>(q.value(0).toUInt())}, {"camelCase", q.value(1).toString()}, {"label", q.value(2).toString()}, {"description", q.value(3).toString()}}); } } r.result = QJsonObject{{"items", items}}; return r; }
    });
    // flagClass.create (table)
    m_router.registerAction(QStringLiteral("flagClass.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("camelCase"), QJsonValue::String, true}, FieldSpec{QStringLiteral("label"), QJsonValue::String, false}, FieldSpec{QStringLiteral("description"), QJsonValue::String, false} },
        /*handler*/ [](const QJsonObject &payload){ QSqlQuery q(QSqlDatabase::database()); q.prepare(QStringLiteral("INSERT INTO flagClass (CamelCase, Label, Description) VALUES (:cc, :lbl, :desc)")); q.bindValue(":cc", payload.value("camelCase").toString()); q.bindValue(":lbl", payload.value("label").toString()); q.bindValue(":desc", payload.value("description").toString()); DispatchResult r; if (!q.exec()) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = q.lastError().text(); return r; } const Key id = q.lastInsertId().toUInt(); r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(id)}, {"camelCase", payload.value("camelCase").toString()}, {"label", payload.value("label").toString()}, {"description", payload.value("description").toString()}}; return r; }
    });
    // flagClass.delete (table)
    m_router.registerAction(QStringLiteral("flagClass.delete"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("id"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){ const Key id = static_cast<Key>(payload.value("id").toDouble()); QSqlQuery q(QSqlDatabase::database()); q.prepare(QStringLiteral("DELETE FROM flagClass WHERE FlagClassId = :id")); q.bindValue(":id", QVariant::fromValue(id)); DispatchResult r; if (!q.exec()) { r.ok = false; r.errorCode = QStringLiteral("EDELETE"); r.errorMessage = q.lastError().text(); return r; } r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(id)}, {"deleted", true}}; return r; }
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
    // object.list (table)
    m_router.registerAction(QStringLiteral("object.list"), ActionSpec{
        /*fields*/ QList<FieldSpec>{},
        /*handler*/ [](const QJsonObject &){ QSqlQuery q(QSqlDatabase::database()); DispatchResult r; r.ok = true; QJsonArray items; if (q.exec(QStringLiteral("SELECT ObjectId, fkObjectType FROM object ORDER BY ObjectId"))) { while (q.next()) { items.append(QJsonObject{{"id", static_cast<double>(q.value(0).toUInt())}, {"fkObjectType", static_cast<double>(q.value(1).toUInt())}}); } } r.result = QJsonObject{{"items", items}}; return r; }
    });
    // object.create
    m_router.registerAction(QStringLiteral("object.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("objectType"), QJsonValue::String, true} },
        /*handler*/ [](const QJsonObject &payload){ const QString type = payload.value("objectType").toString(); DispatchResult r; if (type.trimmed().isEmpty()) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("objectType required"); return r; } const Key id = Nutdb::InsertObject(type); if (id == 0) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = QStringLiteral("Failed to insert object"); return r; } QSqlQuery q(QSqlDatabase::database()); q.prepare(QStringLiteral("SELECT ObjectId, fkObjectType FROM object WHERE ObjectId = :id")); q.bindValue(":id", QVariant::fromValue(id)); if (q.exec() && q.next()) { r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(q.value(0).toUInt())}, {"fkObjectType", static_cast<double>(q.value(1).toUInt())}, {"objectType", type}}; return r; } r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(id)}, {"objectType", type}}; return r; }
    });

    // ==== Create endpoints backed by Nutdb::Insert* ====

    // appointment.create: create appointment (optional taskId, optional utcOffset hours)
    m_router.registerAction(QStringLiteral("appointment.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{
            FieldSpec{QStringLiteral("appointmentTime"), QJsonValue::String, true},
            FieldSpec{QStringLiteral("taskId"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("fkTask"), QJsonValue::Double, false},
            FieldSpec{QStringLiteral("utcOffset"), QJsonValue::Double, false}
        },
        /*handler*/ [](const QJsonObject &payload){
            DispatchResult r;
            const QString t = payload.value(QStringLiteral("appointmentTime")).toString();
            QDateTime dt = QDateTime::fromString(t, Qt::ISODate);
            if (!dt.isValid()) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("Invalid appointmentTime (ISO-8601 required)"); return r; }
            const bool hasTaskId = payload.contains(QStringLiteral("taskId")) || payload.contains(QStringLiteral("fkTask"));
            const Key taskId = hasTaskId ? static_cast<Key>((payload.contains(QStringLiteral("taskId")) ? payload.value(QStringLiteral("taskId")) : payload.value(QStringLiteral("fkTask"))).toDouble()) : 0;
            const bool hasOffset = payload.contains(QStringLiteral("utcOffset"));
            Key id = 0;
            if (hasOffset && hasTaskId) {
                id = Nutdb::InsertAppointmentWithZone(dt, static_cast<int>(payload.value(QStringLiteral("utcOffset")).toDouble()), taskId);
            } else if (hasOffset) {
                id = Nutdb::InsertAppointmentWithZone(dt, static_cast<int>(payload.value(QStringLiteral("utcOffset")).toDouble()));
            } else if (hasTaskId) {
                id = Nutdb::InsertAppointment(dt, taskId);
            } else {
                id = Nutdb::InsertAppointment(dt);
            }
            if (id == 0) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = QStringLiteral("Failed to insert appointment"); return r; }
            const AppointmentData a = Nutdb::GetAppointment(id);
            const Key assoc = Nutdb::GetAppointmentObject(id);
            const QString typeStr = Nutdb::GetAppointmentTypeString(id);
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(a.AppointmentId)},
                {"appointmentTime", a.AppointmentTime.toUTC().toString(Qt::ISODate)},
                {"fkAppointmentType", static_cast<double>(a.fkAppointmentType)},
                {"complete", a.Complete},
                {"needsAgenda", Nutdb::GetFlag(assoc, QStringLiteral("NeedsAgenda"))},
                {"agendaSent", Nutdb::GetFlag(assoc, QStringLiteral("AgendaSent"))},
                {"confirmed", Nutdb::GetFlag(assoc, QStringLiteral("Confirmed"))},
                {"associatedObject", static_cast<double>(assoc)},
                {"typeString", typeStr}
            };
            return r;
        }
    });

    // caseInventor.create: create a new inventor for a patentMatter
    m_router.registerAction(QStringLiteral("caseInventor.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{
            FieldSpec{QStringLiteral("firstName"), QJsonValue::String, true},
            FieldSpec{QStringLiteral("lastName"), QJsonValue::String, true},
            FieldSpec{QStringLiteral("patentMatterId"), QJsonValue::Double, true}
        },
        /*handler*/ [](const QJsonObject &payload){
            const QString first = payload.value(QStringLiteral("firstName")).toString();
            const QString last = payload.value(QStringLiteral("lastName")).toString();
            const Key pm = static_cast<Key>(payload.value(QStringLiteral("patentMatterId")).toDouble());
            DispatchResult r;
            if (first.trimmed().isEmpty() || last.trimmed().isEmpty()) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("firstName and lastName are required"); return r; }
            const Key id = Nutdb::InsertCaseInventor(first, last, pm);
            if (id == 0) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = QStringLiteral("Failed to insert case inventor"); return r; }
            r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(id)}, {"patentMatterId", static_cast<double>(pm)}, {"firstName", first}, {"lastName", last}}; return r;
        }
    });

    // client.createEnterprise
    m_router.registerAction(QStringLiteral("client.createEnterprise"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("enterpriseName"), QJsonValue::String, true} },
        /*handler*/ [](const QJsonObject &payload){
            const QString name = payload.value(QStringLiteral("enterpriseName")).toString();
            DispatchResult r; if (name.trimmed().isEmpty()) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("enterpriseName is required"); return r; }
            const Key id = Nutdb::InsertClientEnterprise(name);
            if (id == 0) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = QStringLiteral("Failed to create client enterprise"); return r; }
            // Return using client.get shape when possible
            QSqlQuery q(QSqlDatabase::database()); q.prepare(QStringLiteral("SELECT * FROM viewClients WHERE ClientEntityId = :id OR EntityId = :id LIMIT 1")); q.bindValue(":id", QVariant::fromValue(id));
            if (q.exec() && q.next()) { const QSqlRecord rec = q.record(); QJsonObject obj; for (int i=0;i<rec.count();++i){ const QString n=rec.fieldName(i); const QVariant v=rec.value(i); switch(v.typeId()){ case QMetaType::Bool: obj.insert(n, v.toBool()); break; case QMetaType::Int: case QMetaType::UInt: case QMetaType::LongLong: case QMetaType::ULongLong: case QMetaType::Double: obj.insert(n, v.toDouble()); break; case QMetaType::QDate: obj.insert(n, v.toDate().toString(Qt::ISODate)); break; case QMetaType::QDateTime: obj.insert(n, v.toDateTime().toString(Qt::ISODate)); break; default: obj.insert(n, v.toString()); break; } } r.ok = true; r.result = obj; return r; }
            r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(id)}, {"enterpriseName", name}}; return r;
        }
    });

    // client.createNatural
    m_router.registerAction(QStringLiteral("client.createNatural"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("firstName"), QJsonValue::String, true}, FieldSpec{QStringLiteral("lastName"), QJsonValue::String, true} },
        /*handler*/ [](const QJsonObject &payload){ const QString first = payload.value("firstName").toString(); const QString last = payload.value("lastName").toString(); DispatchResult r; if (first.trimmed().isEmpty() || last.trimmed().isEmpty()) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("firstName and lastName are required"); return r; } const Key id = Nutdb::InsertClientNatural(first, last); if (id == 0) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = QStringLiteral("Failed to create client person"); return r; } QSqlQuery q(QSqlDatabase::database()); q.prepare(QStringLiteral("SELECT * FROM viewClients WHERE ClientEntityId = :id OR EntityId = :id LIMIT 1")); q.bindValue(":id", QVariant::fromValue(id)); if (q.exec() && q.next()) { const QSqlRecord rec = q.record(); QJsonObject obj; for (int i=0;i<rec.count();++i){ const QString n=rec.fieldName(i); const QVariant v=rec.value(i); switch(v.typeId()){ case QMetaType::Bool: obj.insert(n, v.toBool()); break; case QMetaType::Int: case QMetaType::UInt: case QMetaType::LongLong: case QMetaType::ULongLong: case QMetaType::Double: obj.insert(n, v.toDouble()); break; case QMetaType::QDate: obj.insert(n, v.toDate().toString(Qt::ISODate)); break; case QMetaType::QDateTime: obj.insert(n, v.toDateTime().toString(Qt::ISODate)); break; default: obj.insert(n, v.toString()); break; } } r.ok = true; r.result = obj; return r; } r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(id)}, {"firstName", first}, {"lastName", last}}; return r; }
    });

    // copyrightFiling.create
    m_router.registerAction(QStringLiteral("copyrightFiling.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("matterId"), QJsonValue::Double, true}, FieldSpec{QStringLiteral("triggerDate"), QJsonValue::String, false} },
        /*handler*/ [](const QJsonObject &payload){ const Key matterId = static_cast<Key>(payload.value("matterId").toDouble()); QDate trig = QDate::currentDate(); if (payload.contains("triggerDate")) { const QString s = payload.value("triggerDate").toString(); QDate d = QDate::fromString(s, Qt::ISODate); if (!d.isValid()) { DispatchResult r; r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("Invalid triggerDate"); return r; } trig = d; } const Key id = Nutdb::InsertCopyrightFiling(matterId, trig); DispatchResult r; if (id == 0) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = QStringLiteral("Failed to insert copyrightFiling"); return r; } const CopyrightFilingData d = Nutdb::GetCopyrightFiling(id); r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(d.CopyrightFilingId)}, {"fkMatter", static_cast<double>(matterId)}, {"triggerDate", trig.toString(Qt::ISODate)}}; return r; }
    });

    // copyrightMatter.create
    m_router.registerAction(QStringLiteral("copyrightMatter.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("docketNumber"), QJsonValue::String, true} },
        /*handler*/ [](const QJsonObject &payload){ const QString docket = payload.value("docketNumber").toString(); DispatchResult r; if (docket.trimmed().isEmpty()) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("docketNumber is required"); return r; } const Key id = Nutdb::InsertCopyrightMatter(docket); if (id == 0) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = QStringLiteral("Failed to insert copyrightMatter"); return r; } const CopyrightMatterData m = Nutdb::GetCopyrightMatter(id); r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(m.CopyrightMatterId)}}; return r; }
    });

    // document.create
    m_router.registerAction(QStringLiteral("document.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("title"), QJsonValue::String, true} },
        /*handler*/ [](const QJsonObject &payload){ const QString title = payload.value("title").toString(); DispatchResult r; if (title.trimmed().isEmpty()) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("title is required"); return r; } const Key id = Nutdb::InsertDocument(title); if (id == 0) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = QStringLiteral("Failed to insert document"); return r; } const DocumentData d = Nutdb::GetDocument(id); r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(d.DocumentId)}, {"title", d.Title}}; return r; }
    });

    // enterprise.create
    m_router.registerAction(QStringLiteral("enterprise.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("enterpriseName"), QJsonValue::String, true} },
        /*handler*/ [](const QJsonObject &payload){ const QString name = payload.value("enterpriseName").toString(); DispatchResult r; if (name.trimmed().isEmpty()) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("enterpriseName is required"); return r; } const Key id = Nutdb::InsertEnterprise(name); if (id == 0) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = QStringLiteral("Failed to insert enterprise"); return r; } const EnterpriseData e = Nutdb::GetEnterprise(id); r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(e.EnterpriseId)}, {"enterpriseName", e.EnterpriseName}}; return r; }
    });

    // entity.create
    m_router.registerAction(QStringLiteral("entity.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("entityName"), QJsonValue::String, true} },
        /*handler*/ [](const QJsonObject &payload){ const QString name = payload.value("entityName").toString(); DispatchResult r; if (name.trimmed().isEmpty()) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("entityName is required"); return r; } const Key id = Nutdb::InsertEntity(name); if (id == 0) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = QStringLiteral("Failed to insert entity"); return r; } const EntityData e = Nutdb::GetEntity(id); r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(e.EntityId)}, {"entityName", e.EntityName}}; return r; }
    });

    // examiner.create (generic)
    m_router.registerAction(QStringLiteral("examiner.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("firstName"), QJsonValue::String, true}, FieldSpec{QStringLiteral("lastName"), QJsonValue::String, true} },
        /*handler*/ [](const QJsonObject &payload){ const QString first = payload.value("firstName").toString(); const QString last = payload.value("lastName").toString(); DispatchResult r; if (first.trimmed().isEmpty() || last.trimmed().isEmpty()) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("firstName and lastName are required"); return r; } const Key id = Nutdb::InsertExaminer(first, last); if (id == 0) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = QStringLiteral("Failed to insert examiner"); return r; } const PersonData p = Nutdb::GetPerson(id); r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(p.PersonId)}, {"firstName", p.FirstName}, {"lastName", p.LastName}}; return r; }
    });

    // filing.create
    m_router.registerAction(QStringLiteral("filing.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("matterId"), QJsonValue::Double, true}, FieldSpec{QStringLiteral("triggerDate"), QJsonValue::String, false} },
        /*handler*/ [](const QJsonObject &payload){ const Key matterId = static_cast<Key>(payload.value("matterId").toDouble()); QDate trig = QDate::currentDate(); if (payload.contains("triggerDate")) { const QString s = payload.value("triggerDate").toString(); QDate d = QDate::fromString(s, Qt::ISODate); if (!d.isValid()) { DispatchResult r; r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("Invalid triggerDate"); return r; } trig = d; } const Key id = Nutdb::InsertFiling(matterId, trig); DispatchResult r; if (id == 0) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = QStringLiteral("Failed to insert filing"); return r; } const FilingData f = Nutdb::GetFiling(id); r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(f.FilingId)}, {"fkFilingStatus", static_cast<double>(f.fkFilingStatus)}, {"fkAsFiledDocument", static_cast<double>(f.fkAsFiledDocument)}}; return r; }
    });

    // finalOA.create
    m_router.registerAction(QStringLiteral("finalOA.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("matterId"), QJsonValue::Double, true}, FieldSpec{QStringLiteral("triggerDate"), QJsonValue::String, false} },
        /*handler*/ [](const QJsonObject &payload){ const Key matterId = static_cast<Key>(payload.value("matterId").toDouble()); QDate trig = QDate::currentDate(); if (payload.contains("triggerDate")) { const QString s = payload.value("triggerDate").toString(); QDate d = QDate::fromString(s, Qt::ISODate); if (!d.isValid()) { DispatchResult r; r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("Invalid triggerDate"); return r; } trig = d; } const Key id = Nutdb::InsertFinalOA(matterId, trig); DispatchResult r; if (id == 0) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = QStringLiteral("Failed to insert finalOA"); return r; } const FilingData f = Nutdb::GetFiling(id); r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(f.FilingId)}, {"fkFilingStatus", static_cast<double>(f.fkFilingStatus)}, {"fkAsFiledDocument", static_cast<double>(f.fkAsFiledDocument)}}; return r; }
    });

    // generalMatter.create
    m_router.registerAction(QStringLiteral("generalMatter.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("docketNumber"), QJsonValue::String, true} },
        /*handler*/ [](const QJsonObject &payload){ const QString docket = payload.value("docketNumber").toString(); DispatchResult r; if (docket.trimmed().isEmpty()) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("docketNumber is required"); return r; } const Key id = Nutdb::InsertGeneralMatter(docket); if (id == 0) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = QStringLiteral("Failed to insert generalMatter"); return r; } r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(id)}}; return r; }
    });

    // inventor.create (person role)
    m_router.registerAction(QStringLiteral("inventor.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("firstName"), QJsonValue::String, true}, FieldSpec{QStringLiteral("lastName"), QJsonValue::String, true} },
        /*handler*/ [](const QJsonObject &payload){ const QString first = payload.value("firstName").toString(); const QString last = payload.value("lastName").toString(); DispatchResult r; if (first.trimmed().isEmpty() || last.trimmed().isEmpty()) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("firstName and lastName are required"); return r; } const Key id = Nutdb::InsertInventor(first, last); if (id == 0) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = QStringLiteral("Failed to insert inventor"); return r; } const PersonData p = Nutdb::GetPerson(id); r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(p.PersonId)}, {"firstName", p.FirstName}, {"lastName", p.LastName}}; return r; }
    });

    // matter.create
    m_router.registerAction(QStringLiteral("matter.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("docketNumber"), QJsonValue::String, true} },
        /*handler*/ [](const QJsonObject &payload){ const QString docket = payload.value("docketNumber").toString(); DispatchResult r; if (docket.trimmed().isEmpty()) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("docketNumber is required"); return r; } const Key id = Nutdb::InsertMatter(docket); if (id == 0) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = QStringLiteral("Failed to insert matter"); return r; } const MatterData m = Nutdb::GetMatter(id); r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(m.MatterId)}, {"attorneyDocketNumber", m.AttorneyDocketNumber}, {"clientDocketNumber", m.ClientDocketNumber}, {"title", m.Title}}; return r; }
    });

    // nonfinalOA.create
    m_router.registerAction(QStringLiteral("nonfinalOA.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("matterId"), QJsonValue::Double, true}, FieldSpec{QStringLiteral("triggerDate"), QJsonValue::String, false} },
        /*handler*/ [](const QJsonObject &payload){ const Key matterId = static_cast<Key>(payload.value("matterId").toDouble()); QDate trig = QDate::currentDate(); if (payload.contains("triggerDate")) { const QString s = payload.value("triggerDate").toString(); QDate d = QDate::fromString(s, Qt::ISODate); if (!d.isValid()) { DispatchResult r; r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("Invalid triggerDate"); return r; } trig = d; } const Key id = Nutdb::InsertNonfinalOA(matterId, trig); DispatchResult r; if (id == 0) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = QStringLiteral("Failed to insert nonfinalOA"); return r; } const FilingData f = Nutdb::GetFiling(id); r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(f.FilingId)}, {"fkFilingStatus", static_cast<double>(f.fkFilingStatus)}, {"fkAsFiledDocument", static_cast<double>(f.fkAsFiledDocument)}}; return r; }
    });

    // note.create
    m_router.registerAction(QStringLiteral("note.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("objectId"), QJsonValue::Double, true}, FieldSpec{QStringLiteral("noteText"), QJsonValue::String, true} },
        /*handler*/ [](const QJsonObject &payload){ const Key oid = static_cast<Key>(payload.value("objectId").toDouble()); const QString text = payload.value("noteText").toString(); DispatchResult r; if (text.trimmed().isEmpty()) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("noteText is required"); return r; } const Key id = Nutdb::InsertNote(oid, text); if (id == 0) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = QStringLiteral("Failed to insert note"); return r; } const NoteData n = Nutdb::GetNote(id); r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(n.NoteId)}, {"content", n.Content}, {"objectId", static_cast<double>(oid)}}; return r; }
    });

    // paralegal.create
    m_router.registerAction(QStringLiteral("paralegal.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("firstName"), QJsonValue::String, true}, FieldSpec{QStringLiteral("lastName"), QJsonValue::String, true} },
        /*handler*/ [](const QJsonObject &payload){ const QString first = payload.value("firstName").toString(); const QString last = payload.value("lastName").toString(); DispatchResult r; if (first.trimmed().isEmpty() || last.trimmed().isEmpty()) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("firstName and lastName are required"); return r; } const Key id = Nutdb::InsertParalegal(first, last); if (id == 0) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = QStringLiteral("Failed to insert paralegal"); return r; } const PersonData p = Nutdb::GetPerson(id); r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(p.PersonId)}, {"firstName", p.FirstName}, {"lastName", p.LastName}}; return r; }
    });

    // patentExaminer.create
    m_router.registerAction(QStringLiteral("patentExaminer.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("firstName"), QJsonValue::String, true}, FieldSpec{QStringLiteral("lastName"), QJsonValue::String, true} },
        /*handler*/ [](const QJsonObject &payload){ const QString first = payload.value("firstName").toString(); const QString last = payload.value("lastName").toString(); DispatchResult r; if (first.trimmed().isEmpty() || last.trimmed().isEmpty()) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("firstName and lastName are required"); return r; } const Key id = Nutdb::InsertPatentExaminer(first, last); if (id == 0) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = QStringLiteral("Failed to insert patent examiner"); return r; } const PersonData p = Nutdb::GetPerson(id); r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(p.PersonId)}, {"firstName", p.FirstName}, {"lastName", p.LastName}}; return r; }
    });

    // patentFiling.create
    m_router.registerAction(QStringLiteral("patentFiling.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("matterId"), QJsonValue::Double, true}, FieldSpec{QStringLiteral("triggerDate"), QJsonValue::String, false} },
        /*handler*/ [](const QJsonObject &payload){ const Key matterId = static_cast<Key>(payload.value("matterId").toDouble()); QDate trig = QDate::currentDate(); if (payload.contains("triggerDate")) { const QString s = payload.value("triggerDate").toString(); QDate d = QDate::fromString(s, Qt::ISODate); if (!d.isValid()) { DispatchResult r; r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("Invalid triggerDate"); return r; } trig = d; } const Key id = Nutdb::InsertPatentFiling(matterId, trig); DispatchResult r; if (id == 0) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = QStringLiteral("Failed to insert patentFiling"); return r; } const PatentFilingData pf = Nutdb::GetPatentFiling(id); r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(pf.PatentFilingId)}, {"fkInventorInterview", static_cast<double>(pf.fkInventorInterview)}}; return r; }
    });

    // patentFiling.get
    m_router.registerAction(QStringLiteral("patentFiling.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("id"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            const PatentFilingData pf = Nutdb::GetPatentFiling(id);
            DispatchResult r;
            if (pf.PatentFilingId == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("PatentFiling not found"); return r; }
            r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(pf.PatentFilingId)}, {"fkInventorInterview", static_cast<double>(pf.fkInventorInterview)}}; return r;
        }
    });

    // patentMatter.create
    m_router.registerAction(QStringLiteral("patentMatter.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("docketNumber"), QJsonValue::String, true} },
        /*handler*/ [](const QJsonObject &payload){ const QString docket = payload.value("docketNumber").toString(); DispatchResult r; if (docket.trimmed().isEmpty()) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("docketNumber is required"); return r; } const Key id = Nutdb::InsertPatentMatter(docket); if (id == 0) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = QStringLiteral("Failed to insert patentMatter"); return r; } const PatentMatterData m = Nutdb::GetPatentMatter(id); r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(m.PatentMatterId)}}; return r; }
    });

    // patentResponse.create
    m_router.registerAction(QStringLiteral("patentResponse.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("matterId"), QJsonValue::Double, true}, FieldSpec{QStringLiteral("triggerDate"), QJsonValue::String, false} },
        /*handler*/ [](const QJsonObject &payload){ const Key matterId = static_cast<Key>(payload.value("matterId").toDouble()); QDate trig = QDate::currentDate(); if (payload.contains("triggerDate")) { const QString s = payload.value("triggerDate").toString(); QDate d = QDate::fromString(s, Qt::ISODate); if (!d.isValid()) { DispatchResult r; r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("Invalid triggerDate"); return r; } trig = d; } const Key id = Nutdb::InsertPatentResponse(matterId, trig); DispatchResult r; if (id == 0) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = QStringLiteral("Failed to insert patentResponse"); return r; } const PatentResponseData pr = Nutdb::GetPatentResponse(id); r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(pr.PatentResponseId)}, {"fkOfficeAction", static_cast<double>(pr.fkOfficeAction)}, {"fkAsFiledResponse", static_cast<double>(pr.fkAsFiledResponse)}, {"fkLastFiledResponse", static_cast<double>(pr.fkLastFiledResponse)}, {"fkClaimAmendment", static_cast<double>(pr.fkClaimAmendment)}}; return r; }
    });

    // patentResponse.get
    m_router.registerAction(QStringLiteral("patentResponse.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("id"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            const PatentResponseData pr = Nutdb::GetPatentResponse(id);
            DispatchResult r;
            if (pr.PatentResponseId == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("PatentResponse not found"); return r; }
            r.ok = true;
            r.result = QJsonObject{
                {"id", static_cast<double>(pr.PatentResponseId)},
                {"fkOfficeAction", static_cast<double>(pr.fkOfficeAction)},
                {"fkAsFiledResponse", static_cast<double>(pr.fkAsFiledResponse)},
                {"fkLastFiledResponse", static_cast<double>(pr.fkLastFiledResponse)},
                {"fkClaimAmendment", static_cast<double>(pr.fkClaimAmendment)}
            };
            return r;
        }
    });

    // person.create
    m_router.registerAction(QStringLiteral("person.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("firstName"), QJsonValue::String, true}, FieldSpec{QStringLiteral("lastName"), QJsonValue::String, true} },
        /*handler*/ [](const QJsonObject &payload){ const QString first = payload.value("firstName").toString(); const QString last = payload.value("lastName").toString(); DispatchResult r; if (first.trimmed().isEmpty() || last.trimmed().isEmpty()) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("firstName and lastName are required"); return r; } const Key id = Nutdb::InsertPerson(first, last); if (id == 0) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = QStringLiteral("Failed to insert person"); return r; } const PersonData p = Nutdb::GetPerson(id); r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(p.PersonId)}, {"firstName", p.FirstName}, {"lastName", p.LastName}}; return r; }
    });

    // response.create
    m_router.registerAction(QStringLiteral("response.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("matterId"), QJsonValue::Double, true}, FieldSpec{QStringLiteral("triggerDate"), QJsonValue::String, false} },
        /*handler*/ [](const QJsonObject &payload){ const Key matterId = static_cast<Key>(payload.value("matterId").toDouble()); QDate trig = QDate::currentDate(); if (payload.contains("triggerDate")) { const QString s = payload.value("triggerDate").toString(); QDate d = QDate::fromString(s, Qt::ISODate); if (!d.isValid()) { DispatchResult r; r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("Invalid triggerDate"); return r; } trig = d; } const Key id = Nutdb::InsertResponse(matterId, trig); DispatchResult r; if (id == 0) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = QStringLiteral("Failed to insert response"); return r; } const ResponseData rd = Nutdb::GetResponse(id); r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(rd.ResponseId)}, {"fkClientOfficeHours", static_cast<double>(rd.fkClientOfficeHours)}, {"fkExaminerInterview", static_cast<double>(rd.fkExaminerInterview)}, {"mailingDate", rd.MailingDate.toString(Qt::ISODate)}, {"dateFiled", rd.DateFiled.toString(Qt::ISODate)}, {"fkResponseAsFiled", static_cast<double>(rd.fkResponseAsFiled)}, {"fkActionDocument", static_cast<double>(rd.fkActionDocument)}}; return r; }
    });

    // task.create (generic)
    m_router.registerAction(QStringLiteral("task.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("matterId"), QJsonValue::Double, true}, FieldSpec{QStringLiteral("triggerDate"), QJsonValue::String, false} },
        /*handler*/ [](const QJsonObject &payload){ const Key matterId = static_cast<Key>(payload.value("matterId").toDouble()); QDate trig = QDate::currentDate(); if (payload.contains("triggerDate")) { const QString s = payload.value("triggerDate").toString(); QDate d = QDate::fromString(s, Qt::ISODate); if (!d.isValid()) { DispatchResult r; r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("Invalid triggerDate"); return r; } trig = d; } const Key id = Nutdb::InsertTask(matterId, trig); DispatchResult r; if (id == 0) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = QStringLiteral("Failed to insert task"); return r; } const TaskData td = Nutdb::GetTask(id); r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(td.TaskId)}, {"fkMatter", static_cast<double>(td.fkMatter)}, {"dateAssigned", td.DateAssigned.toString(Qt::ISODate)}, {"fkDeadline", static_cast<double>(td.fkDeadline)}, {"expectedFee", td.ExpectedFee}, {"feePercent", td.FeePercent}, {"approvalRequested", td.ApprovalRequested.toString(Qt::ISODate)}, {"approvalReceived", td.ApprovalReceived.toString(Qt::ISODate)}, {"complete", td.Complete}, {"fkWorkAttorney", static_cast<double>(td.fkWorkAttorney)}, {"fkParalegal", static_cast<double>(td.fkParalegal)}, {"fkAuthorizationDocument", static_cast<double>(td.fkAuthorizationDocument)}, {"oldTaskId", static_cast<double>(td.OldTaskId)}, {"fkTaskType", static_cast<double>(td.fkTaskType)}}; return r; }
    });

    // taskNinety.create
    m_router.registerAction(QStringLiteral("taskNinety.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("matterId"), QJsonValue::Double, true}, FieldSpec{QStringLiteral("triggerDate"), QJsonValue::String, false} },
        /*handler*/ [](const QJsonObject &payload){ const Key matterId = static_cast<Key>(payload.value("matterId").toDouble()); QDate trig = QDate::currentDate(); if (payload.contains("triggerDate")) { const QString s = payload.value("triggerDate").toString(); QDate d = QDate::fromString(s, Qt::ISODate); if (!d.isValid()) { DispatchResult r; r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("Invalid triggerDate"); return r; } trig = d; } const Key id = Nutdb::InsertTaskNinety(matterId, trig); DispatchResult r; if (id == 0) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = QStringLiteral("Failed to insert 90-day task"); return r; } const TaskData td = Nutdb::GetTask(id); r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(td.TaskId)}, {"fkMatter", static_cast<double>(td.fkMatter)}, {"dateAssigned", td.DateAssigned.toString(Qt::ISODate)}, {"fkDeadline", static_cast<double>(td.fkDeadline)}, {"expectedFee", td.ExpectedFee}, {"feePercent", td.FeePercent}, {"approvalRequested", td.ApprovalRequested.toString(Qt::ISODate)}, {"approvalReceived", td.ApprovalReceived.toString(Qt::ISODate)}, {"complete", td.Complete}, {"fkWorkAttorney", static_cast<double>(td.fkWorkAttorney)}, {"fkParalegal", static_cast<double>(td.fkParalegal)}, {"fkAuthorizationDocument", static_cast<double>(td.fkAuthorizationDocument)}, {"oldTaskId", static_cast<double>(td.OldTaskId)}, {"fkTaskType", static_cast<double>(td.fkTaskType)}}; return r; }
    });

    // taskSixty.create
    m_router.registerAction(QStringLiteral("taskSixty.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("matterId"), QJsonValue::Double, true}, FieldSpec{QStringLiteral("triggerDate"), QJsonValue::String, false} },
        /*handler*/ [](const QJsonObject &payload){ const Key matterId = static_cast<Key>(payload.value("matterId").toDouble()); QDate trig = QDate::currentDate(); if (payload.contains("triggerDate")) { const QString s = payload.value("triggerDate").toString(); QDate d = QDate::fromString(s, Qt::ISODate); if (!d.isValid()) { DispatchResult r; r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("Invalid triggerDate"); return r; } trig = d; } const Key id = Nutdb::InsertTaskSixty(matterId, trig); DispatchResult r; if (id == 0) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = QStringLiteral("Failed to insert 60-day task"); return r; } const TaskData td = Nutdb::GetTask(id); r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(td.TaskId)}}; return r; }
    });

    // taskThirty.create
    m_router.registerAction(QStringLiteral("taskThirty.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("matterId"), QJsonValue::Double, true}, FieldSpec{QStringLiteral("triggerDate"), QJsonValue::String, false} },
        /*handler*/ [](const QJsonObject &payload){ const Key matterId = static_cast<Key>(payload.value("matterId").toDouble()); QDate trig = QDate::currentDate(); if (payload.contains("triggerDate")) { const QString s = payload.value("triggerDate").toString(); QDate d = QDate::fromString(s, Qt::ISODate); if (!d.isValid()) { DispatchResult r; r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("Invalid triggerDate"); return r; } trig = d; } const Key id = Nutdb::InsertTaskThirty(matterId, trig); DispatchResult r; if (id == 0) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = QStringLiteral("Failed to insert 30-day task"); return r; } const TaskData td = Nutdb::GetTask(id); r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(td.TaskId)}}; return r; }
    });

    // taskTwoMonth.create
    m_router.registerAction(QStringLiteral("taskTwoMonth.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("matterId"), QJsonValue::Double, true}, FieldSpec{QStringLiteral("triggerDate"), QJsonValue::String, false} },
        /*handler*/ [](const QJsonObject &payload){ const Key matterId = static_cast<Key>(payload.value("matterId").toDouble()); QDate trig = QDate::currentDate(); if (payload.contains("triggerDate")) { const QString s = payload.value("triggerDate").toString(); QDate d = QDate::fromString(s, Qt::ISODate); if (!d.isValid()) { DispatchResult r; r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("Invalid triggerDate"); return r; } trig = d; } const Key id = Nutdb::InsertTaskTwoMonth(matterId, trig); DispatchResult r; if (id == 0) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = QStringLiteral("Failed to insert 2-month task"); return r; } const TaskData td = Nutdb::GetTask(id); r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(td.TaskId)}, {"fkMatter", static_cast<double>(td.fkMatter)}, {"dateAssigned", td.DateAssigned.toString(Qt::ISODate)}, {"fkDeadline", static_cast<double>(td.fkDeadline)}, {"expectedFee", td.ExpectedFee}, {"feePercent", td.FeePercent}, {"approvalRequested", td.ApprovalRequested.toString(Qt::ISODate)}, {"approvalReceived", td.ApprovalReceived.toString(Qt::ISODate)}, {"complete", td.Complete}, {"fkWorkAttorney", static_cast<double>(td.fkWorkAttorney)}, {"fkParalegal", static_cast<double>(td.fkParalegal)}, {"fkAuthorizationDocument", static_cast<double>(td.fkAuthorizationDocument)}, {"oldTaskId", static_cast<double>(td.OldTaskId)}, {"fkTaskType", static_cast<double>(td.fkTaskType)}}; return r; }
    });

    // taskThreeMonth.create
    m_router.registerAction(QStringLiteral("taskThreeMonth.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("matterId"), QJsonValue::Double, true}, FieldSpec{QStringLiteral("triggerDate"), QJsonValue::String, false} },
        /*handler*/ [](const QJsonObject &payload){ const Key matterId = static_cast<Key>(payload.value("matterId").toDouble()); QDate trig = QDate::currentDate(); if (payload.contains("triggerDate")) { const QString s = payload.value("triggerDate").toString(); QDate d = QDate::fromString(s, Qt::ISODate); if (!d.isValid()) { DispatchResult r; r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("Invalid triggerDate"); return r; } trig = d; } const Key id = Nutdb::InsertTaskThreeMonth(matterId, trig); DispatchResult r; if (id == 0) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = QStringLiteral("Failed to insert 3-month task"); return r; } const TaskData td = Nutdb::GetTask(id); r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(td.TaskId)}, {"fkMatter", static_cast<double>(td.fkMatter)}, {"dateAssigned", td.DateAssigned.toString(Qt::ISODate)}, {"fkDeadline", static_cast<double>(td.fkDeadline)}, {"expectedFee", td.ExpectedFee}, {"feePercent", td.FeePercent}, {"approvalRequested", td.ApprovalRequested.toString(Qt::ISODate)}, {"approvalReceived", td.ApprovalReceived.toString(Qt::ISODate)}, {"complete", td.Complete}, {"fkWorkAttorney", static_cast<double>(td.fkWorkAttorney)}, {"fkParalegal", static_cast<double>(td.fkParalegal)}, {"fkAuthorizationDocument", static_cast<double>(td.fkAuthorizationDocument)}, {"oldTaskId", static_cast<double>(td.OldTaskId)}, {"fkTaskType", static_cast<double>(td.fkTaskType)}}; return r; }
    });

    // taskThreeMonthHard.create
    m_router.registerAction(QStringLiteral("taskThreeMonthHard.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("matterId"), QJsonValue::Double, true}, FieldSpec{QStringLiteral("triggerDate"), QJsonValue::String, false} },
        /*handler*/ [](const QJsonObject &payload){ const Key matterId = static_cast<Key>(payload.value("matterId").toDouble()); QDate trig = QDate::currentDate(); if (payload.contains("triggerDate")) { const QString s = payload.value("triggerDate").toString(); QDate d = QDate::fromString(s, Qt::ISODate); if (!d.isValid()) { DispatchResult r; r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("Invalid triggerDate"); return r; } trig = d; } const Key id = Nutdb::InsertTaskThreeMonthHard(matterId, trig); DispatchResult r; if (id == 0) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = QStringLiteral("Failed to insert 3-month hard task"); return r; } const TaskData td = Nutdb::GetTask(id); r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(td.TaskId)}, {"fkMatter", static_cast<double>(td.fkMatter)}, {"dateAssigned", td.DateAssigned.toString(Qt::ISODate)}, {"fkDeadline", static_cast<double>(td.fkDeadline)}, {"expectedFee", td.ExpectedFee}, {"feePercent", td.FeePercent}, {"approvalRequested", td.ApprovalRequested.toString(Qt::ISODate)}, {"approvalReceived", td.ApprovalReceived.toString(Qt::ISODate)}, {"complete", td.Complete}, {"fkWorkAttorney", static_cast<double>(td.fkWorkAttorney)}, {"fkParalegal", static_cast<double>(td.fkParalegal)}, {"fkAuthorizationDocument", static_cast<double>(td.fkAuthorizationDocument)}, {"oldTaskId", static_cast<double>(td.OldTaskId)}, {"fkTaskType", static_cast<double>(td.fkTaskType)}}; return r; }
    });

    // taskWithMatter.create
    m_router.registerAction(QStringLiteral("taskWithMatter.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("docketNumber"), QJsonValue::String, true}, FieldSpec{QStringLiteral("triggerDate"), QJsonValue::String, false} },
        /*handler*/ [](const QJsonObject &payload){ const QString docket = payload.value("docketNumber").toString(); if (docket.trimmed().isEmpty()) { DispatchResult r; r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("docketNumber is required"); return r; } QDate trig = QDate::currentDate(); if (payload.contains("triggerDate")) { const QString s = payload.value("triggerDate").toString(); QDate d = QDate::fromString(s, Qt::ISODate); if (!d.isValid()) { DispatchResult r; r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("Invalid triggerDate"); return r; } trig = d; } const Key id = Nutdb::InsertTaskWithMatter(docket, trig); DispatchResult r; if (id == 0) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = QStringLiteral("Failed to insert task with matter"); return r; } const TaskData td = Nutdb::GetTask(id); r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(td.TaskId)}}; return r; }
    });

    // trademarkFiling.create
    m_router.registerAction(QStringLiteral("trademarkFiling.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("matterId"), QJsonValue::Double, true}, FieldSpec{QStringLiteral("triggerDate"), QJsonValue::String, false} },
        /*handler*/ [](const QJsonObject &payload){ const Key matterId = static_cast<Key>(payload.value("matterId").toDouble()); QDate trig = QDate::currentDate(); if (payload.contains("triggerDate")) { const QString s = payload.value("triggerDate").toString(); QDate d = QDate::fromString(s, Qt::ISODate); if (!d.isValid()) { DispatchResult r; r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("Invalid triggerDate"); return r; } trig = d; } const Key id = Nutdb::InsertTrademarkFiling(matterId, trig); DispatchResult r; if (id == 0) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = QStringLiteral("Failed to insert trademarkFiling"); return r; } const TrademarkFilingData tf = Nutdb::GetTrademarkFiling(id); r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(tf.TrademarkFilingId)}}; return r; }
    });

    // trademarkFiling.get
    m_router.registerAction(QStringLiteral("trademarkFiling.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("id"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            const TrademarkFilingData tf = Nutdb::GetTrademarkFiling(id);
            DispatchResult r;
            if (tf.TrademarkFilingId == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("TrademarkFiling not found"); return r; }
            r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(tf.TrademarkFilingId)}}; return r;
        }
    });

    // trademarkMatter.create
    m_router.registerAction(QStringLiteral("trademarkMatter.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("docketNumber"), QJsonValue::String, true} },
        /*handler*/ [](const QJsonObject &payload){ const QString docket = payload.value("docketNumber").toString(); DispatchResult r; if (docket.trimmed().isEmpty()) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("docketNumber is required"); return r; } const Key id = Nutdb::InsertTrademarkMatter(docket); if (id == 0) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = QStringLiteral("Failed to insert trademarkMatter"); return r; } const TrademarkMatterData m = Nutdb::GetTrademarkMatter(id); r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(m.TrademarkMatterId)}}; return r; }
    });

    // trademarkResponse.create
    m_router.registerAction(QStringLiteral("trademarkResponse.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("matterId"), QJsonValue::Double, true}, FieldSpec{QStringLiteral("triggerDate"), QJsonValue::String, false} },
        /*handler*/ [](const QJsonObject &payload){ const Key matterId = static_cast<Key>(payload.value("matterId").toDouble()); QDate trig = QDate::currentDate(); if (payload.contains("triggerDate")) { const QString s = payload.value("triggerDate").toString(); QDate d = QDate::fromString(s, Qt::ISODate); if (!d.isValid()) { DispatchResult r; r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("Invalid triggerDate"); return r; } trig = d; } const Key id = Nutdb::InsertTrademarkResponse(matterId, trig); DispatchResult r; if (id == 0) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = QStringLiteral("Failed to insert trademarkResponse"); return r; } const TrademarkResponseData tr = Nutdb::GetTrademarkResponse(id); r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(tr.TrademarkResponseId)}, {"fkOfficeAction", static_cast<double>(tr.fkOfficeAction)}}; return r; }
    });

    // trademarkResponse.get
    m_router.registerAction(QStringLiteral("trademarkResponse.get"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("id"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){
            const Key id = static_cast<Key>(payload.value(QStringLiteral("id")).toDouble());
            const TrademarkResponseData tr = Nutdb::GetTrademarkResponse(id);
            DispatchResult r;
            if (tr.TrademarkResponseId == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("TrademarkResponse not found"); return r; }
            r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(tr.TrademarkResponseId)}, {"fkOfficeAction", static_cast<double>(tr.fkOfficeAction)}}; return r;
        }
    });

    // workAttorney.create
    m_router.registerAction(QStringLiteral("workAttorney.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("firstName"), QJsonValue::String, true}, FieldSpec{QStringLiteral("lastName"), QJsonValue::String, true} },
        /*handler*/ [](const QJsonObject &payload){ const QString first = payload.value("firstName").toString(); const QString last = payload.value("lastName").toString(); DispatchResult r; if (first.trimmed().isEmpty() || last.trimmed().isEmpty()) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("firstName and lastName are required"); return r; } const Key id = Nutdb::InsertWorkAttorney(first, last); if (id == 0) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = QStringLiteral("Failed to insert work attorney"); return r; } const PersonData p = Nutdb::GetPerson(id); r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(p.PersonId)}, {"firstName", p.FirstName}, {"lastName", p.LastName}}; return r; }
    });
    // object.delete (table)
    m_router.registerAction(QStringLiteral("object.delete"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("id"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){ const Key id = static_cast<Key>(payload.value("id").toDouble()); QSqlQuery q(QSqlDatabase::database()); q.prepare(QStringLiteral("DELETE FROM object WHERE ObjectId = :id")); q.bindValue(":id", QVariant::fromValue(id)); DispatchResult r; if (!q.exec()) { r.ok = false; r.errorCode = QStringLiteral("EDELETE"); r.errorMessage = q.lastError().text(); return r; } r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(id)}, {"deleted", true}}; return r; }
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

    // person.update: full-record update
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
            DispatchResult r;
            Person person{id};
            if (person.getId() == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Person not found"); return r; }
            PersonData dat = Nutdb::GetPerson(id);
            if (dat.PersonId == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Person not found"); return r; }
            if (payload.contains("firstName")) dat.FirstName = payload.value("firstName").toString();
            if (payload.contains("lastName")) dat.LastName = payload.value("lastName").toString();
            if (payload.contains("fkResidence")) dat.fkResidence = static_cast<Key>(payload.value("fkResidence").toDouble());
            if (payload.contains("fkCitizenship")) dat.fkCitizenship = static_cast<Key>(payload.value("fkCitizenship").toDouble());
            if (payload.contains("oldId")) dat.OldId = static_cast<Key>(payload.value("oldId").toDouble());

            QSqlDatabase db = QSqlDatabase::database();
            bool tx = db.isValid() ? db.transaction() : false;
            if (!person.slotUpdate(dat)) { if (tx) db.rollback(); r.ok = false; r.errorCode = QStringLiteral("EUPDATE"); r.errorMessage = QStringLiteral("Failed to update person"); return r; }
            if (tx) db.commit();

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

    // response.update: full-record update
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
            DispatchResult r;
            Response resp{id};
            if (resp.getId() == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Response not found"); return r; }
            ResponseData dat = Nutdb::GetResponse(id);
            if (dat.ResponseId == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Response not found"); return r; }
            if (payload.contains("fkClientOfficeHours")) dat.fkClientOfficeHours = static_cast<Key>(payload.value("fkClientOfficeHours").toDouble());
            if (payload.contains("fkExaminerInterview")) dat.fkExaminerInterview = static_cast<Key>(payload.value("fkExaminerInterview").toDouble());
            auto parseDate = [](const QJsonObject &obj, const char *key, QDate &out, QString &err) -> bool {
                if (!obj.contains(key)) return true;
                const QString s = obj.value(QString::fromUtf8(key)).toString();
                const QDate d = QDate::fromString(s, Qt::ISODate);
                if (!d.isValid()) { err = QStringLiteral("Invalid date for '%1'").arg(QString::fromUtf8(key)); return false; }
                out = d; return true;
            };
            QString perr; QDate dt;
            if (!parseDate(payload, "mailingDate", dt, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; }
            if (payload.contains("mailingDate")) dat.MailingDate = dt;
            if (!parseDate(payload, "dateFiled", dt, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; }
            if (payload.contains("dateFiled")) dat.DateFiled = dt;
            if (payload.contains("fkResponseAsFiled")) dat.fkResponseAsFiled = static_cast<Key>(payload.value("fkResponseAsFiled").toDouble());
            if (payload.contains("fkActionDocument")) dat.fkActionDocument = static_cast<Key>(payload.value("fkActionDocument").toDouble());

            QSqlDatabase db = QSqlDatabase::database();
            bool tx = db.isValid() ? db.transaction() : false;
            if (!resp.slotUpdate(dat)) { if (tx) db.rollback(); r.ok = false; r.errorCode = QStringLiteral("EUPDATE"); r.errorMessage = QStringLiteral("Failed to update response"); return r; }
            if (tx) db.commit();

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
    // task.list (table)
    m_router.registerAction(QStringLiteral("task.list"), ActionSpec{
        /*fields*/ QList<FieldSpec>{},
        /*handler*/ [](const QJsonObject &){ QSqlQuery q(QSqlDatabase::database()); DispatchResult r; r.ok = true; QJsonArray items; if (q.exec(QStringLiteral("SELECT TaskId, fkTaskType, OldTaskId FROM task ORDER BY TaskId DESC LIMIT 500"))) { while (q.next()) items.append(QJsonObject{{"id", static_cast<double>(q.value(0).toUInt())}, {"fkTaskType", static_cast<double>(q.value(1).toUInt())}, {"oldTaskId", static_cast<double>(q.value(2).toUInt())}}); } r.result = QJsonObject{{"items", items}}; return r; }
    });

    // task.update: full-record update
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
            DispatchResult r;
            Task task{id};
            if (task.getId() == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Task not found"); return r; }
            TaskData dat = Nutdb::GetTask(id);
            if (dat.TaskId == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Task not found"); return r; }
            auto parseDate = [](const QJsonObject &obj, const char *key, QDate &out, QString &err) -> bool { if (!obj.contains(key)) return true; const QString s = obj.value(QString::fromUtf8(key)).toString(); const QDate d = QDate::fromString(s, Qt::ISODate); if (!d.isValid()) { err = QStringLiteral("Invalid date for '%1'").arg(QString::fromUtf8(key)); return false; } out = d; return true; };
            QString perr; QDate dt;
            if (payload.contains("fkMatter")) dat.fkMatter = static_cast<Key>(payload.value("fkMatter").toDouble());
            if (!parseDate(payload, "dateAssigned", dt, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; }
            if (payload.contains("dateAssigned")) dat.DateAssigned = dt;
            if (payload.contains("fkDeadline")) dat.fkDeadline = static_cast<Key>(payload.value("fkDeadline").toDouble());
            if (payload.contains("expectedFee")) dat.ExpectedFee = static_cast<int>(payload.value("expectedFee").toDouble());
            if (payload.contains("feePercent")) dat.FeePercent = static_cast<int>(payload.value("feePercent").toDouble());
            if (!parseDate(payload, "approvalRequested", dt, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; }
            if (payload.contains("approvalRequested")) dat.ApprovalRequested = dt;
            if (!parseDate(payload, "approvalReceived", dt, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; }
            if (payload.contains("approvalReceived")) dat.ApprovalReceived = dt;
            if (payload.contains("complete")) dat.Complete = payload.value("complete").toBool();
            if (payload.contains("fkWorkAttorney")) dat.fkWorkAttorney = static_cast<Key>(payload.value("fkWorkAttorney").toDouble());
            if (payload.contains("fkParalegal")) dat.fkParalegal = static_cast<Key>(payload.value("fkParalegal").toDouble());
            if (payload.contains("fkAuthorizationDocument")) dat.fkAuthorizationDocument = static_cast<Key>(payload.value("fkAuthorizationDocument").toDouble());
            if (payload.contains("oldTaskId")) dat.OldTaskId = static_cast<Key>(payload.value("oldTaskId").toDouble());
            if (payload.contains("fkTaskType")) dat.fkTaskType = static_cast<Key>(payload.value("fkTaskType").toDouble());

            QSqlDatabase db = QSqlDatabase::database();
            bool tx = db.isValid() ? db.transaction() : false;
            if (!task.Update(dat)) { if (tx) db.rollback(); r.ok = false; r.errorCode = QStringLiteral("EUPDATE"); r.errorMessage = QStringLiteral("Failed to update task"); return r; }
            if (tx) db.commit();

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

    // trademarkMatter.update: full-record update
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
            DispatchResult r; TrademarkMatter tm{id};
            if (tm.getId() == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("TrademarkMatter not found"); return r; }
            TrademarkMatterData dat = Nutdb::GetTrademarkMatter(id);
            if (dat.TrademarkMatterId == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("TrademarkMatter not found"); return r; }
            auto parseDate = [](const QJsonObject &obj, const char *key, QDate &out, QString &err) -> bool { if (!obj.contains(key)) return true; const QString s = obj.value(QString::fromUtf8(key)).toString(); const QDate d = QDate::fromString(s, Qt::ISODate); if (!d.isValid()) { err = QStringLiteral("Invalid date for '%1'").arg(QString::fromUtf8(key)); return false; } out = d; return true; };
            QString perr; QDate dt;
            if (!parseDate(payload, "firstUseInCommerce", dt, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; } if (payload.contains("firstUseInCommerce")) dat.FirstUseInCommerce = dt;
            if (payload.contains("internationalClass")) dat.InternationalClass = static_cast<int>(payload.value("internationalClass").toDouble());
            if (payload.contains("fkStatus")) dat.fkStatus = static_cast<Key>(payload.value("fkStatus").toDouble());
            if (payload.contains("serialNumber")) dat.SerialNumber = payload.value("serialNumber").toString();
            if (payload.contains("registrationNumber")) dat.RegistrationNumber = payload.value("registrationNumber").toString();
            if (!parseDate(payload, "publicationDate", dt, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; } if (payload.contains("publicationDate")) dat.PublicationDate = dt;
            if (!parseDate(payload, "windowOpens", dt, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; } if (payload.contains("windowOpens")) dat.WindowOpens = dt;
            if (!parseDate(payload, "nofeeWindowCloses", dt, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; } if (payload.contains("nofeeWindowCloses")) dat.NofeeWindowCloses = dt;
            if (!parseDate(payload, "finalWindowCloses", dt, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; } if (payload.contains("finalWindowCloses")) dat.FinalWindowCloses = dt;
            if (payload.contains("fkTrademarkExaminer")) dat.fkTrademarkExaminer = static_cast<Key>(payload.value("fkTrademarkExaminer").toDouble());
            if (payload.contains("fkFilingBasis")) dat.fkFilingBasis = static_cast<Key>(payload.value("fkFilingBasis").toDouble());
            if (payload.contains("fkTrademarkJurisdiction")) dat.fkTrademarkJurisdiction = static_cast<Key>(payload.value("fkTrademarkJurisdiction").toDouble());
            if (payload.contains("fkSpecimen")) dat.fkSpecimen = static_cast<Key>(payload.value("fkSpecimen").toDouble());
            if (payload.contains("fkEvidenceOfUse")) dat.fkEvidenceOfUse = static_cast<Key>(payload.value("fkEvidenceOfUse").toDouble());
            if (payload.contains("mark")) dat.Mark = payload.value("mark").toString();
            if (payload.contains("goodsServices")) dat.GoodsServices = payload.value("goodsServices").toString();

            QSqlDatabase db = QSqlDatabase::database(); bool tx = db.isValid() ? db.transaction() : false;
            if (!tm.slotUpdate(dat)) { if (tx) db.rollback(); r.ok = false; r.errorCode = QStringLiteral("EUPDATE"); r.errorMessage = QStringLiteral("Failed to update trademarkMatter"); return r; }
            if (tx) db.commit();

            const TrademarkMatterData m = Nutdb::GetTrademarkMatter(id);
            r.ok = true; r.result = QJsonObject{
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
            }; return r;
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

    // patentMatter.update: full-record update
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
            PatentMatterData dat = Nutdb::GetPatentMatter(id);
            if (dat.PatentMatterId == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("PatentMatter not found"); return r; }
            auto parseDate = [](const QJsonObject &obj, const char *key, QDate &out, QString &err) -> bool { if (!obj.contains(key)) return true; const QString s = obj.value(QString::fromUtf8(key)).toString(); const QDate d = QDate::fromString(s, Qt::ISODate); if (!d.isValid()) { err = QStringLiteral("Invalid date for '%1'").arg(QString::fromUtf8(key)); return false; } out = d; return true; };
            QString perr; QDate dt;
            if (!parseDate(payload, "filingDate", dt, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; } if (payload.contains("filingDate")) dat.FilingDate = dt;
            if (payload.contains("applicationSerialNumber")) dat.ApplicationSerialNumber = payload.value("applicationSerialNumber").toString();
            if (payload.contains("confirmationNumber")) dat.ConfirmationNumber = payload.value("confirmationNumber").toString();
            if (payload.contains("artUnit")) dat.ArtUnit = payload.value("artUnit").toString();
            if (payload.contains("patentNumber")) dat.PatentNumber = payload.value("patentNumber").toString();
            if (payload.contains("fkExaminer")) dat.fkExaminer = static_cast<Key>(payload.value("fkExaminer").toDouble());
            if (payload.contains("fkFirstInventor")) dat.fkFirstInventor = static_cast<Key>(payload.value("fkFirstInventor").toDouble());
            if (payload.contains("fkSupervisoryExaminer")) dat.fkSupervisoryExaminer = static_cast<Key>(payload.value("fkSupervisoryExaminer").toDouble());
            if (payload.contains("fkApplicant")) dat.fkApplicant = static_cast<Key>(payload.value("fkApplicant").toDouble());
            if (!parseDate(payload, "barDate", dt, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; } if (payload.contains("barDate")) dat.BarDate = dt;
            if (!parseDate(payload, "criticalDate", dt, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; } if (payload.contains("criticalDate")) dat.CriticalDate = dt;
            if (!parseDate(payload, "dateIssued", dt, perr)) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = perr; return r; } if (payload.contains("dateIssued")) dat.DateIssued = dt;
            if (payload.contains("fkSpecification")) dat.fkSpecification = static_cast<Key>(payload.value("fkSpecification").toDouble());
            if (payload.contains("fkDrawings")) dat.fkDrawings = static_cast<Key>(payload.value("fkDrawings").toDouble());
            if (payload.contains("fkAsFiledClaims")) dat.fkAsFiledClaims = static_cast<Key>(payload.value("fkAsFiledClaims").toDouble());

            QSqlDatabase db = QSqlDatabase::database(); bool tx = db.isValid() ? db.transaction() : false;
            if (!Nutdb::UpdatePatentMatter(dat)) { if (tx) db.rollback(); r.ok = false; r.errorCode = QStringLiteral("EUPDATE"); r.errorMessage = QStringLiteral("Failed to update patentMatter"); return r; }
            if (tx) db.commit();

            const PatentMatterData m = Nutdb::GetPatentMatter(id);
            r.ok = true; r.result = QJsonObject{
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
            }; return r;
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
    // tag.list (table)
    m_router.registerAction(QStringLiteral("tag.list"), ActionSpec{
        /*fields*/ QList<FieldSpec>{},
        /*handler*/ [](const QJsonObject &){ QSqlQuery q(QSqlDatabase::database()); DispatchResult r; r.ok = true; QJsonArray items; if (q.exec(QStringLiteral("SELECT TagId, TagText FROM tag ORDER BY TagText"))) { while (q.next()) items.append(QJsonObject{{"tagId", static_cast<double>(q.value(0).toUInt())}, {"tagText", q.value(1).toString()}}); } r.result = QJsonObject{{"items", items}}; return r; }
    });
    // tag.create (table)
    m_router.registerAction(QStringLiteral("tag.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("tagText"), QJsonValue::String, true} },
        /*handler*/ [](const QJsonObject &payload){ const QString text = payload.value("tagText").toString(); DispatchResult r; if (text.trimmed().isEmpty()) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("tagText required"); return r; } QSqlQuery q(QSqlDatabase::database()); q.prepare(QStringLiteral("INSERT INTO tag (TagText) VALUES (:t)")); q.bindValue(":t", text); if (!q.exec()) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = q.lastError().text(); return r; } const Key id = q.lastInsertId().toUInt(); r.ok = true; r.result = QJsonObject{{"tagId", static_cast<double>(id)}, {"tagText", text}}; return r; }
    });
    // tag.update (table)
    m_router.registerAction(QStringLiteral("tag.update"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("id"), QJsonValue::Double, true}, FieldSpec{QStringLiteral("tagText"), QJsonValue::String, true} },
        /*handler*/ [](const QJsonObject &payload){ const Key id = static_cast<Key>(payload.value("id").toDouble()); const QString text = payload.value("tagText").toString(); DispatchResult r; if (text.trimmed().isEmpty()) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("tagText required"); return r; } QSqlQuery q(QSqlDatabase::database()); q.prepare(QStringLiteral("UPDATE tag SET TagText = :t WHERE TagId = :id")); q.bindValue(":t", text); q.bindValue(":id", QVariant::fromValue(id)); if (!q.exec()) { r.ok = false; r.errorCode = QStringLiteral("EUPDATE"); r.errorMessage = q.lastError().text(); return r; } r.ok = true; r.result = QJsonObject{{"tagId", static_cast<double>(id)}, {"tagText", text}}; return r; }
    });
    // tag.delete (table)
    m_router.registerAction(QStringLiteral("tag.delete"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("id"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){ const Key id = static_cast<Key>(payload.value("id").toDouble()); QSqlQuery q(QSqlDatabase::database()); q.prepare(QStringLiteral("DELETE FROM tag WHERE TagId = :id")); q.bindValue(":id", QVariant::fromValue(id)); DispatchResult r; if (!q.exec()) { r.ok = false; r.errorCode = QStringLiteral("EDELETE"); r.errorMessage = q.lastError().text(); return r; } r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(id)}, {"deleted", true}}; return r; }
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
    // taskType.list (table)
    m_router.registerAction(QStringLiteral("taskType.list"), ActionSpec{
        /*fields*/ QList<FieldSpec>{},
        /*handler*/ [](const QJsonObject &){ QSqlQuery q(QSqlDatabase::database()); DispatchResult r; r.ok = true; QJsonArray items; if (q.exec(QStringLiteral("SELECT TaskTypeId, fkTaskClass, TaskName FROM taskType ORDER BY TaskName"))) { while (q.next()) items.append(QJsonObject{{"id", static_cast<double>(q.value(0).toUInt())}, {"fkTaskClass", static_cast<double>(q.value(1).toUInt())}, {"taskName", q.value(2).toString()}}); } r.result = QJsonObject{{"items", items}}; return r; }
    });
    // taskType.create (table)
    m_router.registerAction(QStringLiteral("taskType.create"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("fkTaskClass"), QJsonValue::Double, true}, FieldSpec{QStringLiteral("taskName"), QJsonValue::String, true} },
        /*handler*/ [](const QJsonObject &payload){ const Key fk = static_cast<Key>(payload.value("fkTaskClass").toDouble()); const QString name = payload.value("taskName").toString(); DispatchResult r; if (name.trimmed().isEmpty()) { r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("taskName required"); return r; } QSqlQuery q(QSqlDatabase::database()); q.prepare(QStringLiteral("INSERT INTO taskType (fkTaskClass, TaskName) VALUES (:fk, :name)")); q.bindValue(":fk", QVariant::fromValue(fk)); q.bindValue(":name", name); if (!q.exec()) { r.ok = false; r.errorCode = QStringLiteral("EINSERT"); r.errorMessage = q.lastError().text(); return r; } const Key id = q.lastInsertId().toUInt(); r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(id)}, {"fkTaskClass", static_cast<double>(fk)}, {"taskName", name}}; return r; }
    });
    // taskType.update (table)
    m_router.registerAction(QStringLiteral("taskType.update"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("id"), QJsonValue::Double, true}, FieldSpec{QStringLiteral("fkTaskClass"), QJsonValue::Double, false}, FieldSpec{QStringLiteral("taskName"), QJsonValue::String, false} },
        /*handler*/ [](const QJsonObject &payload){ const Key id = static_cast<Key>(payload.value("id").toDouble()); QSqlQuery q(QSqlDatabase::database()); QList<QString> sets; if (payload.contains("fkTaskClass")) sets << QStringLiteral("fkTaskClass = :fk"); if (payload.contains("taskName")) sets << QStringLiteral("TaskName = :name"); if (sets.isEmpty()) { DispatchResult r; r.ok = false; r.errorCode = QStringLiteral("EBADREQ"); r.errorMessage = QStringLiteral("No updatable fields"); return r; } const QString sql = QStringLiteral("UPDATE taskType SET ") + sets.join(QStringLiteral(", ")) + QStringLiteral(" WHERE TaskTypeId = :id"); q.prepare(sql); if (payload.contains("fkTaskClass")) q.bindValue(":fk", QVariant::fromValue(static_cast<Key>(payload.value("fkTaskClass").toDouble()))); if (payload.contains("taskName")) q.bindValue(":name", payload.value("taskName").toString()); q.bindValue(":id", QVariant::fromValue(id)); DispatchResult r; if (!q.exec()) { r.ok = false; r.errorCode = QStringLiteral("EUPDATE"); r.errorMessage = q.lastError().text(); return r; } QSqlQuery qr(QSqlDatabase::database()); qr.prepare(QStringLiteral("SELECT TaskTypeId, fkTaskClass, TaskName FROM taskType WHERE TaskTypeId = :id")); qr.bindValue(":id", QVariant::fromValue(id)); if (!qr.exec() || !qr.next()) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("TaskType not found"); return r; } r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(qr.value(0).toUInt())}, {"fkTaskClass", static_cast<double>(qr.value(1).toUInt())}, {"taskName", qr.value(2).toString()}}; return r; }
    });
    // taskType.delete (table)
    m_router.registerAction(QStringLiteral("taskType.delete"), ActionSpec{
        /*fields*/ QList<FieldSpec>{ FieldSpec{QStringLiteral("id"), QJsonValue::Double, true} },
        /*handler*/ [](const QJsonObject &payload){ const Key id = static_cast<Key>(payload.value("id").toDouble()); QSqlQuery q(QSqlDatabase::database()); q.prepare(QStringLiteral("DELETE FROM taskType WHERE TaskTypeId = :id")); q.bindValue(":id", QVariant::fromValue(id)); DispatchResult r; if (!q.exec()) { r.ok = false; r.errorCode = QStringLiteral("EDELETE"); r.errorMessage = q.lastError().text(); return r; } r.ok = true; r.result = QJsonObject{{"id", static_cast<double>(id)}, {"deleted", true}}; return r; }
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

    // matter.update: full-record update in one pass
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
            DispatchResult r;
            Matter matter{id};
            if (matter.getId() == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Matter not found"); return r; }
            MatterData dat = Nutdb::GetMatter(id);
            if (dat.MatterId == 0) { r.ok = false; r.errorCode = QStringLiteral("ENOTFOUND"); r.errorMessage = QStringLiteral("Matter not found"); return r; }
            if (payload.contains("fkParent")) dat.fkParent = static_cast<Key>(payload.value("fkParent").toDouble());
            if (payload.contains("attorneyDocketNumber")) dat.AttorneyDocketNumber = payload.value("attorneyDocketNumber").toString();
            if (payload.contains("clientDocketNumber")) dat.ClientDocketNumber = payload.value("clientDocketNumber").toString();
            if (payload.contains("title")) dat.Title = payload.value("title").toString();
            if (payload.contains("fkClient")) dat.fkClient = static_cast<Key>(payload.value("fkClient").toDouble());
            if (payload.contains("fkAssigningFirm")) dat.fkAssigningFirm = static_cast<Key>(payload.value("fkAssigningFirm").toDouble());
            if (payload.contains("fkDefaultWorkAttorney")) dat.fkDefaultWorkAttorney = static_cast<Key>(payload.value("fkDefaultWorkAttorney").toDouble());
            if (payload.contains("fkDefaultParalegal")) dat.fkDefaultParalegal = static_cast<Key>(payload.value("fkDefaultParalegal").toDouble());
            if (payload.contains("fkKeyDocument")) dat.fkKeyDocument = static_cast<Key>(payload.value("fkKeyDocument").toDouble());
            if (payload.contains("fkMatterJurisdiction")) dat.fkMatterJurisdiction = static_cast<Key>(payload.value("fkMatterJurisdiction").toDouble());
            if (payload.contains("oldMatterId")) dat.OldMatterId = static_cast<Key>(payload.value("oldMatterId").toDouble());

            QSqlDatabase db = QSqlDatabase::database();
            bool tx = db.isValid() ? db.transaction() : false;
            if (!matter.Update(dat)) { if (tx) db.rollback(); r.ok = false; r.errorCode = QStringLiteral("EUPDATE"); r.errorMessage = QStringLiteral("Failed to update matter"); return r; }
            if (tx) db.commit();

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
