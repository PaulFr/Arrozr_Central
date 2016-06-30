#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QMessageBox>
#include <QCheckBox>
#include <QVariant>
#include <QDebug>
#include <QHash>
#include <iostream>
#include <QListWidgetItem>
#include <QStringListModel>
#include "devicewatcher.h"
#include "device.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    DeviceWatcher watcher;
    QHash<int, Device*> devices;
    QHash<int, QListWidgetItem*> devicesItems;
    Device *selectedDevice;
    char ToByte(bool b[8]);
    void afficheBytes(char bytes[42]);
    void setListeners();
    void createTimetable();
    void setTimetableStyle();
    void enableUi();
    void disableUi();
    void fillTimetable();
    void refreshUi();

private slots:
    void selectedItems();

public slots:
    void addDevice(Device *device);
    void removeDevice(Device *device);
    void selectDevice(QModelIndex index);
    void askArrosage();
    void askRefresh();
    void refreshDevice();
    void changeName(QString name);
};

#endif // MAINWINDOW_H
