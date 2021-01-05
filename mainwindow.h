#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "getfilename.h"
#include <iostream>
#include <string>
#include <QMainWindow>
#include <QFileDialog>
#include <QThread>
#include <QLabel>
#include <QPlainTextEdit>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_receivingFilename(QString filename);

    void on_action_open_triggered();
    void on_actionExit_triggered();
    void on_actionTBstart_triggered();
    void on_actionTBpause_triggered();
    void on_actionTBstop_triggered();

private:
    Ui::MainWindow *ui;

    void resizeEvent(QResizeEvent*);
    QThread filename_thread;

    QPlainTextEdit pet;
    QPixmap current_pix;

    QDir directory_files;
    QString photo_path;
    QString w_filename;
    QString start_filename;
    QStringList file_list;

    QString program_name = "Slideshow";
    QString status_message;

    int thread_is_started {0};
    int pause_switch {0};
    int stop_switch {0};
    unsigned int display_time;

    QPixmap scale_phtot(QPixmap pix_photo);
};

#endif // MAINWINDOW_H

