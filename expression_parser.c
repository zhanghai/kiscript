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
 *     [ (AssignmentExpression (, AssignmentExpression)*)? ]
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
            ERROR_EXPRESSION_ARRAY_LITERAL_SQUARE_BRACKET_LEFT)

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
                   ERROR_EXPRESSION_ARRAY_LITERAL_COMMA_OR_SQUARE_BRACKET_RIGHT)
        }

        tokenize_and_add_child_or_free_parent_and_return_error(input,
                position_t, assignment_expression, array_literal_token)
    }

    return array_literal_token;
}

/*
 * AST:
 * ObjectLiteral - PropertyAssignment*
 *
 * GRAMMAR:
 * ObjectLiteral :
 *     { (PropertyAssignment (, PropertyAssignment)*)? }
 */

gboolean object_literal_is_first(GPtrArray *input, gsize position) {
    return token_is_first_punctuator(input, position,
                                     PUNCTUATOR_CURLY_BRACE_LEFT);
}

token_t *object_literal(GPtrArray *input, gsize *position_t) {

    match_punctuator_or_return_error(input, position_t,
            PUNCTUATOR_CURLY_BRACE_LEFT,
            ERROR_EXPRESSION_OBJECT_LITERAL_CURLY_BRACE_LEFT)

    token_t *object_literal_token = token_new_no_data(
            TOKEN_EXPRESSION_OBJECT_LITERAL);

    gboolean first = TRUE;
    while (!token_match_punctuator(input, position_t,
                                   PUNCTUATOR_CURLY_BRACE_RIGHT)) {

        if (first) {
            first = FALSE;
        } else {
            match_punctuator_or_free_and_return_error(input, position_t,
                    PUNCTUATOR_COMMA, object_literal_token,
                    ERROR_EXPRESSION_OBJECT_LITERAL_COMMA_OR_CURLY_BRACE_RIGHT)
        }

        tokenize_and_add_child_or_free_parent_and_return_error(input,
                    position_t, property_assignment, object_literal_token)
    }

    return object_literal_token;
}

/*
 * AST: PropertyAssignment = (PropertyAssignmentPlain|PropertyAssignmentGet
 *         |PropertyAssignmentSet)
 *
 * GRAMMAR:
 * PropertyAssignment :
 *     (PropertyAssignmentPlain|PropertyAssignmentGet|PropertyAssignmentSet)
 */

token_t *property_assignment(GPtrArray *input, gsize *position_t) {

    tokenize_and_return_if_is_first(input, position_t,
                                    property_assignment_plain)
    tokenize_and_return_if_is_first(input, position_t,
                                    property_assignment_get)
    tokenize_and_return_if_is_first(input, position_t,
                                    property_assignment_set)

    return error_new_syntactic(ERROR_EXPRESSION_PROPERTY_ASSIGNMENT,
                               *position_t);
}

/*
 * AST: PropertyAssignmentPlain = PropertyName AssignmentExpression
 *
 * GRAMMAR:
 * PropertyAssignmentPlain :
 *     PropertyName : AssignmentExpression
 */

gboolean property_assignment_plain_is_first(GPtrArray *input, gsize position) {
    return property_name_is_first(input, position);
}

token_t *property_assignment_plain(GPtrArray *input, gsize *position_t) {

    token_t *property_assignment_plain_token = token_new_no_data(
            TOKEN_EXPRESSION_PROPERY_ASSIGNMENT_PLAIN);

    tokenize_and_add_child_or_free_parent_and_return_error(input, position_t,
            property_name, property_assignment_plain_token)

    match_punctuator_or_free_and_return_error(input, position_t,
            PUNCTUATOR_COLON, property_assignment_plain_token,
            ERROR_EXPRESSION_PROPERTY_ASSIGNMENT_PLAIN_COLON)

    tokenize_and_add_child_or_free_parent_and_return_error(input, position_t,
            property_name, property_assignment_plain_token)

    return property_assignment_plain_token;
}

/*
 * AST: PropertyAssignmentGet = PropertyName FunctionBody
 *
 * GRAMMAR:
 * PropertyAssignmentGet :
 *     get PropertyName ( ) { FunctionBody }
 */

gboolean property_assignment_get_is_first(GPtrArray *input, gsize position) {
    return token_is_first_keyword(input, position, KEYWORD_GET);
}

token_t *property_assignment_get(GPtrArray *input, gsize *position_t) {

    match_keyword_or_return_error(input, position_t, KEYWORD_GET,
                                  ERROR_EXPRESSION_PROPERTY_ASSIGNMENT_GET_GET);

    token_t *property_assignment_get_token = token_new_no_data(
            TOKEN_EXPRESSION_PROPERY_ASSIGNMENT_GET);

    tokenize_and_add_child_or_free_parent_and_return_error(input, position_t,
            property_name, property_assignment_get_token)

    match_punctuator_or_free_and_return_error(input, position_t,
            PUNCTUATOR_PARENTHESIS_LEFT, property_assignment_get_token,
            ERROR_EXPRESSION_PROPERTY_ASSIGNMENT_GET_PARENTHESIS_LEFT)

    match_punctuator_or_free_and_return_error(input, position_t,
            PUNCTUATOR_PARENTHESIS_RIGHT, property_assignment_get_token,
            ERROR_EXPRESSION_PROPERTY_ASSIGNMENT_GET_PARENTHESIS_RIGHT)

    match_punctuator_or_free_and_return_error(input, position_t,
            PUNCTUATOR_CURLY_BRACE_LEFT, property_assignment_get_token,
            ERROR_EXPRESSION_PROPERTY_ASSIGNMENT_GET_CURLY_BRACE_LEFT)

    tokenize_and_add_child_or_free_parent_and_return_error(input, position_t,
            property_name, property_assignment_get_token)

    match_punctuator_or_free_and_return_error(input, position_t,
            PUNCTUATOR_CURLY_BRACE_RIGHT, property_assignment_get_token,
            ERROR_EXPRESSION_PROPERTY_ASSIGNMENT_GET_CURLY_BRACE_RIGHT)

    return property_assignment_get_token;
}

/*
 * AST: PropertyAssignmentSet = PropertyName Identifier FunctionBody
 *
 * GRAMMAR:
 * PropertyAssignmentSet :
 *     set PropertyName ( Identifier ) { FunctionBody }
 */

gboolean property_assignment_set_is_first(GPtrArray *input, gsize position) {
    return token_is_first_keyword(input, position, KEYWORD_SET);
}

token_t *property_assignment_set(GPtrArray *input, gsize *position_t) {

    match_keyword_or_return_error(input, position_t, KEYWORD_SET,
                                  ERROR_EXPRESSION_PROPERTY_ASSIGNMENT_SET_SET);

    token_t *property_assignment_get_token = token_new_no_data(
            TOKEN_EXPRESSION_PROPERY_ASSIGNMENT_SET);

    tokenize_and_add_child_or_free_parent_and_return_error(input, position_t,
            property_name, property_assignment_get_token)

    match_punctuator_or_free_and_return_error(input, position_t,
            PUNCTUATOR_PARENTHESIS_LEFT, property_assignment_get_token,
            ERROR_EXPRESSION_PROPERTY_ASSIGNMENT_SET_PARENTHESIS_LEFT)

    match_punctuator_or_free_and_return_error(input, position_t,
            PUNCTUATOR_PARENTHESIS_RIGHT, property_assignment_get_token,
            ERROR_EXPRESSION_PROPERTY_ASSIGNMENT_SET_PARENTHESIS_RIGHT)

    match_token_id_clone_and_add_child_or_free_parent_and_return_error(input,
            position_t, TOKEN_LEXICAL_IDENTIFIER, property_assignment_get_token,
            ERROR_EXPRESSION_PROPERTY_ASSIGNMENT_SET_IDENTIFIER)

    match_punctuator_or_free_and_return_error(input, position_t,
            PUNCTUATOR_CURLY_BRACE_LEFT, property_assignment_get_token,
            ERROR_EXPRESSION_PROPERTY_ASSIGNMENT_SET_CURLY_BRACE_LEFT)

    tokenize_and_add_child_or_free_parent_and_return_error(input, position_t,
            property_name, property_assignment_get_token)

    match_punctuator_or_free_and_return_error(input, position_t,
            PUNCTUATOR_CURLY_BRACE_RIGHT, property_assignment_get_token,
            ERROR_EXPRESSION_PROPERTY_ASSIGNMENT_SET_CURLY_BRACE_RIGHT)

    return property_assignment_get_token;
}

/*
 * AST:
 * PropertyName = (Identifier|StringLiteral|NumericLiteral|AssignmentExpression)
 *
 * GRAMMAR:
 * PropertyName :
 *     Identifier
 *     StringLiteral
 *     NumericLiteral
 *     [ AssignmentExpression ]
 */

gboolean property_name_is_first(GPtrArray *input, gsize position) {
    return token_is_first_id(input, position, TOKEN_LEXICAL_IDENTIFIER)
           || token_is_first_id(input, position, TOKEN_LEXICAL_STRING_LITERAL)
           || token_is_first_id(input, position, TOKEN_LEXICAL_NUMERIC_LITERAL)
           || token_is_first_punctuator(input, position,
                                        PUNCTUATOR_SQUARE_BRACKET_LEFT);
}

token_t *property_name(GPtrArray *input, gsize *position_t) {

    match_token_id_clone_and_return_if_is_first(input, position_t,
                                                TOKEN_LEXICAL_IDENTIFIER)
    match_token_id_clone_and_return_if_is_first(input, position_t,
                                                TOKEN_LEXICAL_STRING_LITERAL)
    match_token_id_clone_and_return_if_is_first(input, position_t,
                                                TOKEN_LEXICAL_NUMERIC_LITERAL);

    if (token_match_punctuator(input, position_t,
                               PUNCTUATOR_SQUARE_BRACKET_LEFT)) {
        token_t *assignment_expression_token = assignment_expression(input,
                position_t);
        match_punctuator_or_free_and_return_error(input, position_t,
                PUNCTUATOR_SQUARE_BRACKET_RIGHT, assignment_expression_token,
                ERROR_EXPRESSION_PROPERTY_NAME_SQUARE_BRACKET_RIGHT)
        return assignment_expression_token;
    }

    return error_new_syntactic(ERROR_EXPRESSION_PROPERTY_NAME, *position_t);
}

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

