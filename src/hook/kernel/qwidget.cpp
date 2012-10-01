#include <QWidget>
#include <QApplication>
#include <stdio.h>
#include <dlfcn.h>
#include <QDebug>

QT_BEGIN_NAMESPACE

typedef QWidget * QWidgetFindMethod(WId);
static QWidgetFindMethod * findMethod = 0;
static bool triedOnce = false;

QWidget * QWidget::find(WId id)
{
    exit(1);

    if (!findMethod && !triedOnce) {
        triedOnce = (QWidgetFindMethod*)dlsym(RTLD_NEXT, "_ZN7QWidget4findEm");
        triedOnce = true;
    }

    if (findMethod) {
        QWidget * w = findMethod(id);
        if (w->windowType() == Qt::Desktop)
        {
            w = qApp->allWidgets()[0];
        }
        return w;
    } else {
        return 0;
    }
}

QT_END_NAMESPACE
