#ifndef PAINTDEVICE_H
#define PAINTDEVICE_H

#include <QPaintDevice>

namespace KAppStream
{
    class PaintDevice : public QPaintDevice
    {
        public:

            explicit PaintDevice();
            virtual QPaintEngine * paintEngine () const;

    };
}

#endif // PAINTDEVICE_H
