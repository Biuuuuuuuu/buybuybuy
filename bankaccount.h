#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H
#include <QtGui>
#include <QtSql>
#include <QMessageBox>

class BankAccount
{
public:
    BankAccount(){}
    BankAccount(qint64 ID_,QString accountID_,QString pw_
                ,QString ownerID_,qint64 bankID_,double money_)
        :ID(ID_),accountID(accountID_),pw(pw_),ownerID(ownerID_)
        ,bankID(bankID_),money(money_){}

    bool signUp(QString accountID_,QString pw_,QString ownerID_
                ,qint64 bankID_,double money_);//注册新账户
    bool logIn(QString accountID_,QString pw_,qint64 bankID_);//登录
    bool addMoney(double add);//money+=add,如果是负数表示取钱
    bool changePw(QString newPw);
    bool checkPw(QString oldPw);
    qint64 getID(){return ID;}
    QString getAccountID(){return accountID;}
    qint64 getBankID(){return bankID;}
    double getMoney(){return money;}
    QString getBankName();

private:
    bool save();
    bool saveNew();
    qint64 ID;
    QString accountID;
    QString pw;
    QString ownerID;
    qint64 bankID;
    double money;

    QString getPw(){return pw;}
};

#endif // BANKACCOUNT_H
