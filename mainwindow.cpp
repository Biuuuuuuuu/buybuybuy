#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "signupdialog.h"
#include "logindialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
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
