#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "signupdialog.h"
#include "logindialog.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"

using namespace rapidjson;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    server = new QTcpServer(this);
    if(!server->listen(QHostAddress::LocalHost,11111)){
        QMessageBox::information(this,"网络连接失败"
                                 ,"无法启动服务器");
        this->close();
        return;
    }
    connect(server,SIGNAL(newConnection())
            ,this,SLOT(newConnect()));
}

void MainWindow::newConnect(){
    qDebug()<<"new connect";
    QTcpSocket* soc=server->nextPendingConnection();
    if(!soc)return;
    pool.append(soc);
    connect(soc,SIGNAL(readyRead())
            ,this,SLOT(serverAnswer()));
}

void MainWindow::serverAnswer(){
    QTcpSocket* soc=qobject_cast<QTcpSocket*>(sender());
    if(!soc)return;
    QByteArray msg=soc->readAll();
    qDebug()<<msg;
    if(msg=="banks"){//bank list reuqest
        QString ans("[");
        QSqlDatabase db=QSqlDatabase::database();
        if(db.open()){
            QSqlQuery q;
            q.prepare("select ID,name from banks");
            if(q.exec()){
                while(q.next()){
                    int bankID=q.value(0).toInt();
                    QString name=q.value(1).toString();
                    ans.append(QString("{\"bankID\":%1,"
                                       "\"name\":\"%2\"},")
                               .arg(bankID).arg(name));
                }
            }
        }
        if(ans.size()>1)ans[ans.size()-1]=']';
        else ans.append("]");
        soc->write(ans.toUtf8());
        qDebug()<<("ans: "+ans);
        soc->close();
    }
    else{//password check/payment request
        Document d;
        if(d.Parse(msg.data()).HasParseError()){
            soc->write("x");
            qDebug()<<"x";
            soc->close();
            return;
        }
        QString accountID=QString::fromUtf8(d["accountID"].GetString());
        int bankID=d["bankID"].GetInt();
        QString pw=QString::fromUtf8(d["pw"].GetString());
        QSqlDatabase db=QSqlDatabase::database();
        if(!db.open()){
            soc->write("d");
            QMessageBox::information(this,"对客户服务报告错误"
                ,"对客户服务报告错误:无法连接到数据库");
            soc->close();
            return;
        }
        QSqlQuery q;
        q.prepare("select money from accounts "
                  "where accountID=? "
                  "and bankID=? "
                  "and pw=?");
        q.addBindValue(accountID);
        q.addBindValue(bankID);
        q.addBindValue(pw);
        if(!q.exec()){
            soc->write("d");
            QMessageBox::information(this,"对客户服务报告错误"
                ,"对客户服务报告错误:无法查询数据库");
            soc->close();
            return;
        }
        if(q.next()){//密码正确
            if(d.HasMember("pay")){
                double money=q.value(0).toDouble();
                double pay=d["pay"].GetDouble();
                money-=pay;
                if(money>=0){
                    q.prepare("update accounts set money=? "
                              "where accountID=? "
                              "and bankID=? "
                              "and pw=?");
                    q.addBindValue(money);
                    q.addBindValue(accountID);
                    q.addBindValue(bankID);
                    q.addBindValue(pw);
                    if(q.exec()){
                        soc->write("1");
                        qDebug()<<"payment complete";
                        soc->close();
                        return;
                    }
                    else{
                        soc->write("d");
                        QMessageBox::information(this,"对客户服务报告错误"
                            ,"对客户服务报告错误:支付失败，无法更新数据库");
                        soc->close();
                        return;
                    }
                }
                else{
                    soc->write("$");
                    qDebug()<<"no enough money";
                    soc->close();
                    return;
                }
            }
            else{
                soc->write("1");
                qDebug()<<"pw correct";
                soc->close();
                return;
            }
        }
        else{
            soc->write("0");
            qDebug()<<"wrong pw";
            soc->close();
            return;
        }
    }
}

MainWindow::~MainWindow()
{
    qDeleteAll(pool);
    delete ui;
}

void MainWindow::on_signUpB_clicked()
{
    SignUpDialog dia(this);
    dia.setWindowModality(Qt::ApplicationModal);
    dia.exec();
}

void MainWindow::on_loginB_clicked()
{
    LoginDialog dia(this);
    dia.setWindowModality(Qt::ApplicationModal);
    dia.exec();
}
