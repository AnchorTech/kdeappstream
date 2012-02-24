#ifndef PAINTDEVICE_H
#define PAINTDEVICE_H

#include <QPaintDevice>

namespace KAppStream
{
    class PaintEngine;

    class PaintDevice : public QPaintDevice
    {
            PaintEngine * engine;

        public:

            explicit PaintDevice();

            virtual ~PaintDevice();

            virtual QPaintEngine * paintEngine() const;

    };
}

#endif // PAINTDEVICE_H
