//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#include "syntactic_parser_utils.h"

static token_t *token_get_unchecked(GPtrArray *input, gsize position) {
    return g_ptr_array_index(input, position);
}

static void token_set_unchecked(GPtrArray *input, gsize position,
                                token_t *token) {
    g_ptr_array_index(input, position) = token;
}

gboolean token_get(GPtrArray *input, gsize position, token_t **token_p) {
    if (g_ptr_array_in_range(input, position)) {
        *token_p = token_get_unchecked(input, position);
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

gboolean token_consume_free_recursive(GPtrArray *input, gsize *position_p) {
    if (g_ptr_array_in_range(input, *position_p)) {
        token_free_recursive(token_get_unchecked(input, *position_p));
        token_set_unchecked(input, *position_p, NULL);
        ++(*position_p);
        return TRUE;
    } else {
        return FALSE;
    }
}
