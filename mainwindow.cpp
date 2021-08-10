#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "singlesnake.h"
#include "ui_singlesnake.h"

//菜单界面
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(1200,800);
    setWindowTitle(tr("SpaceZ の Snake"));
    this->setAutoFillBackground(true);
    QPalette palette=this->palette();
    palette.setBrush(QPalette::Window,QBrush(QPixmap(":/Images/background.jpg").scaled(this->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
    this->setPalette(palette);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//取消功能
void MainWindow::on_pushButton_4_clicked()
{
    this->close();
}

//单人游戏 点击开启新界面
void MainWindow::on_pushButton_clicked()
{
    sSnake =new singleSnake;
    sSnake->show();
}

//双人游戏 点击开启新界面
void MainWindow::on_pushButton_2_clicked()
{
    dSnake=new doubleSnake;
    dSnake->show();
}

//读档功能
void MainWindow::on_pushButton_3_clicked()
{
    std::ifstream in("Snake.txt");
    if(!in.is_open())
        return;
    else{
        rSnake =new singleSnake;
        rSnake->isResume=true;
        rSnake->Load();
        //rSnake->setStyleSheet("QWidget{background-color:col}");
        rSnake->show();
    }
}
