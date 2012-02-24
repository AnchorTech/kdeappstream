#include "PaintDevice.h"
#include "PaintEngine.h"

#include <QDebug>

using namespace KAppStream;

PaintDevice::PaintDevice() :
    QPaintDevice(),
    engine(new PaintEngine)
{
    qDebug() << "PaintDevice::PaintDevice()";
}

PaintDevice::~PaintDevice()
{
    qDebug() << "PaintDevice::~PaintDevice()";
    delete engine;
}

QPaintEngine * PaintDevice::paintEngine() const
{
    qDebug() << "PaintDevice::paintEngine()";
    return engine;
}
