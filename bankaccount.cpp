#include "bankaccount.h"

QString BankAccount::getBankName()
{
    QSqlDatabase db=QSqlDatabase::database();
    if(!db.open()){
        QMessageBox::information(0,"失败","getBankName():打开数据库失败");
        return QString();
    }
    QSqlQuery q;
    q.prepare("select name from banks where id=?");
    q.bindValue(0,bankID);
    if(!q.exec()){
        QMessageBox::information(0,"失败","getBankName():获取银行信息失败");
        return QString();
    }
    QString ret;
    while(q.next()){
        ret=QString::fromUtf8(q.value(0).toByteArray());
    }
    return ret;
}

bool BankAccount::signUp
(QString accountID_, QString pw_, QString ownerID_, qint64 bankID_, double money_){
    QSqlDatabase db=QSqlDatabase::database();
    if(!db.open()){
        QMessageBox::information(0,"失败","signUp():打开数据库失败");
        return false;
    }
    QSqlQuery q;
    q.prepare("select nextID from nextID where tableName = ?");
    q.addBindValue("accounts");
    if(!q.exec()){
        QMessageBox::information(0,"失败","signUp():分配账户主键失败");
        return false;
    }
    qint64 ID_=-1;
    while(q.next()){
        ID_=q.value(0).toLongLong();
    }
    if(ID_==-1){
        QMessageBox::warning(0,"致命错误"
          ,"signUp():没有查询到accounts表的nextID,数据库损坏");
        return false;
    }
    ID=ID_;
    accountID=accountID_;
    pw=pw_;
    ownerID=ownerID_;
    bankID=bankID_;
    money=money_;
    return saveNew();
}

bool BankAccount::save(){
    QSqlDatabase db=QSqlDatabase::database();
    if(!db.open()){
        QMessageBox::information(0,"失败","save():打开数据库失败");
        return false;
    }
    QSqlQuery q;
    q.prepare("update accounts "
              "set pw=?,money=? "
              "where ID=?");
    q.bindValue(0,pw);
    q.bindValue(1,money);
    q.bindValue(2,ID);
    if(!q.exec()){
        return false;
    }
    return true;
}

bool BankAccount::saveNew(){
    QSqlDatabase db=QSqlDatabase::database();
    if(!db.open()){
        QMessageBox::information(0,"失败","saveNew():打开数据库失败");
        return false;
    }
    QSqlQuery q;
    q.prepare("insert into accounts (ID,accountID,pw,ownerID,bankID,money)"
              "values(?,?,?,?,?,?)");
    q.addBindValue(ID);
    q.addBindValue(accountID);
    q.addBindValue(pw);
    q.addBindValue(ownerID);
    q.addBindValue(bankID);
    q.addBindValue(money);
    if(!q.exec()){
        return false;
    }
    QSqlQuery q1;
    q1.prepare("update nextID "
              "set nextID=? where tableName=?");
    q1.addBindValue(ID+1);
    q1.addBindValue("accounts");
    if(!q1.exec()){
        QMessageBox::warning(0,"致命错误"
          ,"saveNew():accounts表nextID更新失败,尝试撤销新建项...");
        QSqlQuery q2;
        q2.prepare("delete from accounts "
                  "where ID=?");
        //q.addBindValue("accounts");
        q2.addBindValue(ID);
        if(!q2.exec()){
            QMessageBox::warning(0,"撤销操作失败"
              ,"saveNew():撤销操作失败,数据库已损坏,请联系管理员");
        }
        return false;
    }
    return true;
}

bool BankAccount::addMoney(double add){
    money+=add;
    if(money>=0&&save())return true;
    else{
        money-=add;
        return false;
    }
}

bool BankAccount::checkPw(QString oldPw){
    return pw==oldPw;
}

bool BankAccount::changePw(QString newPw){
    QString oldPw=pw;
    pw=newPw;
    if(save())return true;
    else{
        pw=oldPw;
        return false;
    }
}


