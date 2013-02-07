#include "widget.h"
#include <QApplication>
#include <QDir>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Widget w;
	w.setWindowIcon( QIcon(":/icons/icon-64.ico") );
    w.show();
    
    return a.exec();
}
