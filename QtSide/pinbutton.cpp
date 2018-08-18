#include "pinbutton.h"

int PinButton::getPinNumber() { return _pinNumber; }

void PinButton::setScript(const QString &script) {
    _script = script;

    QFile file(tr("tmp/%1.py").arg(_pinName));
    if (file.exists())
        file.remove();
    if (file.open(QIODevice::ReadWrite) ) {
        QTextStream stream( &file );
        stream << "#! ../venv/bin/python3\n\n" << _script << endl;
    }
}

QString PinButton::getScript() const { return _script; }

void PinButton::runScript()
{
    _terminalOutput->runCommand("./venv/bin/python3", QStringList() << "./tmp/"+_pinName+".py");
}

int PinButton::getActOnId() const { return _actOnId; }

void PinButton::setActOnId(int actOnId) { _actOnId = actOnId; }

int PinButton::getActionTypeId() const { return _actionTypeId; }

void PinButton::setActionTypeId(int actionTypeId) { _actionTypeId = actionTypeId; }

QString PinButton::getPinName() { return _pinName; }

QString PinButton::getSequence() const {
    return _sequence;
}

void PinButton::setSequence(const QString &sequence) {
    _sequence = sequence;
}

PinButton::PinButton(QWidget* parent) : QPushButton (parent) {
    _actOnId = 0;
    _actionTypeId = 0;
    _script = "";
}

void PinButton::newPinState(bool newState) {
    if (getActOnId() == 0 || (getActOnId()-1 == newState))
        this->runScript();
}

void PinButton::Init(ProcessRunner *output) {
    _terminalOutput = output;
    _pinName = this->objectName().mid(9);
    _pinNumber = _pinName.mid(1).toInt();
}

void PinButton::mousePressEvent(QMouseEvent *e) {
    if (e->button() == Qt::MouseButton::LeftButton)
        emit pinGotSelected(this);
    QPushButton::mousePressEvent(e);
    e->accept();
}
