#include "FunctionOverwriterFactory.h"
#include "UnixFunctionOverwriter.h"

AbstractFunctionOverwriter * FunctionOverwriterFactory::overwriter = 0;

AbstractFunctionOverwriter * FunctionOverwriterFactory::createFunctionOverwriter()
{
#ifdef Q_OS_WIN

#else
    if (!overwriter)
        overwriter = new UnixFunctionOverwriter();
#endif
    return overwriter;
}
