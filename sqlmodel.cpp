#include "sqlmodel.h"

SQLModel::SQLModel(QObject *parent,QSqlDatabase db,const QString &tableName):QSqlTableModel(parent,db)
{
    //init sql model;



    this->setTable(tableName);
    /*
    this->setHeaderData(model->fieldIndex("DATE"),Qt::Horizontal,tr("创建日期"),Qt::DisplayRole);
    this->setHeaderData(model->fieldIndex("PRODUCT"),Qt::Horizontal,tr("产品型号"),Qt::DisplayRole);
    this->setHeaderData(model->fieldIndex("SN"),Qt::Horizontal,tr("服务编号"),Qt::DisplayRole);
    this->setHeaderData(model->fieldIndex("SALES"),Qt::Horizontal,tr("销售代表"),Qt::DisplayRole);
    this->setHeaderData(model->fieldIndex("COMPANY"),Qt::Horizontal,tr("公司名称"),Qt::DisplayRole);
    this->setHeaderData(model->fieldIndex("CONTENT"),Qt::Horizontal,tr("详细信息"),Qt::DisplayRole);
*/


    model=new QSqlTableModel(parent,db);
   // initModel();
    model->setTable(tableName);
    initModel();
}

QSqlTableModel* SQLModel::initModel()
{
    //model->setTable("dellReg");
    model->setHeaderData(model->fieldIndex("DATE"),Qt::Horizontal,tr("创建日期"),Qt::DisplayRole);
    model->setHeaderData(model->fieldIndex("PRODUCT"),Qt::Horizontal,tr("产品型号"),Qt::DisplayRole);
    model->setHeaderData(model->fieldIndex("SN"),Qt::Horizontal,tr("服务编号"),Qt::DisplayRole);
    model->setHeaderData(model->fieldIndex("SALES"),Qt::Horizontal,tr("销售代表"),Qt::DisplayRole);
    model->setHeaderData(model->fieldIndex("COMPANY"),Qt::Horizontal,tr("公司名称"),Qt::DisplayRole);
    model->setHeaderData(model->fieldIndex("CONTENT"),Qt::Horizontal,tr("详细信息"),Qt::DisplayRole);
    model->select();
    return model;

}
