#include "logindialog.h"
#include "ui_logindialog.h"
#include <QtSql>
#include <QMessageBox>
#include "hash.h"
#include "userdialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    QSqlDatabase db=QSqlDatabase::database();
    if(!db.open()){
        QMessageBox::information(this,"无法登录","无法连接到数据库，请稍后重试");
        this->close();
        return;
    }
    QSqlQuery q;
    q.prepare("select ID,name from banks");
    if(!q.exec()){
        QMessageBox::information(this,"无法登录","无法获取银行信息，请稍后重试。");
        this->close();
        return;
    }
    while(q.next()){
        QString name=QString::fromUtf8(q.value(1).toByteArray());
        ui->bankSelect->addItem(name,q.value(0));
    }
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_buttonBox_rejected()
{
    this->reject();
}

void LoginDialog::on_buttonBox_accepted()
{
    QSqlDatabase db=QSqlDatabase::database();
    if(!db.open()){
        QMessageBox::information(this,"无法登录","无法连接到数据库，请稍后重试");
        return;
    }
    QSqlQuery q;
    q.prepare("select ID from accounts where "
              "accountID=? and pw=? and bankID=?");
    q.addBindValue(ui->accountIDIn->text());
    QString pwh=myhash(ui->pwIn->text());
    q.addBindValue(pwh);
    q.addBindValue(ui->bankSelect->currentData());
    if(!q.exec()){
        QMessageBox::information(this,"无法登录","无法查询数据库，请稍后重试");
        return;
    }
    if(!q.next()){
        QMessageBox::information(this,"登录失败","账号或密码错误");
        return;
    }
    qint64 ID=q.value(0).toLongLong();
   // QMessageBox::information(this,"login"
    //          ,QString("ID = %1").arg(ID));
    UserDialog ud(this,ID);
    ud.setWindowModality(Qt::ApplicationModal);
    ud.exec();
    this->close();
    return;
}
