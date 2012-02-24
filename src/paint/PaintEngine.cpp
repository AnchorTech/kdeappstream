#include "PaintEngine.h"

#include <QDebug>

using namespace KAppStream;

PaintEngine::PaintEngine() :
    QPaintEngine()
{
    qDebug() << "PaintEngine::PaintEngine()";
}

PaintEngine::~PaintEngine()
{
    qDebug() << "PaintEngine::~PaintEngine()";
}

bool PaintEngine::begin(QPaintDevice * pdev)
{
    qDebug() << "PaintEngine::begin(" << pdev << ")";
    return false;
}

void PaintEngine::drawEllipse(const QRectF & rect)
{
    qDebug() << "PaintEngine::drawEllipse(" << rect << ")";
}

void PaintEngine::drawEllipse(const QRect & rect)
{
    qDebug() << "PaintEngine::drawEllipse(" << rect << ")";
}

void PaintEngine::drawImage(const QRectF & rectangle, const QImage & image, const QRectF & sr, Qt::ImageConversionFlags flags)
{
    qDebug() << "PaintEngine::drawImage(" << rectangle << image << sr << flags << ")";
}

void PaintEngine::drawLines(const QLineF * lines, int lineCount)
{
    qDebug() << "PaintEngine::drawLines(" << lines << lineCount << ")";
}

void PaintEngine::drawLines(const QLine * lines, int lineCount)
{
    qDebug() << "PaintEngine::drawLines(" << lines << lineCount << ")";
}

void PaintEngine::drawPath(const QPainterPath & path)
{
    qDebug() << "PaintEngine::drawPath(" << path << ")";
}

void PaintEngine::drawPixmap(const QRectF & r, const QPixmap & pm, const QRectF & sr)
{
    qDebug() << "PaintEngine::drawPixmap(" << r << pm << sr << ")";
}

void PaintEngine::drawPoints(const QPointF * points, int pointCount)
{
    qDebug() << "PaintEngine::drawPoints(" << points << pointCount << ")";
}

void PaintEngine::drawPoints(const QPoint * points, int pointCount)
{
    qDebug() << "PaintEngine::drawPoints(" << points << pointCount << ")";
}

void PaintEngine::drawPolygon(const QPointF * points, int pointCount, PolygonDrawMode mode)
{
    qDebug() << "PaintEngine::drawPolygon(" << points << pointCount << mode << ")";
}

void PaintEngine::drawPolygon(const QPoint * points, int pointCount, PolygonDrawMode mode)
{
    qDebug() << "PaintEngine::drawPolygon(" << points << pointCount << mode << ")";
}

void PaintEngine::drawRects(const QRectF * rects, int rectCount)
{
    qDebug() << "PaintEngine::drawRects(" << rects << rectCount << ")";
}

void PaintEngine::drawRects(const QRect * rects, int rectCount)
{
    qDebug() << "PaintEngine::drawRects(" << rects << rectCount << ")";
}

void PaintEngine::drawTextItem(const QPointF & p, const QTextItem & textItem)
{
    qDebug() << "PaintEngine::drawTextItem(" << p.x() << p.y() << textItem.text() << ")";
}

void PaintEngine::drawTiledPixmap(const QRectF & rect, const QPixmap & pixmap, const QPointF & p)
{
    qDebug() << "PaintEngine::drawTiledPixmap(" << rect << pixmap << p << ")";
}

bool PaintEngine::end()
{
    qDebug() << "PaintEngine::end()";
    return true;
}

PaintEngine::Type PaintEngine::type() const
{
    qDebug() << "PaintEngine::type()";
    return QPaintEngine::SVG;
}

void PaintEngine::updateState (const QPaintEngineState & state)
{
    qDebug() << "PaintEngine::updateState(" << "state" << ")";
}
