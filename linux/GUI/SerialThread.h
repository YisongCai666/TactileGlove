#pragma once

#include "GloveWidget.h"

#include <QObject>
#include <QThread>
#include <QMutex>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define SLIDING_AVG 2
#define PACKET_SIZE_BYTES 5

class SerialThread : public QThread
{
    Q_OBJECT
public:
    SerialThread();
    bool connect_device (const char* device);
    bool disconnect_device ();
    void endthread();
    bool get_full_frame (unsigned short* full_frame);
    void get_sensor_data (unsigned short* sensor_frame);
signals:
    void read_frame(unsigned short*);
    void full_frame_update_message (QString q);
public slots:
    void enable_send ();
protected:
    void run();
private:
    struct termios oldtio,newtio;
    unsigned char buf[PACKET_SIZE_BYTES];
    int fd;
    bool connected;
    bool send_update;
    bool keep_going;
    unsigned short sensor_data[NO_TAXELS];
    unsigned short full_frame_sensor_data[SLIDING_AVG][NO_TAXELS];
    unsigned short* slot_frame;
    unsigned long int full_frames_counter;
    bool check_packet();
	 void recover();
    void update_field();
    unsigned int next_index ( int i);
    bool full_frame;
    bool enable_sliding_average;
    unsigned int index_sliding_average;
    QMutex sensor_data_mutex;
    QMutex full_frame_sensor_data_mutex;
};