#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "signupdialog.h"
#include "logindialog.h"

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
    if(msg=="banks"){
        QString ans("[");
        QSqlDatabase db=QSqlDatabase::database();
        QSqlQuery q;
        q.prepare("select ID,name from banks");
        if(db.open()&&q.exec()){
            while(q.next()){
                int bankID=q.value(0).toInt();
                QString name=q.value(1).toString();
                ans.append(QString("{\"bankID\":%1,"
                                   "\"name\":\"%2\"},")
                           .arg(bankID).arg(name));
            }
            if(ans.size()>1)ans[ans.size()-1]=']';
            else ans.append("]");
        }
        soc->write(ans.toUtf8());
        soc->close();
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
