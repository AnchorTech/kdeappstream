#include <QtGui/QApplication>

#include "TestWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    TestWindow w;
    w.show();
    app.exec();
}
