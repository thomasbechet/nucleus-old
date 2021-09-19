#include <nucleus/module/lua/common/allocator.h>

void *nulua_allocator(void *ud, void *ptr, size_t osize, size_t nsize)
{
    (void)ud;
    (void)osize;
    if (nsize == 0) {
        if (ptr) {
            nu_free(ptr);
        }
        return NULL;
    } else {
        if (ptr) {
            return nu_realloc(ptr, nsize);
        } else {
            return nu_malloc(nsize);
        }
    }
}