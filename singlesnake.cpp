#include "singlesnake.h"
#include "ui_singlesnake.h"

singleSnake::singleSnake(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::singlesnake){
    setStyleSheet("QWidget{background:white}");
    ui->setupUi(this);

    //初始化界面为1200*800的大小
    resize(1200,800);

    //设置界面标题
    setWindowTitle(tr("SpaceZ の Snake"));

    //游戏初始化
    snake.append(QRectF(600,600,snakeNodeWidth,snakeNodeHeight));
    upRect();
    lifeApple.append(QRectF(1000,200,snakeNodeWidth,snakeNodeWidth));
    speedUpApple.append(QRectF(800,200,snakeNodeWidth,snakeNodeWidth));
    speedDownApple.append(QRectF(120,600,snakeNodeWidth,snakeNodeWidth));
    specialApple.append(QRectF(80,200,snakeNodeWidth,snakeNodeWidth));
    normalApple.append(QRectF(320,120,snakeNodeWidth,snakeNodeWidth));
    lifeApple.append(QRectF(800,200,snakeNodeWidth,snakeNodeWidth));
    speedUpApple.append(QRectF(800,40,snakeNodeWidth,snakeNodeWidth));
    speedDownApple.append(QRectF(120,400,snakeNodeWidth,snakeNodeWidth));
    specialApple.append(QRectF(800,200,snakeNodeWidth,snakeNodeWidth));

    //如果为读档模式
    //if(isResume)
        //Load();

    //定时器与函数相连接
    timer = new QTimer;
    connect(timer, SIGNAL(timeout()),this,SLOT(snakeUpdate()));

    rewardTimer = new QTimer;
    connect(rewardTimer,SIGNAL(timeout()),this,SLOT(normalAppleUpdate()));

    specialTimer = new QTimer;
    connect(rewardTimer,SIGNAL(timeout()),this,SLOT(specialAppleUpdate()));
    specialTimer->start(5000);
}

singleSnake::~singleSnake(){
    delete ui;
}

void singleSnake::snakeUpdate(){
    if(AISnake)
        aiSnake();

    //判断是否吃到了食物 若吃到食物则蛇增长 该食物消失 否则维持原来长度
    bool flag=true;
    for(int i=0; i<normalApple.length(); i++){
        if(normalApple.at(i)==snake.at(0)){
            flag=false;
            normalApple.removeAt(i);
            break;
        }
    }

    //判断特殊水果 启动特殊作用
    for(int i=0;i<specialAppleNum;i++){
        if(lifeApple.at(i)==snake.at(0)){
            flag=false;
            life++;
            lifeApple.removeAt(i);
        }

        //通过控制timer更新时间间隔控制速度 事件间隔越长速度越慢 并控制速度不要太快
        if(speedUpApple.at(i)==snake.at(0)){
            flag=false;
            if(speed>60)
                speed=speed/1.5;
            timer->start(speed);
            speedUpApple.removeAt(i);
        }
        if(speedDownApple.at(i)==snake.at(0)){
            flag=false;
            speed=speed*1.5;
            timer->start(speed);
            speedDownApple.removeAt(i);
        }
        if(specialApple.at(i)==snake.at(0)){
            //特殊功能 使背景变色
            flag=false;
            int n=snake.length()%5;
            if(n==0)
                setStyleSheet("QWidget{background:white}");
            if(n==1)
                setStyleSheet("QWidget{background:darkGreen}");
            if(n==2)
                setStyleSheet("QWidget{background:yellow}");
            if(n==3)
                setStyleSheet("QWidget{background:darkCyan}");
            if(n==4)
                setStyleSheet("QWidget{background:darkRed}");
            specialApple.removeAt(i);
        }
    }

    //让蛇按照当前方向运动
    switch (direction) {
    case Up:
        upRect();
        break;
    case Down:
        downRect();
        break;
    case Right:
        rightRect();
        break;
    case Left:
        leftRect();
        break;
    default:
        break;
    }

    //flag=true说明当前未吃到食物 蛇的长度不变 向蛇头方向增长一格则蛇尾要减一格
    if(flag)
        deleteLastRect();
    update();

    //更新后使按键处于可相应运动方向改变状态
    isPress=true;
}

//普通果子
void singleSnake::normalAppleUpdate(){
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    //将1200*800的界面分成40*40的小格 即有30*20个小格 最外围的小格为墙预留 因此剩下28*18的小格可以放入普通果子
    normalApple.append(QRectF((qrand()%28)*40+40,(qrand()%18)*40+40,snakeNodeWidth,snakeNodeWidth));
    update();

    //如果果子数目过多则让一部分果子消失 避免界面上存在太多果子
    if(normalApple.size()>15)
        for(int i=0;i<normalApple.size();i++)
            if(i%3==0)
                normalApple.removeAt(i);
}

//特殊水果的更新 定位方式与普通果子一致 但更新时间更缓慢
void singleSnake::specialAppleUpdate(){
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    lifeApple.append(QRectF((qrand()%28)*40+40,(qrand()%18)*40+40,snakeNodeWidth,snakeNodeWidth));
    speedDownApple.append(QRectF((qrand()%28)*40+40,(qrand()%18)*40+40,snakeNodeWidth,snakeNodeWidth));
    speedUpApple.append(QRectF((qrand()%28)*40+40,(qrand()%18)*40+40,snakeNodeWidth,snakeNodeWidth));
    specialApple.append(QRectF((qrand()%28)*40+40,(qrand()%18)*40+40,snakeNodeWidth,snakeNodeWidth));
    update();
}

//移动的思路为在蛇头增加一个运动方向的格子 同时根据吃到果子与否决定是否减少蛇尾一个格子
//如判断在边界处要让蛇头从另一端出现 否则蛇将跑出界面区域
//向上增加
void singleSnake::upRect(){
    if(snake.at(0).y()-snakeNodeHeight < 0)
        snake.insert(0,QRectF(QPointF(snake.at(0).x(),this->height()-snakeNodeHeight),QPointF(snake.at(0).x()+snakeNodeWidth,this->height())));
    else
        snake.insert(0,QRectF(snake.at(0).topLeft()+QPointF(0,-snakeNodeHeight),snake.at(0).topRight()));
}

//向下增加
void singleSnake::downRect(){
    if(snake.at(0).y()+snakeNodeHeight*2 > this->height())
        snake.insert(0,QRectF(QPointF(snake.at(0).x(),0),QPointF(snake.at(0).x()+snakeNodeWidth,snakeNodeHeight)));
    else
        snake.insert(0,QRectF(snake.at(0).bottomLeft(),snake.at(0).bottomRight()+QPointF(0,snakeNodeHeight)));
}

//向左增加
void singleSnake::leftRect(){
    if(snake.at(0).x()-snakeNodeWidth < 0)
        snake.insert(0,QRectF(QPointF(this->width()-snakeNodeWidth,snake.at(0).y()),QPointF(this->width(),snake.at(0).y()+snakeNodeHeight)));
    else
        snake.insert(0,QRectF(snake.at(0).topLeft()+QPointF(-snakeNodeWidth,0),snake.at(0).bottomLeft()));
}

//向右增加
void singleSnake::rightRect(){
    if(snake.at(0).x()+snakeNodeWidth*2 > this->width())
        snake.insert(0,QRectF(QPointF(0,snake.at(0).y()),QPointF(snakeNodeWidth,snake.at(0).y()+snakeNodeHeight)));
    else
        snake.insert(0,QRectF(snake.at(0).topRight(),snake.at(0).bottomRight()+QPointF(snakeNodeWidth,0)));
}

//删除尾巴
void singleSnake::deleteLastRect(){
    snake.removeLast();
}

//绘图
void singleSnake::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QPen pen;
    QBrush brush;
    QFont font;

    pen.setColor(Qt::gray);
    painter.setPen(pen);
    font.setPointSize(15);
    painter.setFont(font);

    //在游戏开始前设置提示文字
    if(!gameStart){
        QRectF pos0(100,300,1000,snakeNodeWidth);
        painter.drawText(pos0,Qt::AlignTop,"TIPS:");
        QRectF pos1(100,300+snakeNodeWidth,1000,snakeNodeWidth);
        painter.drawText(pos1,Qt::AlignTop,"Press F/H/N to set full-wall/half-wall/no-wall(you can skip this step)");
        QRectF pos2(100,300+2*snakeNodeWidth,1000,snakeNodeWidth);
        painter.drawText(pos2,Qt::AlignTop,"Press M/L to set more apples/less apples(you can skip this step)");
        QRectF pos3(100,300+3*snakeNodeWidth,1000,snakeNodeWidth);
        painter.drawText(pos3,Qt::AlignTop,"You can also use F/H/N/M/L when you stop the game or gameover");
        QRectF pos4(100,300+4*snakeNodeWidth,1000,snakeNodeWidth);
        painter.drawText(pos4,Qt::AlignTop,"Press Space to start our game");
        QRectF pos5(100,300+5*snakeNodeWidth,1000,snakeNodeWidth);
        painter.drawText(pos5,Qt::AlignTop,"Press I to start ai snake");
        QRectF pos6(100,300+6*snakeNodeWidth,1000,snakeNodeWidth);
        painter.drawText(pos6,Qt::AlignTop,"Press ESC to archive the game");
    }

    //分别画出蛇和各类果子 为美观全部用矢量图替换 各类果子根据不同选择控制显示数目
    painter.drawPixmap(snake.at(0).x(),snake.at(0).y(),snakeNodeWidth,snakeNodeWidth,QPixmap(":/Images/head.png"));
    for(int i=1; i<snake.length(); i++){
        painter.drawPixmap(snake.at(i).x(),snake.at(i).y(),snakeNodeWidth,snakeNodeWidth,QPixmap(":/Images/body.png"));
    }

    painter.setRenderHint(QPainter::Antialiasing, true);
    int nNum=normalAppleNum;
    if(normalAppleNum>normalApple.length())
        nNum=normalApple.length();

    for(int i=0; i<nNum; i++)
        painter.drawImage(normalApple.at(i),QImage(":/Images/normalApple.png"));

    for(int i=0;i<specialAppleNum;i++){
        painter.drawImage(lifeApple.at(i),QImage(":/Images/lifeApple.png"));
        painter.drawImage(speedUpApple.at(i),QImage(":/Images/speedUpApple.png"));
        painter.drawImage(speedDownApple.at(i),QImage(":/Images/speedDownApple.png"));
        painter.drawImage(specialApple.at(i),QImage(":/Images/specialApple.png"));
    }

    //绘制墙体
    for(int i=0;i<Wall.length();i++)
        painter.drawImage(Wall.at(i),QImage(":/Images/wall.png"));

    //左上角显示当前生命值
    pen.setColor(Qt::black);
    painter.setPen(pen);
    font.setPointSize(18);
    painter.setFont(font);
    QRectF pos(snakeNodeWidth,snakeNodeWidth,1000,snakeNodeWidth);
    painter.drawText(pos,Qt::AlignTop,"LIFE:"+QString("%1").arg(life));

    //当蛇撞击自己或墙体时减少生命
    //当生命值小于等于0时游戏结束显示提示性文字
    pen.setColor(Qt::magenta);
    painter.setPen(pen);

    if(snakeStrike()||wallStrike()){
        life--;
        if(life<=0){
            font.setPointSize(24);
            painter.setFont(font);

            QRectF pos1(100,200+snakeNodeWidth,1000,3*snakeNodeWidth);
            painter.drawText(pos1,"You can use F/H/N/M/L to set settings as well");

            font.setPointSize(40);
            painter.setFont(font);
            painter.drawText(rect(),Qt::AlignCenter,"GAME OVER!");

            QRectF pos2(100,600,1000,3*snakeNodeWidth);
            painter.drawText(pos2,"Press Enter to Restart!");
            timer->stop();
            speed=200;
            rewardTimer->stop();
            gameOver = true;
        }
        else{
            timer->stop();
            painter.drawText(rect(),Qt::AlignCenter,"A LIFE LOST!");

            //不同的死亡方式对应不同继续游戏方式
            //若撞击自己则从当前位置开始保留前三个格子
            //若撞击墙体则从固定位置重新开始
            if(!wallStrike()&&snakeStrike())
                for(int i=snake.length()-1;i>2;i--){
                    snake.removeAt(i);
                }
            else if(!snakeStrike()&&wallStrike()){
                snake.clear();
                direction = Up;
                snake.append(QRectF(600,600,snakeNodeWidth,snakeNodeHeight));
                upRect();
            }
            timer->start(speed);
        }
    }
    QWidget::paintEvent(event);
}

//处理键盘事件
void singleSnake::keyPressEvent(QKeyEvent *event){
    switch(event->key()){

    //键盘处于可响应方向状态时可使用WASD或上下左右键控制方向 同时不可反向
    if(isPress&&gameStart){
    case Qt::Key_Up:
        case Qt::Key_W:
            if(direction!=Down){
                direction=Up;
                isPress=false;
            }
            break;
        case Qt::Key_Down:
        case Qt::Key_S:
            if(direction!=Up){
                direction=Down;
                isPress=false;
            }
            break;
        case Qt::Key_Right:
        case Qt::Key_D:
            if(direction!=Left){
                direction=Right;
                isPress=false;
            }
            break;
        case Qt::Key_Left:
        case Qt::Key_A:
            if(direction!=Right){
                direction=Left;
                isPress=false;
            }
            break;
    }

    //回车键在游戏结束时用于重新开始一局游戏
    case Qt::Key_Return:
    case Qt::Key_Enter:
        if(gameOver){
            setStyleSheet("QWidget{background:white}");
            AISnake=false;
            snake.clear();
            normalApple.clear();
            lifeApple.clear();
            speedDownApple.clear();
            speedUpApple.clear();
            specialApple.clear();
            Wall.clear();
            direction = Up;
            wall=noWall;
            normalAppleNum=12;
            specialAppleNum=1;
            snake.append(QRectF(600,600,snakeNodeWidth,snakeNodeHeight));
            speed=200;
            upRect();
            //首先生成一个奖励节点
            normalApple.append(QRectF(120,120,snakeNodeWidth,snakeNodeWidth));
            lifeApple.append(QRectF(1000,200,snakeNodeWidth,snakeNodeWidth));
            speedUpApple.append(QRectF(800,200,snakeNodeWidth,snakeNodeWidth));
            speedDownApple.append(QRectF(120,600,snakeNodeWidth,snakeNodeWidth));
            specialApple.append(QRectF(80,200,snakeNodeWidth,snakeNodeWidth));
            timer->start(speed);
            rewardTimer->start(1000);
            gameOver = false;
            life=1;
        }
        break;

        //空格键用于游戏的暂停与继续
    case Qt::Key_Space:
        if(!gameStart && !gameOver){
            timer->start(speed);
            rewardTimer->start(1000);
            gameStart = !gameStart;
        }else if(gameStart && !gameOver){
            timer->stop();
            rewardTimer->stop();
            gameStart = !gameStart;
        }
        break;

        //F/H/N键分别对应与fullwall halfwall nowall状态 用于游戏开始前和游戏暂停时对墙体位置的编辑
    case Qt::Key_F:
        if(!gameStart||gameOver){
            wall=fullWall;
            for(int i=0;i<30;i++){
                Wall.append(QRectF(snakeNodeWidth*i,0,snakeNodeWidth,snakeNodeWidth));
                Wall.append(QRectF(snakeNodeWidth*i,760,snakeNodeWidth,snakeNodeWidth));
            }
            for(int i=0;i<20;i++){
                Wall.append(QRectF(0,snakeNodeWidth*i,snakeNodeWidth,snakeNodeWidth));
                Wall.append(QRectF(1160,snakeNodeWidth*i,snakeNodeWidth,snakeNodeWidth));
            }
        }
        break;
    case Qt::Key_N:
        if(!gameStart||gameOver){
            Wall.clear();
            wall=noWall;
        }
        break;
    case Qt::Key_H:
        if(!gameStart||gameOver){
            Wall.clear();
            wall=halfWall;
            for(int i=0;i<30;i++){
                Wall.append(QRectF(snakeNodeWidth*i,0,snakeNodeWidth,snakeNodeWidth));
                Wall.append(QRectF(snakeNodeWidth*i,760,snakeNodeWidth,snakeNodeWidth));
            }
        }
        break;

        //M/L键分别对应与more_apple和less_apple状态 用于游戏开始前和游戏暂停时对果子状态的编辑
    case Qt::Key_M:
        if(!gameStart||gameOver){
            specialAppleNum=2;
            normalAppleNum=15;
        }
        break;
    case Qt::Key_L:
        if(!gameStart||gameOver){
            specialAppleNum=1;
            normalAppleNum=10;
        }
        break;

        //I键用于启动AI蛇
        case Qt::Key_I:
            if(!gameOver&&gameStart){
              AISnake=true;
              //aiSnake();
              break;
           }

        //ESC键用于游戏的存档
    case Qt::Key_Escape:
    {
        gameStart=false;
        isResume=true;
        Record();
        rewardTimer->stop();
        specialTimer->stop();
        timer->stop();
        QTime t;
        t.start();
        while(t.elapsed()<1200)
            QCoreApplication::processEvents();
        this->close();
    }
        break;
    default:
        break;
    }
}

//判断蛇头是否和蛇身相撞
bool singleSnake::snakeStrike(){
    for(int j=1; j<snake.length(); j++){
        if(snake.at(0)==snake.at(j)){
            return true;
        }
    }
    return false;
}

//判断蛇头是否和墙相撞
bool singleSnake::wallStrike(){
    for(int j=0; j<Wall.length(); j++){
        if(snake.at(0)==Wall.at(j)){
            return true;
        }
    }
    return false;
}

//AI蛇
//首先计算蛇头和第一个普通食物的距离，设置xDir和yDir存储它们之间的距离。
//然后判断当前方向，如果方向为向上或向下时，按照xDir的正负选择左右方向；
//如果方向为向左或向右时，按照yDir的正负选择上下方向。
void singleSnake::aiSnake(){
    int xDir=snake[0].x()-normalApple[0].x();
    int yDir=snake[0].y()-normalApple[0].y();
    if(direction==Up||direction==Down){
        if(xDir>0)
            direction=Left;
        else if(xDir<0)
            direction=Right;
    }
    else if(direction==Left||direction==Right){
        if(yDir>0)
            direction=Up;
        else if(yDir<0)
            direction=Down;
    }
}

//存档
void singleSnake::Record(){
    QPalette pal=this->palette();
    //QBrush brush=;
    QColor col=pal.window().color();
    std::ofstream out("Snake.txt");
    int sLength=snake.length();
    int wLength=Wall.length();
    normalAppleNum=normalAppleNum>normalApple.length()?normalApple.length():normalAppleNum;
    out<<direction<<' '<<sLength<<' '<<wLength<<' '<<speed<<' '<<wall<<' '<<gameOver<<' '<<gameStart<<' ';
    out<<isPress<<' '<<specialAppleNum<<' '<<normalAppleNum<<' '<<life<<' '<<AISnake<<' ';
    out<<col.red()<<' '<<col.green()<<' '<<col.blue()<<' ';
    for(int i=0;i<normalAppleNum;i++)
        out<<normalApple[i].x()<<' '<<normalApple[i].y()<<' ';
    for(int i=0;i<sLength;i++)
        out<<snake[i].x()<<' '<<snake[i].y()<<' ';
    for(int i=0;i<specialAppleNum;i++)
        out<<lifeApple[i].x()<<' '<<lifeApple[i].y()<<' ';
    for(int i=0;i<specialAppleNum;i++)
        out<<specialApple[i].x()<<' '<<specialApple[i].y()<<' ';
    for(int i=0;i<specialAppleNum;i++)
        out<<speedUpApple[i].x()<<' '<<speedUpApple[i].y()<<' ';
    for(int i=0;i<specialAppleNum;i++)
        out<<speedDownApple[i].x()<<' '<<speedDownApple[i].y()<<' ';
    for(int i=0;i<wLength;i++)
        out<<Wall[i].x()<<' '<<Wall[i].y()<<' ';
}

void singleSnake::Load(){
    snake.clear();
    normalApple.clear();
    lifeApple.clear();
    speedDownApple.clear();
    speedUpApple.clear();
    specialApple.clear();
    Wall.clear();
    std::ifstream in("Snake.txt");
    int sLength=0;
    int wLength=0;
    int r=0;
    int g=0;
    int b=0;
    in>>direction>>sLength>>wLength>>speed>>wall>>gameOver>>gameStart;
    in>>isPress>>specialAppleNum>>normalAppleNum>>life>>AISnake;
    in>>r>>g>>b;
    for(int i=0;i<normalAppleNum;i++){
        int x=-1;int y=-1;
        in>>x>>y;
        normalApple.append(QRectF(x,y,snakeNodeWidth,snakeNodeHeight));
    }
    for(int i=0;i<sLength;i++){
        int x=-1;int y=-1;
        in>>x>>y;
        snake.append(QRectF(x,y,snakeNodeWidth,snakeNodeHeight));
    }
    for(int i=0;i<specialAppleNum;i++){
        int x=-1;int y=-1;
        in>>x>>y;
        lifeApple.append(QRectF(x,y,snakeNodeWidth,snakeNodeHeight));
    }
    for(int i=0;i<specialAppleNum;i++){
        int x=-1;int y=-1;
        in>>x>>y;
        specialApple.append(QRectF(x,y,snakeNodeWidth,snakeNodeHeight));
    }
    for(int i=0;i<specialAppleNum;i++){
        int x=-1;int y=-1;
        in>>x>>y;
        speedUpApple.append(QRectF(x,y,snakeNodeWidth,snakeNodeHeight));
    }
    for(int i=0;i<specialAppleNum;i++){
        int x=-1;int y=-1;
        in>>x>>y;
        speedDownApple.append(QRectF(x,y,snakeNodeWidth,snakeNodeHeight));
    }
    for(int i=0;i<wLength;i++){
        int x=-1;int y=-1;
        in>>x>>y;
        Wall.append(QRectF(x,y,snakeNodeWidth,snakeNodeHeight));
    }
    //setStyleSheet("QWidget{background:rgb(r,g,b)}");
}
