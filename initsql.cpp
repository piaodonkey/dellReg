#include "initsql.h"

InitSQL::InitSQL(QObject *parent) : QObject(parent)
{
    //qDebug()<<"Test";
}

QSqlError InitSQL::initSQL()
{
    db=QSqlDatabase::addDatabase("QMYSQL","postgres with dell");
    db.setHostName("192.168.1.225");
    db.setUserName("root");
    db.setPassword("root");
    //db.setPort(33489);
    db.setDatabaseName("data");
    if(!db.open())
    {
        return db.lastError();

    }
    
    QStringList tables = db.tables();
    if (!tables.contains("dellReg", Qt::CaseInsensitive))
    {

        QString createTable="CREATE TABLE dellReg ("
                                   "DATE text,"
                                   "PRODUCT text,"
                                   "SN text,"
                                   "SALES text,"
                                   "COMPANY text,"
                                   "CONTENT text,"
                                   "ID INTEGER PRIMARY KEY AUTO_INCREMENT)";
        //product_no integer DEFAULT nextval('products_product_no_seq'),
        query=new QSqlQuery(db);
        if(!query->exec(createTable))
        {
            return query->lastError();
            query->finish();
        }
       // db.close();
    }
    //db.close();
    return QSqlError();

}

 QSqlDatabase InitSQL::sqlConn()
{
   QSqlError err=initSQL();
   qDebug()<<err.text();
   if(err.type()==QSqlError::NoError)
   {
       return db;
   }
   return QSqlDatabase();
}

InitSQL::~InitSQL()
{
    //query->finish();
    //db.close();
   // QSqlDatabase::removeDatabase("postgres with dell");
}
