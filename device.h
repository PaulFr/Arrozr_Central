#ifndef DEVICE_H
#define DEVICE_H

#include <QString>
#include <QThread>
#include <QSerialPort>
#include <QtSerialPort/QtSerialPort>
#include <QSettings>
#include <QDataStream>

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

    void readData(QByteArray requestData);

    QString getName();
    QString getPort() const;
    int getId();
    int getSeuil();
    int getHumidity();
    bool isSprinkling();
    QByteArray getSchedule();
    bool isModule();

signals:
    void authed(Device *device);
    void update();

private:
    void readSettings();
    void processRequest(char *command, char *buffer);
    QString _port;
    QString _name;
    int _id;
    int _seuil;
    QByteArray _schedule;
    bool _sprinkling;
    int _humidity;
    char *lastCommand;
    char askCommand;
    QByteArray buffer;
    QSerialPort *serial;

public slots:



};

#endif // DEVICE_H
