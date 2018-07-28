#ifndef PORTREADER_H
#define PORTREADER_H

#include <QObject>
#include <QThread>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QString>
#include <QTextCodec>
#include <QtDebug>

#include "pinbutton.h"

class WorkerThread : public QThread
{
    Q_OBJECT
    QSerialPort* _port;

    void run() override {
        _port->open(QIODevice::ReadOnly);
        QByteArray buffer;
        while (true) {
            while (_port->waitForReadyRead(5000))
            {
                QByteArray readData = _port->readLine(256);
                qDebug()<<readData;
                emit signalRecieved(readData[0]-'A', readData[1]-'0');
            }
        }
    }

public:
    WorkerThread(QSerialPort* port) { _port = port; }

signals:
    void signalRecieved(int pin, bool state);
};


class PortReader : public QObject
{
    Q_OBJECT

private:
    QMap<int, PinButton*> _pinLookup;
    QSerialPort* _port;
    WorkerThread* _worker;

public:
    PortReader(QString portName="auto", int portRate=9600);
    ~PortReader();
    static QList<QString> getPorts();
    void connectPin(PinButton* pin, int number);

signals:

public slots:
    void pinStageChanged(int button, bool newState);

};

#endif // PORTREADER_H
