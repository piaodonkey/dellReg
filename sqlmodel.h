#ifndef SQLMODEL_H
#define SQLMODEL_H

//#include "initsql.h"

#include <QObject>
#include <QSqlTableModel>

class SQLModel : public QSqlTableModel
{
public:
    //SQLModel();
    explicit SQLModel(QObject *parent = Q_NULLPTR, QSqlDatabase db = QSqlDatabase(),const QString &tableName="");
    QSqlTableModel *initModel();
private:
    QSqlTableModel *model;
     //InitSQL db;
};

#endif // SQLMODEL_H
