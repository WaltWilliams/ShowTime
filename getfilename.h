#ifndef GETFILENAME_H
#define GETFILENAME_H

#include <QObject>
#include <QThread>
#include <QString>
//#include <QStringList>

class GetFilename : public QObject
{
    Q_OBJECT

public:
    GetFilename();//QObject *parent
    ~GetFilename();

    void build_obj(QThread &fn_thread);
    void object_initial_values(int *stop_switch, int *pause_switch, unsigned int pause_amount, QStringList file_list);


signals:
    void filename_signal(QString);

public slots:
    void get_filename();

private:
    QThread *w_fn_thread;
    QString filename;
    QStringList w_file_list;
    unsigned int w_pause_amount;
    int *w_pause_switch;
    int *w_stop_switch;
};

#endif // GETFILENAME_H
