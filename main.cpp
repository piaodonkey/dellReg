#include <QApplication>
#include "viewpad.h"
#include "crd.h"

int main(int argc,char **argv)
{
    QApplication app(argc,argv);

    ViewPad pad;
    Crd crd;
    //crd.show();
     pad.show();
    return app.exec();
}
