#pragma execution_character_set("utf-8")
#include "viewpad.h"
#include "initsql.h"
#include <QLabel>
#include <QWebEngineView>
#include <QWebChannel>
ViewPad::ViewPad(QWidget *parent) : QWidget(parent)
{

    //crd=new Crd(this);
    db=new InitSQL(this);
    rightPopupMenu=nullptr;
    date=QDate::currentDate();
    this->setWindowTitle(QString("工作记录表").append(QDate::currentDate().toString(" yyyy年MMMMdd日 dddd")));
    this->setWindowIcon(QIcon(":/image/logo"));
    initModel(db->sqlConn(),tableName);
    initView(model);

    initWidgets();
    initLayout();
    connect(btnPrv,&QPushButton::clicked,this,[=]()
    {
        date=date.addMonths(-1);
        model->setFilter(QString("date like '%%1%'").arg(date.toString("yyyy-MM")));
        model->setSort(6,Qt::DescendingOrder);
        model->select();

    });
    connect(btnNext,&QPushButton::clicked,this,[=]()
    {
        date=date.addMonths(1);
        model->setFilter(QString("date like '%%1%'").arg(date.toString("yyyy-MM")));
        model->setSort(6,Qt::DescendingOrder);
        model->select();


    });
    connect(btnThis,&QPushButton::clicked,this,[=]()
    {
        //qDebug()<<model->tableName();
        date=date.currentDate();
        model->setFilter(QString("date like '%%1%'").arg(date.toString("yyyy-MM")));
        model->setSort(6,Qt::DescendingOrder);
        model->select();

    });

    connect(btnSrh,&QPushButton::clicked,this,[=]()
    {
        QString srhText=lineSrhText->text();
        if(!srhText.isEmpty())
        {

            QString product=QString("product like '%%1%'").arg(srhText);
            QString sn=QString("sn like '%%1%'").arg(srhText);
            QString sales=QString("sales  like '%%1%'").arg(srhText);
            QString content=QString("content  like '%%1%'").arg(srhText);
            QString company=QString("company like '%%1%'").arg(srhText);

            QString sqlFiled=QString("%1 or %2 or %3 or %3 or %4 or %5").arg(product,sn,sales,content,company);

            model->setFilter(sqlFiled);
            model->setSort(6,Qt::DescendingOrder);
            model->select();
        }

    });

    connect(btnCreate,&QPushButton::clicked,this,[=]()
    {

        //this->addData();
        Crd crd(this);
        connect(&crd,&Crd::record,this,&ViewPad::addData);

        crd.exec();
        //crd->exec();

        qDebug()<<installCount->text();




    });
    connect(viewTable,&QTableView::customContextMenuRequested,this,&ViewPad::showRightMenu);
    lineSrhText->installEventFilter(this);

}

void ViewPad::initWidgets()
{
    btnCreate=new QPushButton(QIcon(":/image/create"),tr("创建"),this);
    btnNext=new QPushButton(QIcon(":image/right"),tr("下一月"),this);
    btnPrv=new QPushButton(QIcon(":image/left"),tr("上一月"),this);
    btnThis=new QPushButton(QIcon(":image/back"),tr("本月"),this);
    btnSrh=new QPushButton(QIcon(":/image/search"),tr("查询"),this);
    lineSrhText=new QLineEdit(this);
    lineSrhText->setPlaceholderText(tr("输入查询内容..."));
    lineSrhText->setFocus();
    installCount=new QLabel(this);
    //installCount->setText(QString("test"));
    installCount->setStyleSheet("color:red");


}
void ViewPad::initLayout()
{
    vLayout=new QHBoxLayout;
    vLayout->addWidget(btnCreate);
    vLayout->addWidget(btnPrv);
    vLayout->addWidget(btnThis);
    vLayout->addWidget(btnNext);
    vLayout->addWidget(installCount);
    vLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding));
    vLayout->addWidget(lineSrhText);
    vLayout->addWidget(btnSrh);
    hLayout=new QVBoxLayout;
    hLayout->addWidget(viewTable);
    layout=new QVBoxLayout(this);
    layout->addLayout(vLayout);
    layout->addLayout(hLayout);
    this->setLayout(layout);
}

void ViewPad::initView(QSqlTableModel *model)
{
    viewTable=new QTableView(this);
    viewTable->setStyleSheet("color:blue");
    //viewTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    viewTable->setContextMenuPolicy(Qt::CustomContextMenu);
    viewTable->setModel(model);
    viewTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    viewTable->setColumnHidden(model->fieldIndex("ID"),true);
    viewTable->setAutoScroll(true);
    viewTable->horizontalHeader()->setStretchLastSection(true);
    viewTable->setSortingEnabled(true);
    viewTable->verticalHeader()->setVisible(false);
    viewTable->setTextElideMode(Qt::ElideRight);
    viewTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    viewTable->setMouseTracking(true);
    //viewTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    viewTable->horizontalHeader()->setDragEnabled(false);
    //设置当公司名字或详细信息过多的时候tooltip
    connect(viewTable,&QTableView::entered,this,[&](const QModelIndex &index)
    {
        if(index.isValid()&&(index.column()==5||index.column()==4||index.column()==2||index.column()==1))
        {

            QToolTip::showText(QCursor::pos(),index.data().toString());
        }
        QCursor::pos();
    });
    date=date.currentDate();
    model->setFilter(QString("date like '%%1%'").arg(date.toString("yyyy-MM")));
    model->setSort(6,Qt::DescendingOrder);
    model->select();

    // viewTable->selectionModel();

}

void ViewPad::initModel(QSqlDatabase db, const QString &tableName)
{
    model=new QSqlTableModel(this,db);  //replace
    model->setTable(tableName);
    model->setHeaderData(model->fieldIndex("DATE"),Qt::Horizontal,tr("创建日期"),Qt::DisplayRole);
    model->setHeaderData(model->fieldIndex("PRODUCT"),Qt::Horizontal,tr("产品型号"),Qt::DisplayRole);
    model->setHeaderData(model->fieldIndex("SN"),Qt::Horizontal,tr("服务编号"),Qt::DisplayRole);
    model->setHeaderData(model->fieldIndex("SALES"),Qt::Horizontal,tr("销售代表"),Qt::DisplayRole);
    model->setHeaderData(model->fieldIndex("COMPANY"),Qt::Horizontal,tr("公司名称"),Qt::DisplayRole);
    model->setHeaderData(model->fieldIndex("CONTENT"),Qt::Horizontal,tr("详细信息"),Qt::DisplayRole);
    model->setFilter(QString("date like '%%1%'").arg(date.toString("yyyy-MM")));
    //model->setSort(6,Qt::DescendingOrder);
    model->select();

}
void ViewPad::addData(const QList<QStringList> &record)
{

    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    int row=0;
    model->database().transaction();
    // rec<<product->text()<<s<<company->text()<<saler->text()<<content->toPlainText();

    foreach (QStringList var, record) {
        model->insertRows(row,1);
        model->setData(model->index(row,0), QDate::currentDate().toString("yyyy-MM-dd"));
        model->setData(model->index(row,1),var.at(0));
        model->setData(model->index(row,2),var.at(1));
        model->setData(model->index(row,3),var.at(3));
        model->setData(model->index(row,4),var.at(2));
        model->setData(model->index(row,5),var.at(4));
    }
    if(model->submitAll())
    {
        model->database().commit();
    }
    else
    {
        model->database().rollback();
        QMessageBox::critical(this,tr("错误"),model->lastError().text());
    }
    model->revertAll();

}

void ViewPad::showRightMenu(const QPoint &)
{
    if(rightPopupMenu)
    {
        delete rightPopupMenu;
        rightPopupMenu=nullptr;
    }
    rightPopupMenu=new QMenu(this);
    QAction *qCopy=new QAction(tr("&复制"),this);
    QAction *qModify=new QAction(tr("&修改"),this);
    QAction *qDel=new QAction(tr("&删除"),this);
    QAction *qWarranty=new QAction(tr("&保修配置驱动查询下载"),this);

    connect(qCopy,&QAction::triggered,this,&ViewPad::copyData);

    connect(qWarranty,&QAction::triggered,this,&ViewPad::qWarranty);
    connect(qModify,&QAction::triggered,this,&ViewPad::modifyData);
    connect(qDel,&QAction::triggered,this,&ViewPad::delData);
    rightPopupMenu->addAction(qCopy);
    rightPopupMenu->addAction(qModify);
    rightPopupMenu->addAction(qDel);

    rightPopupMenu->addAction(qWarranty);
    rightPopupMenu->exec(QCursor::pos());
}

void ViewPad::copyData()
{
    QModelIndexList indexList =viewTable->selectionModel()->selectedIndexes();


    if(indexList.isEmpty())
    {
        return;
    }
    // int indexRow=0;
    QMultiMap<QString,QString> clipContent;
    QString indexHead,indexData;
    QString table1="<table border=\"1\">";
    QString tr1="<tr>";
    QString tr2="</tr>";
    QString th1="<th>";
    QString th2="</th>";
    QString td1="<td>";
    QString td2="</td>";
    QString table2="</table>";

    QStringList headDataList;//表头
    headDataList.clear();
    int temp=-1;


    foreach (QModelIndex index, indexList) {
        int section=index.column();
        if(section>temp)
        {
            temp=section;
            QString headData=index.model()->headerData(section,Qt::Horizontal).toString();
            headDataList<<headData;

        }

        clipContent.insert(index.model()->headerData(section,Qt::Horizontal).toString(),index.data().toString());
    }
    foreach(QString data,headDataList)
    {
        indexHead.append(th1).append(data).append(th2);
    }

    int headSize=headDataList.count();
    int row=indexList.size()/headSize;
    //qDebug()<<row;

    for(int i=0;i<row;++i)
    {
        indexData.push_back(tr1);
        for(int k=0;k<headSize;++k)
        {
            qDebug()<<"error";
            QStringList data=clipContent.values(headDataList.at(k));
            indexData.append(td1).append(data.at(i)).append(td2);
        }

    }

    qDebug()<<indexList.size();
    indexHead.push_front(tr1);
    indexHead.push_back(tr2);
    table1.push_back(indexHead);

    table1.push_back(indexData);
    table1.push_back(table2);

    QClipboard *clipboard = QApplication::clipboard();
    QMimeData *mimeData=new QMimeData;
    mimeData->setHtml(table1);
    clipboard->setMimeData(mimeData);

}
void ViewPad::delData()
{
    //int curRow = viewTable->currentIndex().row();
    QItemSelectionModel *selections = viewTable->selectionModel(); //返回当前的选择模式
    QModelIndexList selecteds = selections->selectedIndexes(); //返回所有选定的模型项目索引列表
    int ok = QMessageBox::warning(this,tr("删除当前所有选中行!"),tr("你确定"
                                                           "删除选中行吗？"),
                                  QMessageBox::Yes,QMessageBox::No);
    if(ok==QMessageBox::Yes)
    {
        bool ok1;
        QString text=QInputDialog::getText(this,tr("删除"),tr("输入删除密码"),QLineEdit::Password,"",&ok1,Qt::Dialog|Qt::WindowCloseButtonHint);

        if(ok1)
        {

            if(text!="dell123!!")
                return;
            model->database().transaction();
            foreach (QModelIndex index, selecteds)
            {
                int curRow = index.row(); //
                model->removeRow(curRow);
            }
            model->submitAll();
            model->database().commit();


        }

    }
    else
        model->revertAll();
    model->setFilter(QString("date like '%%1%'").arg(date.toString("yyyy-MM")));
    model->select();


}
//ViewPad::~ViewPad()
//{
//model->database().removeDatabase(model->database().connectionName());
// qDebug()<<"DD"<<model->lastError().text();
//model->database().close();;

//}


void ViewPad::qWarranty()
{
    QString data=viewTable->currentIndex().data().toString();
    QWebEngineView *view=new QWebEngineView;
    view->setUrl(QUrl(QString("http://www.dell.com/support/home/cn/zh/cnbsd1/product-support/servicetag/%1/configuration").arg(data)));
    view->resize(1024, 768);
    view->show();
}
void ViewPad::modifyData()
{

    bool ok;
    QModelIndex index=viewTable->currentIndex();

    QString text=QInputDialog::getText(this,tr("修改内容"),tr("输入修改内容"),QLineEdit::Normal,index.data().toString(),&ok,Qt::Dialog|Qt::WindowCloseButtonHint);
    if(ok)
    {
        if(text.isEmpty()||text==index.data().toString())
            return;
        model->setData(index,text);
        model->submitAll();
    }

}

bool ViewPad::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == lineSrhText) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            //qDebug()<<keyEvent->key();
            if(keyEvent->key()==Qt::Key_Return)
            {
                emit btnSrh->clicked(true);
                qDebug()<<keyEvent->key();
                return true;
            }
            return false;
        } else {
            return false;
        }
    } else {
        // pass the event on to the parent class
        return QWidget::eventFilter(watched, event);
    }

}
