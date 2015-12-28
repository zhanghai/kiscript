//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#include "syntactic_parser_utils.h"

gboolean token_get(GPtrArray *input, gsize position, token_t **token_p) {
    if (g_ptr_array_in_range(input, position)) {
        *token_p = g_ptr_array_index(input, position);
        return TRUE;
    } else {
        return FALSE;
    }
}

gboolean token_consume(GPtrArray *input, gsize *position_p) {
    if (g_ptr_array_in_range(input, *position_p)) {
        ++(*position_p);
        return TRUE;
    } else {
        return FALSE;
    }
}
