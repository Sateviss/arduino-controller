#include "addkeydialog.h"
#include "ui_addkeydialog.h"

#include <QDesktopServices>
#include <QKeyEvent>
#include <QUrl>

AddKeyDialog::AddKeyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddKeyDialog) {
    key = new Key();
    ui->setupUi(this);
    ui->buttonBox->buttons()[0]->setEnabled(!(ui->editName->text().isEmpty() ||
                                              ui->editPAGName->text().isEmpty() ||
                                              ui->buttonQtKey->text() == "[ None ]"));
}

AddKeyDialog::~AddKeyDialog() {
    delete ui;
}

Key* AddKeyDialog::GetValue() {
    auto myWindow = new AddKeyDialog;
    auto returnValue = myWindow->exec();
    Key* val;
    if (returnValue == QDialog::Accepted)
        val = myWindow->key;
    else
        val = nullptr;
    delete myWindow;
    return val;
}

void AddKeyDialog::on_buttonBrowser_clicked() {
    QDesktopServices::openUrl(QUrl("https://pyautogui.readthedocs.io/en/latest/keyboard.html#keyboard-keys"));
}

void AddKeyDialog::keyPressEvent(QKeyEvent *event) {
    if (awaitKeyPress) {
        event->accept();
        ui->buttonQtKey->setText("0x"+QString::number(event->key(), 16));
        key->keyCode = ui->buttonQtKey->text();
        awaitKeyPress = false;
        ui->buttonBox->buttons()[0]->setEnabled(!(ui->editName->text().isEmpty() ||
                                                  ui->editPAGName->text().isEmpty() ||
                                                  ui->buttonQtKey->text() == "[ None ]"));
    }
    else
        event->ignore();
}

void AddKeyDialog::on_buttonQtKey_clicked() {
    ui->buttonQtKey->setText(tr("Press a key..."));
    this->setFocus();
    awaitKeyPress = true;
}

void AddKeyDialog::on_editPAGName_textEdited(const QString &arg1) {
    key->keyNamePy = ui->editPAGName->text();
    ui->buttonBox->buttons()[0]->setEnabled(!(ui->editName->text().isEmpty() ||
                                              ui->editPAGName->text().isEmpty() ||
                                              ui->buttonQtKey->text() == "[ None ]"));
}

void AddKeyDialog::on_editName_textEdited(const QString &arg1) {
    key->keyName = ui->editName->text();
    ui->buttonBox->buttons()[0]->setEnabled(!(ui->editName->text().isEmpty() ||
                                              ui->editPAGName->text().isEmpty() ||
                                              ui->buttonQtKey->text() == "[ None ]"));

}
