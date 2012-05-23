#include "PaintDevice.h"
#include "PaintEngine.h"

#include <QDebug>
#include <QWidget>

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
    engine->begin(const_cast<PaintDevice*>(this));
    return engine;
}

void PaintDevice::render(QWidget * widget)
{
    QPainter p(this);
    widget->render(&p);
    p.end();
    qDebug() << "rendering widget";
}

int PaintDevice::metric(PaintDeviceMetric metric) const
{
    switch(metric)
    {
        case QPaintDevice::PdmWidth:
            return 9999;
        case QPaintDevice::PdmHeight:
            return 9999;
        case QPaintDevice::PdmWidthMM:
            return 9999;
        case QPaintDevice::PdmHeightMM:
            return 9999;
        case QPaintDevice::PdmNumColors:
            return 9999;
        case QPaintDevice::PdmDepth:
            return 32;
        case QPaintDevice::PdmDpiX:
            return 1;
        case QPaintDevice::PdmDpiY:
            return 1;
        case QPaintDevice::PdmPhysicalDpiX:
            return 1;
        case QPaintDevice::PdmPhysicalDpiY:
            return 1;
    }
    return -1;
}
