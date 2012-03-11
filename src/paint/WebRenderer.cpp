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

using namespace KAppStream;

WebRenderer * WebRenderer::m_instance = 0;

WebRenderer::WebRenderer(QObject * parent) :
    QObject(parent),
    pd(new PaintDevice),
    _sem(1)
{
    QTimer * t = new QTimer(this);
    t->setInterval(500);
    connect(t, SIGNAL(timeout()), this, SLOT(render()));
    t->start();
}

WebRenderer * WebRenderer::instance(QObject * parent)
{
    if (!m_instance)
        m_instance = new WebRenderer(parent);
    return m_instance;
}

void WebRenderer::queue(QWidget * widget)
{
    if (!_sem.tryAcquire(1))
        return;
    if (widget->isVisible() && !_render.contains(widget))
    {
        _render.enqueue(widget);
        qDebug() << widget;
    }
    _sem.release();
}

void WebRenderer::render()
{
    _sem.acquire();
    QQueue<QWidget*> tmp = _render;
    if (!tmp.isEmpty())
    {
        QPainter p(pd);
        do
        {
            qDebug() << tmp.first();
            tmp.dequeue()->render(&p, QPoint(), QRegion(), QWidget::DrawWindowBackground);
        }
        while (!tmp.isEmpty());
        p.end();
    }
    _render.clear();
    JSONBuilder::instance()->flush();
    _sem.release();
}
