#ifndef MYSHAPE_H
#define MYSHAPE_H
#include <QGraphicsItemGroup>
//![#include <QDebug>]
#include <QKeyEvent>
#include <QObject>
#include <QTime> //![ 为了引入 随机数]
#include "elem.h"

enum shapeCode {
    shapeT,shapeL,shapeMirrorL,shapeSquare,shapeZ,shapeMirrorZ,shapeLine,shapeRandom
};//![ 这里面的值  0~7]

enum shapeCoorId {//![ 形状坐标id]
    LeftTop, RightTop,RightBottom,LeftBottom
};//![ 分别是 左上， 右上， 右下， 左下 为原点      在旋转的时候坐标系整体旋转]

class MyShape : public QObject,public QGraphicsItemGroup
{
    Q_OBJECT
public:
    MyShape(QPoint origin = QPoint(300,40)); //![ ???? MyShape ????????????????? ?????????? ????MyShape???????????????? items ?????]
    QPoint getOrigin();
    virtual void rolate();
    virtual void rolateBack();
    int getCurrentStatus();

protected:
    QPoint origin;

    //![ 下面的变量用来标明当前使用的是哪一个形状, 在体现 旋转中心不同 等形状个性 的时候需要用到]
protected:
    int currentStatus;//![ 指明了当前的旋转角度， 同时也表明了当前坐标系的状态，也说明了自己当前属于自己的形态的哪一种]
    int colorId;

    Elem * elem[4];
};

#endif //![ MYSHAPE_H]
