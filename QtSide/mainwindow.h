#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include "portreader.h"
#include "pinbutton.h"

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    bool unsavedChanges;
    PortReader* reader;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionPort_hovered();

public slots:
    void pinSelected(PinButton *pin);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
