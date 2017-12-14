#ifndef CHANGEPWDIALOG_H
#define CHANGEPWDIALOG_H

#include <QDialog>
#include "bankaccount.h"

namespace Ui {
class ChangePWDialog;
}

class ChangePWDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChangePWDialog(BankAccount* account,QWidget *parent = 0);
    ~ChangePWDialog();

private slots:
    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

private:
    Ui::ChangePWDialog *ui;
    BankAccount* account;
};

#endif // CHANGEPWDIALOG_H
