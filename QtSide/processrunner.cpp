#include "processrunner.h"

ProcessRunner::ProcessRunner(QWidget *parent) : QTextBrowser (parent)
{
    setTextColor(Qt::gray);
    _monitor = new StdoutMonitor(this);
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

void ProcessRunner::addText(QColor color, QString text) {
    auto p_color = textColor();
    setTextColor(color);
    append(text);
    setTextColor(p_color);
}
