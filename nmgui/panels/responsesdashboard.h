#ifndef RESPONSESDASHBOARD_H
#define RESPONSESDASHBOARD_H

#include "frame.h"

namespace Nutmeg {

class ResponsesDashboard : public Nutmeg::Frame
{
    Q_OBJECT
public:
    ResponsesDashboard(QWidget *parent=nullptr);
};

} // namespace Nutmeg

#endif // RESPONSESDASHBOARD_H
