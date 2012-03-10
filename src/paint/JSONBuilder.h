#ifndef JSONBUILDER_H
#define JSONBUILDER_H

#include <QRect>
#include <QRectF>
#include <QLine>
#include <QLineF>
#include <QObject>
#include <QQueue>
#include <QSemaphore>
#include <QPixmap>
#include <QPaintEngineState>

namespace KAppStream
{
    class PaintDevice;

    class JSONBuilder : public QObject
    {
            Q_OBJECT

            PaintDevice * pd;
            static JSONBuilder * m_instance;
            QQueue<QWidget*> _render;
            QSemaphore _sem;

            JSONBuilder(QObject * parent = 0);

        public:

            static JSONBuilder * instance(QObject * parent = 0);
            void queue(QWidget * widget);
            static QString color(const QColor & c);
            static QString ellipse(const QRect & r);
            static QString ellipse(const QRectF & r);
            static QString font(const QFont & f);
            static QString image(const QImage & i);
            static QString line(const QLine & l);
            static QString line(const QLineF & l);
            static QString pixmap(const QPixmap & pm);
            static QString pixmap(const QRectF & r, const QPixmap & pm, const QRectF & sr);
            static QString rect(const QRect & r);
            static QString rect(const QRectF & r);
            static QString state(const QPaintEngineState & s);
            static QString pen(const QPen & p);
            static QString brush(const QBrush & b);
            static QString gradient(const QGradient & g);
            static QString transform(const QTransform & t);

        public slots:

            void render();
    };
}

#endif // JSONBUILDER_H
