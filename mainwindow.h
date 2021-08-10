#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "singlesnake.h"
#include "doublesnake.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    //指向新对象的指针
    singleSnake* sSnake=NULL;
    singleSnake* rSnake=NULL;
    doubleSnake* dSnake=NULL;
    ~MainWindow();

private slots:
    void on_pushButton_4_clicked();
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
