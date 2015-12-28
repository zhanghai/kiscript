//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#ifndef KISCRIPT_SYNTACTIC_PARSER_UTILS_H
#define KISCRIPT_SYNTACTIC_PARSER_UTILS_H

#include "parser.h"

gboolean token_get(GPtrArray *input, gsize position, token_t **token_p);

gboolean token_consume(GPtrArray *input, gsize *position_p);

// Be recursive first, see if we need a non-recursive version? Or a
// _nonrecursive?
#define token_consume_free token_consume_free_recursive

gboolean token_consume_free_recursive(GPtrArray *input, gsize *position_p);

#endif //KISCRIPT_SYNTACTIC_PARSER_UTILS_H
