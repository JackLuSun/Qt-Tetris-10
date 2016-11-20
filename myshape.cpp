#include "myshape.h"

MyShape::MyShape(QPoint origin)
{
    this->origin = origin;
    this->setPos(this->origin);
}

QPoint MyShape::getOrigin()
{
    return this->origin;
}

void MyShape::rolate()
{
    this->currentStatus = (this->currentStatus + 90) % 360;
    this->setTransformOriginPoint(20,20);
    this->setRotation(this->currentStatus);

    elem[0]->setCurrentStatus(this->currentStatus);
    elem[1]->setCurrentStatus(this->currentStatus);
    elem[2]->setCurrentStatus(this->currentStatus);
    elem[3]->setCurrentStatus(this->currentStatus);
}

void MyShape::rolateBack()
{
    this->currentStatus = (this->currentStatus-90 + 360) % 360;
    this->setTransformOriginPoint(20,20);
    this->setRotation(this->currentStatus);

    elem[0]->setCurrentStatus(this->currentStatus);
    elem[1]->setCurrentStatus(this->currentStatus);
    elem[2]->setCurrentStatus(this->currentStatus);
    elem[3]->setCurrentStatus(this->currentStatus);
}

int MyShape::getCurrentStatus()
{
    return this->currentStatus;
}

//int MyShape::getShapeId()
//{
//    return this->shapeId;
//}
