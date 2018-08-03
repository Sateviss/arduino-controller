#include "portreader.h"

PortReader::PortReader(QString portName, int portRate)
{
    if (portName == "auto")
        portName = PortReader::getPorts()[0];

    WorkerThread* _worker = new WorkerThread(portName, portRate);
    connect(_worker, &WorkerThread::signalRecieved, this, &PortReader::pinStageChanged);
    _worker->start();
}

QList<QString> PortReader::getPorts()
{
    auto ports = QSerialPortInfo::availablePorts();
    QList<QString> names;
    for (auto port : ports)
        names.append(port.portName());
    return names;
}

PortReader::~PortReader()
{
    _worker->exit();
    _worker->deleteLater();
}

void PortReader::pinStageChanged(int pin, bool newState)
{
    _pinLookup[pin]->newPinState(newState);
}

void PortReader::connectPin(PinButton* pin, int pinNumber)
{
    _pinLookup[pinNumber] = pin;
}
