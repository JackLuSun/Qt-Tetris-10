#ifndef SHAPET_H
#define SHAPET_H
#include "myshape.h"

class ShapeT : public MyShape
{
public:
    ShapeT(int currentStatus = 0,QPoint origin = QPoint(300,40),int colorId = elemColor::red);
};

#endif // SHAPET_H
