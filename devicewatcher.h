#ifndef DEVICEWATCHER_H
#define DEVICEWATCHER_H

#include <QObject>
#include <QThread>
#include <QStringList>
#include <QHash>
#include "device.h"

class DeviceWatcher : public QThread
{
    Q_OBJECT

public:
    DeviceWatcher(QObject *parent=0);
    ~DeviceWatcher();
    virtual void run();
signals:
    void detected(QStringList *deviceList);
    void disconnected(Device *device);
    void connected(Device *device);

private:
    QStringList *deviceList;
    QStringList *oldDeviceList;

    QHash<QString, Device*> *connectedDevices;

};

#endif // DEVICEWATCHER_H
