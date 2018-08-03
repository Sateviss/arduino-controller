#ifndef PROCESSRUNNER_H
#define PROCESSRUNNER_H

#include <QWidget>
#include <QTextBrowser>
#include <QProcess>
#include <QThread>
#include <QMutex>

class StdoutMonitor : public QThread
{
    Q_OBJECT
    QList<QProcess*> *_processPool;
    QTextBrowser *_output;
    QMutex *_mutex;

    void run() override {
        while (true) {
            for (auto i = 0; i < _processPool->length();)
            {
                _mutex->lock();
                auto process = _processPool->at(i++);
                while (process->waitForReadyRead(30000));
                auto stdout = process->readAllStandardOutput();
                auto stderr = process->readAllStandardError();
                if (stdout.length())
                {
                    _output->append(QString::fromUtf8(stdout.mid(0, stdout.length()-1)));
                }
                if (stderr.length())
                {
                    auto p_colour = _output->textColor();
                    _output->setTextColor(Qt::red);
                    _output->append(QString::fromUtf8(stderr.mid(0, stderr.length()-1)));
                    _output->setTextColor(p_colour);
                }
                if (process->state() == QProcess::NotRunning)
                    _processPool->removeOne(process);
                _mutex->unlock();
            }
        }
    }

public:
    StdoutMonitor(QTextBrowser *outputBrowser, QList<QProcess*> *processPool, QMutex *mutex)
    {
        _processPool = processPool;
        _output = outputBrowser;
        _mutex = mutex;
    }
};



class ProcessRunner : public QTextBrowser
{
    Q_OBJECT
private:
    QList<QProcess*> *_processPool;
    StdoutMonitor *_monitor;
    QMutex *_mutex;


public:
    explicit ProcessRunner(QWidget *parent = nullptr);
    ~ProcessRunner();

    void runCommand(QString command, QStringList args);

public slots:
};

#endif // PROCESSRUNNER_H
