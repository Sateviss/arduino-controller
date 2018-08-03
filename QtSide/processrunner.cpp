#include "processrunner.h"

ProcessRunner::ProcessRunner(QWidget *parent) : QTextBrowser (parent)
{
    _mutex = new QMutex();
    _processPool = new QList<QProcess*>();
    _monitor = new StdoutMonitor(this, _processPool, _mutex);
    _monitor->start();
}

ProcessRunner::~ProcessRunner()
{
    _monitor->exit();
    _monitor->deleteLater();
    while (_processPool->length())
    {
        _processPool->removeFirst();
    }

}

void ProcessRunner::runCommand(QString command, QStringList args)
{
    auto newProcess = new QProcess();
    newProcess->setProgram(command);
    newProcess->setArguments(args);
    newProcess->start();
    _mutex->lock();
    _processPool->append(newProcess);
    _mutex->unlock();
}

