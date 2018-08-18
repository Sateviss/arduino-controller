#include "portreader.h"

PortReader::PortReader(QString& portName, int portRate) {
    if (portName == "")
        portName = PortReader::getPorts()[0];

    WorkerThread* _worker = new WorkerThread(portName, portRate);
    connect(_worker, &WorkerThread::signalRecieved, this, &PortReader::pinStageChanged);
    connect(this, &PortReader::killWorker, _worker, &WorkerThread::kill);
    _worker->start();
}

QList<QString> PortReader::getPorts() {
    auto ports = QSerialPortInfo::availablePorts();
    QList<QString> names;
    for (auto port : ports)
        names.append(port.portName());
    return names;
}

PortReader::~PortReader() {
    emit killWorker();
}

void PortReader::pinStageChanged(int pin, bool newState) {
    if (_pinLookup.keys().contains(pin))
        _pinLookup[pin]->newPinState(newState);
}

void PortReader::connectPin(PinButton* pin, int pinNumber)
{
    _pinLookup[pinNumber] = pin;
}
