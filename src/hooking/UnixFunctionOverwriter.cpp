#include "UnixFunctionOverwriter.h"

#if !defined(Q_OS_WIN)

#define __STDC_LIMIT_MACROS

#include <dlfcn.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/errno.h>

#include <iostream>

using namespace std;

UnixFunctionOverwriter::UnixFunctionOverwriter()
{
  m_pagesize = sysconf(_SC_PAGESIZE);
}

bool UnixFunctionOverwriter::unprotectMemory(void *mem, size_t size)
{
  Q_ASSERT(!((size_t)mem & (pagesize() - 1)));
  Q_ASSERT(!((size_t)size & (pagesize() - 1)));
  const bool writable = (mprotect(mem, size, PROT_READ|PROT_WRITE|PROT_EXEC) == 0);
  Q_ASSERT(writable);
  return writable;
}

bool UnixFunctionOverwriter::reprotectMemory(void *mem, size_t size)
{
  Q_ASSERT(!((size_t)mem & (pagesize() - 1)));
  Q_ASSERT(!((size_t)size & (pagesize() - 1)));
  const bool readOnly = (mprotect(mem, size, PROT_READ|PROT_EXEC) == 0);
  Q_ASSERT(readOnly);
  return readOnly;
}

bool UnixFunctionOverwriter::getAddressRange(size_t &min, size_t &max)
{
  min = INTPTR_MIN;
  max = INTPTR_MAX;

  return true;
}

bool UnixFunctionOverwriter::isMemoryFree(void * const mem, size_t size)
{
  Q_ASSERT(!((size_t)mem & (pagesize() - 1)));
  Q_ASSERT(!((size_t)size & (pagesize() - 1)));
  Q_UNUSED(mem);
  Q_UNUSED(size);

  //under unix there is no easy way to find out if a region is free or not

  return true;
}

void *UnixFunctionOverwriter::reserveMemory(void *mem, size_t size)
{
  Q_ASSERT(!((size_t)mem & (pagesize() - 1)));
  Q_ASSERT(!((size_t)size & (pagesize() - 1)));

  // reserve a memory region with a hint and hope that it is close to the other address
  void *retmem = mmap(mem, size, PROT_READ|PROT_EXEC, MAP_ANON | MAP_PRIVATE, -1, 0);

  if (retmem == MAP_FAILED) {
    return 0;
  }

  return retmem;
}

bool UnixFunctionOverwriter::commitMemory(void *mem, size_t size)
{
  Q_ASSERT(!((size_t)mem & (pagesize() - 1)));
  Q_ASSERT(!((size_t)size & (pagesize() - 1)));
  Q_UNUSED(mem);
  Q_UNUSED(size);

  //under unix we don't have a commit
  return true;
}

void *UnixFunctionOverwriter::qtCoreFunctionLookup(const QString &function)
{
  return dlsym(RTLD_NEXT, function.toLatin1());
}

long UnixFunctionOverwriter::pagesize() const
{
  return m_pagesize;
}

#endif // Q_OS_WIN
