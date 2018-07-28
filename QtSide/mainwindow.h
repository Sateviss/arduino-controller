#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include "portreader.h"

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    PortReader* reader;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionPort_hovered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
