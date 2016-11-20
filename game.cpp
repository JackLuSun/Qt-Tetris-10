#include "game.h"

Game::Game()
{//![ �ڹ��캯���еĴ��붼���������������й�����ֻ��Ҫ��һ�εĴ���]
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowTitle(QStringLiteral("����"));
    this->setWindowIcon(QIcon(":/image/icon.png"));

    //![ ����������Ϸ����ĳߴ�]
    this->setMinimumSize(810,510);
    this->setMaximumSize(810,510);

    //![ ������ ���� ��������]
    this->areaOrigin.setX(200);
    this->areaOrigin.setY(40);
    this->areaWidth = 240;
    this->areaHeight = 400;

    //![ ������ ׼������]
    QGraphicsScene * scene = new QGraphicsScene;
    this->setScene(scene);
    scene->setSceneRect(5,5,800,500);
    scene->setBackgroundBrush(QBrush(QPixmap(":/image/Tetris_background_1.jpg")));

    //![ ���Ʒ�������߽��� �Լ���������ɫ]
    this->leftLine = scene->addLine(this->areaOrigin.x(),this->areaOrigin.y(),this->areaOrigin.x(),this->areaOrigin.y()+this->areaHeight,QPen(QBrush(QColor(40,40,120,80)),2));
    this->bottomLine = scene->addLine(this->areaOrigin.x(),this->areaOrigin.y()+this->areaHeight,this->areaOrigin.x()+this->areaWidth,this->areaOrigin.y()+this->areaHeight,QPen(QBrush(QColor(40,40,120,80)),2));
    this->rightLine = scene->addLine(this->areaOrigin.x()+this->areaWidth,this->areaOrigin.y()+this->areaHeight,this->areaOrigin.x()+this->areaWidth,this->areaOrigin.y(),QPen(QBrush(QColor(40,40,120,80)),2));
    this->topLine = scene->addLine(this->areaOrigin.x()+this->areaWidth,this->areaOrigin.y(),this->areaOrigin.x(),this->areaOrigin.y(),QPen(QBrush(QColor(40,40,120,80)),2));

    //![ ׼��ʱ����]
    this->timer = new QTimer(this);
    this->timer->start(80);
    this->timer->stop();
    QObject::connect(this->timer,SIGNAL(timeout()),this,SLOT(timeSlice()));

    //![ �滮��������]
    this->sceneArray.resize(this->areaHeight/20);

    for (int i = 0; i < this->sceneArray.size(); i ++)
        this->sceneArray[i].resize(this->areaWidth/20);

    //![ �����ǹ��ڷ���]
    this->gradeText = scene->addText(QStringLiteral("�÷�: "),QFont("Times",22,QFont::Bold));
    this->gradeNumber = scene->addText(QString::number(0),QFont("Times",22,QFont::Bold));
    this->gradeNumber->moveBy(600,80);
    this->gradeText->moveBy(520,80);

    this->pauseButton = new QPushButton(QStringLiteral("��ͣ"),this);
    //this->pauseButton->setStyleSheet("QPushButton{border-radius:10px;background-color:white;}");
    this->pauseButton->move(520,150);
    QObject::connect(this->pauseButton,SIGNAL(clicked(bool)),this,SLOT(pause()));

    this->exitButton_1 = new QPushButton(QStringLiteral("����"),this);
    this->exitButton_1->move(520,190);
    QObject::connect(this->exitButton_1,SIGNAL(clicked(bool)),this,SLOT(close()));

    this->replay = new QPushButton(QStringLiteral("����"),this);
    this->replay->move(520,230);
    QObject::connect(this->replay,SIGNAL(clicked(bool)),this,SLOT(init()));

    this->screenShotButton = new QPushButton(QStringLiteral("��ͼ"),this);
    this->screenShotButton->move(520,270);
    QObject::connect(this->screenShotButton,SIGNAL(clicked(bool)),this,SLOT(screenShotSlot()));


    //![ ��Ч]
    //this->bgm = new QMediaPlayer(this);

    //![ ��ʼ��������]
    this->startMenu();

}

void Game::startMenu()
{
    this->mousePressed = false; //![ ��ʼ���û�б�����]
    this->cnt = 0;
    //![ �տ�ʼ�����ص�]
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

    this->startButton = new QPushButton(QStringLiteral("��ʼ"),this);
    this->optionButton = new QPushButton(QStringLiteral("ѡ��"),this);
    this->helpButton = new QPushButton(QStringLiteral("����"),this);
    this->exitButton = new QPushButton(QStringLiteral("�˳�"),this);

    this->gameOverLabel = new QLabel(QStringLiteral("��Ϸ����"),this);
    this->gameOverLabel->hide();
    this->gameOverLabel->setGeometry(300,80,150,50);
    this->gameOverLabel->setFont(QFont("Times",20,QFont::Bold));
    this->gameOverLabel->setPalette(QPalette(QColor(120,40,20)));
    this->scene()->addWidget(this->gameOverLabel);

    this->gradeLabel = new QLabel(QStringLiteral("�÷� : "),this);
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

void Game::init()//![�ᱻ��ε��� ÿ�����¿��ֶ��ǵ��� �������]
{
    this->mousePressed = false; //![ ��ʼ���û�б�����]
    //this->bgm->setMedia(QUrl::fromLocalFile("/home/jacklu/music/Summer.mp3"));
    //this->bgm->play();

    //![ �����������״�б� ���ǵ������ʱ����Ҫ�峡]
    QList<MyShape*>::iterator i = this->currentShapelist.begin();
    while (i !=this->currentShapelist.end()){
            this->scene()->removeItem(*i);//![ ɾ������״]
            i = this->currentShapelist.erase(i);//![ �ӵ�ǰ��Ч��״����ɾ��]
    }

    //![ ���������ʼֵ]
    for (int i = 0; i < this->sceneArray.size(); i ++)
        for (int j = 0; j < this->sceneArray[i].size(); j ++)
            this->sceneArray[i][j] = false;

    this->gameOverLabel->hide();
    this->pauseButton->setEnabled(true);

    this->gameStatus = true;//![ ֻ�Ǳ������Ϸ��״̬ ��ǰΪ����״̬]
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
    //![ ���ڷ���]
    this->totalNumOfShapesUsed=0;
    this->rowsNumRemoves = 0;
    this->grades = 0;
    this->gradeNumber->setPlainText(QString::number(this->grades));
    this->myshape = this->newShape(shapeCode::shapeRandom);
    this->scene()->addItem(this->myshape);

    //![ ʱ������ʼ]
    this->timer->start();
    this->timeCount = 0;
    //![ ���÷����½��ٶ�]
    this->speed = 1;
}

void Game::keyPressEvent(QKeyEvent *event)
{
    if (!this->myshape)
        return ;
    if (!this->gameStatus)//![ �����������̬��������̬ͣ]
        return ;
    if (event->key() == Qt::Key_Left) {
        this->setFlag(false);
        this->myshape->moveBy(-20,0);
        //![ �߽��߳�ͻ�ж�  �Լ� ����֮����жȰ�ɭ��]
        if (this->isBorderColliding() || this->isShapeColliding()) {
            this->myshape->moveBy(20,0);
            this->setFlag(true);
            return ;
        }
    }
    else if (event->key() == Qt::Key_Right) {
        this->setFlag(false);
        this->myshape->moveBy(20,0);
        //![ �߽��߳�ͻ�ж� �Լ� ��״֮���ͻ�ж�]
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
        for (int i = 0; i < 2; i ++) { //![ �� for ѭ���������ͬ��������]
            if (!this->moveDownOneStep())
                break;
        }
    }
    else if (event->key() == Qt::Key_Space) {
        this->setFlag(false);//![���ԭ�б�־]
        this->myshape->rolate();
        //![ �߽��߳�ͻ�ж� �Լ� ����֮���ͻ�ж�]
        //![ ����Ĵ���ͬʱʹ����תʱ����Ϊ����ԭ�������㹻�Ŀռ�ȴ������ת������õ��ܺõĽ�������ȵõ�����]
        if (this->isBorderColliding() || this->isShapeColliding()) {//![��ͻ]
            this->myshape->moveBy(20,0);//![ �������ƶ�]
            if (this->isBorderColliding() || this->isShapeColliding()) {//![ ��Ȼ��ͻ]
                this->myshape->moveBy(-40,0); //![ �������ƶ�]
                if (this->isBorderColliding() || this->isShapeColliding()) { //![ ���Ƿ�����ͻ]
                    if (this->currentShapeId == shapeCode::shapeLine) {//![ shapeLine ����һ��]
                        this->myshape->moveBy(-20,0);
                        if (!(this->isBorderColliding() || this->isShapeColliding())) {
                            return ;
                        }
                    }
                    this->myshape->moveBy(20,0);
                    this->myshape->rolateBack();
                    this->setFlag(true);//![ �ָ���־]
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
    //![ ֻ���жϷ���֮��ĳ�ͻ]
    int i, j;
    foreach (QGraphicsItem * item, this->myshape->childItems()) {
        switch(this->myshape->getCurrentStatus()) {//![ Ҳ����Ϊ����ԭ����Ϊ��ת�������˸ı�����Ķ�̬����]
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

        if (this->sceneArray[i][j]) //![ ����漰�ķ�Χ�д��� true ����˵���������ͻ]
            return true;
    }

    return false;
}

bool Game::isBorderColliding()
{
    //![ �жϵķ��� : ���� sceneBoundingRect() �ṩ��ֵ���ǵõ�����״�İ�Χ���Σ��жϸþ��θ������Ƿ���߽��߳�ͻ]
   QRectF rect = this->getShapeCurrentBoundingRectInScene();
   //![ if ( minX < this->areaOrigin.x() || minY < this->areaOrigin.y() || maxX > (this->areaOrigin.x()+this->areaWidth) || maxY > (this->areaOrigin.y() + this->areaHeight))]
   //![ ���������ϱ߽���жȣ���Ϊֻ����Ϸ������ʱ��Żᴥ���ϱ߽磬��Ϸ�����еĴ����������������԰��ϱ߽���������������жϱ߽��ͻ�ĺ�����д��]
    if ( rect.x() < this->areaOrigin.x() || (rect.x()+rect.width()) > (this->areaOrigin.x()+this->areaWidth) || (rect.y()+rect.height()) > (this->areaOrigin.y() + this->areaHeight))
         return true;
    return false;
}

bool Game::moveDownOneStep()//![ ֻ�ǵ������½�һ��������ֵ��ʾ�½��Ƿ�ɹ�]
{
    this->setFlag(false);//![ �����ԭռ�б�־]
    this->myshape->moveBy(0,20);
    //![������ �߽��߳�ͻ�ж� �Լ� ��״֮���ͻ�ж�]
    if (this->isBorderColliding() || this->isShapeColliding()) {//![ ��⵽�г�ͻ �����ƶ�ʧ�� ��ȡ��Ӧ�Ķ���]
       // qDebug() << "colliding"<< endl;
        this->myshape->moveBy(0,-20);
        this->setFlag(true);//![ �ָ�ԭռ�б�־]
        //![ �½�ʧ�� ����ζ�ű�����״���н���]
        //![ ���������жϣ�Ȼ������� �ж��Ƿ����ϱ߽磬��Ϊ������ɺ󻹴������ϱ߽���ζ����Ϸ���������û�н�����֮����� ����һ����״]
        this->clearUpAndRenewShape();
        return false;
    }

    //![ û�з�����ͻ ֱ��ռ�е�ǰλ�õ����б�־]
    this->setFlag(true);

    return true;
}

void Game::clearUpAndRenewShape()
{
    int count = 0; //![ ������¼�Ѿ������˵�����]
    //![ �������������жϺʹ���]
    for (int y = this->areaOrigin.y()+this->areaHeight-20; y >= 0; y -= 20) {
        QList<QGraphicsItem *> list = this->scene()->items(QRectF(this->areaOrigin.x()-1,y-1,this->areaWidth+2,22),Qt::ContainsItemShape);

        if (list.count() == 0) {

            break;
        }
        QList<QGraphicsItem *>::iterator i = list.begin();
        while (i != list.end()) {
            if (!((*i)->sceneBoundingRect().width() <25)) {//![ ���˵� ���ǻ�����λ����� item]
                i = list.erase(i);//![ erase֮�󷵻�ֵΪԭ��i����һ���ڵ�]
            }
            else
                i ++;
        }

        int rowsForArray = (y-this->areaOrigin.y())/20;
        if (list.count() == (this->areaWidth/20)) {//![ ��ͨ����]
            for (int k = 0; k < this->sceneArray[rowsForArray].size(); k ++)
                this->sceneArray[rowsForArray][k] = false;

            foreach (QGraphicsItem * item, list) {
                this->scene()->removeItem(item);
            }

             count ++;
        }
        else if (count > 0){//![ û����������֮ǰ������������ ��ȡ�Ĵ�ʩ���ǣ��½���Ӧ������(count����Ҫ�½�������)]
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
                    default: QMessageBox::warning(this,"����","Here , something wrong in game.cpp"); break;
                }
            }
        }
    }
    //![ ����Ĵ�����е��Ǽ�������������鹤���������ţ�����������������������ǾͶԵ�ǰ������״��飬�Ƿ�����״��Ч(��û�к���)]
    if (count > 0) { //![ �����������������ж��Ƿ���������]
        this->clearShapeWithNoChild();
    }
   // qDebug() << "jack is here";
    //![ �������жϵ�ǰ�����Ƿ�Ӵ����ϱ߽� ��Ҳ������ζ���ж���Ϸ�Ƿ����]
    //qDebug() << "this->getShapeCurrentBoundingInScene y: " << this->getShapeCurrentBoundingRectInScene().y() << "    this->areaOrigin.y() " << this->areaOrigin.y();
    if (this->getShapeCurrentBoundingRectInScene().y() <= this->areaOrigin.y()) {
        /* ��Ϸ���� */
        this->gameOver();
        return ;
    }

    //![ �����Ƿ�������]
    this->totalNumOfShapesUsed ++;
    this->grades = this->totalNumOfShapesUsed*5 + this->rowsNumRemoves*20;
    this->gradeNumber->setPlainText(QString::number(this->grades));

    //![ ���ݷ������ٶȽ��е���]
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
    //![ ����������һ����״]
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
            this->scene()->removeItem(*i);//![ ɾ������״]
            i = this->currentShapelist.erase(i);//![ �ӵ�ǰ��Ч��״����ɾ��]
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

    //![ ������Ϊ������״������״����]
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
    //![ ע����ǣ� ��״�� x,y ��������ת���ƶ���Ҳ����˵��x,y���ǹ̶������Ͻǡ����ǵ���㣬������Ҫ�ֱ�����������]
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
    this->gradeLabel->setText(QString(QStringLiteral("�÷� : "))+QString::number(this->grades));
    this->gradeLabel->show();
    this->pauseButton->setEnabled(false);
}

void Game::setFlag(bool flag)
{
   // qDebug() << "setFlag   flag = " << flag << this->myshape->getCurrentStatus() <<endl;
    int i, j;
    switch(this->myshape->getCurrentStatus()) {//![ ���������������ۣ�����Ϊ���������]
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
        this->pauseButton->setText(QStringLiteral("����"));
        this->gameStatus = false;
    }
    else {
        this->timer->start();
        this->pauseButton->setText(QStringLiteral("��ͣ"));
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
   //![ qDebug() << screenshotDir.count();// ��Ϊ һ���ļ����������� .  ..  �������ļ���]
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
    if (this->mousePressed) { //![ ���Ŷ��걻����,��������з�������϶�]
        if (this->cnt ++ == 4) {//![ cnt ������������ ��Ϊ���ÿ�ƶ�һ�����ص� �������嶼Ҫִ��һ���ƶ����������¿���϶������еĶ�������Ĳ���  �����Ҽ���һ�� cnt ����������ʹ��ÿ�ƶ�10�����ص�Ż�ִ��һ���������ƶ��� ��Ȼ�кܺõ�Ч��]
            this->move((this->pos()+=(event->pos()-=this->mousePressPos)));
            this->cnt = 0;
        }
    }
}

void Game::mouseReleaseEvent(QMouseEvent *event)
{
    this->mousePressed = false;
}
