#include "getfilename.h"

GetFilename::GetFilename(){

}

GetFilename::~GetFilename(){
    w_fn_thread->quit();
}

void GetFilename::build_obj(QThread &fn_thread){
    w_fn_thread = &fn_thread;
    QObject::connect(&fn_thread, SIGNAL(started()), this, SLOT(get_filename()));
}

void GetFilename::object_initial_values(int *stop_switch, int *pause_switch, unsigned int pause_amount, QStringList file_list){
    w_file_list = file_list;
    w_pause_switch = pause_switch;
    w_pause_amount = pause_amount;
    w_stop_switch = stop_switch;
}

// A slot
void GetFilename::get_filename(){
    int i = 0;
    while(*w_stop_switch == 0) {
        while(*w_pause_switch == 0) {
            if(*w_stop_switch == 1){
                break;
            }
            filename = w_file_list.at(i);
            i++;
            emit filename_signal(filename);
            if(i == w_file_list.count()-1){
                i = 0;
            }
            QThread::sleep(w_pause_amount);
        }

        while (*w_pause_switch == 1) {
            if(*w_stop_switch == 1){
                break;
            }
            QThread::msleep(250);
        }
    }
}
