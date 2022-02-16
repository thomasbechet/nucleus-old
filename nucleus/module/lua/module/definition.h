/* Generated file: DO NOT EDIT ! */
#ifndef NULUA_MODULE_DEFINITION_H
#define NULUA_MODULE_DEFINITION_H

#include <nucleus/nucleus.h>
#include <nucleus/module/lua/plugin/types_public.h>

/* module */
#define NULUA_MODULE_NAME "nucleus-lua"
#define NULUA_MODULE_ID 9

/* interface */
#define NULUA_MANAGER_INTERFACE_NAME "nulua_manager_interface"

typedef struct {
    nu_result_t (*load_plugin)(const char*, nulua_plugin_t*);
} nulua_manager_interface_t;


/* plugin */
#define NULUA_MANAGER_PLUGIN_NAME "nulua_manager_plugin"

#endif