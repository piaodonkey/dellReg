#ifndef VIEWPAD_H
#define VIEWPAD_H

#include <QWidget>
#include <QtWidgets>
#include <QSqlTableModel>
#include "initsql.h"
#include <QSqlDatabase>
//#include "createdlg.h"
#include "crd.h"

class ViewPad : public QWidget
{
    Q_OBJECT
public:
    explicit ViewPad(QWidget *parent = 0);
   //~ViewPad();
signals:
protected:
    bool eventFilter(QObject *watched, QEvent *event);
public slots:
private:
    QSqlTableModel *model;
    QTableView *viewTable;

    //Crd *crd;
    QPushButton *btnCreate,*btnNext,*btnPrv,*btnThis,*btnSrh;
    QLineEdit *lineSrhText;
    QLabel *installCount;
    QHBoxLayout *vLayout;
    QVBoxLayout *hLayout;
    QVBoxLayout *layout;


    void initWidgets();
    InitSQL *db;
    void addData(const QList<QStringList> &record);//temp
    void modifyData();
    void initModel(QSqlDatabase db, const QString &tableName) ;
    void initView(QSqlTableModel *model);
    void initLayout();
    void copyData();//copyData;
    void delData();
    QDate date;
    QMenu *rightPopupMenu;

   // createDlg *dlg;

private slots:
    //void copyData();
    void showRightMenu(const QPoint&);
protected:


};

#endif // VIEWPAD_H
