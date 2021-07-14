#include <nucleus/core/utils/array.h>

#include <nucleus/core/memory/memory.h>

struct nu_array_t_ {
    uint32_t length;
    uint32_t capacity;
    uint32_t object_size;
    void *data;
    void *swap_space;
};

nu_array_t *nu_array_new(uint32_t object_size)
{
    NU_ASSERT(object_size >= 0);

    nu_array_t *array = (nu_array_t*)nu_malloc(sizeof(nu_array_t));
    array->length      = 0;
    array->capacity    = 5;
    array->object_size = object_size;
    array->data        = nu_malloc(object_size * array->capacity);
    array->swap_space  = NULL;
    return array;
}
void nu_array_delete(nu_array_t *array)
{
    if (array->swap_space) {
        nu_free(array->swap_space);
    }
    nu_free(array->data);
    nu_free(array);
}
void nu_array_clear(nu_array_t *array)
{
    array->length = 0;
}
void *nu_array_get(const nu_array_t *array, uint32_t index)
{
    NU_ASSERT(index >= 0 && index < array->length);

    return array->data + array->object_size * index;
}
void *nu_array_get_last(const nu_array_t *array)
{
    NU_ASSERT(array->length > 0);

    return array->data + array->object_size * (array->length - 1);
}
uint32_t nu_array_get_length(const nu_array_t *array)
{
    return array->length;
}
uint32_t nu_array_get_capacity(const nu_array_t *array)
{
    return array->capacity;
}
void nu_array_push(nu_array_t *array, const void *object)
{
    if (array->length >= array->capacity) {
        array->capacity *= 2;
        array->data = nu_realloc(array->data, array->capacity * array->object_size);
    }
    memcpy(array->data + array->object_size * array->length++, object,
        array->object_size);
}
bool nu_array_pop(nu_array_t *array)
{
    if (array->length > 0) {
        array->length--;
        return true;
    }
    return false;
}
void nu_array_swap_last(nu_array_t *array, uint32_t index)
{
    NU_ASSERT(index >= 0 && index < array->length);

    if (index != array->length - 1) {
        /* create swap space is required */
        if (!array->swap_space) {
            array->swap_space = nu_malloc(array->object_size);
        }
        
        /* copy object to the swap space */
        memcpy(array->swap_space, array->data + index * array->object_size, array->object_size);
        /* copy last to index */
        memcpy(array->data + index * array->object_size,
            array->data + (array->length - 1) * array->object_size, array->object_size);
        /* copy swap space to last */
        memcpy(array->data + (array->length - 1) * array->object_size, array->swap_space, array->object_size);
    }
}