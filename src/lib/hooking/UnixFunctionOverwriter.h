#ifndef UNIXFUNCTIONOVERWRITER_H
#define UNIXFUNCTIONOVERWRITER_H

#include <QtCore/QtGlobal>

#if !defined(Q_OS_WIN)

#include "AbstractFunctionOverwriter.h"

class FunctionOverwriterFactory;

class UnixFunctionOverwriter : public AbstractFunctionOverwriter
{
      protected:

        virtual bool unprotectMemory(void *mem, size_t size);
        virtual bool reprotectMemory(void *mem, size_t size);

        virtual bool getAddressRange(size_t &min, size_t &max);

        virtual bool isMemoryFree(void * const mem, size_t size);
        virtual void *reserveMemory(void *mem, size_t size);
        virtual bool commitMemory(void *mem, size_t size);

        virtual void *qtCoreFunctionLookup(const QString &function);
        virtual long pagesize() const;

      private:

        UnixFunctionOverwriter();

        long m_pagesize;

        friend class FunctionOverwriterFactory;
};
#endif

#endif // UNIXFUNCTIONOVERWRITER_H
