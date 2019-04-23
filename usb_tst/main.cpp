#include "mainwindow.h"
#include <QApplication>
#include "devmanager.h"
#include <iostream>
using namespace std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    cout<<"getUSBMountFlag"<<getUSBMountFlag()<<endl;
   //w.show();

    return a.exec();
}
