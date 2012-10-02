#include <QApplication>
#include <QWidget>
#include <stdio.h>
#include <dlfcn.h>
#include <QDebug>

QT_BEGIN_NAMESPACE

typedef QWidget * QWidgetFindMethod(WId);
typedef QWidget * QWidget_focusWidget(QWidget *);
typedef QWidget * QApplication_focusWidget();
typedef void QApplication_setActiveWindow(QWidget *);
typedef QWidgetList QApplication_topLevelWidgets();

static QWidgetFindMethod * findMethod = 0;
static QWidget_focusWidget * QWidget_focusWidget_method = 0;
static QApplication_focusWidget * QApplication_focusWidget_method = 0;
static QApplication_setActiveWindow * QApplication_setActiveWindow_method = 0;
static QApplication_topLevelWidgets * QApplication_topLevelWidgets_method = 0;

//QWidget * QWidget::find(WId id)
//{
//    static bool triedOnce = false;

//    if (!findMethod && !triedOnce) {
//        triedOnce = (QWidgetFindMethod*)dlsym(RTLD_NEXT, "_ZN7QWidget4findEm");
//        triedOnce = true;
//    }

//    if (findMethod) {
//        QWidget * w = findMethod(id);
//        if (w->windowType() == Qt::Desktop)
//        {
//            w = qApp->allWidgets()[0];
//        }
//        return w;
//    } else {
//        return 0;
//    }
//}

QWidget * QApplication::focusWidget()
{
    static bool triedOnce = false;

    qDebug() << "Dupa sraka";

    if (!QApplication_focusWidget_method && !QApplication_topLevelWidgets_method && !triedOnce) {
        QWidget_focusWidget_method = (QWidget_focusWidget*)dlsym(RTLD_NEXT, "_ZNK7QWidget11focusWidgetEv");
        QApplication_focusWidget_method = (QApplication_focusWidget*)dlsym(RTLD_NEXT, "_ZN12QApplication11focusWidgetEv");
        QApplication_topLevelWidgets_method = (QApplication_topLevelWidgets*)dlsym(RTLD_NEXT, "_ZN12QApplication15topLevelWidgetsEv");
        triedOnce = true;
    }

    QWidget * result = QApplication_focusWidget_method();

    if (!result)
    {
        QWidgetList list = QApplication_topLevelWidgets_method();
        foreach(QWidget * w, list)
        {
            result = QWidget_focusWidget_method(w);
            if (result)
                break;
        }
    }

    return result;
}

//void QApplication::setActiveWindow(QWidget* act)
//{
//    static bool triedOnce = false;

//    qDebug() << "Dupa sraka";

//    if (!QApplication_setActiveWindow_method && !triedOnce) {
//        QApplication_setActiveWindow_method = (QApplication_setActiveWindow*)dlsym(RTLD_NEXT, "_ZN12QApplication15setActiveWindowEP7QWidget");
//        triedOnce = true;
//    }

//    if (!QApplication_setActiveWindow_method)
//        return;

//    if (act)
//        QApplication_setActiveWindow_method(act);
//}

QT_END_NAMESPACE
