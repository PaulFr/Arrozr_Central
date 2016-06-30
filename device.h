#ifndef DEVICE_H
#define DEVICE_H

#include <QString>
#include <QThread>
#include <QSerialPort>
#include <QtSerialPort/QtSerialPort>
#include <QSettings>

class Device : public QThread
{
    Q_OBJECT
public:
    Device(QString port, QObject *parent=0);
    ~Device();
    virtual void run();

    void setName(QString name);
    void setId(int id);
    void setSeuil(int seuil);
    void setSchedule(QByteArray schedule);
    void saveSettings();
    void refresh();
    void arrose();

    QString getName();
    QString getPort() const;
    int getId();
    int getSeuil();
    QByteArray getSchedule();
    bool isModule();

signals:
    void authed(Device *device);

private:
    void readSettings();
    void processRequest(char *command, char *buffer, QSerialPort &serial);
    QString _port;
    QString _name;
    int _id;
    int _seuil;
    QByteArray _schedule;
    char *lastCommand;
    char askCommand;


};

#endif // DEVICE_H
