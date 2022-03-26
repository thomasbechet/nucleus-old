/* Generated file: DO NOT EDIT ! */
#ifndef NULUA_MODULE_DEFINITION_H
#define NULUA_MODULE_DEFINITION_H

#include <nucleus/nucleus.h>
#include <nucleus/module/lua/plugin/types_public.h>

/* module */
#define NULUA_MODULE_NAME "nucleus-lua"
#define NULUA_MODULE_ID 9

/* interface */
#define NULUA_PLUGIN_INTERFACE_NAME "nulua_plugin_interface"

typedef struct {
    nu_result_t (*load)(const char*, nulua_plugin_t*);
} nulua_plugin_interface_t;


/* plugin */
#define NULUA_MANAGER_PLUGIN_NAME "nulua_manager_plugin"

#endif