#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <glib.h>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent, GstElement *pipeline, GstElement *volume) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), pipeline(pipeline), volume(volume)
{
  ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::get_time_string(int timeInSecond)
{
    return QObject::tr("%1:%2:%3").arg(timeInSecond/3600).arg(timeInSecond/60).arg(timeInSecond);
}

WId MainWindow::get_movieFrame_winId()
{
    return ui->movieFrame->winId();
}

QSlider* MainWindow::get_timeSlider_ptr()

{
    return ui->timeSlider;
}

QLabel* MainWindow::get_curTimeLabel()
{
    return ui->curTimelabel;
}

QLabel* MainWindow::get_durTimeLabel()
{
    return ui->durTimelabel;
}

void MainWindow::on_playButton_clicked()
{
    gst_element_set_state (pipeline, GST_STATE_PLAYING);
}

void MainWindow::on_pauseButton_clicked()
{
        gst_element_set_state (pipeline, GST_STATE_PAUSED);
}

void MainWindow::on_stopButton_clicked()
{
    gst_element_set_state (pipeline, GST_STATE_READY);
    ui->curTimelabel->setText(MainWindow::get_time_string(0));
    ui->timeSlider->setValue(0);
}
void MainWindow::on_timeSlider_rangeChanged(int min, int max)
{
   (void)min; (void)max;
}

void MainWindow::on_timeSlider_sliderMoved(int position)
{
    //g_print("[on_timeSlider_sliderMoved] to %d", position);
    gst_element_seek_simple (pipeline, GST_FORMAT_TIME, GstSeekFlags(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT), (gint64)(position * GST_SECOND));
    ui->curTimelabel->setText(get_time_string(position));
}

void MainWindow::on_volumeSlider_sliderMoved(int position)
{
    g_object_set(volume,"volume",position * 1.0/ 100,NULL);
    ui->volumelabel->setText(QObject::tr("%1").arg(int(position*1.0/99*100)));
    g_print("[volumelabel] to %d",  position);
}

void MainWindow::on_backButton_clicked()
{
        int position = ui->timeSlider->value() - 5;
        position = (position < 0) ? 0 : position;
        gst_element_seek_simple (pipeline, GST_FORMAT_TIME, GstSeekFlags(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT), (gint64)(position* GST_SECOND));
}

void MainWindow::on_forwardButton_clicked()
{
    int position = ui->timeSlider->value() + 5;
    gst_element_seek_simple (pipeline, GST_FORMAT_TIME, GstSeekFlags(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT), (gint64)(position * GST_SECOND));

}


