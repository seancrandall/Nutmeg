#ifndef NUTMEG_MESSAGEBOX_H
#define NUTMEG_MESSAGEBOX_H

#include <QMessageBox>
#include <QObject>
#include <QWidget>

#include "nutmeg.h"
#include "property.h"

namespace Nutmeg
{

class MessageBox : public QMessageBox
{
    Q_OBJECT
  public:
    MessageBox(QWidget *parent = nullptr);

    // Properties
    Property(detailedText, setDetailedText) QString detailedText;
    Property(icon, setIcon) Icon icon;
    Property(iconPixmap, setIconPixmap) QPixmap iconPixmap;
    Property(informativeText, setInformativeText) QString informativeText;
    Property(options, setOptions) Options options;
    Property(standardButtons, setStandardButtons) StandardButtons standardButtons;
    Property(text, setText) QString text;
    Property(textFormat, setTextFormat) Qt::TextFormat textFormat;
    Property(textInteractionFlags, setTextInteractionFlags) Qt::TextInteractionFlags textInteractionFlags;
};

} // namespace Nutmeg

#endif // NUTMEG_MESSAGEBOX_H
