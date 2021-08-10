#include "doublesnake.h"
#include "ui_doublesnake.h"

doubleSnake::doubleSnake(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::doublesnake){
    setStyleSheet("QWidget{background:white}");
    ui->setupUi(this);

    //初始化界面为1200*800的大小
    resize(1200,800);

    //设置界面标题
    setWindowTitle(tr("SpaceZ の Snake"));

    //游戏初始化
    snake1.append(QRectF(600,600,snakeNodeWidth,snakeNodeHeight));
    upRect(snake1);
    snake2.append(QRectF(800,800,snakeNodeWidth,snakeNodeHeight));
    upRect(snake2);
    normalApple.append(QRectF(120,120,snakeNodeWidth,snakeNodeWidth));
    normalApple.append(QRectF(320,120,snakeNodeWidth,snakeNodeWidth));

    //定时器与函数相连接
    timer1 = new QTimer;
    timer2 = new QTimer;
    connect(timer1, SIGNAL(timeout()),this,SLOT(snakeUpdate1()));
    connect(timer2, SIGNAL(timeout()),this,SLOT(snakeUpdate2()));
    timer1->start(200);
    timer2->start(200);

    rewardTimer = new QTimer;
    connect(rewardTimer,SIGNAL(timeout()),this,SLOT(normalAppleUpdate()));
    rewardTimer->start(1000);
}

doubleSnake::~doubleSnake(){
    delete ui;
}

void doubleSnake::snakeUpdate1(){
    //判断是否吃到了食物 若吃到食物则增长 否则维持原来长度
    bool flag=true;
    for(int i=0; i<normalApple.length(); i++){
        if(normalApple.at(i)==snake1.at(0)){
            flag=false;
            normalApple.removeAt(i);
            break;
        }
    }

    //让蛇身按当前方向运动
    switch (direction1) {
    case Up:
        upRect(snake1);
        break;
    case Down:
        downRect(snake1);
        break;
    case Right:
        rightRect(snake1);
        break;
    case Left:
        leftRect(snake1);
        break;
    default:
        break;
    }

    if(flag)
        deleteLastRect(snake1);
    update();
}

void doubleSnake::snakeUpdate2(){
    //判断是否吃到了食物 若吃到食物则增长 否则维持原来长度
    bool flag=true;
    for(int i=0; i<normalApple.length(); i++){
        if(normalApple.at(i)==snake2.at(0)){
            flag=false;
            normalApple.removeAt(i);
            break;
        }
    }

    //让蛇按当前方向运动
    switch (direction2) {
    case Up:
        upRect(snake2);
        break;
    case Down:
        downRect(snake2);
        break;
    case Right:
        rightRect(snake2);
        break;
    case Left:
        leftRect(snake2);
        break;
    default:
        break;
    }
    if(flag)
        deleteLastRect(snake2);
    update();
}

//果子的生成
void doubleSnake::normalAppleUpdate(){
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    normalApple.append(QRectF((qrand()%28)*40+40,(qrand()%18)*40+40,snakeNodeWidth,snakeNodeWidth));
    update();

    //若果子太多则让一些果子消失 避免界面上太多果子
    if(normalApple.size()>15)
        for(int i=0;i<normalApple.size();i++)
            if(i%3==0)
                normalApple.removeAt(i);
}

//移动的思路为在蛇头增加一个运动方向的格子 同时根据吃到果子与否决定是否减少蛇尾一个格子
//如判断在边界处要让蛇头从另一端出现 否则蛇将跑出界面区域
//向上增加
void doubleSnake::upRect(QList<QRectF> &snake){
    if(snake.at(0).y()-snakeNodeHeight < 0)
        snake.insert(0,QRectF(QPointF(snake.at(0).x(),this->height()-snakeNodeHeight),QPointF(snake.at(0).x()+snakeNodeWidth,this->height())));
    else
        snake.insert(0,QRectF(snake.at(0).topLeft()+QPointF(0,-snakeNodeHeight),snake.at(0).topRight()));
}

//向下增加
void doubleSnake::downRect(QList<QRectF> &snake){
    if(snake.at(0).y()+snakeNodeHeight*2 > this->height())
        snake.insert(0,QRectF(QPointF(snake.at(0).x(),snakeNodeHeight),QPointF(snake.at(0).x()+snakeNodeWidth,0)));
    else
        snake.insert(0,QRectF(snake.at(0).bottomLeft(),snake.at(0).bottomRight()+QPointF(0,snakeNodeHeight)));
}

//向左增加
void doubleSnake::leftRect(QList<QRectF> &snake){
    if(snake.at(0).x()-snakeNodeWidth < 0)
        snake.insert(0,QRectF(QPointF(this->width()-snakeNodeWidth,snake.at(0).y()),QPointF(this->width(),snake.at(0).y()+snakeNodeHeight)));
    else
        snake.insert(0,QRectF(snake.at(0).topLeft()+QPointF(-snakeNodeWidth,0),snake.at(0).bottomLeft()));
}

//向右增加
void doubleSnake::rightRect(QList<QRectF> &snake){
    if(snake.at(0).x()+snakeNodeWidth*2 > this->width())
        snake.insert(0,QRectF(QPointF(0,snake.at(0).y()),QPointF(snakeNodeWidth,snake.at(0).y()+snakeNodeHeight)));
    else
        snake.insert(0,QRectF(snake.at(0).topRight(),snake.at(0).bottomRight()+QPointF(snakeNodeWidth,0)));
}

//删除结尾数据
void doubleSnake::deleteLastRect(QList<QRectF> &snake){
    snake.removeLast();
}

//绘图
void doubleSnake::paintEvent(QPaintEvent *event){
    //如果游戏结束则在当前界面停留3s后关闭
    if(gameOver){
        timer1->stop();
        timer2->stop();
        rewardTimer->stop();
        QTime t;
        t.start();
        while(t.elapsed()<3000)
            QCoreApplication::processEvents();
        this->close();
    }
    QPainter painter(this);
    QPen pen;
    QBrush brush;
    QFont font;

    //绘制两条蛇和果子
    painter.drawPixmap(snake1.at(0).x(),snake1.at(0).y(),snakeNodeWidth,snakeNodeWidth,QPixmap(":/Images/head.png"));
    for(int i=1; i<snake1.length(); i++)
        painter.drawPixmap(snake1.at(i).x(),snake1.at(i).y(),snakeNodeWidth,snakeNodeWidth,QPixmap(":/Images/body.png"));

    painter.drawPixmap(snake2.at(0).x(),snake2.at(0).y(),snakeNodeWidth,snakeNodeWidth,QPixmap(":/Images/another.png"));
    for(int i=1; i<snake2.length(); i++)
        painter.drawPixmap(snake2.at(i).x(),snake2.at(i).y(),snakeNodeWidth,snakeNodeWidth,QPixmap(":/Images/anotherbody.png"));

    painter.setRenderHint(QPainter::Antialiasing, true);
    int num=normalApple.length()>15?15:normalApple.length();
    for(int i=0; i<num; i++)
        painter.drawImage(normalApple.at(i),QImage(":/Images/normalApple.png"));

    pen.setColor(Qt::magenta);
    painter.setPen(pen);

    //如果发生撞击（撞到自己或撞到另一条蛇）则游戏结束
    if(snakeStrike(snake1)||snakeStrike(snake2)||anotherSnakeStrike()){
        font.setPointSize(40);
        painter.setFont(font);
        painter.drawText(rect(),Qt::AlignCenter,"GAME OVER!");
        gameOver=true;
    }

    QWidget::paintEvent(event);
}

//处理键盘事件
void doubleSnake::keyPressEvent(QKeyEvent *event){
    switch(event->key()){
    if(gameStart){
        //WASD控制第一条蛇的移动
    case Qt::Key_W:
            if(direction1!=Down){
                direction1=Up;
            }
            break;
        case Qt::Key_S:
            if(direction1!=Up){
                direction1=Down;
            }
            break;
        case Qt::Key_D:
            if(direction1!=Left){
                direction1=Right;
            }
            break;
        case Qt::Key_A:
            if(direction1!=Right){
                direction1=Left;
            }
            break;

            //上下左右控制第二条蛇的移动
        case Qt::Key_Up:
            if(direction2!=Down){
                direction2=Up;
            }
            break;
        case Qt::Key_Down:
            if(direction2!=Up){
                direction2=Down;
            }
            break;
        case Qt::Key_Right:
            if(direction2!=Left){
                direction2=Right;
            }
            break;
        case Qt::Key_Left:
            if(direction2!=Right){
                direction2=Left;
            }
            break;
    }
    default:
        break;
    }
}

//判断蛇头是否和蛇身相撞
bool doubleSnake::snakeStrike(QList<QRectF> snake){
    for(int j=1; j<snake.length(); j++){
        if(snake.at(0)==snake.at(j)){
            return true;
        }
    }
    return false;
}

//判断蛇头是否和另一条蛇相撞
bool doubleSnake::anotherSnakeStrike(){
    for(int j=0; j<snake2.length(); j++)
        if(snake1.at(0)==snake2.at(j))
            return true;
    for(int j=0; j<snake1.length(); j++)
        if(snake2.at(0)==snake1.at(j))
            return true;
    return false;
}
