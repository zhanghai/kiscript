//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#ifndef KISCRIPT_EXPRESSION_PARSER_H
#define KISCRIPT_EXPRESSION_PARSER_H

#include "lexical_parser.h"

token_t *primary_expression(GPtrArray *input, gsize *position_p);

gboolean array_literal_is_first(GPtrArray *input, gsize position);

token_t *array_literal(GPtrArray *input, gsize *position_t);

gboolean object_literal_is_first(GPtrArray *input, gsize position);

token_t *object_literal(GPtrArray *input, gsize *position_t);

token_t *expression(GPtrArray *input, gsize *position_t);

#endif //KISCRIPT_EXPRESSION_PARSER_H