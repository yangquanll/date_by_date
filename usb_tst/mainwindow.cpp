#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

void MainWindow:: on_checkusb(void)
{
    startGetDevInfo();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_checkButton_clicked()
{
    startGetDevInfo();
}

void MainWindow::on_getnwtButton_clicked()
{
   printf("yL --- >getUSBMountFlag = %d \n",getUSBMountFlag());
}
