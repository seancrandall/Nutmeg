#ifndef OPTIONSDOCUMENTPANEL_H
#define OPTIONSDOCUMENTPANEL_H

#include <QObject>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

#include "property.h"
#include "widgets/labeledwidgetleft.h"
#include "widgets/lineedit.h"

namespace Nutmeg
{

class OptionsDocumentPanel : public QWidget
{
    Q_OBJECT
  public:
    explicit OptionsDocumentPanel(QWidget *parent = nullptr);

    // Properties
    Property(getDocsLocation, setDocsLocation) QString documentsLocation;
    Property(getMaxFilenameLength, setMaxFilenameLength) uint maxFilenameLength;

    const QString getDocsLocation(void) const { return eDocsLocation->text; };

    uint getMaxFilenameLength(void) const { return eMaxFilenameLength->value(); };

    void setDocsLocation(const QString newloc) { eDocsLocation->text = newloc; };

    void setMaxFilenameLength(const uint newlength) { eMaxFilenameLength->setValue(newlength); };

  private:
    LineEdit *eDocsLocation;
    QSpinBox *eMaxFilenameLength;

    LabeledWidgetLeft *wDocsLocation;
    LabeledWidgetLeft *wMaxFilenameLength;

    QVBoxLayout *vlayout;

  signals:
};

} // namespace Nutmeg

#endif // OPTIONSDOCUMENTPANEL_H
