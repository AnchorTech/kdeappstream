#include "JSONBuilder.h"

#include "paint/PaintDevice.h"
#include "paint/PaintEngine.h"

#include <QString>
#include <QWidget>
#include <QTimer>
#include <QDebug>
#include <QByteArray>
#include <QBuffer>

using namespace KAppStream;

JSONBuilder * JSONBuilder::m_instance = 0;

JSONBuilder::JSONBuilder(QObject * parent) :
    QObject(parent),
    pd(new PaintDevice),
    _sem(1)
{
    QTimer * t = new QTimer(this);
    t->setInterval(500);
    connect(t, SIGNAL(timeout()), this, SLOT(render()));
    t->start();
}

JSONBuilder * JSONBuilder::instance(QObject * parent)
{
    if (!m_instance)
        m_instance = new JSONBuilder(parent);
    return m_instance;
}

void JSONBuilder::queue(QWidget * widget)
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

QString JSONBuilder::ellipse(const QRect & r)
{
    return QString("\"ellipse\":{\"x\":%1,\"y\":%2,\"w\":%3,\"h\":%4}").arg(r.x()).arg(r.y()).arg(r.width()).arg(r.height());
}

QString JSONBuilder::ellipse(const QRectF & r)
{
    return QString("\"ellipse\":{\"x\":%1,\"y\":%2,\"w\":%3,\"h\":%4}").arg(r.x()).arg(r.y()).arg(r.width()).arg(r.height());
}

QString JSONBuilder::line(const QLine & l)
{
    return QString("\"line\":{\"xs\":%1,\"ys\":%2,\"xe\":%3,\"ye\":%4}").arg(l.x1()).arg(l.y1()).arg(l.x2()).arg(l.y2());
}

QString JSONBuilder::line(const QLineF & l)
{
    return QString("\"line\":{\"xs\":%1,\"ys\":%2,\"xe\":%3,\"ye\":%4}").arg(l.x1()).arg(l.y1()).arg(l.x2()).arg(l.y2());
}

QString JSONBuilder::pixmap(const QRectF & r, const QPixmap & pm, const QRectF & sr)
{
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    pm.copy(sr.toRect()).save(&buffer, "PNG");
    return QString("\"pixmap\":{\"x\":%1,\"y\":%2,\"w\":%3,\"h\":%4,\"data\":\"data:image/png;base64," + byteArray.toBase64() + "\"}")
            .arg(r.x())
            .arg(r.y())
            .arg(r.width())
            .arg(r.height());
}

QString JSONBuilder::rect(const QRect & r)
{
    return QString("\"rect\":{\"x\":%1,\"y\":%2,\"w\":%3,\"h\":%4}").arg(r.x()).arg(r.y()).arg(r.width()).arg(r.height());
}

QString JSONBuilder::rect(const QRectF & r)
{
    return QString("\"rect\":{\"x\":%1,\"y\":%2,\"w\":%3,\"h\":%4}").arg(r.x()).arg(r.y()).arg(r.width()).arg(r.height());
}

QString JSONBuilder::state(const QPaintEngineState & s)
{
    QStringList result;

    QPaintEngine::DirtyFlags f = s.state();
    if (f & QPaintEngine::DirtyPen)
        result += "";
    if (f & QPaintEngine::DirtyBrush)
        result += "";
    if (f & QPaintEngine::DirtyBrushOrigin)
        result += "";
    if (f & QPaintEngine::DirtyFont)
        result += "";
    if (f & QPaintEngine::DirtyBackground)
        result += QString("\"bgmode\":%1").arg(s.backgroundMode());
    if (f & QPaintEngine::DirtyBackgroundMode)
        result += "";
    if (f & QPaintEngine::DirtyTransform)
        result += "";
    if (f & QPaintEngine::DirtyClipRegion)
        result += "";
    if (f & QPaintEngine::DirtyClipPath)
        result += "";
    if (f & QPaintEngine::DirtyHints)
        result += "";
    if (f & QPaintEngine::DirtyCompositionMode)
        result += "";
    if (f & QPaintEngine::DirtyClipEnabled)
        result += "";
    if (f & QPaintEngine::DirtyOpacity)
        result += QString("\"opacity\":%1").arg(s.opacity());
    return "\"state\":{" + result.join(",") + "}";
}

void JSONBuilder::render()
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
    _sem.release();
}
