#include "userdialog.h"
#include "changepwdialog.h"
#include "ui_userdialog.h"
#include <QtSql>
#include <QtGui>
#include <QInputDialog>

UserDialog::UserDialog(QWidget *parent,qint64 id_) :
    QDialog(parent),
    ui(new Ui::UserDialog)
{
    if (id_==-1){
        QMessageBox::information(0,"UserDialog"
                ,"UserDialog construction error:id=-1 passed.");
        this->close();
        return;
    }
    ui->setupUi(this);
    id=id_;
    QSqlDatabase db=QSqlDatabase::database();
    if(!db.open()){
       QMessageBox::information(this,"无法办理业务","无法连接到数据库，请稍后重试。");
       this->close();
       return;
    }
    QSqlQuery q;
    q.prepare("select bankID,accountID,ownerID,money,pw from accounts "
              "where ID=?");
    q.addBindValue(id);
    if(!q.exec()){
        QMessageBox::information(this,"无法办理业务","无法查询账户数据库，请稍后重试。");
        this->close();
        return;
    }
    if(!q.next()){
        QMessageBox::information(this,"账户错误","账户不存在，请联系柜台。");
        this->close();
        return;
    }
    qint64 bankID=q.value(0).toLongLong();
    QString accountID=q.value(1).toString();
    QString ownerID=q.value(2).toString();
    double money=q.value(3).toDouble();
    QString pw=q.value(4).toString();
    account = BankAccount(id,accountID,pw,ownerID,bankID,money);

    ui->bankL->setText(account.getBankName());
    ui->accountIDL->setText(QString("账户：%1").arg(accountID));
    ui->ownerIDL->setText(QString("持卡人：%1").arg(ownerID));
    ui->moneyL->setText(QString("账户余额：%1 元").arg(money));
}

UserDialog::~UserDialog()
{
    delete ui;
}

void UserDialog::on_logoutB_clicked()
{
    this->accept();
}

void UserDialog::on_addMoneyB_clicked()
{
    double input=0;
    bool ok;
    input = QInputDialog::getDouble(this,"存款","输入存款金额：",0,0,1e9,1,&ok);
    if(!ok){
        return;
    }
    if(input==0){
        QMessageBox::information(this,"存款","未存入款项");
        return;
    }
    int confirm=QMessageBox::question(this,"确认存款"
                  ,QString("确认存入：%1 元").arg(input)
                  ,QMessageBox::Ok,QMessageBox::Cancel);
    if(confirm==QMessageBox::Cancel){
        return;
    }
    if(account.addMoney(input)){
        QMessageBox::information(this,"存款","存款成功");
        ui->moneyL->setText(QString("账户余额：%1 元").arg(account.getMoney()));
    }
    else{
        QMessageBox::information(this,"存款","存款失败，请稍后重试");
    }
}

void UserDialog::on_takeMoneyB_clicked()
{
    double input=0;
    bool ok;
    input = QInputDialog::getDouble(this,"取款","输入取款金额：",0,0,1e9,1,&ok);
    if(!ok){
        return;
    }
    if(input==0){
        QMessageBox::information(this,"取款","未取出款项");
        return;
    }
    int confirm=QMessageBox::question(this,"确认取款"
                  ,QString("确认取款：%1 元").arg(input)
                  ,QMessageBox::Ok,QMessageBox::Cancel);
    if(confirm==QMessageBox::Cancel){
        return;
    }
    if(account.addMoney(0-input)){
        QMessageBox::information(this,"取款","取款成功");
        ui->moneyL->setText(QString("账户余额：%1 元").arg(account.getMoney()));
    }
    else{
        QMessageBox::information(this,"取款","取款失败，请重试");
    }
}

void UserDialog::on_changePwB_clicked()
{
    ChangePWDialog dia(&account,this);
    dia.exec();
}
