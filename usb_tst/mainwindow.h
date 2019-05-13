#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "devmanager.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void on_checkusb(void);
    void on_checkButton_clicked();
    void on_getnwtButton_clicked();
};

class Search
{

public:
    Search() {}
    virtual ~Search();
    void getlocalfiles(void);
    void getusbst(void);
    void cpyusb(void);

};

class Netwok : public Search
{
    public:
        Netwok();
};

#endif // MAINWINDOW_H
