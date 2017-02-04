
#include "crd.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLayout>
#include <QGroupBox>
#include <QLabel>
#include <QRegExp>
#include <QDate>
#include <QCompleter>
#include <QStringListModel>
#include <QJsonDocument>
#include <QJsonParseError>

Crd::Crd(QWidget *parent , Qt::WindowFlags f ):QDialog(parent)
{

    this->setWindowTitle(QString("工作记录表").append(QDate::currentDate().toString(" yyyy年MMMMdd日 dddd")));
    this->setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    initWidgets();
    initCompleter();
    connect(sumbit,&QPushButton::clicked,this,&Crd::commit);
    seriaNumber->installEventFilter(this);

}
void Crd::initWidgets()
{
    sumbit=new QPushButton("提交",this);
    product=new QLineEdit(this);
    seriaNumber=new QLineEdit(this);
    seriaNumber->setPlaceholderText("多个序列号以空格分开...");
    saler=new QLineEdit(this);
    company=new QLineEdit(this);
    content=new QTextEdit(this);
    QHBoxLayout *psLay=new QHBoxLayout;
    psLay->addWidget(new QLabel("产品型号:"));
    psLay->addWidget(product);
    psLay->addWidget(new QLabel("服务编号:"));
    psLay->addWidget(seriaNumber);
    psLay->setAlignment(Qt::AlignLeft);


    QHBoxLayout *csLay=new QHBoxLayout;
    csLay->addWidget(new QLabel("公司名称:"));
    csLay->addWidget(company);
    csLay->addWidget(new QLabel("销售代表:"));
    csLay->addWidget(saler);
    csLay->setAlignment(Qt::AlignLeft);

    QVBoxLayout  *layout=new QVBoxLayout(this);
    layout->addLayout(psLay);
    layout->addLayout(csLay);
    //layout->addWidget(new QLabel("内容详情:"));
    QGroupBox *box=new QGroupBox(tr("内容详情"),this);
    QVBoxLayout *conLay=new QVBoxLayout;
    conLay->addWidget(content);
    box->setLayout(conLay);
    layout->addWidget(box);
    layout->addWidget(sumbit);
    this->setLayout(layout);

}

void Crd::commit()
{
    if(product->text().isEmpty()||seriaNumber->text().isEmpty()||saler->text().isEmpty()||company->text().isEmpty())
   // if(product->currentText().isEmpty()||seriaNumber->text().isEmpty()||saler->currentText().isEmpty()||company->currentText().isEmpty())
        return;
    QMultiHash<QString,QStringList> data;

    QStringList sn=seriaNumber->text().split(QRegExp(" "));
    QList<QStringList> record1;
    foreach (QString s, sn) {
        if(!s.isEmpty()&&s!=" ")
        {
            QStringList rec;
            rec<<product->text().toUpper()<<s.toUpper()<<company->text()<<saler->text()<<content->toPlainText();
            record1<<rec;
        }
    }
    if(record1.isEmpty())
        return;
    emit record(record1);
    this->close();
}

void Crd::initCompleter()
{
    QFile file("Json.dat");
    if(!file.open(QIODevice::ReadWrite))
    {
       
        return;

    }
    QJsonDocument jsonDocument=QJsonDocument::fromBinaryData(file.readAll());
    word_list=jsonDocument.toVariant().toStringList();

    model=new QStringListModel(word_list,this);
    completer=new QCompleter(word_list,this);
    completer->setCompletionMode(QCompleter::InlineCompletion);

    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setModel(model);
    //    QLineEdit *seriaNumber, *product,*saler,*company;

    // seriaNumber->setCompleter(completer);
    product->setCompleter(completer);
    saler->setCompleter(completer);
    company->setCompleter(completer);
    file.flush();
    file.close();
    connect(product,&QLineEdit::editingFinished,this,&Crd::productCompleter);
    connect(saler,&QLineEdit::editingFinished,this,&Crd::salerCompleter);
    connect(company,&QLineEdit::editingFinished,this,&Crd::companyCompleter);
}
void Crd::productCompleter()
{
    editCompleter(product->text());
}
void Crd::salerCompleter()
{
    editCompleter(saler->text());
}
void Crd::companyCompleter()
{
    editCompleter(company->text());
}

void Crd::editCompleter(const QString &s)
{
    QString text=s;
    if(QString::compare(text,QString(""))!=0)
    {
        bool is_contains=word_list.contains(text,Qt::CaseInsensitive);
        if(is_contains)
            return;
        word_list<<text;
        model->setStringList(word_list);
        QJsonDocument json=QJsonDocument::fromVariant(word_list);
        QFile file("Json.dat");
        if(!(file.open(QIODevice::ReadWrite|QIODevice::Truncate)))
        {
            return;
        }
        file.write(json.toBinaryData());
        //file.write(json.toJson());
        // qDebug()<<json.toJson();
        file.flush();
        file.close();
    }

}

bool Crd::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == seriaNumber) {
             if (event->type() == QEvent::KeyPress) {
                 QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
                 // qDebug()<<keyEvent->key();
                 if(keyEvent->key()==Qt::Key_Return)
                 {
                    
                     seriaNumber->setText(seriaNumber->text().append(" "));
                    return true;
                 }
                     return false;
             } else {
                 return false;
             }
         } else {
             // pass the event on to the parent class
             return QDialog::eventFilter(watched, event);
         }

}
