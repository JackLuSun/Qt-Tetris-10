#include "shapel.h"
//#include <QDebug>

ShapeL::ShapeL(int currentStatus, QPoint origin,int colorId) : MyShape(origin)
{
    if (currentStatus % 90 || currentStatus < 0) { //![ 不是 90 的整倍数的 或者是负数的情况]
        currentStatus = 0;
        this->currentStatus = 0;
    }
    else {
        qsrand(QTime::currentTime().second());
        this->currentStatus = (qrand() % 4) * 90;
    }
    this->colorId = colorId;

    elem[0] = new Elem(QPointF(0,0),this->colorId,this);
    elem[1] = new Elem(QPointF(0,20),this->colorId,this);
    elem[2] = new Elem(QPointF(0,40),this->colorId,this);
    elem[3] = new Elem(QPointF(20,40),this->colorId,this);

    this->addToGroup(elem[0]);
    this->addToGroup(elem[1]);
    this->addToGroup(elem[2]);
    this->addToGroup(elem[3]);

    while (currentStatus != 0) {//![ 这里用的就是  currentStatus  而不应该使用this->currentStatus]
        this->rolate();
        currentStatus -= 90;
    }

    elem[0]->setCurrentStatus(this->currentStatus);
    elem[1]->setCurrentStatus(this->currentStatus);
    elem[2]->setCurrentStatus(this->currentStatus);
    elem[3]->setCurrentStatus(this->currentStatus);
}

void ShapeL::rolate()
{
    this->currentStatus = (this->currentStatus + 90) % 360;
    this->setTransformOriginPoint(20,40);
    this->setRotation(this->currentStatus);

    elem[0]->setCurrentStatus(this->currentStatus);
    elem[1]->setCurrentStatus(this->currentStatus);
    elem[2]->setCurrentStatus(this->currentStatus);
    elem[3]->setCurrentStatus(this->currentStatus);

}

void ShapeL::rolateBack()
{
   //![ this->currentId = (this->currentId-1+4)%4;]
    this->currentStatus = (this->currentStatus-90+360)%360;
    this->setTransformOriginPoint(20,40);
    this->setRotation(this->currentStatus);

    elem[0]->setCurrentStatus(this->currentStatus);
    elem[1]->setCurrentStatus(this->currentStatus);
    elem[2]->setCurrentStatus(this->currentStatus);
    elem[3]->setCurrentStatus(this->currentStatus);
}

