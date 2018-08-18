#ifndef PROCESSRUNNER_H
#define PROCESSRUNNER_H

#include <QWidget>
#include <QTextBrowser>
#include <QProcess>
#include <QThread>
#include <QMutex>

#include <exception>

class StdoutMonitor : public QThread {
    Q_OBJECT

    QList<QProcess*> *_processPool;
    QTextBrowser *_output;
    QMutex *_mutex;

    void run() override {
        while (true) {
            _mutex->lock();
            for (auto i = 0; i < _processPool->length();) {
                auto process = _processPool->at(i++);
                auto stdout = process->readAllStandardOutput();
                auto stderr = process->readAllStandardError();
                if (stdout.length())
                    emit outputText(Qt::gray, QString::fromUtf8(stdout.mid(0, stdout.length()-1)));
                if (stderr.length())
                    emit outputText(Qt::red, QString::fromUtf8(stderr.mid(0, stderr.length()-1)));
                if (process->state() == QProcess::NotRunning)
                    _processPool->removeAt(--i);
            }
            _mutex->unlock();
            thread()->msleep(50);
        }
    }

public:
    StdoutMonitor(QTextBrowser *outputBrowser) {
        _output = outputBrowser;
        _mutex = new QMutex();
        _processPool = new QList<QProcess*>();
    }

public slots:
    void addProcess(QString command, QStringList args){
        _mutex->lock();
        emit outputText(Qt::yellow, command+" "+args.join(" "));
        auto newProcess = new QProcess(this);
        newProcess->start(command, args);
        _processPool->append(newProcess);
        _mutex->unlock();
    }

signals:
    void outputText(QColor color, QString text);

};



class ProcessRunner : public QTextBrowser {
    Q_OBJECT

private:
    StdoutMonitor *_monitor;

public:
    explicit ProcessRunner(QWidget *parent = nullptr);
    ~ProcessRunner();
    void runCommand(QString command, QStringList args);

signals:
    void addCommandToSlave(QString command, QStringList args);

public slots:
    void addText(QColor textColor, QString text);

};

#endif // PROCESSRUNNER_H
