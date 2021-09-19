#ifndef NULUA_INTERFACE_H
#define NULUA_INTERFACE_H

#include <nucleus/nucleus.h>

/* module */
#define NULUA_MODULE_NAME "nucleus-lua"
#define NULUA_MODULE_ID 0x9

/* plugin */
#define NULUA_PLUGIN_NAME "nulua_plugin"
#define NULUA_PLUGIN_INTERFACE_NAME "nulua_plugin_interface"

NU_DECLARE_HANDLE(nulua_plugin_t);

typedef struct {
    nu_result_t (*load_plugin)(const char*, nulua_plugin_t*);
} nulua_plugin_interface_t;

#endif