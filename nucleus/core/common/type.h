#ifndef NU_TYPE_H
#define NU_TYPE_H

typedef uint32_t nu_id_t;

typedef void* nu_ptr_t;
typedef void* nu_pfn_t;

typedef enum {
    NU_TYPE_INT32,
    NU_TYPE_UINT32,
    NU_TYPE_BOOL,
    NU_TYPE_FLOAT,
    NU_TYPE_DOUBLE,
    NU_TYPE_STRING
} nu_type_t;

#endif