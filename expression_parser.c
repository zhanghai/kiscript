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

token_t *array_literal(GPtrArray *input, gsize *position_t) {
    // TODO

    token_t *array_literal_token_or_error = token_new_no_data(TOKEN_EXPRESSION_ARRAY_LITERAL);
    while (!token_match_punctuator(input, position_t,
                                   PUNCTUATOR_SQUARE_BRACKET_RIGHT)) {
        if (element_list_is_first(input, *position_t)) {
            tokenize_and_add_child_or_free_parent_and_return_error(input, position_t, element_list,
                                                                   array_literal_token_or_error)
        }
        else if (token_match_punctuator(input, position_t,
                                        PUNCTUATOR_COMMA)) {
            match_punctuator_or_free_and_return_error(input, position_t,
                                                      PUNCTUATOR_PARENTHESIS_RIGHT,
                                                      array_literal_token_or_error,
                                                      ERROR_EXPRESSION_PRIMARY_EXPRESSION_PARENTHESIS_LEFT)
        }
    }

    return array_literal_token_or_error;
}

/*
 * ElementList :
 *        Elision? AssignmentExpression
 *        ElementList , Elision? AssignmentExpression
 */
gboolean element_list_is_first(GPtrArray *input, gsize position) {
    return assignment_expression_is_first(input, position)
            || token_is_first_punctuator(input, position, PUNCTUATOR_COMMA);
}

token_t *element_list(GPtrArray *input, gsize *position_t) {
    return NULL;
}

gboolean object_literal_is_first(GPtrArray *input, gsize position) {
    return token_is_first_punctuator(input, position,
                                     PUNCTUATOR_CURLY_BRACE_LEFT);
}

//ObjectLiteral :
//      { }
//      { PropertyNameAndValueList }
//      { PropertyNameAndValueList , }
token_t *object_literal(GPtrArray *input, gsize *position_t) {
    // TODO
    match_punctuator_or_return_error(input, position_t,
                                     PUNCTUATOR_CURLY_BRACE_LEFT,
                                     ERROR_EXPRESSION_OBJECT_LITERAL)
    token_t *object_literal_token_or_error =
            property_name_and_value_list(input, position_t);
    return_if_error(object_literal_token_or_error);
    token_match_punctuator(input, position_t,
                           PUNCTUATOR_COMMA);
    match_punctuator_or_return_error(input, position_t,
                                     PUNCTUATOR_CURLY_BRACE_RIGHT,
                                     ERROR_EXPRESSION_OBJECT_LITERAL)

    return object_literal_token_or_error;
}

gboolean left_hand_side_expression_is_left_hand_side_expression(
        token_t *token) {
    // TODO
    return FALSE;
}

/*
 * AssignmentExpression :
 *         ConditionalExpression
 *         LeftHandSideExpression = AssignmentExpression
 *         LeftHandSideExpression AssignmentOperator AssignmentExpression
 */

gboolean assignment_expression_is_first(GPtrArray *input, gsize position) {
    return conditional_expression_is_first(input, position)
            || left_hand_side_expression_is_first(input, position);
}

token_t *assignment_expression(GPtrArray *input, gsize *position_p) {
    // TODO
    tokenize_and_return_if_is_first(input, position_p, conditional_expression)

    token_t *assignment_expression_token_or_error =
            token_new_no_data(TOKEN_EXPRESSION_ASSIGNMENT_EXPRESSION);
    tokenize_and_add_child_or_free_parent_and_return_error(input, position_p,
                                                           left_hand_side_expression,
                                                           assignment_expression_token_or_error)
    if (token_match_punctuator(input, position_p,
                               PUNCTUATOR_EQUALS_SIGN)) {
        tokenize_and_add_child_or_free_parent_and_return_error(input, position_p,
                                                               assignment_expression,
                                                               assignment_expression_token_or_error)
        /* TODO: mark it as LeftHandSideExpression = AssignmentExpression and return */
    } else {
        tokenize_and_add_child_or_free_parent_and_return_error(input, position_p,
                                                               assignment_operator,
                                                               assignment_expression_token_or_error)
        tokenize_and_add_child_or_free_parent_and_return_error(input, position_p,
                                                               assignment_expression,
                                                               assignment_expression_token_or_error)
        /*TODO: mark it as LeftHandSideExpression AssignmentOperator AssignmentExpression and return */
    }

    return assignment_expression_token_or_error;
}

gboolean expression_is_first(GPtrArray *input, gsize position) {
    // TODO
    return FALSE;
}

token_t *expression(GPtrArray *input, gsize *position_t) {
    // TODO
    return NULL;
}

gboolean left_hand_side_expression_is_first(GPtrArray *input, gsize position) {
    /*return blahblah... */return 0;

}

gboolean conditional_expression_is_first(GPtrArray *input, gsize position) {
    /*return blahblah... */return 0;

}

token_t *left_hand_side_expression(GPtrArray *input, gsize *position_t) {
    // TODO
    return NULL;
}

token_t *conditional_expression(GPtrArray *input, gsize *position_t) {
    // TODO
    return NULL;
}

//PropertyNameAndValueList :
//        PropertyAssignment
//        PropertyNameAndValueList , PropertyAssignment
token_t *property_name_and_value_list(GPtrArray *input, gsize *position_t) {

    token_t *property_name_and_value_list_token_or_error =
            token_new_no_data(TOKEN_EXPRESSION_ASSIGNMENT_EXPRESSION);
    while (property_assignment_is_first(input, position_t)) {
        tokenize_and_add_child_or_free_parent_and_return_error(input, position_t, property_assignment,
                                                               property_name_and_value_list_token_or_error);
        if (!token_match_punctuator(input,position_t,PUNCTUATOR_COMMA))
            break;
    }

    return property_name_and_value_list_token_or_error;
}

//PropertyAssignment :
//        PropertyName : AssignmentExpression
//        get PropertyName ( ) { FunctionBody }
//        set PropertyName ( PropertySetParameterList ) { FunctionBody }
gboolean property_assignment_is_first(GPtrArray *input, gsize position) {
    return token_is_first_keyword(input, position, KEYWORD_GET)
           || token_is_first_keyword(input, position, KEYWORD_SET)
           || property_name_is_first(input, position);
}

token_t *property_assignment(GPtrArray *input, gsize *position_t) {
    token_t *property_assignment_token_or_error =
            token_new_no_data(TOKEN_EXPRESSION_PROPERTY_ASSIGNMENT);
    if (token_match_keyword(input, position_t, KEYWORD_GET)) {
        /* get PropertyName ( ) { FunctionBody } */
        tokenize_and_add_child_or_free_parent_and_return_error(input,position_t,
                                                               property_name,
                                                               property_assignment_token_or_error)
        match_punctuator_or_free_and_return_error(input, position_t,
                                                  PUNCTUATOR_PARENTHESIS_LEFT,
                                                  property_assignment_token_or_error,
                                                  ERROR_EXPRESSION_PROPERTY_ASSIGNMENT)
        match_punctuator_or_free_and_return_error(input, position_t,
                                                  PUNCTUATOR_PARENTHESIS_RIGHT,
                                                  property_assignment_token_or_error,
                                                  ERROR_EXPRESSION_PROPERTY_ASSIGNMENT)
        match_punctuator_or_free_and_return_error(input, position_t,
                                                  PUNCTUATOR_CURLY_BRACE_LEFT,
                                                  property_assignment_token_or_error,
                                                  ERROR_EXPRESSION_PROPERTY_ASSIGNMENT)
        tokenize_and_add_child_or_free_parent_and_return_error(input,position_t,
                                                               function_body,
                                                               property_assignment_token_or_error)
        match_punctuator_or_free_and_return_error(input, position_t,
                                                  PUNCTUATOR_CURLY_BRACE_RIGHT,
                                                  property_assignment_token_or_error,
                                                  ERROR_EXPRESSION_PROPERTY_ASSIGNMENT)
        /* TODO: mark it as GET and return */
    }
    else if (token_match_keyword(input, position_t, KEYWORD_SET)) {
        /* set PropertyName ( PropertySetParameterList ) { FunctionBody } */
        tokenize_and_add_child_or_free_parent_and_return_error(input,position_t,
                                                               property_name,
                                                               property_assignment_token_or_error)
        match_punctuator_or_free_and_return_error(input, position_t,
                                                  PUNCTUATOR_PARENTHESIS_LEFT,
                                                  property_assignment_token_or_error,
                                                  ERROR_EXPRESSION_PROPERTY_ASSIGNMENT)
        tokenize_and_add_child_or_free_parent_and_return_error(input,position_t,
                                                               property_set_parameter_list,
                                                               property_assignment_token_or_error)
        match_punctuator_or_free_and_return_error(input, position_t,
                                                  PUNCTUATOR_PARENTHESIS_RIGHT,
                                                  property_assignment_token_or_error,
                                                  ERROR_EXPRESSION_PROPERTY_ASSIGNMENT)
        match_punctuator_or_free_and_return_error(input, position_t,
                                                  PUNCTUATOR_CURLY_BRACE_LEFT,
                                                  property_assignment_token_or_error,
                                                  ERROR_EXPRESSION_PROPERTY_ASSIGNMENT)
        tokenize_and_add_child_or_free_parent_and_return_error(input,position_t,
                                                               function_body,
                                                               property_assignment_token_or_error)
        match_punctuator_or_free_and_return_error(input, position_t,
                                                  PUNCTUATOR_CURLY_BRACE_RIGHT,
                                                  property_assignment_token_or_error,
                                                  ERROR_EXPRESSION_PROPERTY_ASSIGNMENT)
        /* TODO: mark it as SET and return */
    }
    else {
        /* PropertyName : AssignmentExpression */
        tokenize_and_add_child_or_free_parent_and_return_error(input,position_t,
                                                               property_name,
                                                               property_assignment_token_or_error)
        match_punctuator_or_free_and_return_error(input, position_t,
                                                  PUNCTUATOR_COLON,
                                                  property_assignment_token_or_error,
                                                  ERROR_EXPRESSION_PROPERTY_ASSIGNMENT)
        tokenize_and_add_child_or_free_parent_and_return_error(input,position_t,
                                                               assignment_expression,
                                                               property_assignment_token_or_error)
    }
    return NULL;
}

gboolean property_name_is_first(GPtrArray *input, gsize position) {
    return FALSE;
}

//PropertyName :
//        IdentifierName
//        StringLiteral
//        NumericLiteral
token_t *property_name(GPtrArray *input, gsize *position_t) {
    token_t *property_name_token_or_error =
            token_new_no_data(TOKEN_EXPRESSION_PROPERTY_NAME);

//    tokenize_and_return_if_is_first(input, position_t, identifier_name)
//    tokenize_and_return_if_is_first(input, position_t, string_literal)
//    tokenize_and_return_if_is_first(input, position_t, numeric_literal)
    /*TODO: I don't know how to do this */
    return NULL;
}

