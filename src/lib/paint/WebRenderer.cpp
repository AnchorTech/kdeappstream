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

using namespace KAppStream;

WebRenderer * WebRenderer::m_instance = 0;

WebRenderer::WebRenderer(QObject * parent) :
    QObject(parent),
    pd(new PaintDevice),
    _sem(1)
{
    QTimer * t = new QTimer(this);
    t->setInterval(100);
    connect(t, SIGNAL(timeout()), this, SLOT(render()));
    t->start();
}

WebRenderer * WebRenderer::instance(QObject * parent)
{
    if (!m_instance)
        m_instance = new WebRenderer(parent);
    return m_instance;
}

void WebRenderer::queue(QWidget * widget, QPaintEvent * event)
{
    _sem.acquire();
    if (widget->isVisible() && !_render.contains(widget))
    {
        _render.enqueue(widget);
        _regions.enqueue(event->region());
        _rects.enqueue(event->rect());
    }
    _sem.release();
}

void WebRenderer::render()
{
    if (!_sem.tryAcquire())
        return;
    QQueue<QWidget*> tmp = _render;
    QQueue<QRegion> tmp1 = _regions;
    QQueue<QRect> tmp2 = _rects;
    _render.clear();
    _regions.clear();
    _rects.clear();
    _sem.release();
    if (!tmp.isEmpty())
    {
        QPainter p(this->pd);
        do
        {
            QWidget * w = tmp.first();
            JSONBuilder::instance()->beginRender(w, tmp1.first(), tmp2.first());
            w->render(&p, QPoint(), QRegion(), QWidget::DrawWindowBackground);
            tmp.dequeue();
            tmp1.dequeue();
            tmp2.dequeue();
            JSONBuilder::instance()->endRender();

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
        while (!tmp.isEmpty());
        p.end();
        JSONBuilder::instance()->finish();
    }
}
