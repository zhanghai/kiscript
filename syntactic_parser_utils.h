//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#ifndef KISCRIPT_SYNTACTIC_PARSER_UTILS_H
#define KISCRIPT_SYNTACTIC_PARSER_UTILS_H

#include "parser.h"

gboolean token_get(GPtrArray *input, gsize position, token_t **token_p);

gboolean token_consume_no_free(GPtrArray *input, gsize *position_p);

// Be recursive first, see if we need a non-recursive version? Or a
// _nonrecursive?
#define token_consume_free token_consume_free_recursive

gboolean token_consume_free_recursive(GPtrArray *input, gsize *position_p);

gboolean token_get_is_first_punctuator_with_id(GPtrArray *input,
                                               gsize position,
                                               punctuator_id_t punctuator_id);

gboolean token_get_consume_free_punctuator_with_id(GPtrArray *input,
        gsize *position_p, punctuator_id_t punctuator_id);


#define return_token_if_is_first(input, position_p, token_name) \
    if (token_name##_is_first((input), *(position_p))) { \
        return token_name((input), (position_p));\
    }


#endif //KISCRIPT_SYNTACTIC_PARSER_UTILS_H
