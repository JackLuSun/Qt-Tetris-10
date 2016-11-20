#include "elem.h"

Elem::Elem(QPointF pos,int colorId, QGraphicsItem *parent):QGraphicsObject(parent)
{
   this->currentStatus = 0;

   this->setPos(this->mapFromParent(pos));

   this->colorId = colorId;

   if (this->colorId == elemColor::randomColor) {
       qsrand(QTime().currentTime().second());
       this->colorId = qrand() % 7;
   }
}

void Elem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(0,0,20,20,QPixmap(":/image/box.gif"));
    painter->setBrush(QBrush(ElemColorArray[this->colorId]));
    painter->setPen(QPen(ElemColorArray[this->colorId]));
    painter->drawRect(0,0,19,19);
}

QRectF Elem::boundingRect() const
{
    return QRectF(-0.5,-0.5,21,21);
}

int Elem::getCurrentStatus()
{
    return this->currentStatus;
}

void Elem::setCurrentStatus(int status)
{
    this->currentStatus = status;
}
