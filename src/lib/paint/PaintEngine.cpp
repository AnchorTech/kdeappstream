#include "PaintEngine.h"

#include "paint/JSONBuilder.h"

#include <QDebug>
#include <QByteArray>
#include <QBuffer>

using namespace KAppStream;

PaintEngine::PaintEngine() :
    QPaintEngine()
{
}

PaintEngine::~PaintEngine()
{
}

bool PaintEngine::begin(QPaintDevice * pdev)
{
    setPaintDevice(pdev);
    return true;
}

void PaintEngine::drawEllipse(const QRectF & rect)
{
    JSONBuilder::instance()->ellipse(rect);
}

void PaintEngine::drawEllipse(const QRect & rect)
{
    JSONBuilder::instance()->ellipse(rect);
}

void PaintEngine::drawImage(const QRectF & rectangle, const QImage & image, const QRectF & sr, Qt::ImageConversionFlags flags)
{
    JSONBuilder::instance()->image(rectangle, image, sr, flags);
}

void PaintEngine::drawLines(const QLineF * lines, int lineCount)
{
    while (lineCount--)
        JSONBuilder::instance()->line(*(lines++));
}

void PaintEngine::drawLines(const QLine * lines, int lineCount)
{
    while (lineCount--)
        JSONBuilder::instance()->line(*(lines++));
}

void PaintEngine::drawPath(const QPainterPath & path)
{
    JSONBuilder::instance()->path(path);
}

void PaintEngine::drawPixmap(const QRectF & r, const QPixmap & pm, const QRectF & sr)
{
    JSONBuilder::instance()->pixmap(r, pm, sr);
}

void PaintEngine::drawPoints(const QPointF * points, int pointCount)
{
    JSONBuilder::instance()->points(points, pointCount);
}

void PaintEngine::drawPoints(const QPoint * points, int pointCount)
{
    JSONBuilder::instance()->points(points, pointCount);
}

void PaintEngine::drawPolygon(const QPointF * points, int pointCount, PolygonDrawMode mode)
{
    JSONBuilder::instance()->polygon(points, pointCount, mode);
}

void PaintEngine::drawPolygon(const QPoint * points, int pointCount, PolygonDrawMode mode)
{
    JSONBuilder::instance()->polygon(points, pointCount, mode);
}

void PaintEngine::drawRects(const QRectF * rects, int rectCount)
{
    while (rectCount--)
        JSONBuilder::instance()->rect(*(rects++));
}

void PaintEngine::drawRects(const QRect * rects, int rectCount)
{
    while (rectCount--)
        JSONBuilder::instance()->rect(*(rects++));
}

void PaintEngine::drawTextItem(const QPointF & p, const QTextItem & textItem)
{
    JSONBuilder::instance()->text(p, textItem);
}

void PaintEngine::drawTiledPixmap(const QRectF & rect, const QPixmap & pixmap, const QPointF & p)
{
    JSONBuilder::instance()->tiledPixmap(rect, pixmap, p);
}

bool PaintEngine::end()
{
    setPaintDevice(0);
    return true;
}

PaintEngine::Type PaintEngine::type() const
{
    return QPaintEngine::Raster;
}

void PaintEngine::updateState (const QPaintEngineState & state)
{
    JSONBuilder::instance()->state(state);
}
