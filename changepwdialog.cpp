#include "changepwdialog.h"
#include "ui_changepwdialog.h"
#include "hash.h"

ChangePWDialog::ChangePWDialog(BankAccount *account_, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangePWDialog)
{
    ui->setupUi(this);
    account=account_;
}

ChangePWDialog::~ChangePWDialog()
{
    delete ui;
}

void ChangePWDialog::on_buttonBox_rejected()
{
    this->reject();
}

void ChangePWDialog::on_buttonBox_accepted()
{
    QString newPw=ui->newpwIn->text();
    if(newPw!=ui->newpwIn2->text()){
        QMessageBox::information(this,"请重试","两次输入的新密码不一致，请检查");
        return;
    }
    if (newPw.size()<6){
        QMessageBox::information(this,"请重试","请输入长度至少为6的密码");
        return;
    }
    QString oldPw=ui->oldpwIn->text();
    oldPw=myhash(oldPw);
    newPw=myhash(newPw);
    if(!account->checkPw(oldPw)){
        QMessageBox::information(this,"请重试","旧密码输入错误，请检查");
        this->reject();
        return;
    }
    if(account->changePw(newPw)){
        QMessageBox::information(this,"修改密码","修改密码成功");
        this->accept();
        return;
    }
    else{
        QMessageBox::information(this,"修改密码","修改密码失败，请稍后重试");
        return;
    }
}
