#include "WebRenderer.h"
#include "JSONBuilder.h"
#include "PaintDevice.h"

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
        Widget w(widget, event->region(), event->rect());
        _render.enqueue(w);
        if (renderSemaphore.available())
            t->start();
    }
    else
    {
        if (dynamic_cast<QTextEdit*>(currentRenderingWidget))
            qDebug() << "Disable render current widget";
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
    if (!renderSemaphore.tryAcquire())
        return;

    int i = 0;

    while (!_render.isEmpty())
    {
        Widget w = _render.dequeue();
        JSONBuilder::instance()->beginRender(w.w, w.region, w.rect);
        currentRenderingWidget = w.w;
        if (dynamic_cast<QTextEdit*>(currentRenderingWidget))
            qDebug() << "Render current widget";
        w.w->render(pd, QPoint(), QRegion(), 0);
        JSONBuilder::instance()->endRender();
        ++i;
    }

    renderSemaphore.release();

    if (i)
    {
        JSONBuilder::instance()->finish();
    }


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
