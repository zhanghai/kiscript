//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#ifndef KISCRIPT_SYNTACTIC_PARSER_UTILS_H
#define KISCRIPT_SYNTACTIC_PARSER_UTILS_H

#include "lexical_parser.h"

token_t *token_get(GPtrArray *input, gsize position);

gboolean token_consume(GPtrArray *input, gsize *position_p);

gboolean token_match_id_clone(GPtrArray *input, gsize *position_p,
                              token_id_t token_id, token_t **token_clone_p);

gboolean token_is_first_keyword(GPtrArray *input, gsize position,
                                keyword_id_t keyword_id);

gboolean token_match_keyword(GPtrArray *input, gsize *position_p,
                             keyword_id_t keyword_id);

gboolean token_is_first_punctuator(GPtrArray *input, gsize position,
                                   punctuator_id_t punctuator_id);

gboolean token_match_punctuator(GPtrArray *input, gsize *position_p,
                                punctuator_id_t punctuator_id);

#define return_if_error(token_or_error) \
    if (error_is_error((token_or_error))) {\
        return (token_or_error); \
    }

#define return_and_free_if_error(token_or_error, token_to_free) \
    if (error_is_error((token_or_error))) {\
        token_free(&(token_to_free)); \
        return (token_or_error); \
    }

#define tokenize_and_return_if_is_first(input, position_p, token_name) \
    if (token_name##_is_first((input), *(position_p))) { \
        return token_name((input), (position_p));\
    }

#define tokenize_and_return_or_return_error(input, position_p, tokenize_func) \
    { \
        token_t *token_or_error = tokenize_func((input), (position_p)); \
        return_if_error(child_token_or_error) \
        token_add_child(parent_token, child_token_or_error); \
    }

#define tokenize_and_add_child_or_free_parent_and_return_error(input, \
        position_p, tokenize_func, parent_token) \
    { \
        token_t *child_token_or_error = tokenize_func((input), (position_p)); \
        return_and_free_if_error(child_token_or_error, parent_token) \
        token_add_child(parent_token, child_token_or_error); \
    }

#define match_token_id_clone_and_add_child_or_free_parent_and_return_error( \
        input, position_p, token_id, parent_token, error_id) \
    { \
        token_t *child_token_clone; \
        if (!token_match_id_clone((input), (position_p), (token_id), \
                &child_token_clone)) { \
            token_free(&parent_token); \
            return error_new_syntactic((error_id), *(position_p)); \
        } \
        token_add_child((parent_token), child_token_clone); \
    }

#define match_keyword_or_return_error(input, position_p, keyword_id, error_id) \
    if (!token_match_keyword((input), (position_p), (keyword_id))) { \
        return error_new_syntactic((error_id), *(position_p)); \
    }

#define match_keyword_or_free_and_return_error(input, position_p, keyword_id, \
        token_to_free, error_id) \
    if (!token_match_keyword((input), (position_p), (keyword_id))) { \
        token_free(&(token_to_free)); \
        return error_new_syntactic((error_id), *(position_p)); \
    }

#define match_punctuator_or_return_error(input, position_p, punctuator_id, \
    error_id) \
    if (!token_match_punctuator((input), (position_p), (punctuator_id))) { \
        return error_new_syntactic((error_id), *(position_p)); \
    }

#define match_punctuator_or_free_and_return_error(input, position_p, \
        punctuator_id, token_to_free, error_id) \
    if (!token_match_punctuator((input), (position_p), (punctuator_id))) { \
        token_free(&(token_to_free)); \
        return error_new_syntactic((error_id), *(position_p)); \
    }

#endif //KISCRIPT_SYNTACTIC_PARSER_UTILS_H
