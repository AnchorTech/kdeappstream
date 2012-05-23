#ifndef PAINTDEVICE_H
#define PAINTDEVICE_H

#include <QObject>
#include <QPaintDevice>

namespace KAppStream
{
    class PaintEngine;

    class PaintDevice : public QObject, public QPaintDevice
    {
            Q_OBJECT

            mutable PaintEngine * engine;

        public:

            explicit PaintDevice();

            virtual ~PaintDevice();

            virtual QPaintEngine * paintEngine() const;

        public slots:

            void render(QWidget *);

        protected:

            virtual int metric(PaintDeviceMetric metric) const;

    };
}

#endif // PAINTDEVICE_H
