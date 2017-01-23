#ifndef INITSQL_H
#define INITSQL_H

#include <QObject>
#include <QtSql>

class InitSQL : public QObject
{
    Q_OBJECT
public:
    explicit InitSQL(QObject *parent = 0);

    QSqlDatabase sqlConn();
   ~InitSQL();
signals:

public slots:
private:
    QSqlDatabase db;
    QSqlError initSQL();
   // QSqlQuery *query;
   //void addData(QSqlQuery &q, const QString &title, int year, const QVariant &authorId,
   //             const QVariant &genreId, int rating)
     QSqlQuery *query;

};

#endif // INITSQL_H
