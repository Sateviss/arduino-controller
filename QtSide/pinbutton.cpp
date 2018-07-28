#include "pinbutton.h"
#include <QDebug>

int PinButton::getPinNumber() const { return _pinNumber; }

void PinButton::setScript(const QString &script) { _script = script; }

int PinButton::getActOnId() const { return _actOnId; }

void PinButton::setActOnId(int actOnId) { _actOnId = actOnId; }

int PinButton::getActionTypeId() const { return _actionTypeId; }

void PinButton::setActionTypeId(int actionTypeId) { _actionTypeId = actionTypeId; }

QString PinButton::getPinName() const { return _pinName; }

PinButton::PinButton(QWidget* parent) : QPushButton (parent) {}

void PinButton::newPinState(bool newState)
{
    qDebug() << "Hello from pin "<<this->_pinNumber<<"! I am now "<<(newState?"HIGH":"LOW");
}
