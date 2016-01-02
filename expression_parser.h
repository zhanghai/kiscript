//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#ifndef KISCRIPT_EXPRESSION_PARSER_H
#define KISCRIPT_EXPRESSION_PARSER_H

#include "parser.h"

token_t *primary_expression(GPtrArray *input, gsize *position_p);

gboolean array_literal_is_first(GPtrArray *input, gsize position);

token_t *array_literal(GPtrArray *input, gsize *position_p);

gboolean object_literal_is_first(GPtrArray *input, gsize position);

token_t *object_literal(GPtrArray *input, gsize *position_p);

token_t *property_assignment(GPtrArray *input, gsize *position_p);

gboolean property_assignment_plain_is_first(GPtrArray *input, gsize position);

token_t *property_assignment_plain(GPtrArray *input, gsize *position_p);

gboolean property_assignment_get_is_first(GPtrArray *input, gsize position);

token_t *property_assignment_get(GPtrArray *input, gsize *position_p);

gboolean property_assignment_set_is_first(GPtrArray *input, gsize position);

token_t *property_assignment_set(GPtrArray *input, gsize *position_p);

gboolean property_name_is_first(GPtrArray *input, gsize position);

token_t *property_name(GPtrArray *input, gsize *position_p);

gboolean callable_expression_is_first(GPtrArray *input, gsize position);

token_t *callable_expression(GPtrArray *input, gsize *position_p);

gboolean argument_list_is_first(GPtrArray *input, gsize position);

token_t *argument_list(GPtrArray *input, gsize *position_p);

gboolean new_expression_is_first(GPtrArray *input, gsize position);

token_t *new_expression(GPtrArray *input, gsize *position_p);

gboolean left_hand_side_expression_is_first(GPtrArray *input, gsize position);

token_t *left_hand_side_expression(GPtrArray *input, gsize *position_p);

gboolean postfix_expression_is_first(GPtrArray *input, gsize position);

token_t *postfix_expression(GPtrArray *input, gsize *position_p);

gboolean multiplicative_expression_is_first(GPtrArray *input, gsize position);

token_t *multiplicative_expression(GPtrArray *input, gsize *position_p);

gboolean additive_expression_is_first(GPtrArray *input, gsize position);

token_t *additive_expression(GPtrArray *input, gsize *position_p);

gboolean shift_expression_is_first(GPtrArray *input, gsize position);

token_t *shift_expression(GPtrArray *input, gsize *position_p);

gboolean relational_expression_is_first(GPtrArray *input, gsize position);

token_t *relational_expression(GPtrArray *input, gsize *position_p);

gboolean equality_expression_is_first(GPtrArray *input, gsize position);

token_t *equality_expression(GPtrArray *input, gsize *position_p);

gboolean bitwise_and_expression_is_first(GPtrArray *input, gsize position);

token_t *bitwise_and_expression(GPtrArray *input, gsize *position_p);

gboolean bitwise_xor_expression_is_first(GPtrArray *input, gsize position);

token_t *bitwise_xor_expression(GPtrArray *input, gsize *position_p);

gboolean bitwise_or_expression_is_first(GPtrArray *input, gsize position);

token_t *bitwise_or_expression(GPtrArray *input, gsize *position_p);

gboolean logical_and_expression_is_first(GPtrArray *input, gsize position);

token_t *logical_and_expression(GPtrArray *input, gsize *position_p);

gboolean logical_or_expression_is_first(GPtrArray *input, gsize position);

token_t *logical_or_expression(GPtrArray *input, gsize *position_p);

gboolean conditional_expression_is_first(GPtrArray *input, gsize position);

token_t *conditional_expression(GPtrArray *input, gsize *position_p);

gboolean assignment_expression_is_first(GPtrArray *input, gsize position);

token_t *assignment_expression(GPtrArray *input, gsize *position_p);

gboolean expression_is_first(GPtrArray *input, gsize position);

token_t *expression(GPtrArray *input, gsize *position_p);

#endif //KISCRIPT_EXPRESSION_PARSER_H
