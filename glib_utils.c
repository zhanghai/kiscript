//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#include "glib_utils.h"

GString *g_string_clone(GString *string) {
    return g_string_new_len(string->str, string->len);
}

gboolean g_ptr_array_in_range(GPtrArray *array, gsize position) {
    return position < array->len;
}
