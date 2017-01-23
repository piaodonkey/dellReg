#pragma execution_character_set("utf-8")
#include <QApplication>
#include "viewpad.h"
#include "crd.h"

int main(int argc,char **argv)
{
    QApplication app(argc,argv);

    ViewPad pad;
    Crd crd;
    //crd.show();
     pad.showMaximized();
    // pad.show();
    return app.exec();
}
