#ifndef CHANGEPWDIALOG_H
#define CHANGEPWDIALOG_H

#include <QDialog>

namespace Ui {
class ChangePWDialog;
}

class ChangePWDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChangePWDialog(QWidget *parent = 0);
    ~ChangePWDialog();

private:
    Ui::ChangePWDialog *ui;
};

#endif // CHANGEPWDIALOG_H
