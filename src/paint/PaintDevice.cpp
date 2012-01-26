#include "PaintDevice.h"

using namespace KAppStream;

PaintDevice::PaintDevice() :
    QPaintDevice()
{
}

QPaintEngine * PaintDevice::paintEngine() const
{
}
