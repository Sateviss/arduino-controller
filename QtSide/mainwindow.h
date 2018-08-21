#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "portreader.h"
#include "pinbutton.h"
#include "addkeydialog.h"

#include <QAction>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QKeyEvent>
#include <QKeySequence>
#include <QSettings>

namespace Ui {
class MainWindow;
}

class Port : public QObject {
    Q_OBJECT
public :
    QList<Port *> *oPorts;
    QAction* mAction;
    Port(QAction *myAction, QList<Port *> *otherPorts) {
        oPorts = otherPorts;
        mAction = myAction;
        connect(myAction, &QAction::toggled, this, &Port::selected);
    }

signals:
    void selectedPort(QString myName);

public slots:
    void selected(bool newState) {
        if (newState)
        {
            for (auto a : *oPorts)
                if (a != this)
                    a->mAction->setChecked(false);
            emit selectedPort(mAction->text());
        }
    }
};

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    bool unsavedChanges;
    PortReader *reader;
    PinButton *selectedPin;
    QList<Port *> *ports;

    QJsonObject *keyConfig;
    QSettings *settings;

    bool awaitKeyPress = false;
    QString configSavePath;
    QString scriptSavePath;

    QStringList keys;

    void checkUnsavedChanges();
    void saveChanges();
    void loadPin(PinButton *newPin);
    void unloadPin();

    void dumpKeyConfig();
    void loadKeyConfig();

    void dumpPinConfig();
    void loadPinConfig();

public:

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionPort_hovered();

    void pinSelected(PinButton *pin);

    void on_comboBoxActon_currentIndexChanged(int index);

    void on_editScript_textChanged();

    void on_buttonReset_clicked();

    void on_buttonApply_clicked();

    void on_comboBoxActionType_currentIndexChanged(int index);

    void on_actionAbout_triggered();

    void on_actionReport_an_issue_triggered();

    void on_actionExit_triggered();

    void on_actionSave_as_2_triggered();

    void on_actionSave_2_triggered();

    void on_actionLoad_2_triggered();

    void on_actionSave_triggered();

    void on_actionSave_as_triggered();

    void on_actionLoad_triggered();

    void on_buttonKeyCombination_clicked();

    void on_actionAdd_key_triggered();

    void on_actionOpen_PyAutoGUI_help_triggered();
    
    void on_comboBoxInterpreter_currentIndexChanged(int index);

public slots:
    void setPort(QString portName);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
