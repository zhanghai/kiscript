//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#ifndef KISCRIPT_SYNTACTIC_PARSER_UTILS_H
#define KISCRIPT_SYNTACTIC_PARSER_UTILS_H

#include "parser.h"

gboolean token_get(GPtrArray *input, gsize position, token_t **token_p);

gboolean token_consume(GPtrArray *input, gsize *position_p);

#endif //KISCRIPT_SYNTACTIC_PARSER_UTILS_H
