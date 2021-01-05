#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <unistd.h>

//#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setWindowTitle(program_name);

    // Turn off the slideshow buttons
    ui->actionTBstart->setEnabled(false);
    ui->actionTBpause->setEnabled(false);
    ui->actionTBstop->setEnabled(false);

    // Set label background to black.
    ui->photoLabel->setStyleSheet("QLabel{ background-color : black; color : white; }");

    // Starting photo
    start_filename = ":icons/Hesperus_Mountain.jpg";
    current_pix.load(start_filename);
    ui->photoLabel->setPixmap(current_pix);

    // Temporarly hide the planned Settings menu selection.
    ui->actionSettings->setVisible(false);

    // The delay inbetween slides. This will end up being
    // adjustable on a "Setting" dialog.
    display_time = 3;

}


MainWindow::~MainWindow() {
    if(filename_thread.isRunning()){
        stop_switch = 1;
        filename_thread.terminate();
        filename_thread.wait();
    }
    delete ui;
}



void MainWindow::resizeEvent(QResizeEvent* event)
{
    if((w_filename != start_filename) && (w_filename != "")){
        current_pix.load(directory_files.path() + QDir::separator() + w_filename);
    }
    else {
        current_pix.load(start_filename);
    }
    // As the QLabel, where the photo resides, is bound inside the QScrollArea grabbing
    // the x and y sizes of the QScrollArea as the window is resized and then subtracting
    // 5 from each so the photo just fits inside QLabel without activating the scroll bars.
    current_pix = scale_phtot(current_pix);
    ui->photoLabel->setPixmap(current_pix);
    QMainWindow::resizeEvent(event);
}


QPixmap MainWindow::scale_phtot(QPixmap pix_photo){
    int sx = ui->photoScrollArea->width();
    int sy = ui->photoScrollArea->height();

    pix_photo = pix_photo.scaled(sx-25, sy-25, Qt::KeepAspectRatio, Qt::FastTransformation);
    return pix_photo;
}


// Slots
// ===============================================================================================================
void MainWindow::on_receivingFilename(QString filename){
    // Hand off the filename to a local variable for use in the resizeEvent slot (above).
    w_filename = filename;
    current_pix.load(directory_files.path() + QDir::separator() + filename);
    QThread::msleep(50); // A slight pause the give .load() above a chance to finish. 1/20 of a second.
    current_pix = scale_phtot(current_pix);
    ui->statusBar->showMessage(filename);
    ui->photoLabel->setPixmap(current_pix);
}


void MainWindow::on_action_open_triggered() {
    // This stop_switch passes a 3 into the thread via pointer.
    // So it happens in real time. It causes the file_list
    // inside the thread to be cleared in preparation for a new list of filenames.
    stop_switch = 3;


    // This is so the last selected directory path is preserved. If photo_path is empty
    // it will be set to the home directory.
    if(photo_path.count() ==0){
        photo_path = QDir::homePath();
    }
    photo_path = QFileDialog::getExistingDirectory(nullptr, ("Select A Directory"), photo_path);

    // ----------------------------------------------------------------------------------
    // A couple simple test to insure the program doesn't crash.
    if(photo_path != "") {
        setWindowTitle(program_name + " " + QChar(0x2014) + " Current Directory: " + photo_path);
    }
    else {
        setWindowTitle(program_name);
        file_list.clear();
        ui->actionTBstart->setEnabled(false);
        ui->statusBar->showMessage("Please select a directory that contains photos.");
    }


    // ----------------------------------------------------------------------------------
    // This cleans out the filenames in the file_list when selecting another (different) directory.
    // A pointer to this variable is passed into the thread. So what happens here happens there instantly.
    file_list.clear();

    QStringList fileFilterList = {"*.jpg", "*.JPG", "*.png", "*.PNG", "*.bmp", "*.BMP"};

    directory_files.setPath(photo_path); // QDir object.
    directory_files.setNameFilters(fileFilterList);
    file_list = directory_files.entryList(); // QDir entryList() returns a QStringList.

    // The start button on the tool bar is disabled when selecting different directory.
    // This is to ensure that if you select a directory that has NO photos that the
    // start button cannot be clicked. Thou shalt not attempt to display non-existent
    // photos. CRASH!
    ui->actionTBstart->setEnabled(false);
    if(file_list.count() > 0) {
        ui->actionTBstart->setEnabled(true);
        ui->statusBar->showMessage("Click Start (>) when ready");
    }

    // Testing to see if the file_list has any file names in before proceeding and enabling the play button.
    if(file_list.size() == 0 && photo_path != "") {
        ui->statusBar->showMessage(tr("There are no photos in this directory."));
    }

    // Retreiving a filename from the QStringList of file names.
    // This is done by instantiating the GetFilename class and running
    // it in a thread. Each time it emits a another filename it
    // triggers the on_receiving_filename(QString) function (just above)
    // which loads the QLabel with a photo.
    GetFilename *get_the_filename = new GetFilename();
    get_the_filename->build_obj(filename_thread);
    get_the_filename->object_initial_values(&stop_switch, &pause_switch, &display_time, &file_list);
    get_the_filename->moveToThread(&filename_thread);
    QObject::connect(get_the_filename, SIGNAL(filename_signal(QString)), this, SLOT(on_receivingFilename(QString)));

}

// ===============================================================================================================


void MainWindow::on_actionTBstart_triggered()
{
    ui->action_open->setEnabled(false);
    ui->actionTBstart->setEnabled(false);
    ui->actionTBpause->setEnabled(true);
    ui->actionTBstop->setEnabled(true);

    //   Grab image file name and path and insert it into the QPixMap below.
    //===================================================
    pause_switch = 0;
    // filename_thread.isRunning() doesn't work in this if statement.
    // Thats the reason for the thread_is_started variable.
    if(thread_is_started == 0){
        thread_is_started = 1;
        stop_switch = 0;
        filename_thread.start();
    }
}


void MainWindow::on_actionTBpause_triggered()
{
    ui->action_open->setEnabled(false);
    ui->actionTBstart->setEnabled(true);
    ui->actionTBpause->setEnabled(false);
    ui->actionTBstop->setEnabled(true);

    pause_switch = 1; // 1 = true
}


void MainWindow::on_actionTBstop_triggered()
{
    ui->action_open->setEnabled(true);
    ui->actionTBstart->setEnabled(true);
    ui->actionTBpause->setEnabled(false);
    ui->actionTBstop->setEnabled(false);

    thread_is_started = 0;  // 0 = false
    // Pause and resets the index inside the thread to 0 (zero)
    // This leaves the thread still running.
    pause_switch = 1; // 1 = true
    stop_switch = 2;

    // A complete stop of the thread is performed upon exiting the program.
}


void MainWindow::on_actionExit_triggered() {
    pause_switch = 0;
    stop_switch = 1;
    filename_thread.terminate();
    filename_thread.wait();
    QApplication::quit();
}
