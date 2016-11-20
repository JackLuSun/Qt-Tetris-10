#ifndef SHAPESQUARE_H
#define SHAPESQUARE_H
#include "myshape.h"

class ShapeSquare : public MyShape
{
public:
     ShapeSquare(int currentStatus = 0,QPoint origin = QPoint(300,40),int colorId = elemColor::blue);
    virtual void rolate();
    virtual void rolateBack();
};

#endif // SHAPESQUARE_H
