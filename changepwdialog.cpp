#include "changepwdialog.h"
#include "ui_changepwdialog.h"

ChangePWDialog::ChangePWDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangePWDialog)
{
    ui->setupUi(this);
}

ChangePWDialog::~ChangePWDialog()
{
    delete ui;
}
