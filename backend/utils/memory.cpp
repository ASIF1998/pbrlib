#include <backend/utils/memory.hpp>

#include <cassert>
#include <cstdlib>

namespace pbrlib::backend::utils
{
    void* allocAligned(size_t size)
    {
#if defined (_WIN32) || defined (_WIN64)
        return _aligned_malloc(size, PBRLIB_L1_CACHE_LINE_SIZE);
#else
        void* vptr;
        assert(!posix_memalign(&vptr, PBRLIB_L1_CACHE_LINE_SIZE, size));
        return vptr;  
#endif
    }

    void freeAligned(void* ptr)
    {
#if defined (_WIN32) || defined (_WIN64)
        _aligned_free(ptr);
#else
        free(ptr);
#endif
    }
}
