#include "signupdialog.h"
#include "ui_signupdialog.h"
#include <QtSql>
#include "hash.h"
#include "bankaccount.h"

SignUpDialog::SignUpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SignUpDialog)
{
    ui->setupUi(this);
    QSqlDatabase db=QSqlDatabase::database();
    if(!db.open()){
       QMessageBox::information(this,"无法注册","无法连接到数据库，请稍后重试。");
       this->close();
       return;
    }
    QSqlQuery q;
    q.prepare("select ID,name from banks");
    if(!q.exec()){
        QMessageBox::information(this,"无法注册","无法获取银行信息，请稍后重试。");
        this->close();
        return;
    }
    while(q.next()){
        QString name=QString::fromUtf8(q.value(1).toByteArray());
        ui->bankSelect->addItem(name,q.value(0));
    }
}

SignUpDialog::~SignUpDialog()
{
    delete ui;
}

void SignUpDialog::on_buttonBox_rejected()
{
    this->reject();
}

void SignUpDialog::on_buttonBox_accepted()
{
    QString accountID=ui->accountIDIn->text();
    QString ownerID=ui->ownerIDIn->text();
    QString pw=ui->pwIn->text();
    QString pw2=ui->pwIn2->text();
    qint64 bankID=ui->bankSelect->currentData().toLongLong();
    double money=ui->moneyIn->text().toDouble();

    if(accountID.size()<6){
        QMessageBox::information(this,"请重试","请输入长度至少为6的卡号");
        return;
    }
    if(ownerID.size()!=18){
        QMessageBox::information(this,"请重试","请输入长度为18的身份证号");
        return;
    }
    for(int i=0;i<ownerID.size();i++){
        if(ownerID[i]!='X'&&ownerID[i]!='x'
                &&!ownerID[i].isDigit()){
            QMessageBox::information(this,"请重试","身份证号不合法");
            return;
        }
    }
    if(pw!=pw2){
        QMessageBox::information(this,"请重试","两次密码输入不一致");
        return;
    }
    if(pw.size()<6){
        QMessageBox::information(this,"请重试","请输入长度至少为6的密码");
        return;
    }    
    pw=myhash(pw);

    if(money<=0){
        QMessageBox::information(this,"请重试","请存入初始金额");
        return;
    }

    QSqlDatabase db=QSqlDatabase::database();
    if(!db.open()){
       QMessageBox::information(this,"无法注册","无法连接到账户数据库，请稍后重试。");
       return;
    }
    QSqlQuery q;
    q.prepare("select ID from accounts where accountID = ?"
              "and bankID = ?");
    q.addBindValue(accountID);
    q.addBindValue(bankID);
    if(!q.exec()){
        QMessageBox::information(this,"无法注册","无法查询数据库，请稍后重试。");
        return;
    }
    if(q.next()){
        QMessageBox::information(this,"卡号已存在","卡号已被注册，请重试。");
        return;
    }
    BankAccount acc;
    if(acc.signUp(accountID,pw,ownerID,bankID,money)){
        QMessageBox::information(this,"成功","成功注册银行卡");
        this->accept();
        return;
    }
    else{
        QMessageBox::information(this,"失败","注册银行卡失败");
        return;
    }
}
