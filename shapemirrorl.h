#ifndef SHAPEMIRRORL_H
#define SHAPEMIRRORL_H
#include "myshape.h"

class ShapeMirrorL : public MyShape
{
public:
    ShapeMirrorL(int currentStatus = 0,QPoint origin = QPoint(300,40),int colorId = elemColor::green);
    virtual void rolate();
    virtual void rolateBack();
};

#endif // SHAPEMIRRORL_H
