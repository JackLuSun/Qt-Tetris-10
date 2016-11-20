#include "game.h"

Game::Game()
{//![ 在构造函数中的代码都是在整个程序运行过程中只需要做一次的代码]
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowTitle(QStringLiteral("方块"));
    this->setWindowIcon(QIcon(":/image/icon.png"));

    //![ 设置整个游戏界面的尺寸]
    this->setMinimumSize(810,510);
    this->setMaximumSize(810,510);

    //![ 下面是 设置 方块活动区域]
    this->areaOrigin.setX(200);
    this->areaOrigin.setY(40);
    this->areaWidth = 240;
    this->areaHeight = 400;

    //![ 下面是 准备场景]
    QGraphicsScene * scene = new QGraphicsScene;
    this->setScene(scene);
    scene->setSceneRect(5,5,800,500);
    scene->setBackgroundBrush(QBrush(QPixmap(":/image/Tetris_background_1.jpg")));

    //![ 绘制方块活动区域边界线 以及线条的颜色]
    this->leftLine = scene->addLine(this->areaOrigin.x(),this->areaOrigin.y(),this->areaOrigin.x(),this->areaOrigin.y()+this->areaHeight,QPen(QBrush(QColor(40,40,120,80)),2));
    this->bottomLine = scene->addLine(this->areaOrigin.x(),this->areaOrigin.y()+this->areaHeight,this->areaOrigin.x()+this->areaWidth,this->areaOrigin.y()+this->areaHeight,QPen(QBrush(QColor(40,40,120,80)),2));
    this->rightLine = scene->addLine(this->areaOrigin.x()+this->areaWidth,this->areaOrigin.y()+this->areaHeight,this->areaOrigin.x()+this->areaWidth,this->areaOrigin.y(),QPen(QBrush(QColor(40,40,120,80)),2));
    this->topLine = scene->addLine(this->areaOrigin.x()+this->areaWidth,this->areaOrigin.y(),this->areaOrigin.x(),this->areaOrigin.y(),QPen(QBrush(QColor(40,40,120,80)),2));

    //![ 准备时间器]
    this->timer = new QTimer(this);
    this->timer->start(80);
    this->timer->stop();
    QObject::connect(this->timer,SIGNAL(timeout()),this,SLOT(timeSlice()));

    //![ 规划场景数组]
    this->sceneArray.resize(this->areaHeight/20);

    for (int i = 0; i < this->sceneArray.size(); i ++)
        this->sceneArray[i].resize(this->areaWidth/20);

    //![ 下面是关于分数]
    this->gradeText = scene->addText(QStringLiteral("得分: "),QFont("Times",22,QFont::Bold));
    this->gradeNumber = scene->addText(QString::number(0),QFont("Times",22,QFont::Bold));
    this->gradeNumber->moveBy(600,80);
    this->gradeText->moveBy(520,80);

    this->pauseButton = new QPushButton(QStringLiteral("暂停"),this);
    //this->pauseButton->setStyleSheet("QPushButton{border-radius:10px;background-color:white;}");
    this->pauseButton->move(520,150);
    QObject::connect(this->pauseButton,SIGNAL(clicked(bool)),this,SLOT(pause()));

    this->exitButton_1 = new QPushButton(QStringLiteral("结束"),this);
    this->exitButton_1->move(520,190);
    QObject::connect(this->exitButton_1,SIGNAL(clicked(bool)),this,SLOT(close()));

    this->replay = new QPushButton(QStringLiteral("重玩"),this);
    this->replay->move(520,230);
    QObject::connect(this->replay,SIGNAL(clicked(bool)),this,SLOT(init()));

    this->screenShotButton = new QPushButton(QStringLiteral("截图"),this);
    this->screenShotButton->move(520,270);
    QObject::connect(this->screenShotButton,SIGNAL(clicked(bool)),this,SLOT(screenShotSlot()));


    //![ 音效]
    //this->bgm = new QMediaPlayer(this);

    //![ 开始界面设置]
    this->startMenu();

}

void Game::startMenu()
{
    this->mousePressed = false; //![ 初始鼠标没有被按下]
    this->cnt = 0;
    //![ 刚开始是隐藏的]
    this->pauseButton->hide();
    this->exitButton_1->hide();
    this->replay->hide();
    this->screenShotButton->hide();
    this->gradeText->hide();
    this->gradeNumber->hide();

    this->leftLine->hide();
    this->bottomLine->hide();
    this->rightLine->hide();
    this->topLine->hide();

    QWidget * mask = new QWidget();
    mask->setAutoFillBackground(true);
    mask->setPalette(QPalette(QColor(0,0,0,120)));
    mask->resize(810,510);
    this->mask = (QGraphicsWidget *)this->scene()->addWidget(mask);
    this->mask->setZValue(1);

    this->startButton = new QPushButton(QStringLiteral("开始"),this);
    this->optionButton = new QPushButton(QStringLiteral("选项"),this);
    this->helpButton = new QPushButton(QStringLiteral("帮助"),this);
    this->exitButton = new QPushButton(QStringLiteral("退出"),this);

    this->gameOverLabel = new QLabel(QStringLiteral("游戏结束"),this);
    this->gameOverLabel->hide();
    this->gameOverLabel->setGeometry(300,80,150,50);
    this->gameOverLabel->setFont(QFont("Times",20,QFont::Bold));
    this->gameOverLabel->setPalette(QPalette(QColor(120,40,20)));
    this->scene()->addWidget(this->gameOverLabel);

    this->gradeLabel = new QLabel(QStringLiteral("得分 : "),this);
    this->gradeLabel->hide();
    this->gradeLabel->setGeometry(300,120,150,50);
    this->gradeLabel->setFont(QFont("Times",20,QFont::Bold));
    this->gradeLabel->setPalette(QPalette(QColor(120,40,20)));
    this->scene()->addWidget(this->gradeLabel);



    this->startButton->move(350,100);
    this->optionButton->move(350,150);
    this->helpButton->move(350,200);
    this->exitButton->move(350,250);

    QObject::connect(startButton,SIGNAL(clicked(bool)),this,SLOT(init()));
    QObject::connect(exitButton,SIGNAL(clicked(bool)),this,SLOT(close()));

    return ;
}

void Game::init()//![会被多次调用 每次重新开局都是调用 这个函数]
{
    this->mousePressed = false; //![ 初始鼠标没有被按下]
    //this->bgm->setMedia(QUrl::fromLocalFile("/home/jacklu/music/Summer.mp3"));
    //this->bgm->play();

    //![ 下面是清空形状列表 考虑到重玩的时候需要清场]
    QList<MyShape*>::iterator i = this->currentShapelist.begin();
    while (i !=this->currentShapelist.end()){
            this->scene()->removeItem(*i);//![ 删除该形状]
            i = this->currentShapelist.erase(i);//![ 从当前有效形状表中删除]
    }

    //![ 设置数组初始值]
    for (int i = 0; i < this->sceneArray.size(); i ++)
        for (int j = 0; j < this->sceneArray[i].size(); j ++)
            this->sceneArray[i][j] = false;

    this->gameOverLabel->hide();
    this->pauseButton->setEnabled(true);

    this->gameStatus = true;//![ 只是标记下游戏的状态 当前为运行状态]
    this->mask->hide();
    this->startButton->hide();
    this->optionButton->hide();
    this->helpButton->hide();
    this->exitButton->hide();


    this->leftLine->show();
    this->bottomLine->show();
    this->rightLine->show();
    this->topLine->show();

    this->pauseButton->show();
    this->exitButton_1->show();
    this->replay->show();
    this->screenShotButton->show();
    this->gradeText->show();
    this->gradeNumber->show();
    //![ 关于分数]
    this->totalNumOfShapesUsed=0;
    this->rowsNumRemoves = 0;
    this->grades = 0;
    this->gradeNumber->setPlainText(QString::number(this->grades));
    this->myshape = this->newShape(shapeCode::shapeRandom);
    this->scene()->addItem(this->myshape);

    //![ 时间器开始]
    this->timer->start();
    this->timeCount = 0;
    //![ 设置方块下降速度]
    this->speed = 1;
}

void Game::keyPressEvent(QKeyEvent *event)
{
    if (!this->myshape)
        return ;
    if (!this->gameStatus)//![ 如果不是运行态，比如暂停态]
        return ;
    if (event->key() == Qt::Key_Left) {
        this->setFlag(false);
        this->myshape->moveBy(-20,0);
        //![ 边界线冲突判断  以及 方块之间的判度阿森纳]
        if (this->isBorderColliding() || this->isShapeColliding()) {
            this->myshape->moveBy(20,0);
            this->setFlag(true);
            return ;
        }
    }
    else if (event->key() == Qt::Key_Right) {
        this->setFlag(false);
        this->myshape->moveBy(20,0);
        //![ 边界线冲突判断 以及 形状之间冲突判断]
        if (this->isBorderColliding() || this->isShapeColliding()) {
            this->myshape->moveBy(-20,0);
            this->setFlag(true);
            return ;
        }
    }
//![    else if (event->key() == Qt::Key_Up) {]
//![        this->setFlag(false);]
//![        this->myshape->moveBy(0,-20);]
//![        this->setFlag(true);]
//![    }]
    else if (event->key() == Qt::Key_Down) {
        for (int i = 0; i < 2; i ++) { //![ 用 for 循环来表达做同样的事情]
            if (!this->moveDownOneStep())
                break;
        }
    }
    else if (event->key() == Qt::Key_Space) {
        this->setFlag(false);//![清除原有标志]
        this->myshape->rolate();
        //![ 边界线冲突判断 以及 方块之间冲突判断]
        //![ 下面的代码同时使得旋转时候因为靠边原因导致有足够的空间却不能旋转的问题得到很好的解决，灵活度得到提升]
        if (this->isBorderColliding() || this->isShapeColliding()) {//![冲突]
            this->myshape->moveBy(20,0);//![ 尝试右移动]
            if (this->isBorderColliding() || this->isShapeColliding()) {//![ 仍然冲突]
                this->myshape->moveBy(-40,0); //![ 尝试左移动]
                if (this->isBorderColliding() || this->isShapeColliding()) { //![ 还是发生冲突]
                    if (this->currentShapeId == shapeCode::shapeLine) {//![ shapeLine 特殊一点]
                        this->myshape->moveBy(-20,0);
                        if (!(this->isBorderColliding() || this->isShapeColliding())) {
                            return ;
                        }
                    }
                    this->myshape->moveBy(20,0);
                    this->myshape->rolateBack();
                    this->setFlag(true);//![ 恢复标志]
                    return ;
                }
            }
        }
        this->setFlag(true);
    }

    return ;
}

bool Game::isShapeColliding()
{
    //![ 只是判断方块之间的冲突]
    int i, j;
    foreach (QGraphicsItem * item, this->myshape->childItems()) {
        switch(this->myshape->getCurrentStatus()) {//![ 也是因为坐标原点因为旋转而发生了改变引起的动态需求]
            case 0:
                i = (item->scenePos().y()-this->areaOrigin.y())/20;
                j = (item->scenePos().x()-this->areaOrigin.x())/20;
                break;
            case 90:
                i = (item->scenePos().y()-this->areaOrigin.y())/20;
                j = (item->scenePos().x()-this->areaOrigin.x())/20-1;
                break;
            case 180:
                i = (item->scenePos().y()-this->areaOrigin.y())/20-1;
                j = (item->scenePos().x()-this->areaOrigin.x())/20-1;
                break;
            case 270:
                i = (item->scenePos().y()-this->areaOrigin.y())/20-1;
                j = (item->scenePos().x()-this->areaOrigin.x())/20;
                break;
        }

        if (this->sceneArray[i][j]) //![ 如果涉及的范围中存在 true ，则说明会产生冲突]
            return true;
    }

    return false;
}

bool Game::isBorderColliding()
{
    //![ 判断的方法 : 根据 sceneBoundingRect() 提供的值我们得到该形状的包围矩形，判断该矩形各条边是否与边界线冲突]
   QRectF rect = this->getShapeCurrentBoundingRectInScene();
   //![ if ( minX < this->areaOrigin.x() || minY < this->areaOrigin.y() || maxX > (this->areaOrigin.x()+this->areaWidth) || maxY > (this->areaOrigin.y() + this->areaHeight))]
   //![ 不包括对上边界的判度，因为只有游戏结束的时候才会触碰上边界，游戏过程中的触碰都不作数，所以把上边界独立出来，不在判断边界冲突的函数中写出]
    if ( rect.x() < this->areaOrigin.x() || (rect.x()+rect.width()) > (this->areaOrigin.x()+this->areaWidth) || (rect.y()+rect.height()) > (this->areaOrigin.y() + this->areaHeight))
         return true;
    return false;
}

bool Game::moveDownOneStep()//![ 只是单纯的下降一步，返回值表示下降是否成功]
{
    this->setFlag(false);//![ 先清除原占有标志]
    this->myshape->moveBy(0,20);
    //![下面是 边界线冲突判断 以及 形状之间冲突判断]
    if (this->isBorderColliding() || this->isShapeColliding()) {//![ 检测到有冲突 则下移动失败 采取相应的动作]
       // qDebug() << "colliding"<< endl;
        this->myshape->moveBy(0,-20);
        this->setFlag(true);//![ 恢复原占有标志]
        //![ 下降失败 这意味着本次形状运行结束]
        //![ 进入消除判断，然后紧接着 判断是否触碰上边界，因为消除完成后还触碰着上边界意味着游戏结束，如果没有结束，之后才是 新生一个形状]
        this->clearUpAndRenewShape();
        return false;
    }

    //![ 没有发生冲突 直接占有当前位置的所有标志]
    this->setFlag(true);

    return true;
}

void Game::clearUpAndRenewShape()
{
    int count = 0; //![ 用来记录已经消除了的行数]
    //![ 下面是行消除判断和处理]
    for (int y = this->areaOrigin.y()+this->areaHeight-20; y >= 0; y -= 20) {
        QList<QGraphicsItem *> list = this->scene()->items(QRectF(this->areaOrigin.x()-1,y-1,this->areaWidth+2,22),Qt::ContainsItemShape);

        if (list.count() == 0) {

            break;
        }
        QList<QGraphicsItem *>::iterator i = list.begin();
        while (i != list.end()) {
            if (!((*i)->sceneBoundingRect().width() <25)) {//![ 过滤掉 不是基本单位方块的 item]
                i = list.erase(i);//![ erase之后返回值为原来i的下一个节点]
            }
            else
                i ++;
        }

        int rowsForArray = (y-this->areaOrigin.y())/20;
        if (list.count() == (this->areaWidth/20)) {//![ 普通行满]
            for (int k = 0; k < this->sceneArray[rowsForArray].size(); k ++)
                this->sceneArray[rowsForArray][k] = false;

            foreach (QGraphicsItem * item, list) {
                this->scene()->removeItem(item);
            }

             count ++;
        }
        else if (count > 0){//![ 没有满，但是之前发生了消除， 采取的措施就是，下降相应的行数(count即是要下降的行数)]
            this->rowsNumRemoves+=count;
            for (int k = 0; k < this->sceneArray[rowsForArray].size(); k ++) {
                this->sceneArray[rowsForArray+count][k] = this->sceneArray[rowsForArray][k];
                this->sceneArray[rowsForArray][k] = false;
            }

            foreach (QGraphicsItem * item, list) {
                Elem * p = (Elem *)item;
                switch(p->getCurrentStatus()) {
                    case 0:
                        p->moveBy(0,count*20);
                        break;
                    case 90:
                        p->moveBy(count*20,0);
                        break;
                    case 180:
                        p->moveBy(0,0-count*20);
                        break;
                    case 270:
                        p->moveBy(0-count*20,0);
                        break;
                    default: QMessageBox::warning(this,"错误","Here , something wrong in game.cpp"); break;
                }
            }
        }
    }
    //![ 上面的代码进行的是检查和完成消除方块工作，紧接着，如果上面有消除发生，我们就对当前所有形状检查，是否有形状无效(即没有孩子)]
    if (count > 0) { //![ 根据消除的行数来判断是否发生了消除]
        this->clearShapeWithNoChild();
    }
   // qDebug() << "jack is here";
    //![ 下面是判断当前方块是否接触了上边界 ，也就是意味着判断游戏是否结束]
    //qDebug() << "this->getShapeCurrentBoundingInScene y: " << this->getShapeCurrentBoundingRectInScene().y() << "    this->areaOrigin.y() " << this->areaOrigin.y();
    if (this->getShapeCurrentBoundingRectInScene().y() <= this->areaOrigin.y()) {
        /* 游戏结束 */
        this->gameOver();
        return ;
    }

    //![ 下面是分数设置]
    this->totalNumOfShapesUsed ++;
    this->grades = this->totalNumOfShapesUsed*5 + this->rowsNumRemoves*20;
    this->gradeNumber->setPlainText(QString::number(this->grades));

    //![ 根据分数对速度进行调整]
    if (this->grades > this->speed*this->speed*(100 - this->speed * 2)) {
        if (++(this->speed) > 10) {
            this->speed--;
        }

        QString path(":/image/Tetris_background_");
        qsrand(QTime().currentTime().second());
        path += (QString::number(qrand() % 21 + 1) + ".png");
        QPixmap pixmap(path);
        this->scene()->setBackgroundBrush(QBrush(pixmap));
    }
    //qDebug() << "here start";
    //![ 下面是新生一个形状]
    this->myshape = this->newShape(shapeCode::shapeRandom);
    //qDebug() << "here over1";
    this->scene()->addItem(this->myshape);
    //qDebug() << "here over2";
    //qDebug() << "clear Up And Renw shape over" ;
}

void Game::clearShapeWithNoChild()
{
    QList<MyShape*>::iterator i = this->currentShapelist.begin();

    while (i !=this->currentShapelist.end()){
        if ((*i)->childItems().count() == 0) {
            this->scene()->removeItem(*i);//![ 删除该形状]
            i = this->currentShapelist.erase(i);//![ 从当前有效形状表中删除]
        }
        else
            i ++;
    }

}

MyShape * Game::newShape(int shapeId,int status, QPoint landedPoint)
{
    if (shapeId == shapeCode::shapeRandom) {
        qsrand(QTime().currentTime().second());
        shapeId = qrand() % 7;
    }

    this->currentShapeId = shapeId;

    MyShape * newShape = NULL;
    switch (shapeId) {
        case shapeCode::shapeT:
            newShape = new ShapeT(status,landedPoint);
            break;
        case shapeCode::shapeL :
            newShape = new ShapeL(status,landedPoint);
            break;
        case shapeCode::shapeMirrorL :
            newShape = new ShapeMirrorL(status,landedPoint);
            break;
        case shapeCode::shapeSquare :
            newShape = new ShapeSquare(status,landedPoint);
            break;
        case shapeCode::shapeZ :
            newShape = new ShapeZ(status,landedPoint);
            break;
        case shapeCode::shapeMirrorZ :
            newShape = new ShapeMirrorZ(status,landedPoint);
            break;
        case shapeCode::shapeLine :
            newShape = new ShapeLine(status,landedPoint);
            break;
        default: qDebug() << "Here, something is wrong with function  newShape ";
    }
    this->currentShapelist.append(newShape);

    //![ 下面是为新生形状设置形状数组]
    int i, j;
    foreach (QGraphicsItem * item, newShape->childItems()) {
        i = (item->scenePos().y()-this->areaOrigin.y())/20;
        j = (item->scenePos().x()-this->areaOrigin.x())/20;
        //![cout << "i = " << i <<  "  j = " << j << endl;]
        this->sceneArray[i][j] = true;
    }

    return newShape;
}

QRectF Game::getShapeCurrentBoundingRectInScene()
{
    //![ 注意的是： 形状的 x,y 会随着旋转而移动，也就是说，x,y不是固定的左上角。考虑到这点，我们需要分别计算四种情况]
    int minX, minY, maxX, maxY;

    switch(this->myshape->getCurrentStatus()) {
        case 0:
            minX = this->myshape->scenePos().x();
            minY = this->myshape->scenePos().y();
            maxX = minX + (this->myshape->sceneBoundingRect().width()-1);
            maxY = minY + (this->myshape->sceneBoundingRect().height()-1);
            break;
        case 90:
            maxX = this->myshape->scenePos().x();
            minY = this->myshape->scenePos().y();
            minX = maxX - (this->myshape->sceneBoundingRect().width()-1);
            maxY = minY + (this->myshape->sceneBoundingRect().height()-1);
            break;
        case 180:
            maxX = this->myshape->scenePos().x();
            maxY = this->myshape->scenePos().y();
            minX = maxX - (this->myshape->sceneBoundingRect().width()-1);
            minY = maxY - (this->myshape->sceneBoundingRect().height()-1);
            break;
        case 270:
            minX = this->myshape->scenePos().x();
            maxY = this->myshape->scenePos().y();
            maxX = minX + (this->myshape->sceneBoundingRect().width()-1);
            minY = maxY - (this->myshape->sceneBoundingRect().height()-1);
            break;
    }

    return QRectF(minX,minY,maxX-minX,maxY-minY);
}

void Game::gameOver()
{
    this->myshape = NULL;
    this->timer->stop();
    this->mask->show();
    this->gameOverLabel->show();\
    this->gradeLabel->setText(QString(QStringLiteral("得分 : "))+QString::number(this->grades));
    this->gradeLabel->show();
    this->pauseButton->setEnabled(false);
}

void Game::setFlag(bool flag)
{
   // qDebug() << "setFlag   flag = " << flag << this->myshape->getCurrentStatus() <<endl;
    int i, j;
    switch(this->myshape->getCurrentStatus()) {//![ 这里分四种情况讨论，是因为坐标的问题]
        case 0:
            foreach (QGraphicsItem * item, this->myshape->childItems()) {
                i = (item->scenePos().y()-this->areaOrigin.y())/20;
                j = (item->scenePos().x()-this->areaOrigin.x())/20;
                this->sceneArray[i][j] = flag;
            }
            break;
        case 90:
            foreach (QGraphicsItem * item, this->myshape->childItems()) {
                i = (item->scenePos().y()-this->areaOrigin.y())/20;
                j = (item->scenePos().x()-this->areaOrigin.x()-20)/20;
                this->sceneArray[i][j] = flag;
            }
            break;
        case 180:
            foreach (QGraphicsItem * item, this->myshape->childItems()) {
                i = (item->scenePos().y()-this->areaOrigin.y()-20)/20;
                j = (item->scenePos().x()-this->areaOrigin.x()-20)/20;
                this->sceneArray[i][j] = flag;
            }
            break;
        case 270:
            foreach (QGraphicsItem * item, this->myshape->childItems()) {
                i = (item->scenePos().y()-this->areaOrigin.y()-20)/20;
                j = (item->scenePos().x()-this->areaOrigin.x())/20;
                this->sceneArray[i][j] = flag;
            }
            break;
        default: qDebug() << "Here , setFlag is not certain in game.cpp" << endl; break;
    }
}

void Game::timeSlice()
{
    if (++(this->timeCount) == (11-this->speed)) {
        this->timeCount = 0;
        this->moveDownOneStep();
    }
}

void Game::pause()
{
    if (this->gameStatus) {
        this->timer->stop();
        this->pauseButton->setText(QStringLiteral("继续"));
        this->gameStatus = false;
    }
    else {
        this->timer->start();
        this->pauseButton->setText(QStringLiteral("暂停"));
        this->gameStatus = true;
    }

    this->pauseButton->clearFocus();
}

void Game::screenShotSlot()
{
    QScreen * screen = QGuiApplication::primaryScreen();
    QDir * screenshot = new QDir;
    screenshot->mkdir("screenshot");
    //![QDir::mkdir("screenshot");]
    QDir screenshotDir("screenshot");
   //![ qDebug() << screenshotDir.count();// 因为 一个文件夹下至少有 .  ..  这两个文件夹]
    int cnt = screenshotDir.count();
    screen->grabWindow(this->winId()).save(QString("screenshot/")+QString::number(cnt-2)+QString(".jpg"),"jpg");
    //![qDebug() << "screen shot slot";]
}

void Game::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        this->mousePressed = true;
        this->mousePressPos = event->pos();
    }
}

void Game::mouseMoveEvent(QMouseEvent *event)
{
    if (this->mousePressed) { //![ 如果哦鼠标被按下,这个过程中发生鼠标拖动]
        if (this->cnt ++ == 4) {//![ cnt 用来消除抖动 因为鼠标每移动一个像素点 整个框体都要执行一次移动，这样导致框口拖动过程中的抖动现象的产生  现在我加入一个 cnt 变量来控制使得每移动10个像素点才会执行一次真正的移动， 显然有很好的效果]
            this->move((this->pos()+=(event->pos()-=this->mousePressPos)));
            this->cnt = 0;
        }
    }
}

void Game::mouseReleaseEvent(QMouseEvent *event)
{
    this->mousePressed = false;
}
