#ifndef NUTMEG_WSROUTER_H
#define NUTMEG_WSROUTER_H

#include <QJsonObject>
#include <QJsonValue>
#include <QMap>
#include <QString>
#include <QList>
#include <functional>

namespace Nutmeg {

struct FieldSpec {
    QString name;
    QJsonValue::Type type;
    bool required{false};
};

using Handler = std::function<QJsonObject(const QJsonObject& payload)>;

struct ActionSpec {
    QList<FieldSpec> fields; // schema
    Handler handler;
};

struct DispatchResult {
    bool ok{false};
    QJsonObject result; // when ok
    QString errorCode;  // when !ok
    QString errorMessage;
    QJsonObject errorDetails; // e.g., validation issues
};

class WSRouter {
  public:
    void registerAction(const QString &name, const ActionSpec &spec);
    bool hasAction(const QString &name) const;

    // Validates payload against schema, then invokes handler.
    // If unknown action or validation fails, returns !ok with EBADREQ/ENOACTION.
    DispatchResult dispatch(const QString &action, const QJsonObject &payload) const;

  private:
    QMap<QString, ActionSpec> m_actions;

    bool validate(const QJsonObject &payload,
                  const QList<FieldSpec> &fields,
                  QJsonObject &details,
                  QString &message) const;
    static QString typeName(QJsonValue::Type t);
};

} // namespace Nutmeg

#endif // NUTMEG_WSROUTER_H

