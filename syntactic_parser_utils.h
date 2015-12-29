//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#ifndef KISCRIPT_SYNTACTIC_PARSER_UTILS_H
#define KISCRIPT_SYNTACTIC_PARSER_UTILS_H

#include "lexical_parser.h"

gboolean token_get(GPtrArray *input, gsize position, token_t **token_p);

gboolean token_consume_no_free(GPtrArray *input, gsize *position_p);

gboolean token_consume(GPtrArray *input, gsize *position_p);

gboolean token_is_first_punctuator(GPtrArray *input, gsize position,
                                   punctuator_id_t punctuator_id);

gboolean token_match_free_punctuator(GPtrArray *input, gsize *position_p,
                                     punctuator_id_t punctuator_id);


typedef token_t *(*tokenize_func_t)(GPtrArray *input, gsize *position_p);

gboolean token_tokenize(GPtrArray *input, gsize *position_p,
                        tokenize_func_t tokenize_func, token_t **token_p);


#define return_token_if_is_first(input, position_p, token_name) \
    if (token_name##_is_first((input), *(position_p))) { \
        return token_name((input), (position_p));\
    }

#endif //KISCRIPT_SYNTACTIC_PARSER_UTILS_H
