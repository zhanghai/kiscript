//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#ifndef KISCRIPT_UTILS_H_H
#define KISCRIPT_UTILS_H_H

#include <glib.h>

#define DEFINE_ENUM_NEW(enum_name) \
    enum_name##_t *enum_name##_new(enum_name##_t value) { \
        enum_name##_t *value_p = g_new(enum_name##_t, 1); \
        *value_p = value; \
        return value_p; \
    }

typedef gpointer (*clone_func_t)(gpointer);
typedef void (*free_func_t)(gpointer);

#endif //KISCRIPT_UTILS_H_H
