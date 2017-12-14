#ifndef USERDIALOG_H
#define USERDIALOG_H

#include <QDialog>
#include "bankaccount.h"

namespace Ui {
class UserDialog;
}

class UserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserDialog(QWidget *parent = 0,qint64 id_=-1);
    ~UserDialog();

private slots:
    void on_logoutB_clicked();

    void on_addMoneyB_clicked();

    void on_takeMoneyB_clicked();

    void on_changePwB_clicked();

private:
    Ui::UserDialog *ui;
    qint64 id;
    BankAccount account;
    bool addMoney(double add);
};

#endif // USERDIALOG_H
