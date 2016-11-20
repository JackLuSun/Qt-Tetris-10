#ifndef SHAPEMIRRORZ_H
#define SHAPEMIRRORZ_H
#include "myshape.h"

class ShapeMirrorZ : public MyShape
{
public:
    ShapeMirrorZ(int currentStatus = 0,QPoint origin = QPoint(300,40),int colorId = elemColor::purple);
    virtual void rolate();
};

#endif // SHAPEMIRRORZ_H
