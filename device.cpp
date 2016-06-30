#include "device.h"


Device::Device(QString port, QObject *parent) :QThread(parent), _port(port), _id(0), _seuil(0), _schedule({})
{
    _schedule.resize(42);
    _schedule.fill(0);
    lastCommand = new char;
    *lastCommand = 0x00;
    askCommand = 0x00;
}

Device::~Device(){

}

void Device::readSettings(){
    QSettings settings;
    _seuil = settings.value(QString("module/%1/seuil").arg(getId()), 0).toInt();
    _name  = settings.value(QString("module/%1/name").arg(getId()), QString("Module %1").arg(getId())).toString();
    _schedule = settings.value(QString("module/%1/schedule").arg(getId()), 0).toByteArray();
}

void Device::saveSettings(){
    QSettings settings;
    settings.setValue(QString("module/%1/seuil").arg(getId()), getSeuil());
    settings.setValue(QString("module/%1/name").arg(getId()), getName());
    settings.setValue(QString("module/%1/schedule").arg(getId()), getSchedule());
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
        //qDebug() << "boucle" << isModule() << _id;
        //Si le module n'est pas un arroseur reconnu, on lui demande
        if(!isModule()){
            //qDebug() << "Not approuved module";

            QByteArray authCommand;
            authCommand.resize(1);
            authCommand[0] = 0x01;
            serial.write(authCommand);
            serial.flush();
            //serial.waitForBytesWritten(1000);


        }

        if(askCommand != 0x00){
            QByteArray command;
            command.resize(1);
            command[0] = askCommand;
            serial.write(command);
            serial.flush();
            qDebug() << "Ask : " << askCommand;
            askCommand = 0x00;
        }

        if(serial.waitForReadyRead(10)){
            if(serial.bytesAvailable() >= 1){
                //QByteArray ba = serial.readAll();

                char *buffer = new char[4];
                int requiredBytes = 0;

                if(*lastCommand == 0x00){
                    serial.read(lastCommand, 1);
                }

                if(*lastCommand == 0x01){
                    requiredBytes = 1;
                }else if(*lastCommand == 0x02){
                    requiredBytes = 0;
                }else if(*lastCommand == 0x03){
                    requiredBytes = 2;
                }else if(*lastCommand == 0x04){
                    requiredBytes = 2;
                }
                qDebug()<< QString("Commande %1 en attente de %2 octets").arg(*lastCommand).arg(requiredBytes) << endl;
                if(serial.bytesAvailable() >= requiredBytes){
                    if(requiredBytes > 0){
                        serial.read(buffer, requiredBytes);
                    }
                    processRequest(lastCommand, buffer, serial);
                    *lastCommand = 0x00;
                }


                //qDebug()<<"read:"<<ba ;
            }
        }
        this->msleep(500);
    }


}

void Device::processRequest(char *command, char *buffer, QSerialPort &serial){
    if(*command == 0x01){
        qDebug() << "Auth";
       this->setId(buffer[0]);
       emit authed(this);
       readSettings();

    }else if(*command == 0x02){
        qDebug() << "Demande EDT";
        QByteArray scheduleCommand;
        scheduleCommand.resize(44);
        scheduleCommand[0] = 0x02;
        scheduleCommand.fromHex(_schedule.toHex());
        scheduleCommand[43] = _seuil;
        serial.write(scheduleCommand);
        serial.waitForBytesWritten(100);
        serial.flush();
    }
}

void Device::refresh(){
    qDebug() << "refresh";
    askCommand = 0x03;
}

void Device::arrose(){
    qDebug() << "Arrosage";
    askCommand = 0x04;
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
