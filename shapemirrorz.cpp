#include "shapemirrorz.h"

ShapeMirrorZ::ShapeMirrorZ(int currentStatus, QPoint origin ,int colorId) : MyShape(origin)
{
    if (currentStatus % 90 && currentStatus < 0) {
        currentStatus = 0;
        this->currentStatus = currentStatus %180;
    }
    else {
        qsrand(QTime::currentTime().second());
        this->currentStatus = (qrand() % 2) * 90;
    }

    this->colorId = colorId;
    elem[0] = new Elem(QPointF(20,0),this->colorId,this);
    elem[1] = new Elem(QPointF(40,0),this->colorId,this);
    elem[2] = new Elem(QPointF(20,20),this->colorId,this);
    elem[3] = new Elem(QPointF(0,20),this->colorId,this);

    this->addToGroup(elem[0]);
    this->addToGroup(elem[1]);
    this->addToGroup(elem[2]);
    this->addToGroup(elem[3]);

    while (currentStatus != 0) {
        this->rolate();
        currentStatus -= 90;
    }

    elem[0]->setCurrentStatus(this->currentStatus);
    elem[1]->setCurrentStatus(this->currentStatus);
    elem[2]->setCurrentStatus(this->currentStatus);
    elem[3]->setCurrentStatus(this->currentStatus);
}

void ShapeMirrorZ::rolate()
{
    this->currentStatus = (this->currentStatus + 90) % 180;
    this->setTransformOriginPoint(20,20);
    this->setRotation(this->currentStatus);

    elem[0]->setCurrentStatus(this->currentStatus);
    elem[1]->setCurrentStatus(this->currentStatus);
    elem[2]->setCurrentStatus(this->currentStatus);
    elem[3]->setCurrentStatus(this->currentStatus);
}

