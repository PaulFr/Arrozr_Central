#ifndef DEVICE_H
#define DEVICE_H

#include <QString>
#include <QThread>
#include <QSerialPort>
#include <QtSerialPort/QtSerialPort>

class Device : public QThread
{
    Q_OBJECT
public:
    Device(QString port, QObject *parent=0);
    ~Device();
    virtual void run();

    void setName(QString name);
    void setId(int id);

    QString getName();
    QString getPort() const;
    int getId();
    bool isModule();

signals:
    void authed();

private:
    QString _port;
    QString _name;
    int _id;

};

#endif // DEVICE_H
