#ifndef ABSTRACTFUNCTIONOVERWRITER_H
#define ABSTRACTFUNCTIONOVERWRITER_H

#include <QString>
#include <QList>

#if defined(_M_X64) || defined(__amd64) || defined(__x86_64)
    #define ARCH_64
#elif defined(_M_IX86) || defined(__i386__)
    #define ARCH_X86
#endif

class AbstractFunctionOverwriter
{
    public:

        virtual ~AbstractFunctionOverwriter();

        bool overwriteFunction(const QString & originalFunction,
                               void * const replacementFunction);

    protected:

        virtual bool unprotectMemory(void * memory, size_t size) = 0;

        virtual bool reprotectMemory(void * memory, size_t size) = 0;

        virtual bool writeShortJump(void * target, void * const function);

        virtual bool writeLongJump(void * target, void * const function);

        virtual void * getMemoryNearAddress(void * const address, size_t size);

        virtual void * createTrampoline(void * const function, void * const replacement);

        virtual bool getAddressRange(size_t & min, size_t & max) = 0;

        virtual bool isMemoryFree(void * const memory, size_t size) = 0;

        virtual void * reserveMemory(void * memory, size_t size) = 0;

        virtual bool commitMemory(void * memory, size_t size) = 0;

        virtual void * qtCoreFunctionLookup(const QString & function) = 0;

        virtual long pagesize() const = 0;

        virtual size_t blocksize();

    private:

        void * pageAlign(void * address) const;

        size_t roundToNextPage(size_t address) const;

        struct MemorySegment
        {
            void * memory;
            size_t size;
            size_t free;
        };

        QList<MemorySegment> memoryPool;
};

#endif // ABSTRACTFUNCTIONOVERWRITER_H
