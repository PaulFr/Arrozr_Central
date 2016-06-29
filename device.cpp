#include "device.h"


Device::Device(QString port, QObject *parent) :QThread(parent), _port(port), _id(0)
{

}

Device::~Device(){

}

void Device::run(){
    QSerialPort serial;
    serial.setPortName(this->getPort());
    serial.setBaudRate(QSerialPort::Baud9600);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setDataBits(QSerialPort::Data8);
    serial.setFlowControl(QSerialPort::NoFlowControl);

    serial.open(QIODevice::ReadWrite);

    while(serial.isOpen()){
        qDebug() << "boucle" << isModule() << _id;
        if(!isModule()){
            qDebug() << "Not approuved module";

            QByteArray authCommand;
            authCommand.resize(1);
            authCommand[0] = 0x01;
            serial.write(authCommand);
            serial.flush();
            //serial.waitForBytesWritten(1000);

            if(serial.waitForReadyRead(10)){
                if(serial.bytesAvailable() >= 2){
                    QByteArray ba = serial.readAll();
                    if(ba.at(0) == 0x01){
                        this->setId(ba.at(1));
                        emit authed(this);
                    }
                    return;
                    qDebug()<<"read:"<<ba ;
                }
            }
        }
        this->msleep(500);
    }


}

void Device::setId(int id){
    _id = id;
}

void Device::setName(QString name){
    _name = name;
}

int Device::getId(){
    return _id;
}

QString Device::getName(){
    return _name;
}

QString Device::getPort() const{
    return _port;
}

bool Device::isModule(){
    return _id > 0;
}
