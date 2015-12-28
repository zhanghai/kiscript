//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#include "expression_parser.h"

#include "lexical_parser.h"
#include "syntactic_parser_utils.h"

// PrimaryExpression = Keyword(this)|Identifier|NullLiteral|BooleanLiteral
//         |NumericLiteral|StringLiteral|ArrayLiteral|ObjectLiteral|Expression
//
// GRAMMAR:
// PrimaryExpression :
//     this
//     Identifier
//     Literal
//     ArrayLiteral
//     ObjectLiteral
//     ( Expression )
token_t *primary_expression(GPtrArray *input, gsize *position_p) {

    token_t *token;
    if (token_get(input, *position_p, &token)) {
        if (keyword_is_keyword_with_id(token, KEYWORD_THIS)) {
            return token;
        }
        if (token->id == TOKEN_LEXICAL_IDENTIFIER) {
            return token;
        }
        if (token->id == TOKEN_LEXICAL_NULL_LITERAL
            || token->id == TOKEN_LEXICAL_BOOLEAN_LITERAL
            || token->id == TOKEN_LEXICAL_NUMERIC_LITERAL
            || token->id == TOKEN_LEXICAL_STRING_LITERAL) {
            return token;
        }
        return_token_if_is_first(input, position_p, array_literal)
        return_token_if_is_first(input, position_p, object_literal)
        if (token_get_consume_free_punctuator_with_id(input, position_p,
                PUNCTUATOR_PARENTHESIS_LEFT)) {
            token_t *expression_token = expression(input, position_p);
            if (token_get_consume_free_punctuator_with_id(input, position_p,
                    PUNCTUATOR_PARENTHESIS_RIGHT)) {
                return expression_token;
            }
        }
    }

    // TODO: Error!
    return NULL;
}

gboolean array_literal_is_first(GPtrArray *input, gsize position) {
    return token_get_is_first_punctuator_with_id(input, position,
            PUNCTUATOR_SQUARE_BRACKET_LEFT);
}

/*
 * TODO: AST Specification.
 *
 * GRAMMAR:
 * TODO: Grammar.
 */
token_t *array_literal(GPtrArray *input, gsize *position_t) {
    // TODO
    return NULL;
}

gboolean object_literal_is_first(GPtrArray *input, gsize position) {
    return token_get_is_first_punctuator_with_id(input, position,
                                                 PUNCTUATOR_CURLY_BRACE_LEFT);
}

token_t *object_literal(GPtrArray *input, gsize *position_t) {
    // TODO
    return NULL;
}

token_t *expression(GPtrArray *input, gsize *position_t) {
    // TODO
    return NULL;
}
