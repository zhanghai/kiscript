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

token_t *property_assignment(GPtrArray *input, gsize *position_t);

gboolean property_assignment_plain_is_first(GPtrArray *input, gsize position);

token_t *property_assignment_plain(GPtrArray *input, gsize *position_t);

gboolean property_assignment_get_is_first(GPtrArray *input, gsize position);

token_t *property_assignment_get(GPtrArray *input, gsize *position_t);

gboolean property_assignment_set_is_first(GPtrArray *input, gsize position);

token_t *property_assignment_set(GPtrArray *input, gsize *position_t);

gboolean property_name_is_first(GPtrArray *input, gsize position);

token_t *property_name(GPtrArray *input, gsize *position_t);

gboolean left_hand_side_expression_is_left_hand_side_expression(token_t *token);

token_t *assignment_expression(GPtrArray *input, gsize *position_p);

gboolean expression_is_first(GPtrArray *input, gsize position);

token_t *expression(GPtrArray *input, gsize *position_t);

#endif //KISCRIPT_EXPRESSION_PARSER_H
