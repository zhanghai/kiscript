//
// Copyright (c) 2016 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#include "interpreter.h"

GString *PROPERTY_NAME___PROTOTYPE__;

static void object_init(object_t *object) {
    object->properties = g_hash_table_new(g_string_hash_func,
                                          g_string_equal_func);
}

object_t *object_new() {
    object_t *object = g_new_1(object_t);
    object_init(object);
    return object;
}

void object_free(object_t *object) {
    g_hash_table_destroy(object->properties);
    g_free(object);
}

void interpreter_init() {
    PROPERTY_NAME___PROTOTYPE__ = g_string_new("__prototype__");
}
