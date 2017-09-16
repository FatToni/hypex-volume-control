/****************************************************************************
*
Copyright (C) 20016-2017 ferdl <ferdl@online.de>
All rights reserved.

This file is part of hypex-volume (hvc) (hypex-volume-control).

hvc is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or (at
your option) any later version.

hvc is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with usb-svc; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA
or see <http://www.gnu.org/licenses/>.

Hypex is a seperate company and I have nothing to with it! Please see 
https://www.hypex.nl

*
****************************************************************************/

#ifndef HYPEX_H
#define HYPEX_H

#include <QObject>
#include <hidapi.h>
#include <QTimer>
#include <QDebug>

typedef enum {

    ID_INPUT = 1,       // Automatic: 0x00
                        // Analog:    0x01
                        // AES:       0x02
    ID_LEFTRIGHT = 3,   //  Right: 0x06
                        //  Left:  0x05
    ID_VOLUME = 8,
    ID_MUTE = 9,        //  OFF: 0x00
                        //  ON:  0x01
    ID_FILTER = 17      //  Filter Preset 1:  0x00
                        //  Filter Preset 2:  0x01
                        //  Filter Preset 3:  0x02
                        //  Filter Preset 4:  0x03

}COMMANDS;

#define LEFT  0x05
#define RIGHT 0x06
#define MUTE  0x01
#define UNMUTE 0x00
#define IN_AUTO   0x00
#define IN_ANALOG 0x01
#define IN_AES    0x02

#define MAX_VOLUME 232
#define MIN_VOLUME 196

class hypex : public QObject
{
    Q_OBJECT

private:

    int hypexvolume = 176;              // 176 = -80dB
                                        // 221 = -35dB
    bool hypexvolume_mute = false;

    struct hid_device_info *devices;
    hid_device *connected_device;    
    int iDevice;
    int countDevices;

    int aktuelleHypexHID;
    QString aktuellerDevPath;

    struct amp{

        //hid_device *connected_device;
        QString dev_path;

        bool initdone = false;

        unsigned char outVolumeBuffer[32];
        unsigned char inBuffer[32];
        int HID_device_number = -1;
    };
    amp hypex_left;
    amp hypex_right;


    unsigned char outBufferInit[32];    //outBuffer[0] is for endpoint number
    unsigned char inBufferInit[32];
    int io_result;

    QTimer *RXtimer;
    bool daten_gesendet = false;

    QString usblist;

    unsigned short const hypex_vendor_id = 1240;
    unsigned short const hypex_product_id = 65417;

    QString hypexlist_qml;

    int HypexNummer1 = -1;
    int HypexNummer2 = -1;

    void initArrays();

    void printDatenFrame(QString text, unsigned char *buffer);

private slots:

    void on_usbScan();
    void on_ConnectAndSendInit(int HypexNr);
    void on_Disconnect();
    void rxHandler();

    void on_ConnectAndSend();

public:
    hypex();
    ~hypex();

    Q_INVOKABLE int volume_up() {
        if(hypexvolume < MAX_VOLUME) hypexvolume = hypexvolume +1;
        on_ConnectAndSend();
        return hypexvolume-256;
    }

    Q_INVOKABLE int volume_down() {
        if(hypexvolume > MIN_VOLUME) hypexvolume = hypexvolume -1;
        on_ConnectAndSend();
        return hypexvolume-256;
    }

    Q_INVOKABLE bool volume_mute() {
        if(hypexvolume_mute){
            hypexvolume_mute = false;
        }
        else{
            hypexvolume_mute = true;
        }
        return hypexvolume_mute;
    }
    Q_INVOKABLE QString get_usblist() const{
        return usblist;
    }
    Q_INVOKABLE QString get_hypexlist() const{
        return hypexlist_qml;
    }


signals:

public slots:

};

#endif // HYPEX_H
