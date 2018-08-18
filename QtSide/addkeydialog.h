#ifndef ADDKEYDIALOG_H
#define ADDKEYDIALOG_H

#include <QDialog>

namespace Ui {
class AddKeyDialog;
}

struct Key {
    QString keyCode;
    QString keyName;
    QString keyNamePy;
};


class AddKeyDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddKeyDialog(QWidget *parent = nullptr);
    ~AddKeyDialog();

    static Key* GetValue();

private slots:
    void on_buttonBrowser_clicked();

    void on_buttonQtKey_clicked();

    void on_editPAGName_textEdited(const QString &arg1);

    void on_editName_textEdited(const QString &arg1);

private:
    Ui::AddKeyDialog *ui;
    Key *key;
    bool awaitKeyPress;

    void keyPressEvent(QKeyEvent *event);
};

#endif // ADDKEYDIALOG_H
