#include "device.h"


Device::Device(QString port, QObject *parent) :QThread(parent), _port(port), _id(0), _seuil(0), _schedule({})
{
    _schedule.resize(42);
}

Device::~Device(){

}

void Device::readSettings(){
    QSettings settings;
    _seuil = settings.value(QString("module/%1/seuil").arg(getId()), 0).toInt();
    _name  = settings.value(QString("module/%1/name").arg(getId()), QString("Module %1 - "+getPort()).arg(getId())).toString();
    _schedule = settings.value(QString("module/%1/schedule").arg(getId()), 0).toByteArray();
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
        //Si le module n'est pas un arroseur reconnu, on lui demande
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

                    readSettings();

                    return;
                    qDebug()<<"read:"<<ba ;
                }
            }
        }else{
            //C'est bon, on sait que c'est bien un module à nous

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

void Device::setSeuil(int seuil){
    _seuil = seuil;
}

void Device::setSchedule(QByteArray schedule){
    _schedule = schedule;
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

int Device::getSeuil(){
    return _seuil;
}

QByteArray Device::getSchedule(){
    return _schedule;
}

bool Device::isModule(){
    return _id > 0;
}
