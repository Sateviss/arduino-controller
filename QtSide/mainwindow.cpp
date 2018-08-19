#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopServices>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    if (QSysInfo::kernelType() == "darwin") {
        ui->menuFile->setAsDockMenu();
    }
    // Read and initialize all pin buttons
    auto pinList = ui->frameSelection->findChildren<PinButton *>();

    for (auto pin : pinList) {
        pin->Init(ui->textBrowser);
        connect(pin, &PinButton::pinGotSelected,
                this, &MainWindow::pinSelected);
    }

    reader = nullptr;
    setPort(tr(""));
    unloadPin();
    selectedPin = nullptr;
    keyConfig = nullptr;
    checkUnsavedChanges();
    settings = new QSettings("Sateviss", "ArduinoManager");
    ports = new QList<Port *>();
    on_actionPort_hovered();
    awaitKeyPress = false;
    QDir().mkdir("./tmp");
    if (settings->contains("keyConfig"))
        loadKeyConfig();
    else
        keyConfig = new QJsonObject();
    if (settings->contains("config"))
        loadPinConfig();
    if (settings->contains("lastPin"))
        this->pinSelected(this->findChild<PinButton *>(settings->value("lastPin").toString()));
    ui->actionPort->menu()->actions()[0]->setChecked(true);
}

MainWindow::~MainWindow() {
    QDir dir("./tmp");
    dir.removeRecursively();
    delete ui;
}

void MainWindow::on_actionPort_hovered() {
    // When the port selection action is hovered above
    // we check available ports and add the ones that
    // were not added before
    auto menu = ui->actionPort->menu();
    auto portNames = PortReader::getPorts();
    QMenu* newMenu = new QMenu();
    QList<QString> portNamesPresent;

    // When the program starts we do not know the available ports
    // so no menu is available and thus no actions should be filtered
    if (menu != nullptr) {
        QList<QAction*> actionsPresent = menu->actions();
        if (actionsPresent.length() != 0)
            for (auto act : actionsPresent) {
                newMenu->addAction(act);
                portNamesPresent.append(act->text());
            }
    }

    // Check available ports and add as needed
    for (QString port : portNames) {
        if (portNamesPresent.contains(port))
            continue;
        auto act = newMenu->addAction(port);
        act->setCheckable(true);
        auto newPort = new Port(act, ports);
        ports->append(newPort);
        connect(newPort, &Port::selectedPort, this, &MainWindow::setPort);
    }

    ui->actionPort->setMenu(newMenu);
}

void MainWindow::pinSelected(PinButton *pin) {
    if (pin == selectedPin)
        return;

    if (unsavedChanges) {
        int w = QMessageBox::warning(
                    this,
                    tr("Warning"),
                    tr("There are unsaved changes for this pin"),
                    QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                    QMessageBox::Save);
        if (w == QMessageBox::Cancel)
            return;
        if (w == QMessageBox::Save)
            saveChanges();
    }
    loadPin(pin);
}

void MainWindow::saveChanges() {
    if (selectedPin == nullptr)
            return;

    selectedPin->setActOnId(ui->comboBoxActon->currentIndex());
    selectedPin->setActionTypeId(ui->comboBoxActionType->currentIndex());
    selectedPin->setScript(ui->editScript->toPlainText());
    selectedPin->setSequence(ui->buttonKeyCombination->text());
    dumpPinConfig();
}

void MainWindow::loadPin(PinButton* newPin) {
    unloadPin();
    ui->frame->setEnabled(true);
    selectedPin = newPin;
    ui->labelEditing->setText(tr("Editing pin %1:").arg(selectedPin->getPinName()));
    ui->comboBoxActon->setCurrentIndex(selectedPin->getActOnId());
    ui->comboBoxActionType->setCurrentIndex(selectedPin->getActionTypeId());
    ui->buttonKeyCombination->setText(selectedPin->getSequence());
    ui->editScript->setText(selectedPin->getScript());
    settings->setValue("lastPin", selectedPin->objectName());
}

void MainWindow::unloadPin() {
    ui->labelEditing->setText(tr("No pin selected"));
    ui->frame->setEnabled(false);
}

void MainWindow::checkUnsavedChanges() {
    unsavedChanges =
            (selectedPin != nullptr) &&
            (ui->comboBoxActon->currentIndex() != selectedPin->getActOnId() ||
            ui->comboBoxActionType->currentIndex() != selectedPin->getActionTypeId() ||
            ui->buttonKeyCombination->text() != selectedPin->getSequence() ||
            ui->editScript->toPlainText() != selectedPin->getScript());
    if (unsavedChanges) {
        ui->labelState->setStyleSheet("color: red");
        ui->labelState->setText("There are unsaved changes");
    }
    else {
        ui->labelState->setStyleSheet("color: green");
        ui->labelState->setText("Everything's dandy!");
    }
}

void MainWindow::on_comboBoxActon_currentIndexChanged(int index) {
    checkUnsavedChanges();
}

void MainWindow::on_editScript_textChanged() {
    checkUnsavedChanges();
}

void MainWindow::on_buttonReset_clicked() {
    unloadPin();
    loadPin(selectedPin);
    checkUnsavedChanges();
}

void MainWindow::on_buttonApply_clicked() {
    saveChanges();
    checkUnsavedChanges();
}

void MainWindow::on_comboBoxActionType_currentIndexChanged(int index) {
    ui->editScript->setEnabled(!index);
    ui->buttonKeyCombination->setEnabled(index);

    checkUnsavedChanges();
}

void MainWindow::setPort(QString portName) {
    if (reader != nullptr)
        delete reader;
    reader = new PortReader(portName, 9600);
    auto pinList = ui->frameSelection->findChildren<PinButton *>();
    for (auto pin : pinList)
        reader->connectPin(pin, pin->getPinNumber());
}


void MainWindow::on_actionAbout_triggered() {
    QMessageBox::about(this, "About",
                                       "Arduino macro editing software<br>"
                                       "Made using Qt Creator<br>"
                                       "<a href=\"https://github.com/Sateviss/arduino-controller\">Source on GitHub</a><br>"
                                       "<a href=\"https://github.com/Sateviss/arduino-controller/blob/master/LICENSE\">Licensed under GPL 3.0</a>"
                                       "<br>"
                                       "© 2018 Eugene Pisarchick"
                       );
}

void MainWindow::on_actionReport_an_issue_triggered() {
    QDesktopServices::openUrl(QUrl("https://github.com/Sateviss/arduino-controller/issues"));
}

void MainWindow::on_actionExit_triggered() {
    exit(0);
}

void MainWindow::on_actionSave_as_2_triggered() {
    auto path = QFileDialog::getSaveFileName(
                this,
                "Save script as...",
                QDir::currentPath(),
                "Python script (*.py);;Text file (*.txt);;Any file (*.*)");
    if (path.length() == 0)
        return;
    scriptSavePath = path;
    auto info = QFileInfo(path);
    info.makeAbsolute();
    if (info.suffix().length() == 0)
        path+=".py";

    on_actionSave_2_triggered();
}

void MainWindow::on_actionSave_2_triggered() {
    if (scriptSavePath.length() == 0) {
        on_actionSave_as_2_triggered();
        return;
    }
    QFile file(scriptSavePath);
    if (file.exists())
        file.remove();
    if (file.open(QIODevice::ReadWrite) ) {
        QTextStream stream( &file );
        stream << ui->editScript->toPlainText() << endl;
    }
    file.close();
}

void MainWindow::on_actionLoad_2_triggered() {
    auto path = QFileDialog::getOpenFileName(
                this,
                "Load script from...",
                scriptSavePath.length()?scriptSavePath:QDir::currentPath(),
                "Python script (*.py);;Text file (*.txt);;Any file (*.*)");
    if (path.length() == 0)
        return;
    QFile file(path);
    if (file.open(QIODevice::ReadWrite) )
        ui->editScript->setPlainText(QString::fromUtf8(file.readAll()));
}

void MainWindow::on_actionSave_triggered() {
    saveChanges();
    if (configSavePath.length() == 0) {
        on_actionSave_as_triggered();
        return;
    }

    QJsonDocument json;
    QJsonArray arrayJson;
    auto pinList = ui->frameSelection->findChildren<PinButton *>();
    for (int i = 0; i < pinList.length(); i++) {
        auto pin = pinList[i];
        QJsonObject pinJson;
        pinJson["name"] = pin->objectName();
        pinJson["act_on_id"] = pin->getActOnId();
        pinJson["action_type"] = pin->getActionTypeId();
        pinJson["script"] = pin->getScript();
        pinJson["sequence"] = pin->getSequence();
        arrayJson.append(pinJson);
    }

    QFile file(configSavePath);
    if (file.exists())
        file.remove();
    QJsonObject container;
    container["configuration"] = arrayJson;
    json.setObject(container);
    if (file.open(QIODevice::ReadWrite) )
        file.write(json.toJson());
    file.close();
}

void MainWindow::on_actionSave_as_triggered() {
    auto path = QFileDialog::getSaveFileName(
                this,
                "Save configuration as...",
                configSavePath.length()?configSavePath:QDir::currentPath(),
                "Json file (*.json)");
    if (path.length() == 0)
        return;
    auto info = QFileInfo(path);
    info.makeAbsolute();
    if (info.suffix().length() == 0)
        path+=".json";
    configSavePath = path;

    on_actionSave_triggered();
}

void MainWindow::on_actionLoad_triggered() {
    auto path = QFileDialog::getOpenFileName(
                this,
                "Load config from...",
                configSavePath.length()?configSavePath:QDir::currentPath(),
                "Json file (*.json)");

    if (path.length() == 0)
        return;


    QFile file(path);
    file.open(QIODevice::ReadOnly);
    auto data = file.readAll();
    file.close();

    QJsonDocument json = QJsonDocument::fromJson(data);
    QJsonArray arrayJson = json.object()["configuration"].toArray();
    for (int i = 0; i < arrayJson.count(); i++) {
        QJsonObject pinJson = arrayJson[i].toObject();
        auto pin = ui->frameSelection->findChild<PinButton *>(pinJson["name"].toString());
        pin->setActOnId(pinJson["act_on_id"].toInt());
        pin->setActionTypeId(pinJson["action_type"].toInt());
        pin->setScript(pinJson["script"].toString());
        pin->setSequence(pinJson["sequence"].toString());
    }
    if (selectedPin != nullptr)
        loadPin(selectedPin);
}

void MainWindow::on_buttonKeyCombination_clicked() {
    ui->buttonKeyCombination->setText("Press keys...");
    this->setFocus();
    awaitKeyPress = true;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (!awaitKeyPress) {
        event->ignore();
        return;
    }
    event->accept();
    auto keyName = "0x"+QString::number(event->key(), 16);
    if (!keyConfig->contains(keyName)) {
        QMessageBox().warning(this, "Key error", "Please register all of the keys you want to use before using them");
        ui->buttonKeyCombination->setText(selectedPin->getSequence());
        keys.clear();
        return;
    }
    keys.append(keyName);
    if (ui->buttonKeyCombination->text() == "Press keys...")
        ui->buttonKeyCombination->setText((*keyConfig)[keyName].toObject()["name"].toString());
    else
        ui->buttonKeyCombination->setText(ui->buttonKeyCombination->text()+"+"+(*keyConfig)[keyName].toObject()["name"].toString());
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    if (!awaitKeyPress) {
        event->ignore();
        return;
    }
    event->accept();
    awaitKeyPress = false;
    ui->editScript->setPlainText("import pyautogui\n");
    auto text = ui->editScript->toPlainText();

    for (int i = 0; i < keys.length(); i++)
        ui->editScript->append(tr("pyautogui.keyDown('%1')").arg((*keyConfig)[keys[i]].toObject()["pyname"].toString()));
    for (int i = keys.length()-1; i >= 0; i--)
        ui->editScript->append(tr("pyautogui.keyUp('%1')").arg((*keyConfig)[keys[i]].toObject()["pyname"].toString()));
    keys.clear();
}

void MainWindow::on_actionAdd_key_triggered() {
    auto key = AddKeyDialog::GetValue();
    if (key != nullptr) {
        QJsonObject keymap;
        keymap["name"] = key->keyName;
        keymap["pyname"] = key->keyNamePy;
        (*keyConfig)[key->keyCode] = keymap;
        dumpKeyConfig();
    }
}

void MainWindow::dumpKeyConfig() {
    QJsonDocument doc;
    QJsonObject obj;
    obj["config"] = *keyConfig;
    doc.setObject(obj);
    qDebug()<<QString::fromUtf8(doc.toJson().data());
    settings->setValue("keyConfig", QString::fromUtf8(doc.toJson().data()));
}

void MainWindow::loadKeyConfig() {
    QJsonDocument doc = QJsonDocument::fromJson(settings->value("keyConfig").toByteArray());
    qDebug()<<doc.toJson();
    auto object = new QJsonObject;
    *object = doc.object()["config"].toObject();
    if (keyConfig != nullptr) {
        keyConfig->swap(*object);
        delete object;
    }
    else
        keyConfig = object;
}

void MainWindow::dumpPinConfig() {
    QJsonDocument json;
    QJsonArray arrayJson;
    auto pinList = ui->frameSelection->findChildren<PinButton *>();
    for (int i = 0; i < pinList.length(); i++) {
        auto pin = pinList[i];
        QJsonObject pinJson;
        pinJson["name"] = pin->objectName();
        pinJson["act_on_id"] = pin->getActOnId();
        pinJson["action_type"] = pin->getActionTypeId();
        pinJson["script"] = pin->getScript();
        pinJson["sequence"] = pin->getSequence();
        arrayJson.append(pinJson);
    }
    QJsonObject obj;
    obj["configuration"] = arrayJson;
    json.setObject(obj);
    settings->setValue("config", QString::fromUtf8(json.toJson()));
}

void MainWindow::loadPinConfig() {
    QJsonDocument json = QJsonDocument::fromJson(settings->value("config").toByteArray());
    QJsonArray arrayJson = json.object()["configuration"].toArray();
    for (int i = 0; i < arrayJson.count(); i++) {
        QJsonObject pinJson = arrayJson[i].toObject();
        auto pin = ui->frameSelection->findChild<PinButton *>(pinJson["name"].toString());
        pin->setActOnId(pinJson["act_on_id"].toInt());
        pin->setActionTypeId(pinJson["action_type"].toInt());
        pin->setScript(pinJson["script"].toString());
        pin->setSequence(pinJson["sequence"].toString());
    }
}

void MainWindow::on_actionOpen_PyAutoGUI_help_triggered() {
    QDesktopServices::openUrl(QUrl("https://pyautogui.readthedocs.io/en/latest/"));
}
