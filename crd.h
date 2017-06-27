#ifndef CRD_H
#define CRD_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QCompleter>
#include <QStringListModel>
#include <QComboBox>

class Crd : public QDialog
{
    Q_OBJECT
public:
    Crd(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
signals:
    void record(const QList<QStringList> &records);
private:
    QPushButton *sumbit;
    QTextEdit *content;
    QLineEdit *seriaNumber;
    QLineEdit  *product,*saler,*company;
    //QComboBox *product,*saler,*company;
    //QLineEdit *seriaNumber;
    QStringList sumbitList;
    void initWidgets();
    void commit();
    void initCompleter();
    void editCompleter(const QString &completer);
    QCompleter *completer;
    QStringList word_list;
    QStringListModel *model;
private slots:
     void salerCompleter();
     void productCompleter();
     void companyCompleter();
protected:
    bool eventFilter(QObject *watched, QEvent *event);

};

#endif // CRD_H
