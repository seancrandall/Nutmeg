#ifndef NUTMEG_TITLEEDIT_H
#define NUTMEG_TITLEEDIT_H

#include "objects/matter.h"
#include "property.h"
#include "textedit.h"
#include <QObject>
#include <QWidget>

namespace Nutmeg
{

class TitleEdit : public Nutmeg::TextEdit
{
    Q_OBJECT
  public:
    TitleEdit(std::shared_ptr<Matter> matter, QWidget *parent = nullptr);

    Property(getText, setText) QString text;

    QString getText(void) { return toPlainText(); }

  protected:
    std::shared_ptr<Matter> mMatter;
};

} // namespace Nutmeg

#endif // NUTMEG_TITLEEDIT_H
