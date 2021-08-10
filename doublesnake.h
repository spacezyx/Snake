#ifndef DOUBLESNAKE_H
#define DOUBLESNAKE_H

#include <QWidget>

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

QT_BEGIN_NAMESPACE
namespace Ui { class doublesnake; }
QT_END_NAMESPACE

class doubleSnake : public QWidget
{
    Q_OBJECT

public:
    explicit doubleSnake(QWidget *parent = 0);
    ~doubleSnake();
protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);

private:

    //控制蛇的移动及增长
    void upRect(QList<QRectF> &snake);
    void downRect(QList<QRectF> &snake);
    void leftRect(QList<QRectF> &snake);
    void rightRect(QList<QRectF> &snake);
    void deleteLastRect(QList<QRectF> &snake);

    //判断是否咬到自己
    bool snakeStrike(QList<QRectF> snake);
    bool anotherSnakeStrike();

    //运动方向
    enum Move{Left,Right,Up,Down};

protected slots:
    void snakeUpdate1();
    void snakeUpdate2();
    void normalAppleUpdate();

private:
    Ui::doublesnake *ui;
    //将界面分成40*40的小格
    int snakeNodeWidth = 40;
    int snakeNodeHeight = 40;
    //设置定时器
    QTimer *timer1;
    QTimer *timer2;
    QTimer *rewardTimer;

    //设置矩形列表snake1，snake2
    QList<QRectF> snake1;
    QList<QRectF> snake2;
    QList<QRectF> normalApple;

    //设定方向及游戏状态
    int direction1 = Up;
    int direction2 = Up;
    bool gameOver = false;
    bool gameStart = true;
};
#endif // DOUBLESNAKE_H
