//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#ifndef KISCRIPT_FUNCTION_PROGRAM_PARSER_H
#define KISCRIPT_FUNCTION_PROGRAM_PARSER_H

#include "parser.h"

gboolean function_declaration_is_first(GPtrArray *input, gsize position);

token_t *function_declaration(GPtrArray *input, gsize *position_p);

gboolean function_expression_is_first(GPtrArray *input, gsize position);

token_t *function_expression(GPtrArray *input, gsize *position_p);

token_t *formal_parameter_list(GPtrArray *input, gsize *position_p);

token_t *function_body(GPtrArray *input, gsize *position_p);

token_t *source_element(GPtrArray *input, gsize *position_p);

#endif //KISCRIPT_FUNCTION_PROGRAM_PARSER_H
