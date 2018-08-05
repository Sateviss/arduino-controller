#include <QDebug>
#include <QColor>
#include <QFile>

#include "mainwindow.h"
#include "pinbutton.h"

int PinButton::getPinNumber() { return _pinNumber; }

void PinButton::setScript(const QString &script) {
    _script = script;

    QFile file("/tmp/"+_pinName+".py");
    if (file.exists())
        file.remove();
    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );
        stream << "#! ../venv/bin/python3\n\n" << _script << endl;
    }
}

void PinButton::runScript()
{
    _terminalOutput->runCommand("./venv/bin/python3", QStringList() << "./tmp/"+_pinName+".py");
}

int PinButton::getActOnId() const { return _actOnId; }

void PinButton::setActOnId(int actOnId) { _actOnId = actOnId; }

int PinButton::getActionTypeId() const { return _actionTypeId; }

void PinButton::setActionTypeId(int actionTypeId) { _actionTypeId = actionTypeId; }

QString PinButton::getPinName() { return _pinName; }

PinButton::PinButton(QWidget* parent) : QPushButton (parent) {}

void PinButton::newPinState(bool newState)
{
    this->runScript();
}

void PinButton::Init(ProcessRunner *output, QMainWindow *window)
{
    connect(this, &PinButton::pinSelected, (MainWindow*)window, &MainWindow::pinSelected);
    _terminalOutput = output;
    _pinName = this->objectName().mid(9);
    _pinNumber = _pinName.mid(1).toInt();
}

void PinButton::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::MouseButton::LeftButton)
    {
        emit pinSelected(this);
    }
    QPushButton::mousePressEvent(e);
    e->accept();
}
