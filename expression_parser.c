//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#include "expression_parser.h"

#include "lexical_parser.h"
#include "syntactic_parser_utils.h"

/*
 * AST:
 * PrimaryExpression = Keyword(this)|Identifier|NullLiteral|BooleanLiteral
 *         |NumericLiteral|StringLiteral|ArrayLiteral|ObjectLiteral|Expression
 *
 * GRAMMAR:
 * PrimaryExpression :
 *     this
 *     Identifier
 *     Literal : NullLiteral|BooleanLiteral|NumericLiteral|StringLiteral
 *     ArrayLiteral
 *     ObjectLiteral
 *     ( Expression )
 */
token_t *primary_expression(GPtrArray *input, gsize *position_p) {

    token_t *token = token_get_or_null(input, *position_p);
    if (token) {
        if (keyword_is_keyword_with_id(token, KEYWORD_THIS)
            || token->id == TOKEN_LEXICAL_IDENTIFIER
            || token->id == TOKEN_LEXICAL_NULL_LITERAL
            || token->id == TOKEN_LEXICAL_BOOLEAN_LITERAL
            || token->id == TOKEN_LEXICAL_NUMERIC_LITERAL
            || token->id == TOKEN_LEXICAL_STRING_LITERAL) {
            return token_clone(token);
        }
    }

    tokenize_and_return_if_is_first(input, position_p, array_literal)

    tokenize_and_return_if_is_first(input, position_p, object_literal)

    if (token_match_punctuator(input, position_p,
                               PUNCTUATOR_PARENTHESIS_LEFT)) {
        token_t *expression_token_or_error = expression(input, position_p);
        return_if_error(expression_token_or_error)
        match_punctuator_or_free_and_return_error(input, position_p,
                PUNCTUATOR_PARENTHESIS_RIGHT, expression_token_or_error,
                ERROR_EXPRESSION_PRIMARY_EXPRESSION_PARENTHESIS_LEFT)
        return expression_token_or_error;
    }

    return error_new_syntactic(ERROR_EXPRESSION_PRIMARY_EXPRESSION,
                               *position_p);
}

/*
 * AST:
 * ArrayLiteral - AssignmentExpression*
 *
 * GRAMMAR:
 * NONSTANDARD: Disallow multiple or trailing comma. Anyway JSON disallows this.
 * ArrayLiteral :
 *     [ AssignmentExpression (, AssignmentExpression)* ]
 * STANDARD:
 * ArrayLiteral :
 *     [ ElementList? ,* ]
 * ElementList :
 *     ,* AssignmentExpression (,+ AssignmentExpression)*
 */

gboolean array_literal_is_first(GPtrArray *input, gsize position) {
    return token_is_first_punctuator(input, position,
                                     PUNCTUATOR_SQUARE_BRACKET_LEFT);
}

token_t *array_literal(GPtrArray *input, gsize *position_t) {

    match_punctuator_or_return_error(input, position_t,
            PUNCTUATOR_SQUARE_BRACKET_LEFT,
            ERROR_EXPRESSION_ARRAY_EXPRESSION_SQUARE_BRACKET_LEFT)

    token_t *array_literal_token = token_new_no_data(
            TOKEN_EXPRESSION_ARRAY_LITERAL);

    gboolean first = TRUE;
    while (!token_match_punctuator(input, position_t,
                                   PUNCTUATOR_SQUARE_BRACKET_RIGHT)) {

        if (first) {
            first = FALSE;
        } else {
            match_punctuator_or_free_and_return_error(input, position_t,
                    PUNCTUATOR_COMMA, array_literal_token,
                    ERROR_EXPRESSION_ARRAY_EXPRESSION_COMMA)
        }

        tokenize_and_add_child_or_free_parent_and_return_error(input,
                position_t, assignment_expression, array_literal_token)
    }

    return array_literal_token;
}

gboolean object_literal_is_first(GPtrArray *input, gsize position) {
    // TODO
    return FALSE;
}

token_t *object_literal(GPtrArray *input, gsize *position_t);

gboolean left_hand_side_expression_is_left_hand_side_expression(
        token_t *token) {
    // TODO
    return FALSE;
}

token_t *assignment_expression(GPtrArray *input, gsize *position_p);

gboolean expression_is_first(GPtrArray *input, gsize position) {
    // TODO
    return FALSE;
}

token_t *expression(GPtrArray *input, gsize *position_t);

