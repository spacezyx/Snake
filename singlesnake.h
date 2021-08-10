#ifndef SINGLESNAKE_H
#define SINGLESNAKE_H

#include <QWidget>
#include <QFile>
#include <QWidget>
#include <QKeyEvent>
#include <QRectF>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QColor>
#include <fstream>
#include <sstream>

QT_BEGIN_NAMESPACE
namespace Ui { class singlesnake; }
QT_END_NAMESPACE

class singleSnake : public QWidget
{
    Q_OBJECT

public:
    explicit singleSnake(QWidget *parent = 0);
    ~singleSnake();
    bool isResume=false;
    void Load();
protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);

private:

    //控制蛇的移动及增长
    void upRect();
    void downRect();
    void leftRect();
    void rightRect();
    void deleteLastRect();
    void Record();


    //判断是否咬到自己
    bool snakeStrike();

    //判断是否撞墙
    bool wallStrike();

    //ai蛇
    void aiSnake();

    //运动方向
    enum Move{Left,Right,Up,Down};

    //墙的类型
    enum WallType{fullWall,halfWall,noWall};

    //界面显示的食物数量
    int specialAppleNum=1;
    int normalAppleNum=10;

protected slots:
    void snakeUpdate();
    void normalAppleUpdate();
    void specialAppleUpdate();

private:
    Ui::singlesnake *ui;

    //生命值 与lifeApple相关
    int life=1;

    //速度 控制每一次更新的时间间隔 与speedApple相关
    int speed=200;

    //将界面划分成小块 小块的长和宽固定
    int snakeNodeWidth = 40;
    int snakeNodeHeight = 40;

    //设置定时器 分别控制蛇 普通果子和特殊果子的更新速度
    QTimer *timer;
    QTimer *rewardTimer;
    QTimer *specialTimer;

    //1200*800的界面被分割成40*40的小块 小块为每一种物质的最小单元
    //贪吃蛇本体 用存储小块的列表储存
    QList<QRectF> snake;

    //其余各类物质 同样用列表储存
    QList<QRectF> Wall;
    QList<QRectF> normalApple;
    QList<QRectF> lifeApple;
    QList<QRectF> specialApple;
    QList<QRectF> speedUpApple;
    QList<QRectF> speedDownApple;

    //设置变量分别表示方向 墙的状态 按键状态以及游戏状态
    int direction = Up;
    int wall=noWall;
    bool gameOver = false;
    bool gameStart = false;
    bool isPress=true;
    bool AISnake=false;
};
#endif // SINGLESNAKE_H
