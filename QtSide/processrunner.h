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
                QByteArray std_out = process->readAllStandardOutput();
                QByteArray std_err = process->readAllStandardError();
                if (std_out.length())
                    emit outputText(Qt::gray, QString::fromUtf8(std_out.mid(0, std_out.length()-1)));
                if (std_err.length())
                    emit outputText(Qt::red, QString::fromUtf8(std_err.mid(0, std_err.length()-1)));
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
        emit outputText(Qt::darkYellow, command+" "+args.join(" "));
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
