#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "portreader.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    reader = new PortReader("/dev/ttyUSB0");
    ui->setupUi(this);
    auto pinList = ui->frameSelection->findChildren<PinButton *>();
    for (int i = 0; i < pinList.length(); i++)
        reader->connectPin(pinList[i], pinList[i]->getPinNumber());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionPort_hovered()
{
    // When the port selection action is hovered above
    // we check available ports and add the ones that
    // were not added before
    auto menu = ui->actionPort->menu();
    auto portNames = PortReader::getPorts();
    QMenu* newMenu = new QMenu();

    QList<QString> portNamesPresent;
    // When the program starts we do not know the available ports
    // so no menu is available and thus no actions should be filtered
    if (menu != nullptr)
    {
        QList<QAction*> actionsPresent = menu->actions();
        if (actionsPresent.length() != 0)
            for (auto act : actionsPresent)
            {
                newMenu->addAction(act);
                portNamesPresent.append(act->text());
            }
    }

    // Check available ports and add as needed
    for (QString port : portNames)
    {
        if (portNamesPresent.contains(port))
            continue;
        auto act = newMenu->addAction(port);
        act->setCheckable(true);
    }

    ui->actionPort->setMenu(newMenu);
}
