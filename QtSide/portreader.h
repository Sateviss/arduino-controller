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
    bool running;

    void run() override {
        // Start a loop that reads all incoming communication
        while (running)
            while (_port->waitForReadyRead(100)) {
                auto lines = QString::fromLocal8Bit(_port->readAll().data()).split("\n");
                // Log
                qDebug()<<lines;
                for (auto line : lines)
                    if (line.length())
                        emit signalRecieved(line[0].unicode()-'A', line[1].unicode()-'0');
        }
    }

public:    
    WorkerThread(QString& port_name, int rate) {
        _port = new QSerialPort(port_name);
        _port->setBaudRate(rate);
        _port->open(QIODevice::ReadOnly);
        while (_port->waitForReadyRead(100));
        _port->flush();
    }

signals:
    void signalRecieved(int pin, bool state);

public slots:
    void kill(){
        running = false;
        _port->close();
        _port->deleteLater();
        this->exit();
    }
};


class PortReader : public QObject
{
    Q_OBJECT

private:
    QMap<int, PinButton*> _pinLookup;
    WorkerThread* _worker;

public:
    PortReader(QString& portName, int portRate=9600);
    ~PortReader();
    static QList<QString> getPorts();
    void connectPin(PinButton* pin, int number);

signals:
    void killWorker();

public slots:
    void pinStageChanged(int button, bool newState);
};

#endif // PORTREADER_H
