/****************************************************************************
*
Copyright (C) 20016-2017 ferdl <ferdl@online.de>
All rights reserved.

This file is part of usb-svc (usb-sound-volume-control).

usb-svc is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or (at
your option) any later version.

usb-svc is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with usb-svc; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA
or see <http://www.gnu.org/licenses/>.
*
****************************************************************************/

#include "hypex.h"

hypex::hypex()
{
    devices = 0;

    RXtimer = new QTimer(this);
    connect(RXtimer, SIGNAL(timeout()), this, SLOT(rxHandler()));

    on_usbScan();

    initArrays();

    on_ConnectAndSendInit(HypexNummer1);
}

hypex::~hypex()
{

}

void hypex::initArrays()
{
    for(int i=0; i < 32; i++){
        hypex_left.outVolumeBuffer[i]  = 0;
        hypex_right.outVolumeBuffer[i] = 0;
    }

    for(int i=0; i < 32; i++){
        outBufferInit[i] = 0;
        inBufferInit[i] = 0;
    }

    outBufferInit[0] = 0x06;
    outBufferInit[1] = 0x02;

    //hypex_left.connected_device = NULL;
    //hypex_right.connected_device = NULL;

    qDebug("Init Arrays done!");
}

//##############################################################################
void hypex::on_usbScan()
{
    struct hid_device_info *cur_dev;
    QString s;

    if(devices)hid_free_enumeration(devices);
    devices = hid_enumerate(0x0, 0x0);
    cur_dev = devices;

    iDevice = 1;
    while (cur_dev) {
        s  = QString("%1").arg(iDevice,0,16);
        s += ": ";
        s += QString("%1").arg(cur_dev->vendor_id,4,16);
        s += "  ";
        s += QString("%1").arg(cur_dev->product_id,4,16);
        s += "  ";
        s += QString::fromWCharArray(cur_dev->manufacturer_string);
        s += "  ";
        s += QString::fromWCharArray(cur_dev->product_string);
        s += "\n";

        if(HypexNummer1 == -1 && cur_dev->vendor_id == hypex_vendor_id){

            HypexNummer1 = iDevice;
            hypexlist_qml.append("Hypex 1: ");
            hypexlist_qml += QString("HID %1  ").arg(iDevice,0,16);
            hypexlist_qml.append("Path: ");
            hypexlist_qml.append(cur_dev->path);
            hypexlist_qml.append("\n-----------\n");
        }
        else if(HypexNummer2 == -1 && cur_dev->vendor_id == hypex_vendor_id){

            HypexNummer2 = iDevice;
            hypexlist_qml.append("Hypex 2: ");
            hypexlist_qml += QString("HID %1  ").arg(iDevice,0,16);
            hypexlist_qml.append("Path: ");
            hypexlist_qml.append(cur_dev->path);
        }
        usblist.append(s);
        iDevice++;
        countDevices = iDevice;
        cur_dev = cur_dev->next;
    }
}

void hypex::on_Disconnect()
{
    RXtimer->stop();
    hid_close(connected_device);
    connected_device = NULL;

    /*
    if(hypex_left.connected_device != NULL){
        hid_close(hypex_left.connected_device);
        hypex_left.connected_device = NULL;
    }
    if(hypex_right.connected_device != NULL){
        hid_close(hypex_right.connected_device);
        hypex_right.connected_device = NULL;
    }
    */
}

void hypex::on_ConnectAndSendInit(int HypexNr)
{
    struct hid_device_info *cur_dev;
    QString s;
    int connectToDev = -1;

    connectToDev = HypexNr;

    if(connectToDev == -1){
        qDebug() << "Device not valid!";
        return;
    }

    hid_free_enumeration(devices);
    devices = hid_enumerate(0x0, 0x0);
    cur_dev = devices;

    while (cur_dev){
        for (iDevice = 1 ; iDevice <= countDevices; iDevice++){
            if( iDevice == connectToDev){
                connected_device = hid_open_path(cur_dev->path);
                qDebug() << "HID device error: " << hid_error(connected_device);
                aktuelleHypexHID = HypexNr;
                aktuellerDevPath = cur_dev->path;
                hid_set_nonblocking(connected_device,1);
                cur_dev = NULL;
                iDevice = countDevices;
                RXtimer->start(50);
                io_result = hid_write(connected_device,outBufferInit,32);
                daten_gesendet = true;
            }
            else{
                if (cur_dev != NULL)
                    cur_dev = cur_dev->next;
            }
        }
    }
}


void hypex::on_ConnectAndSend()
{    
    struct hid_device_info *cur_dev;
    QString s;

    int numberOfDevices = 2;

    if(hypex_left.HID_device_number==-1 || hypex_right.HID_device_number==-1){
        qDebug("HID Device Numbers not ready!");
        return;
    }

    if(hypexvolume <= MAX_VOLUME && hypexvolume >= MIN_VOLUME){

        hypex_left.outVolumeBuffer[ID_VOLUME] = hypexvolume;
        hypex_right.outVolumeBuffer[ID_VOLUME] = hypexvolume;
    }

    hid_free_enumeration(devices);
    devices = hid_enumerate(0x0, 0x0);
    cur_dev = devices;

    while (cur_dev){
        for (iDevice = 1 ; iDevice <= countDevices; iDevice++){

            if( iDevice == hypex_left.HID_device_number){

                connected_device = hid_open_path(hypex_left.dev_path.toStdString().c_str());
                hid_set_nonblocking(connected_device,1);
                io_result = hid_write(connected_device,hypex_left.outVolumeBuffer,32);
                printDatenFrame("Links gesendet",hypex_left.outVolumeBuffer);
                daten_gesendet = true;
                numberOfDevices--;
                qDebug() << "HID device error: " << hypex_left.HID_device_number << hid_error(connected_device);
                hid_close(connected_device);
                connected_device = NULL;
            }
            else if( iDevice == hypex_right.HID_device_number){

                connected_device = hid_open_path(hypex_right.dev_path.toStdString().c_str());
                hid_set_nonblocking(connected_device,1);
                io_result = hid_write(connected_device,hypex_right.outVolumeBuffer,32);
                printDatenFrame("Rechts gesendet",hypex_right.outVolumeBuffer);
                daten_gesendet = true;
                numberOfDevices--;
                qDebug() << "HID device error: " << hypex_right.HID_device_number << hid_error(connected_device);
                hid_close(connected_device);
                connected_device = NULL;
                //RXtimer->start(100);
            }
            else{
                if (cur_dev != NULL)
                    cur_dev = cur_dev->next;
            }
            if(numberOfDevices <= 0){
                cur_dev = NULL;
                iDevice = countDevices;
            }
        }
    }    
}

//##############################################################################
void hypex::rxHandler()
{
    static int errorcount = 0;

    //if(daten_gesendet && hypex_left.connected_device != NULL && hypex_right.connected_device != NULL){

        //io_result = hid_read(hypex_left.connected_device, hypex_left.inBuffer, 32);
        //io_result = hid_read(hypex_right.connected_device, hypex_right.inBuffer, 32);
        //on_Disconnect();
        //daten_gesendet = false;
    //}
    if(daten_gesendet && (!hypex_left.initdone || !hypex_right.initdone)){
        io_result = hid_read(connected_device, inBufferInit, 32);
        daten_gesendet = false;
    }

    if (io_result == 32){

        if(!hypex_left.initdone || !hypex_right.initdone){

            printDatenFrame("Income",inBufferInit);

            switch (inBufferInit[ID_LEFTRIGHT])
            {
            case LEFT:  for(int i = 0; i < 32;i++){
                    hypex_left.outVolumeBuffer[i] = inBufferInit[i];
                }
                hypex_left.outVolumeBuffer[ID_INPUT] = IN_AUTO;
                hypex_left.outVolumeBuffer[ID_LEFTRIGHT] = LEFT;
                hypex_left.outVolumeBuffer[ID_FILTER] = 0x00;
                hypex_left.outVolumeBuffer[31] = 0x00;
                hypexvolume = inBufferInit[ID_VOLUME];                  // einmal Zentrale Lautstärke laden
                hypex_left.initdone=true;
                hypex_left.HID_device_number = aktuelleHypexHID;        // wichtig um linken und rechten Kanal zu identifizieren
                hypex_left.dev_path = aktuellerDevPath;
                printDatenFrame("Left Out",hypex_left.outVolumeBuffer);
                qDebug("Left init done!");
                errorcount++;
                break;
            case RIGHT: for(int i = 0; i < 32;i++){
                    hypex_right.outVolumeBuffer[i] = inBufferInit[i];
                }
                hypex_right.outVolumeBuffer[ID_INPUT] = IN_AUTO;
                hypex_right.outVolumeBuffer[ID_LEFTRIGHT] = RIGHT;
                hypex_right.outVolumeBuffer[ID_FILTER] = 0x00;
                hypex_right.outVolumeBuffer[31] = 0x00;
                hypex_right.initdone=true;
                hypex_right.HID_device_number = aktuelleHypexHID;
                hypex_right.dev_path = aktuellerDevPath;
                printDatenFrame("Right Out",hypex_right.outVolumeBuffer);
                qDebug("Right init done!");
                errorcount++;
                break;
            default: break;
            }
            on_Disconnect();
            on_ConnectAndSendInit(HypexNummer2);
            return;
        }
        else{
            on_Disconnect();
            qDebug() << "No Data!";
        }
    }
    on_Disconnect();
    if(errorcount>20) qDebug("Can not connect to to Hypex!");
}

void hypex::printDatenFrame(QString text, unsigned char *buffer)
{
    QString s;

    s.append(text);
    s += "\n  ";
    for (int i = 0; i < 32;i++){
        s += QString("0x%1 ").arg(buffer[i],2,16,QChar('0'));
        if(i==15) s += "\n  ";
    }
    s += "\n";
    qDebug() << s;
}
