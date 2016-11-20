#ifndef SHAPELINE_H
#define SHAPELINE_H
#include "myshape.h"

class ShapeLine : public MyShape
{
public:
    ShapeLine(int currentStatus = 0,QPoint origin = QPoint(300,40),int colorId = elemColor::gray);
    virtual void rolate();
};

#endif // SHAPELINE_H
