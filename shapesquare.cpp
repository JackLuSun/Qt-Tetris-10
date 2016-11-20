#include "shapesquare.h"

ShapeSquare::ShapeSquare(int currentStatus, QPoint origin ,int colorId) : MyShape(origin)
{
    this->currentStatus = 0;//![ 这句话漏写了，导致后来出现 方形 一直边界冲突的问题]

    this->colorId = colorId;

    elem[0] = new Elem(QPointF(0,0),this->colorId,this);
    elem[1] = new Elem(QPointF(0,20),this->colorId,this);
    elem[2] = new Elem(QPointF(20,0),this->colorId,this);
    elem[3] = new Elem(QPointF(20,20),this->colorId,this);

    this->addToGroup(elem[0]);
    this->addToGroup(elem[1]);
    this->addToGroup(elem[2]);
    this->addToGroup(elem[3]);

}

void ShapeSquare::rolate()
{//![ do nothing]
    return ;
}

void ShapeSquare::rolateBack()
{//![ do nothing]
    return ;
}
