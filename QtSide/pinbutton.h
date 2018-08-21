#ifndef PINBUTTON_H
#define PINBUTTON_H

#include <QDebug>
#include <QColor>
#include <QFile>
#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QString>
#include <QTextEdit>
#include <QProcess>
#include <QMouseEvent>

#include "processrunner.h"

class PinButton : public QPushButton
{

    Q_OBJECT

private:
    
    int _pinNumber;

    int _actionTypeId;
    int _actOnId;

    QString _pinName;
    QString _script;
    QString _sequence;
    bool _useGlobalInt;

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

    void mousePressEvent(QMouseEvent *e);

    QString getSequence() const;
    void setSequence(const QString &sequence);

    bool getUseGlobalInt() const;
    void setUseGlobalInt(bool useGlobalInt);

signals:

    void pinGotSelected(PinButton *pin);

};

#endif // PINBUTTON_H
