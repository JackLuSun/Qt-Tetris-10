#ifndef SHAPEZ_H
#define SHAPEZ_H
#include "myshape.h"

class ShapeZ : public MyShape
{
public:
    ShapeZ(int currentStatus = 0,QPoint origin = QPoint(300,40),int colorId = elemColor::lightBlue);
    virtual void rolate();
};

#endif // SHAPEZ_H
