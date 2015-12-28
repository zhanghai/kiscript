//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#include "expression_parser.h"

#include "syntactic_parser_utils.h"

// PrimaryExpression = Keyword(this)|Identifier|NullLiteral|BooleanLiteral
//         |NumericLiteral|StringLiteral|ArrayLiteral|ObjectLiteral|Expression
//
// STANDARD:
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
        if (array_literal_is_first(input, *position_p)) {
            return array_literal(input, position_p);
        }
        if (object_literal_is_first(input, *position_p)) {
            return object_literal(input, position_p);
        }
        if (punctuator_is_punctuator_with_id(token,
                                             PUNCTUATOR_PARENTHESIS_LEFT)) {
            token_consume(input, position_p);
            token_t *expression_token = expression(input, position_p);
            if (token_get(input, *position_p, &token)) {
                if (punctuator_is_punctuator_with_id(token,
                        PUNCTUATOR_PARENTHESIS_RIGHT)) {
                    token_consume(input, position_p);
                    return expression_token;
                }
            }
        }
    }

    // TODO: Error!
    return NULL;
}

gboolean array_literal_is_first(GPtrArray *input, gsize position) {
    token_t *token;
    return token_get(input, position, &token)
           && punctuator_is_punctuator_with_id(token,
                                               PUNCTUATOR_SQUARE_BRACKET_LEFT);
}

/*
 * TODO: AST Specification.
 *
 * STANDARD:
 * TODO: Standard.
 */
token_t *array_literal(GPtrArray *input, gsize *position_t) {
    // TODO
    return NULL;
}

gboolean object_literal_is_first(GPtrArray *input, gsize position) {
    token_t *token;
    return token_get(input, position, &token)
           && punctuator_is_punctuator_with_id(token,
                                               PUNCTUATOR_CURLY_BRACE_LEFT);
    return FALSE;
}

token_t *object_literal(GPtrArray *input, gsize *position_t) {
    // TODO
    return NULL;
}

token_t *expression(GPtrArray *input, gsize *position_t) {
    // TODO
    return NULL;
}
