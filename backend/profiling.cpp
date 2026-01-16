#include <backend/profiling.hpp>

#ifdef PBRLIB_ENABLE_PROPFILING

#include <stdexcept>
#include <stdlib.h>

void* operator new(std::size_t count)
{
    auto ptr = malloc(count);

    if (!ptr) [[unlikely]]
        throw std::bad_alloc();

    TracyAlloc(ptr, count);
    return ptr;
}

void operator delete(void* ptr)
{
    TracyFree(ptr);
    free(ptr);
}

#endif
