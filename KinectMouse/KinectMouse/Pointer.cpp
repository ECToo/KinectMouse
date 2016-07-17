#include "Pointer.h"
#include <windows.h> 


Pointer::Pointer()
{
}


Pointer::~Pointer()
{
}

void Pointer::nextPosition()
{
    POINT p;
    GetCursorPos(&p);
    QPoint qp{ p.x,p.y };
    Q_EMIT newPosition(qp);
}
