/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLabel *label;
    QSlider *timeSlider;
    QSlider *volumeSlider;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *curTimelabel;
    QLabel *label_4;
    QLabel *durTimelabel;
    QLabel *volumelabel;
    QLabel *label_6;
    QPushButton *playButton;
    QPushButton *pauseButton;
    QPushButton *stopButton;
    QFrame *movieFrame;
    QPushButton *backButton;
    QPushButton *forwardButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(689, 547);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(180, 120, 301, 171));
        timeSlider = new QSlider(centralWidget);
        timeSlider->setObjectName(QStringLiteral("timeSlider"));
        timeSlider->setGeometry(QRect(50, 373, 160, 29));
        timeSlider->setOrientation(Qt::Horizontal);
        volumeSlider = new QSlider(centralWidget);
        volumeSlider->setObjectName(QStringLiteral("volumeSlider"));
        volumeSlider->setGeometry(QRect(450, 373, 160, 29));
        volumeSlider->setValue(30);
        volumeSlider->setOrientation(Qt::Horizontal);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 373, 41, 31));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(390, 373, 51, 31));
        curTimelabel = new QLabel(centralWidget);
        curTimelabel->setObjectName(QStringLiteral("curTimelabel"));
        curTimelabel->setGeometry(QRect(210, 370, 61, 31));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(250, 370, 20, 31));
        durTimelabel = new QLabel(centralWidget);
        durTimelabel->setObjectName(QStringLiteral("durTimelabel"));
        durTimelabel->setGeometry(QRect(260, 370, 67, 31));
        volumelabel = new QLabel(centralWidget);
        volumelabel->setObjectName(QStringLiteral("volumelabel"));
        volumelabel->setGeometry(QRect(610, 370, 31, 31));
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(650, 370, 41, 31));
        playButton = new QPushButton(centralWidget);
        playButton->setObjectName(QStringLiteral("playButton"));
        playButton->setGeometry(QRect(60, 420, 51, 27));
        pauseButton = new QPushButton(centralWidget);
        pauseButton->setObjectName(QStringLiteral("pauseButton"));
        pauseButton->setGeometry(QRect(190, 420, 51, 27));
        stopButton = new QPushButton(centralWidget);
        stopButton->setObjectName(QStringLiteral("stopButton"));
        stopButton->setGeometry(QRect(310, 420, 51, 27));
        movieFrame = new QFrame(centralWidget);
        movieFrame->setObjectName(QStringLiteral("movieFrame"));
        movieFrame->setGeometry(QRect(39, 19, 601, 341));
        movieFrame->setFrameShape(QFrame::StyledPanel);
        movieFrame->setFrameShadow(QFrame::Raised);
        backButton = new QPushButton(centralWidget);
        backButton->setObjectName(QStringLiteral("backButton"));
        backButton->setGeometry(QRect(120, 450, 51, 27));
        forwardButton = new QPushButton(centralWidget);
        forwardButton->setObjectName(QStringLiteral("forwardButton"));
        forwardButton->setGeometry(QRect(250, 450, 51, 27));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 689, 28));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "Time", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "Volume", nullptr));
        curTimelabel->setText(QApplication::translate("MainWindow", "0.0.0", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "/", nullptr));
        durTimelabel->setText(QApplication::translate("MainWindow", "0.0.0", nullptr));
        volumelabel->setText(QApplication::translate("MainWindow", "30", nullptr));
        label_6->setText(QApplication::translate("MainWindow", "%", nullptr));
        playButton->setText(QApplication::translate("MainWindow", "Play", nullptr));
        pauseButton->setText(QApplication::translate("MainWindow", "Pause", nullptr));
        stopButton->setText(QApplication::translate("MainWindow", "Stop", nullptr));
        backButton->setText(QApplication::translate("MainWindow", "<<", nullptr));
        forwardButton->setText(QApplication::translate("MainWindow", ">>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
