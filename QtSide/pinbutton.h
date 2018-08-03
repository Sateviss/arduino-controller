#ifndef PINBUTTON_H
#define PINBUTTON_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QString>
#include <QTextEdit>
#include <QProcess>
#include <processrunner.h>

class PinButton : public QPushButton
{
    
private:
    
    int _pinNumber;

    int _actionTypeId;
    int _actOnId;

    QString _pinName;
    QString _script;
    QKeySequence _sequence;

    QList<QProcess*> _processPool;

    ProcessRunner *_terminalOutput;

public:
    
    PinButton(QWidget* parent);
    void Init(ProcessRunner *textEdit);

    QString getPinName();

    QString getScript() const;
    void setScript(const QString &script);
    void runScript();
    
    int getPinNumber();

    int getActOnId() const;
    void setActOnId(int actOnId);

    int getActionTypeId() const;
    void setActionTypeId(int actionTypeId);

    void newPinState(bool newState);
};

#endif // PINBUTTON_H
