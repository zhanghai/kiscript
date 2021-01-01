//
// Copyright (c) 2016 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#ifndef KISCRIPT_INTERPRETER_H
#define KISCRIPT_INTERPRETER_H

#include "glib_utils.h"
#include "utils.h"

typedef struct {
    GHashTable *properties;
} object_t;

extern GString *PROPERTY_NAME___PROTOTYPE__;

object_t *object_new();

gboolean object_has_property(GString *name);

object_t *object_get_property(GString *name);

void *object_set_property(GString *name, object_t *value);

void *object_remove_property(GString *name);

void object_free(object_t *object);

typedef struct {
    object_t object;
    gboolean value;
} boolean_object_t;

typedef struct {
    object_t object;
    gdouble value;
} number_object_t;

typedef struct {
    object_t object;
    GString value;
} string_object_t;

#endif //KISCRIPT_INTERPRETER_H
