#ifndef NUTMEG_SEARCHCOMPLETER_H
#define NUTMEG_SEARCHCOMPLETER_H

#include <QEvent>
#include <QHeaderView>
#include <QLineEdit>
#include <QObject>
#include <QTimer>
#include <QTreeWidget>

#include "dbaccess/nutdb.h"

// #include<QNetworkAccessManager> //requires QT += network

namespace Nutmeg
{

class SearchCompleter : public QObject
{
    Q_OBJECT
  public:
    explicit SearchCompleter(QLineEdit *parent = nullptr);
    ~SearchCompleter();
    bool eventFilter(QObject *obj, QEvent *ev) override;
    void showCompletion(const QVector<QString> &choices);

  public slots:

    void doneCompletion();
    void preventSuggest();
    void autoSuggest();
    // void handleNetworkData(QNetworkReply *networkReply);
    void handleDbResults(void);

  private:
    QLineEdit *editor = nullptr;
    QTreeWidget *popup = nullptr;
    QTimer timer;
    // QNetworkAccessManager networkManager;
};

} // namespace Nutmeg

#endif // NUTMEG_SEARCHCOMPLETER_H
