#ifndef GAME_H
#define GAME_H
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGuiApplication>
#include <QLabel>
#include <QPushButton>
#include "myshape.h"
#include <QTimer>
#include <QList>
#include <QPalette>
#include <QLabel>
#include <QGraphicsWidget>
#include <QMediaPlayer>
#include <QScreen>
#include <QDir>
#include "shapet.h"
#include "shapel.h"
#include "shapemirrorl.h"
#include "shapez.h"
#include "shapemirrorz.h"
#include "shapeline.h"
#include "shapesquare.h"
#include <QVector>
#include <iostream>
#include <QMessageBox>

using namespace std;

class Game : public QGraphicsView
{
    Q_OBJECT
public:
    Game();
    void keyPressEvent(QKeyEvent *event);
private slots:
    void init(); //![ 需要写一个初始化函数，明明已经有了构造函数，构造函数不久可一实现初始化吗，为什么还要重写一个init函数呢？ 因为游戏会重新开局！！这个重新开局不是关闭游戏再重新打开游戏，而构造函数在整个过程中只是会执行一次，除非关闭程序重新运行才可以执行构造函数，显然着不符合用户需求，用户希望的是在不需要重新打开游戏的前提下，重新游戏]
private:
    void startMenu();
    bool isShapeColliding(); //![ 方块之间冲突]
    bool isBorderColliding(); //![ 方块和边界冲突]
    bool moveDownOneStep();//![ 只是单纯的下降一步，返回值表示下降是否成功]
    void clearUpAndRenewShape(); //![ 消除判断 和 新生一个形状]
    void clearShapeWithNoChild(); //![ 清除没有孩子的形状]
    MyShape * newShape(int shapeId = shapeCode::shapeRandom,int status = -1,QPoint landedPoint = QPoint(300,40));//![ shapeId 用于选择哪个形状， status 用于确定形状初始的形态 -1 表示随机， landedPoint 指定该形状的落点]
    QRectF getShapeCurrentBoundingRectInScene();//![这个是用来获取当前形状，无论是否被旋转过，都一律返回当前在scene中的以左上角为中心点的包围矩形]
    void gameOver(); //![ 当消除后，当时形状的上界能够触碰到方块活动区域上边界，那么游戏结束,调用gameOver,做好一些后续处理工作]
    void setFlag(bool flag);
protected slots:
    void timeSlice();
    void pause();
    void screenShotSlot(); //![ 保存游戏界面截图]
protected:
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent *event);
private:
    MyShape * myshape;
    //![ 下面变量和用来指定方块活动区域范围]
    QPoint areaOrigin; //![ 区域原点]
    int areaWidth; //![![  区域宽]]
    int areaHeight; //![![ 区域高]]

    QGraphicsItem * leftLine; //![ 左边界线]
    QGraphicsItem * bottomLine;//![ 下边界线]
    QGraphicsItem * rightLine;//![ 右边界线]
    QGraphicsItem * topLine;//![ 上边界线]

    //![ 定时器]
    QTimer * timer;
    int timeCount; //![ 时间计数器   用来配合时间片的分配]

    //![ 下面的变量用来记录方块下降速度 数值越大代表速度越高]
    int speed;

    //![ 下面的变量用来存放 当前所有的形状， 这些形状包括是完整的形状和残缺的形状，一旦该形状一个孩子都没有，就把它删除并且从 下面的这个链表中删除]
    QList<MyShape * > currentShapelist;

    int currentShapeId;

    QVector<QVector<bool>> sceneArray;//![ 这个大概就是我的终极方案了]

    //![ 下面是关于分数的设置]
    int grades;
    int rowsNumRemoves; //![消除的行数]
    int totalNumOfShapesUsed;
    QGraphicsTextItem * gradeText;
    QGraphicsTextItem * gradeNumber;

    QGraphicsWidget * mask;//![ 遮罩]
    QPushButton * startButton;//![ 游戏开始按钮]
    QPushButton * optionButton;//![ 选择项按钮]
    QPushButton * helpButton;//![ 帮助按钮]
    QPushButton * exitButton;//![ 游戏退出按钮]
    QPushButton * exitButton_1;
    QPushButton * replay;//![ 重新游戏]

    QPushButton * pauseButton; //![ 游戏暂停按钮]

    QLabel * gameOverLabel;//![ 游戏结束文本显示]
    QLabel * gradeLabel;

    bool gameStatus; //![ 游戏状态 运行 或者暂停  这个变量是为辅助实现暂停功能的]

    //![ 下面是音效 背景音乐]
    QMediaPlayer * bgm;

    //![ 下面是关于重新实现框口拖动需要的变量]
    QPoint mousePressPos;
    bool mousePressed; //![ 标记鼠标按下]
    int cnt;//![用来辅助消除框体移动过程发生抖动现象 对应代码中有详解]

    //![ 保存游戏截图]
    QPixmap screenShot;
    QPushButton * screenShotButton;
};

#endif //![ GAME_H]
