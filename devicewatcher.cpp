#include "devicewatcher.h"

#include <QSerialPortInfo>
#include <QDebug>
#include <iostream>
#include <QSet>

QT_USE_NAMESPACE

DeviceWatcher::DeviceWatcher(QObject *parent):QThread(parent)
{

}

DeviceWatcher::~DeviceWatcher(){

}

void DeviceWatcher::run(){
    oldDeviceList = new QStringList();
    connectedDevices = new QHash<QString, Device*>();

    while(1){
        deviceList = new QStringList();

        foreach (
                 const QSerialPortInfo &info, QSerialPortInfo::availablePorts()
                 ) {

            *deviceList << info.portName();


        }
        QSet<QString> newDevices = deviceList->toSet().subtract(oldDeviceList->toSet());
        QSet<QString> oldDevices = oldDeviceList->toSet().subtract(deviceList->toSet());

        foreach (const QString &device, oldDevices){
            emit disconnected(*connectedDevices->find(device));
            connectedDevices->remove(device);
         }

        foreach (const QString &device, newDevices){
            Device *newOne = new Device(device);
            newOne->start();
            connectedDevices->insert(device, newOne);

            emit connected(newOne);
        }


        *oldDeviceList = *deviceList;
        emit detected(deviceList);


        this->msleep(500);
    }
}

