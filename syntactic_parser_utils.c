//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#include "lexical_parser.h"
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

gboolean token_consume_no_free(GPtrArray *input, gsize *position_p) {
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

gboolean token_get_is_first_punctuator_with_id(GPtrArray *input,
                                               gsize position,
                                               punctuator_id_t punctuator_id) {
    token_t *token;
    return token_get(input, position, &token)
           && punctuator_is_punctuator_with_id(token, punctuator_id);
}

gboolean token_get_consume_free_punctuator_with_id(GPtrArray *input,
        gsize *position_p, punctuator_id_t punctuator_id) {
    return token_get_is_first_punctuator_with_id(input, *position_p,
                                                 punctuator_id)
           && token_consume_free(input, position_p);
}
