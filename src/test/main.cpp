#include <QtGui/QApplication>

#include "TestWindow.h"
#include "events/EventFilter.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    TestWindow w;
    w.show();
    app.exec();
}
