#ifndef PINBUTTON_H
#define PINBUTTON_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QString>

class PinButton : public QPushButton
{
    
private:
    
    int _pinNumber;
    QString _pinName;

    int _actionTypeId;
    int _actOnId;

    QString _script;
    QKeySequence _sequence;
    

public:
    
    PinButton(QWidget* parent);

    QString getPinName() const;

    QString getScript() const;
    void setScript(const QString &script);
    void runScript();
    
    int getPinNumber() const;

    int getActOnId() const;
    void setActOnId(int actOnId);

    int getActionTypeId() const;
    void setActionTypeId(int actionTypeId);

    void newPinState(bool newState);
};

#endif // PINBUTTON_H
