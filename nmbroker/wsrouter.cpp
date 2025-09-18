#include "wsrouter.h"

#include <QJsonArray>

namespace Nutmeg {

void WSRouter::registerAction(const QString &name, const ActionSpec &spec)
{
    m_actions.insert(name, spec);
}

bool WSRouter::hasAction(const QString &name) const
{
    return m_actions.contains(name);
}

DispatchResult WSRouter::dispatch(const QString &action, const QJsonObject &payload) const
{
    DispatchResult out;
    if (!m_actions.contains(action)) {
        out.ok = false;
        out.errorCode = QStringLiteral("ENOACTION");
        out.errorMessage = QStringLiteral("Unknown action '") + action + QStringLiteral("'");
        return out;
    }

    const ActionSpec &spec = m_actions[action];
    QJsonObject details;
    QString msg;
    if (!validate(payload, spec.fields, details, msg)) {
        out.ok = false;
        out.errorCode = QStringLiteral("EBADREQ");
        out.errorMessage = msg.isEmpty() ? QStringLiteral("Invalid payload") : msg;
        out.errorDetails = details;
        return out;
    }

    if (!spec.handler) {
        out.ok = true;
        out.result = QJsonObject{};
        return out;
    }

    return spec.handler(payload);
}

bool WSRouter::validate(const QJsonObject &payload,
                        const QList<FieldSpec> &fields,
                        QJsonObject &details,
                        QString &message) const
{
    QJsonArray missing;
    QJsonArray typeErrors;

    for (const FieldSpec &f : fields) {
        const bool present = payload.contains(f.name);
        if (f.required && !present) {
            missing.append(f.name);
            continue;
        }
        if (present) {
            const QJsonValue v = payload.value(f.name);
            if ((f.type == QJsonValue::Double && !v.isDouble()) ||
                (f.type == QJsonValue::String && !v.isString()) ||
                (f.type == QJsonValue::Bool && !v.isBool()) ||
                (f.type == QJsonValue::Object && !v.isObject()) ||
                (f.type == QJsonValue::Array && !v.isArray()) ||
                (f.type == QJsonValue::Null && !v.isNull()))
            {
                QJsonObject te{{"field", f.name}, {"expected", typeName(f.type)}, {"actual", typeName(v.type())}};
                typeErrors.append(te);
            }
        }
    }

    if (!missing.isEmpty() || !typeErrors.isEmpty()) {
        if (!missing.isEmpty()) details.insert("missing", missing);
        if (!typeErrors.isEmpty()) details.insert("typeErrors", typeErrors);
        message = QStringLiteral("Invalid payload");
        return false;
    }

    return true;
}

QString WSRouter::typeName(QJsonValue::Type t)
{
    switch (t) {
    case QJsonValue::Null: return QStringLiteral("null");
    case QJsonValue::Bool: return QStringLiteral("bool");
    case QJsonValue::Double: return QStringLiteral("number");
    case QJsonValue::String: return QStringLiteral("string");
    case QJsonValue::Array: return QStringLiteral("array");
    case QJsonValue::Object: return QStringLiteral("object");
    case QJsonValue::Undefined: default: return QStringLiteral("undefined");
    }
}

} // namespace Nutmeg
