#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QMessageBox>
#include <QCheckBox>
#include <QVariant>
#include <QDebug>
#include <iostream>
#include <QStringListModel>
#include "devicewatcher.h"


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
    QStringListModel model;
    unsigned char ToByte(bool b[8]);
    void afficheBytes(unsigned char bytes[42]);

private slots:
    void selectedItems();

public slots:
    void updateDevices(QStringList *deviceList);
    void selectDevice(QModelIndex index);
};

#endif // MAINWINDOW_H
