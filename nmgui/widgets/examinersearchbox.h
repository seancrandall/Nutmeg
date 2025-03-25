#ifndef NUTMEG_EXAMINERSEARCHBOX_H
#define NUTMEG_EXAMINERSEARCHBOX_H

#include "completers/examinercompleter.h"
#include "widgets/personsearchbox.h"
#include <QObject>
#include <QWidget>

namespace Nutmeg
{

class ExaminerSearchBox : public Nutmeg::PersonSearchBox
{
    Q_OBJECT
  public:
    explicit ExaminerSearchBox(QWidget *parent = nullptr);
    explicit ExaminerSearchBox(Key initkey, QWidget *parent = nullptr);

    ReadOnlyProperty(currentText) QString ExaminerName;

  public slots:
    virtual void slotAddRecord(const QString &input) override;

  private:
    ExaminerCompleter *completer;
    viewPatentExaminersModel *mModel;

    void Initialize(void);
};

} // namespace Nutmeg

#endif // NUTMEG_EXAMINERSEARCHBOX_H
