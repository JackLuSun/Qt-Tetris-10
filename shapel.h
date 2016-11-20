#ifndef SHAPEL_H
#define SHAPEL_H
#include "myshape.h"

class ShapeL : public MyShape
{
public:
    ShapeL(int currentStatus = 0,QPoint origin = QPoint(300,40),int colorId = elemColor::yellow);
    virtual void rolate();
    virtual void rolateBack(); // 旋转回退
};

#endif // SHAPEL_H
