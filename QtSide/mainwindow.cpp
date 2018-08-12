#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "portreader.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    reader = new PortReader(QString::fromStdString("/dev/ttyUSB0"), 9600);
    ui->setupUi(this);

    // Read and initialize all pin buttons
    auto pinList = ui->frameSelection->findChildren<PinButton *>();
    for (auto pin : pinList)
    {
        pin->Init(ui->textBrowser);
        connect(pin, &PinButton::pinSelected, this, &MainWindow::pinSelected);
        reader->connectPin(pin, pin->getPinNumber());

    }
    unsavedChanges = true;
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

void MainWindow::pinSelected(PinButton *pin)
{
    if (unsavedChanges)
    {
        int w = QMessageBox::warning(
                    this,
                    tr("Before selecting another button..."),
                    tr("There are unsaved changes"),
                    QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                    QMessageBox::Save);
        ui->textBrowser->append(QString::number(w));
    }
}
