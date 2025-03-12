#ifndef DYNAMICSTACKEDWIDGET_H
#define DYNAMICSTACKEDWIDGET_H

#include <QStackedWidget>

namespace Nutmeg {

class DynamicStackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit DynamicStackedWidget(QWidget *parent=nullptr);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
};

} // namespace Nutmeg

#endif // DYNAMICSTACKEDWIDGET_H
