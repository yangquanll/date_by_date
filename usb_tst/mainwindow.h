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
};

class Search : public QMainWindow
{
    Q_OBJECT

public:
    Search() {}
    virtual ~Search();
    void getlocalfiles(void);
    void getusbst(void);
    void cpyusb(void);

};

class Netwok
{
    public:


};

#endif // MAINWINDOW_H
