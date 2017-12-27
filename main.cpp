#include "mainwindow.h"
#include <QApplication>
#include <QtGui>
#include <QtSql>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    QSqlDatabase bankDB=QSqlDatabase::addDatabase("QSQLITE");
    bankDB.setDatabaseName("./bank.db3");
    bankDB.open();
    qDebug()<<bankDB.tables();
    return a.exec();
}
