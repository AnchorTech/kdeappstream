#include "WebRenderer.h"
#include "JSONBuilder.h"
#include "PaintDevice.h"
#include "hooking/WidgetsCollection.h"

#include <QString>
#include <QWidget>
#include <QTimer>
#include <QDebug>
#include <QByteArray>
#include <QBuffer>
#include <QPainter>
#include <QFrame>
#include <QStyle>
#include <QApplication>
#include <QPaintEvent>
#include <QTextEdit>
#include <QDateTime>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>

using namespace KAppStream;

WebRenderer * WebRenderer::m_instance = 0;

WebRenderer::WebRenderer(QObject * parent) :
    QObject(parent),
    pd(new PaintDevice),
    renderSemaphore(1),
    currentRenderingWidget(0)
{
    t = new QTimer(this);
    t->setSingleShot(true);
    connect(t, SIGNAL(timeout()), this, SLOT(render()));
}

WebRenderer * WebRenderer::instance(QObject * parent)
{
    if (!m_instance)
        m_instance = new WebRenderer(parent);
    return m_instance;
}

void WebRenderer::queue(QWidget * widget, QPaintEvent * event)
{
    if (currentRenderingWidget != widget)
    {
        QRect r = event->rect();
        QRegion rg = event->region();
        if (!r.isValid())
        {
            if(!rg.isEmpty())
                r = rg.boundingRect();
            else
                r = QRect();
        }
        Widget w(widget, r);
        if (_render.contains(w))
            _render.removeAll(w);
        _render.enqueue(w);
        if (renderSemaphore.available())
        {
            renderSemaphore.acquire();
            t->start(50);
        }
    }
    else
    {
        currentRenderingWidget = 0;
    }
}

void WebRenderer::dequeue(QWidget * widget)
{
    QCoreApplication::removePostedEvents(widget);
    _render.removeAll(Widget(widget));
}

void WebRenderer::render()
{
    while (!_render.isEmpty())
    {
        Widget w = _render.dequeue();
        currentRenderingWidget = w.w;

//        if (dynamic_cast<QGraphicsView*>(w.w))
//        {
//            QGraphicsView * view = (QGraphicsView*)w.w;
//            qDebug() << view;
//            if (view->scene())
//            {
//                qDebug() << view->sceneRect();
//                qDebug() << view->scene();
//                foreach (QGraphicsItem * item, view->scene()->items())
//                    qDebug() << item << item->pos() << item->scenePos();
//            }
//        }
//        qDebug() << w.w << w.w->graphicsProxyWidget() << w.w->parentWidget() << w.w->focusProxy();

        if (WidgetsCollection::instance()->contains(w.w))
        {
            JSONBuilder::instance()->beginRender(w.w, w.rect);
            w.w->render(pd, w.rect.topLeft(), QRegion(w.rect), 0);
            JSONBuilder::instance()->endRender();
            JSONBuilder::instance()->finish();
        }
    }

    renderSemaphore.release();


    //            static bool f = true;
    //            QFrame * frame = dynamic_cast<QFrame*>(w);
    //            if (f && frame)
//            {
//                qDebug() << QApplication::style() << frame->frameRect() << frame->rect();
//                for (int i = 0; i < 60; ++i)
//                {
//                    QIcon ic = QApplication::style()->standardIcon((QStyle::StandardPixmap)i);
//                    if (!ic.isNull())
//                    {
//                        if (ic.availableSizes().count() > 0)
//                        {

//                            QImage im(ic.availableSizes()[ic.availableSizes().count()-1], QImage::Format_ARGB32);
//                            im.fill(0xFFFFFFFF);
//                            QPainter p(&im);
//                            p.drawPixmap(QPoint(0,0), ic.pixmap(ic.availableSizes()[ic.availableSizes().count()-1]));
//                            p.end();
//                            im.save("/home/coder89/dupa" + QString::number(i) + ".jpg");
//                        }
//                        else
//                            qDebug() << "no sizes available!";
//                    }
//                    else
//                        qDebug() << "is null";
//                }
//                f = false;
//            }
}
