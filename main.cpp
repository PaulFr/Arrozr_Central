#include "mainwindow.h"
#include <QApplication>
#include <QCoreApplication>
#include<QSerialPort>
#include <QtSerialPort/QtSerialPort>
#include<QDebug>
#include<iostream>
#include<QSerialPortInfo>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();


        /*QCoreApplication a(argc, argv);
        foreach (
                 const QSerialPortInfo &info, QSerialPortInfo::availablePorts()
                 ) {
            qDebug() << "Name        : " << info.portName();
            qDebug() << "Description : " << info.description();
            qDebug() << "Manufacturer: " << info.manufacturer();

            // Example use QSerialPort
            QSerialPort serial;
            serial.setPort(info);
            serial.setBaudRate(QSerialPort::Baud9600);
            if (serial.open(QIODevice::ReadWrite)){
                qDebug()<<serial.isOpen();

                while(1){
                    if(serial.waitForReadyRead(10)){
                        QByteArray ba = serial.readAll();


                        qDebug()<<ba ;
                    }
                }
                serial.close();
            }
        }*/
        return a.exec();

}
