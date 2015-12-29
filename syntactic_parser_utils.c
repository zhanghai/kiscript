//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#include "lexical_parser.h"
#include "syntactic_parser_utils.h"

static token_t *token_get_unchecked(GPtrArray *input, gsize position) {
    return g_ptr_array_index(input, position);
}

token_t *token_get(GPtrArray *input, gsize position) {
    if (g_ptr_array_in_range(input, position)) {
        return token_get_unchecked(input, position);
    } else {
        return NULL;
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

gboolean token_match_id_clone(GPtrArray *input, gsize *position_p,
                              token_id_t token_id, token_t **token_clone_p) {
    token_t *token = token_get(input, *position_p);
    if (token && token->id == token_id) {
        *token_clone_p = token_clone(token);
        return TRUE;
    } else {
        return FALSE;
    }
}

gboolean token_is_first_keyword(GPtrArray *input, gsize position,
                                keyword_id_t keyword_id) {
    token_t *token = token_get(input, position);
    return token && keyword_is_keyword_with_id(token, keyword_id);
}

gboolean token_match_keyword(GPtrArray *input, gsize *position_p,
                             keyword_id_t keyword_id) {
    return token_is_first_keyword(input, *position_p, keyword_id)
           && token_consume(input, position_p);
}

gboolean token_is_first_punctuator(GPtrArray *input, gsize position,
                                   punctuator_id_t punctuator_id) {
    token_t *token = token_get(input, position);
    return token && punctuator_is_punctuator_with_id(token, punctuator_id);
}

gboolean token_match_punctuator(GPtrArray *input, gsize *position_p,
                                punctuator_id_t punctuator_id) {
    return token_is_first_punctuator(input, *position_p, punctuator_id)
           && token_consume(input, position_p);
}
