#include "device.h"


Device::Device(QString port, QObject *parent) :QThread(parent), _port(port), _id(0), _seuil(0), _schedule({}), _sprinkling(false), _humidity(0)
{
    _schedule.resize(42);
    _schedule.fill(0,42);
    lastCommand = new char;
    *lastCommand = 0x00;
    askCommand = 0x00;
    _lastSprinkle = "dernièrement...";
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

    serial = new QSerialPort();
    serial->setPortName(this->getPort());
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setDataBits(QSerialPort::Data8);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    serial->open(QIODevice::ReadWrite);

    while(serial->isOpen()){
        serial->setDataTerminalReady(true);
        //qDebug() << "boucle" << isModule() << _id;
        //Si le module n'est pas un arroseur reconnu, on lui demande
        if(!isModule()){
            //qDebug() << "Not approuved module";

            QByteArray authCommand;
            authCommand.resize(1);
            authCommand[0] = 0x01;
            serial->write(authCommand);
            serial->flush();
            //serial->waitForBytesWritten(1000);


        }

        if(askCommand != 0x00){
            if(askCommand == 0x02){
                sendConf();
            }else{
                serial->write(&askCommand);
                serial->flush();
                qDebug() << QString("Ask : %1").arg(askCommand);
            }
            askCommand = 0x00;
        }

        QByteArray requestData = serial->readAll();
        while (serial->waitForReadyRead(10)){
            requestData += serial->readAll();
        }
        readData(requestData);


        this->msleep(500);
    }


}

void Device::readData(QByteArray requestData){
    buffer += requestData;
    QDataStream stream(buffer);


    if(buffer.size() > 0){


            char *data = new char[4];
            int requiredBytes = 0;

            if(*lastCommand == 0x00){
                stream.readRawData(lastCommand, 1);
                buffer.remove(0,1);
            }

            if(*lastCommand == 0x01){
                requiredBytes = 1;
            }else if(*lastCommand == 0x02){
                requiredBytes = 0;
            }else if(*lastCommand == 0x03){
                requiredBytes = 2;
            }else if(*lastCommand == 0x04){
                requiredBytes = 0;
            }
            qDebug()<< QString("Commande %1 en attente de %2 octets : %2/%3").arg(*lastCommand).arg(requiredBytes).arg(buffer.size()) << endl;

            if(buffer.size() >= requiredBytes){
                if(requiredBytes > 0){
                    stream.readRawData(data, requiredBytes);
                    buffer.remove(0,requiredBytes);
                }
                processRequest(lastCommand, data);
                *lastCommand = 0x00;
            }


            //qDebug()<<"read:"<<ba ;

    }
}

QString Device::getLastSprinkle(){
    return _lastSprinkle;
}

void Device::processRequest(char *command, char *buffer){
    if(*command == 0x01 && !isModule()){
        qDebug() << "Auth";
       this->setId(buffer[0]);
       emit authed(this);
       readSettings();

    }else if(*command == 0x02){
        qDebug() << "Demande EDT";
        sendConf();

    }else if(*command == 0x03){
        qDebug() << "Reception état";
        _humidity = buffer[0];
        _sprinkling = buffer[1] == 0x01;
        if(_sprinkling){
            _lastSprinkle = QDateTime::currentDateTime().toString("dd/MM/yyyy, HH:mm:ss");
        }
        emit update();
    }
}

void Device::sendConf(){
    qDebug() << "Envoi de la conf";
    QByteArray scheduleCommand;
    scheduleCommand.resize(44);
    scheduleCommand[0] = 0x02;
    for(int i =0; i < _schedule.count(); i++){
      scheduleCommand[i+1] = _schedule[i];
    }
    scheduleCommand[43] = _seuil;
    serial->write(scheduleCommand);
    serial->flush();
}

void Device::askSendConf(){
    askCommand = 0x02;
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

int Device::getHumidity(){
    return _humidity;
}

bool Device::isSprinkling(){
    return _sprinkling;
}

QByteArray Device::getSchedule(){
    return _schedule;
}

bool Device::isModule(){
    return _id > 0;
}
