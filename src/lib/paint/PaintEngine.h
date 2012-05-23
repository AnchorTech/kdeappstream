#ifndef PAINTENGINE_H
#define PAINTENGINE_H

#include <QPaintEngine>

namespace KAppStream
{
    class PaintEngine : public QPaintEngine
    {
        public:

            explicit PaintEngine();
            virtual ~PaintEngine();
            virtual bool begin(QPaintDevice * pdev);
            virtual void drawEllipse(const QRectF & rect);
            virtual void drawEllipse(const QRect & rect);
            virtual void drawImage(const QRectF & rectangle, const QImage & image, const QRectF & sr, Qt::ImageConversionFlags flags = Qt::AutoColor);
            virtual void drawLines(const QLineF * lines, int lineCount);
            virtual void drawLines(const QLine * lines, int lineCount);
            virtual void drawPath(const QPainterPath & path);
            virtual void drawPixmap(const QRectF & r, const QPixmap & pm, const QRectF & sr);
            virtual void drawPoints(const QPointF * points, int pointCount);
            virtual void drawPoints(const QPoint * points, int pointCount);
            virtual void drawPolygon(const QPointF * points, int pointCount, PolygonDrawMode mode);
            virtual void drawPolygon(const QPoint * points, int pointCount, PolygonDrawMode mode);
            virtual void drawRects (const QRectF * rects, int rectCount);
            virtual void drawRects (const QRect * rects, int rectCount);
            virtual void drawTextItem(const QPointF & p, const QTextItem & textItem);
            virtual void drawTiledPixmap(const QRectF & rect, const QPixmap & pixmap, const QPointF & p);
            virtual bool end();
            virtual Type type() const;
            virtual void updateState (const QPaintEngineState & state);
    };
}

#endif // PAINTENGINE_H
