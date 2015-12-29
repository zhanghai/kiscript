//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#ifndef KISCRIPT_EXPRESSION_PARSER_H
#define KISCRIPT_EXPRESSION_PARSER_H

#include "parser.h"

token_t *primary_expression(GPtrArray *input, gsize *position_p);

gboolean array_literal_is_first(GPtrArray *input, gsize position);

token_t *array_literal(GPtrArray *input, gsize *position_t);

gboolean object_literal_is_first(GPtrArray *input, gsize position);

token_t *object_literal(GPtrArray *input, gsize *position_t);

token_t *assignment_expression(GPtrArray *input, gsize *position_p);

token_t *expression(GPtrArray *input, gsize *position_t);

//============================================
token_t *assignment_expression(GPtrArray *input, gsize *position_t);

token_t *conditional_expression(GPtrArray *input, gsize *position_t);

token_t *conditional_expression_is_first(GPtrArray *input, gsize position);

#endif //KISCRIPT_EXPRESSION_PARSER_H
