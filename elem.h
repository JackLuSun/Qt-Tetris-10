#ifndef ELEM_H
#define ELEM_H
#include <QGraphicsObject>
#include <QPainter>
#include <QTime>
//#include <QDebug>

enum elemColor {
    red, yellow, blue, green, lightBlue, purple,gray,randomColor
};
const QColor ElemColorArray[] = {
    QColor(255,0,0,100),  QColor(255,255,0,100),QColor(0,0,255,100),
    QColor(0,255,0,100),QColor(0,255,255,100),QColor(255,0,255,100),
    QColor(150,100,100,100)
};
class Elem : public QGraphicsObject
{
public:
    Elem(QPointF pos = QPointF(0,0), int colorId = elemColor::randomColor, QGraphicsItem *parent = Q_NULLPTR);
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget);
    QRectF boundingRect() const;
    int getCurrentStatus();
    void setCurrentStatus(int status);
private:
    int colorId;
    int currentStatus;
};

#endif // ELEM_H
