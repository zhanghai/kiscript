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

    token_t *token = token_get(input, *position_p);
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

gboolean array_literal_is_first(GPtrArray *input, gsize position) {
    return token_is_first_punctuator(input, position,
                                     PUNCTUATOR_SQUARE_BRACKET_LEFT);
}

/*
 * AST:
 * TODO
 *
 * GRAMMAR:
 * TODO
 */

//#define CHARACTER_ELISION ','
token_t *array_literal(GPtrArray *input, gsize *position_t) {
    // TODO

    while (/*not ended*/1) {
        if (/*begin with ElementList*/1) {
            tokenize_and_return_if_is_first(input, position_t, element_list)
        }
        else if (/*begin with CHARACTER_ELISION*/1) {
            /*move pointer to read next token*/
        }
    }

    return NULL;
}

gboolean element_list_is_first(GPtrArray *input, gsize position) {
    /*return blahblah... */return 0;
}

token_t *element_list(GPtrArray *input, gsize *position_t) {

    return NULL;
}

gboolean object_literal_is_first(GPtrArray *input, gsize position) {
    return token_is_first_punctuator(input, position,
                                     PUNCTUATOR_CURLY_BRACE_LEFT);
}

token_t *object_literal(GPtrArray *input, gsize *position_t) {
    // TODO
    return NULL;
}

token_t *assignment_expression(GPtrArray *input, gsize *position_p) {
    // TODO
    return NULL;
}

token_t *expression(GPtrArray *input, gsize *position_t) {
    // TODO
    return NULL;
}
