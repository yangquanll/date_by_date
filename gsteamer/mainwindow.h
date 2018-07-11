#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <gst/gst.h>
#include <QSlider>
#include <QLabel>
#include <QFrame>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, GstElement *pipeline = NULL, GstElement *volume = NULL);
    ~MainWindow();
    WId get_movieFrame_winId();
    QSlider* get_timeSlider_ptr();
    QLabel* get_curTimeLabel();
    QLabel* get_durTimeLabel();
    static QString get_time_string(int timeInSecond);

private slots:
    void on_playButton_clicked();

    void on_pauseButton_clicked();

    void on_stopButton_clicked();

    void on_timeSlider_rangeChanged(int min, int max);

    void on_timeSlider_sliderMoved(int position);

    void on_volumeSlider_sliderMoved(int position);
    
    void on_backButton_clicked();
    
    void on_forwardButton_clicked();
    

private:
    Ui::MainWindow *ui;
    GstElement *pipeline;
    GstElement *volume;
};

#endif // MAINWINDOW_H
