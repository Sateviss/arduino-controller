#include "processrunner.h"

ProcessRunner::ProcessRunner(QWidget *parent) : QTextBrowser (parent)
{
    setTextColor(Qt::gray);
    _monitor = new StdoutMonitor(this);
    connect(this, &ProcessRunner::runInSlaveInterpreter, _monitor, &StdoutMonitor::runInInterpreter);
    connect(this, &ProcessRunner::addCommandToSlave, _monitor, &StdoutMonitor::addProcess);
    connect(_monitor, &StdoutMonitor::outputText, this, &ProcessRunner::addText);
    _monitor->start();
}

ProcessRunner::~ProcessRunner() {
    _monitor->exit();
    _monitor->deleteLater();
}

void ProcessRunner::runCommand(QString command, QStringList args) {
    emit addCommandToSlave(command, args);
}

void ProcessRunner::runInterpeter(QString script) {
    emit runInSlaveInterpreter(script);
}

void ProcessRunner::addText(QColor color, QString text) {
    auto p_color = textColor();
    setTextColor(color);
    append(text);
    setTextColor(p_color);
}
