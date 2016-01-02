//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#include "expression_parser.h"

#include "function_parser.h"
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

gboolean primary_expression_is_first(GPtrArray *input, gsize position) {
    return token_is_first_keyword(input, position, KEYWORD_THIS)
           || token_is_first_id(input, position, TOKEN_LEXICAL_IDENTIFIER)
           || token_is_first_id(input, position, TOKEN_LEXICAL_NULL_LITERAL)
           || token_is_first_id(input, position, TOKEN_LEXICAL_BOOLEAN_LITERAL)
           || token_is_first_id(input, position, TOKEN_LEXICAL_NUMERIC_LITERAL)
           || token_is_first_id(input, position, TOKEN_LEXICAL_STRING_LITERAL)
           || array_literal_is_first(input, position)
           || object_literal_is_first(input, position)
           || token_is_first_punctuator(input, position,
                                        PUNCTUATOR_PARENTHESIS_LEFT);
}

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

token_t *array_literal(GPtrArray *input, gsize *position_p) {

    match_punctuator_or_return_error(input, position_p,
            PUNCTUATOR_SQUARE_BRACKET_LEFT,
            ERROR_EXPRESSION_ARRAY_LITERAL_SQUARE_BRACKET_LEFT)

    token_t *array_literal_token = token_new_no_data(
            TOKEN_EXPRESSION_ARRAY_LITERAL);

    gboolean first = TRUE;
    while (!token_match_punctuator(input, position_p,
                                   PUNCTUATOR_SQUARE_BRACKET_RIGHT)) {

        if (first) {
            first = FALSE;
        } else {
            match_punctuator_or_free_and_return_error(input, position_p,
                    PUNCTUATOR_COMMA, array_literal_token,
                   ERROR_EXPRESSION_ARRAY_LITERAL_COMMA_OR_SQUARE_BRACKET_RIGHT)
        }

        tokenize_and_add_child_or_free_parent_and_return_error(input,
                position_p, assignment_expression, array_literal_token)
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

token_t *object_literal(GPtrArray *input, gsize *position_p) {

    match_punctuator_or_return_error(input, position_p,
            PUNCTUATOR_CURLY_BRACE_LEFT,
            ERROR_EXPRESSION_OBJECT_LITERAL_CURLY_BRACE_LEFT)

    token_t *object_literal_token = token_new_no_data(
            TOKEN_EXPRESSION_OBJECT_LITERAL);

    gboolean first = TRUE;
    while (!token_match_punctuator(input, position_p,
                                   PUNCTUATOR_CURLY_BRACE_RIGHT)) {

        if (first) {
            first = FALSE;
        } else {
            match_punctuator_or_free_and_return_error(input, position_p,
                    PUNCTUATOR_COMMA, object_literal_token,
                    ERROR_EXPRESSION_OBJECT_LITERAL_COMMA_OR_CURLY_BRACE_RIGHT)
        }

        tokenize_and_add_child_or_free_parent_and_return_error(input,
                    position_p, property_assignment, object_literal_token)
    }

    return object_literal_token;
}

/*
 * AST:
 * PropertyAssignment = PropertyAssignmentPlain|PropertyAssignmentGet
 *     |PropertyAssignmentSet
 *
 * GRAMMAR:
 * PropertyAssignment :
 *     (PropertyAssignmentPlain|PropertyAssignmentGet|PropertyAssignmentSet)
 */

token_t *property_assignment(GPtrArray *input, gsize *position_p) {

    tokenize_and_return_if_is_first(input, position_p,
                                    property_assignment_plain)
    tokenize_and_return_if_is_first(input, position_p,
                                    property_assignment_get)
    tokenize_and_return_if_is_first(input, position_p,
                                    property_assignment_set)

    return error_new_syntactic(ERROR_EXPRESSION_PROPERTY_ASSIGNMENT,
                               *position_p);
}

/*
 * AST:
 * PropertyAssignmentPlain = PropertyName AssignmentExpression
 *
 * GRAMMAR:
 * PropertyAssignmentPlain :
 *     PropertyName : AssignmentExpression
 */

gboolean property_assignment_plain_is_first(GPtrArray *input, gsize position) {
    return property_name_is_first(input, position);
}

token_t *property_assignment_plain(GPtrArray *input, gsize *position_p) {

    token_t *property_assignment_plain_token = token_new_no_data(
            TOKEN_EXPRESSION_PROPERTY_ASSIGNMENT_PLAIN);

    tokenize_and_add_child_or_free_parent_and_return_error(input, position_p,
            property_name, property_assignment_plain_token)

    match_punctuator_or_free_and_return_error(input, position_p,
            PUNCTUATOR_COLON, property_assignment_plain_token,
            ERROR_EXPRESSION_PROPERTY_ASSIGNMENT_PLAIN_COLON)

    tokenize_and_add_child_or_free_parent_and_return_error(input, position_p,
            property_name, property_assignment_plain_token)

    return property_assignment_plain_token;
}

/*
 * AST:
 * PropertyAssignmentGet = PropertyName FunctionBody
 *
 * GRAMMAR:
 * PropertyAssignmentGet :
 *     get PropertyName ( ) { FunctionBody }
 */

gboolean property_assignment_get_is_first(GPtrArray *input, gsize position) {
    return token_is_first_keyword(input, position, KEYWORD_GET);
}

token_t *property_assignment_get(GPtrArray *input, gsize *position_p) {

    match_keyword_or_return_error(input, position_p, KEYWORD_GET,
                                  ERROR_EXPRESSION_PROPERTY_ASSIGNMENT_GET_GET);

    token_t *property_assignment_get_token = token_new_no_data(
            TOKEN_EXPRESSION_PROPERTY_ASSIGNMENT_GET);

    tokenize_and_add_child_or_free_parent_and_return_error(input, position_p,
            property_name, property_assignment_get_token)

    match_punctuator_or_free_and_return_error(input, position_p,
            PUNCTUATOR_PARENTHESIS_LEFT, property_assignment_get_token,
            ERROR_EXPRESSION_PROPERTY_ASSIGNMENT_GET_PARENTHESIS_LEFT)

    match_punctuator_or_free_and_return_error(input, position_p,
            PUNCTUATOR_PARENTHESIS_RIGHT, property_assignment_get_token,
            ERROR_EXPRESSION_PROPERTY_ASSIGNMENT_GET_PARENTHESIS_RIGHT)

    match_punctuator_or_free_and_return_error(input, position_p,
            PUNCTUATOR_CURLY_BRACE_LEFT, property_assignment_get_token,
            ERROR_EXPRESSION_PROPERTY_ASSIGNMENT_GET_CURLY_BRACE_LEFT)

    tokenize_and_add_child_or_free_parent_and_return_error(input, position_p,
            property_name, property_assignment_get_token)

    match_punctuator_or_free_and_return_error(input, position_p,
            PUNCTUATOR_CURLY_BRACE_RIGHT, property_assignment_get_token,
            ERROR_EXPRESSION_PROPERTY_ASSIGNMENT_GET_CURLY_BRACE_RIGHT)

    return property_assignment_get_token;
}

/*
 * AST:
 * PropertyAssignmentSet = PropertyName Identifier FunctionBody
 *
 * GRAMMAR:
 * PropertyAssignmentSet :
 *     set PropertyName ( Identifier ) { FunctionBody }
 */

gboolean property_assignment_set_is_first(GPtrArray *input, gsize position) {
    return token_is_first_keyword(input, position, KEYWORD_SET);
}

token_t *property_assignment_set(GPtrArray *input, gsize *position_p) {

    match_keyword_or_return_error(input, position_p, KEYWORD_SET,
                                  ERROR_EXPRESSION_PROPERTY_ASSIGNMENT_SET_SET);

    token_t *property_assignment_get_token = token_new_no_data(
            TOKEN_EXPRESSION_PROPERTY_ASSIGNMENT_SET);

    tokenize_and_add_child_or_free_parent_and_return_error(input, position_p,
            property_name, property_assignment_get_token)

    match_punctuator_or_free_and_return_error(input, position_p,
            PUNCTUATOR_PARENTHESIS_LEFT, property_assignment_get_token,
            ERROR_EXPRESSION_PROPERTY_ASSIGNMENT_SET_PARENTHESIS_LEFT)

    match_punctuator_or_free_and_return_error(input, position_p,
            PUNCTUATOR_PARENTHESIS_RIGHT, property_assignment_get_token,
            ERROR_EXPRESSION_PROPERTY_ASSIGNMENT_SET_PARENTHESIS_RIGHT)

    match_token_id_clone_and_add_child_or_free_parent_and_return_error(input,
            position_p, TOKEN_LEXICAL_IDENTIFIER, property_assignment_get_token,
            ERROR_EXPRESSION_PROPERTY_ASSIGNMENT_SET_IDENTIFIER)

    match_punctuator_or_free_and_return_error(input, position_p,
            PUNCTUATOR_CURLY_BRACE_LEFT, property_assignment_get_token,
            ERROR_EXPRESSION_PROPERTY_ASSIGNMENT_SET_CURLY_BRACE_LEFT)

    tokenize_and_add_child_or_free_parent_and_return_error(input, position_p,
            property_name, property_assignment_get_token)

    match_punctuator_or_free_and_return_error(input, position_p,
            PUNCTUATOR_CURLY_BRACE_RIGHT, property_assignment_get_token,
            ERROR_EXPRESSION_PROPERTY_ASSIGNMENT_SET_CURLY_BRACE_RIGHT)

    return property_assignment_get_token;
}

/*
 * AST:
 * PropertyName = Identifier|StringLiteral|NumericLiteral|AssignmentExpression
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

token_t *property_name(GPtrArray *input, gsize *position_p) {

    match_token_id_clone_and_return_if_is_first(input, position_p,
                                                TOKEN_LEXICAL_IDENTIFIER)
    match_token_id_clone_and_return_if_is_first(input, position_p,
                                                TOKEN_LEXICAL_STRING_LITERAL)
    match_token_id_clone_and_return_if_is_first(input, position_p,
                                                TOKEN_LEXICAL_NUMERIC_LITERAL);

    if (token_match_punctuator(input, position_p,
                               PUNCTUATOR_SQUARE_BRACKET_LEFT)) {
        token_t *assignment_expression_token = assignment_expression(input,
                position_p);
        match_punctuator_or_free_and_return_error(input, position_p,
                PUNCTUATOR_SQUARE_BRACKET_RIGHT, assignment_expression_token,
                ERROR_EXPRESSION_PROPERTY_NAME_SQUARE_BRACKET_RIGHT)
        return assignment_expression_token;
    }

    return error_new_syntactic(ERROR_EXPRESSION_PROPERTY_NAME, *position_p);
}

/*
 * AST:
 * CallableExpression = PrimaryExpression|FunctionExpression|NewExpression
 *     |PropertyAccessor|CallExpression
 * PropertyAccessor - CallableExpression (Identifier|Expression)
 * CallExpression - CallableExpression ArgumentList
 *
 * GRAMMAR:
 * NonLeftRecursiveCallableExpression :
 *     PrimaryExpression
 *     FunctionExpression
 *     NewExpression
 * CallableExpression :
 *     NonLeftRecursiveCallableExpression
 *     PropertyAccessor
 *     CallExpression
 * PropertyAccessor :
 *     CallableExpression . Identifier
 *     CallableExpression [ Expression ]
 * CallExpression :
 *     CallableExpression ArgumentList
 */

static gboolean non_left_recursive_callable_expression_is_first(
        GPtrArray *input, gsize position) {
    return primary_expression_is_first(input, position)
           || function_expression_is_first(input, position)
           || new_expression_is_first(input, position);
}

static token_t *non_left_recursive_callable_expression(GPtrArray *input,
                                                       gsize *position_p) {

    tokenize_and_return_if_is_first(input, position_p, primary_expression)
    tokenize_and_return_if_is_first(input, position_p, function_expression)
    tokenize_and_return_if_is_first(input, position_p, new_expression)

    return error_new_syntactic(
            ERROR_EXPRESSION_CALLABLE_EXPRESSION_CALLABLE_EXPRESSION,
            *position_p);
}

gboolean callable_expression_is_first(GPtrArray *input, gsize position) {
    return non_left_recursive_callable_expression_is_first(input, position);
}

token_t *callable_expression(GPtrArray *input, gsize *position_p) {

    token_t *callable_expression_token =
            non_left_recursive_callable_expression(input, position_p);
    return_if_error(callable_expression_token);

    // LEFT_RECURSION: lookahead with the power of recursive-descent parsing.
    while (TRUE) {

        if (token_match_punctuator(input, position_p, PUNCTUATOR_DOT)) {

            token_t *property_accessor_token = token_new_no_data(
                    TOKEN_EXPRESSION_PROPERTY_ACCESSOR);
            token_add_child(property_accessor_token, callable_expression_token);
            match_token_id_clone_and_add_child_or_free_parent_and_return_error(
                    input, position_p, TOKEN_LEXICAL_IDENTIFIER,
                    property_accessor_token,
                    ERROR_EXPRESSION_PROPERTY_ACCESSOR_IDENTIFIER)
            callable_expression_token = property_accessor_token;

        } else if (token_match_punctuator(input, position_p,
                                   PUNCTUATOR_SQUARE_BRACKET_LEFT)) {

            token_t *property_accessor_token = token_new_no_data(
                    TOKEN_EXPRESSION_PROPERTY_ACCESSOR);
            token_add_child(property_accessor_token, callable_expression_token);
            tokenize_and_add_child_or_free_parent_and_return_error(input,
                    position_p, expression, property_accessor_token);
            match_punctuator_or_free_and_return_error(input, position_p,
                    PUNCTUATOR_SQUARE_BRACKET_RIGHT, property_accessor_token,
                    ERROR_EXPRESSION_PROPERTY_ACCESSOR_SQUARE_BRACKET_RIGHT)
            callable_expression_token = property_accessor_token;

        } else if (argument_list_is_first(input, *position_p)) {

            token_t *call_expression_token = token_new_no_data(
                    TOKEN_EXPRESSION_CALL_EXPRESSION);
            token_add_child(call_expression_token, callable_expression_token);
            tokenize_and_add_child_or_free_parent_and_return_error(input,
                    position_p, argument_list, call_expression_token)
            callable_expression_token = call_expression_token;

        } else {
            break;
        }
    }

    return callable_expression_token;
}

/*
 * AST:
 * ArgumentList - AssignmentExpression*
 *
 * GRAMMAR:
 * ArgumentList :
 *     ( (AssignmentExpression (, AssignmentExpression)*)? )
 */

gboolean argument_list_is_first(GPtrArray *input, gsize position) {
    return token_is_first_punctuator(input, position,
                                     PUNCTUATOR_PARENTHESIS_LEFT);
}

token_t *argument_list(GPtrArray *input, gsize *position_p) {

    match_punctuator_or_return_error(input, position_p,
            PUNCTUATOR_PARENTHESIS_LEFT,
            ERROR_EXPRESSION_ARGUMENT_LIST_PARENTHESIS_LEFT)

    token_t *argument_list_token = token_new_no_data(
            TOKEN_EXPRESSION_ARGUMENT_LIST);

    gboolean first = TRUE;
    while (!token_match_punctuator(input, position_p,
                                   PUNCTUATOR_PARENTHESIS_RIGHT)) {

        if (first) {
            first = FALSE;
        } else {
            match_punctuator_or_free_and_return_error(input, position_p,
                    PUNCTUATOR_COMMA, argument_list_token,
                    ERROR_EXPRESSION_ARGUMENT_LIST_COMMA_OR_PARENTHESIS_RIGHT)
        }

        tokenize_and_add_child_or_free_parent_and_return_error(input,
                position_p, assignment_expression, argument_list_token)
    }

    return argument_list_token;
}

/**
 * AST:
 * NewExpression - CallableExpression ArgumentList
 *
 * GRAMMAR:
 * // NONSTANDARD: Disallow NewExpression without ArgumentList.
 * NewExpression :
 *     new CallableExpression ArgumentList
 *
 */
gboolean new_expression_is_first(GPtrArray *input, gsize position) {
    return token_is_first_keyword(input, position, KEYWORD_NEW);
}

token_t *new_expression(GPtrArray *input, gsize *position_p) {

    match_keyword_or_return_error(input, position_p, KEYWORD_NEW,
                                  ERROR_EXPRESSION_NEW_EXPRESSION_NEW)

    token_t *new_expression_token = token_new_no_data(
            TOKEN_EXPRESSION_NEW_EXPRESSION);

    tokenize_and_add_child_or_free_parent_and_return_error(input, position_p,
                                                           callable_expression,
                                                           new_expression_token)

    tokenize_and_add_child_or_free_parent_and_return_error(input, position_p,
                                                           argument_list,
                                                           new_expression_token)

    return new_expression_token;
}

/*
 * NOTE: LeftHandSideExpression here is only a syntactic symbol, and the real
 * check is performed at execution time.
 *
 * AST:
 * LeftHandSideExpression = CallableExpression
 *
 * GRAMMAR:
 * LeftHandSideExpression :
 *     CallableExpression
 */

token_t *left_hand_side_expression(GPtrArray *input, gsize *position_p) {
    return callable_expression(input, position_p);
}

/*
 * AST:
 * PostfixExpression = LeftHandSideExpression
 * PostfixExpression - LeftHandSideExpression (Punctuator(++)|Punctuator(--))
 *
 * GRAMMAR:
 * NONSTANDARD: Allow LineTerminator.
 * PostfixExpression :
 *     LeftHandSideExpression
 *     LeftHandSideExpression ++
 *     LeftHandSideExpression --
 */

token_t *postfix_expression(GPtrArray *input, gsize *position_p) {

    token_t *left_hand_side_expression_token = left_hand_side_expression(input,
            position_p);
    return_if_error(left_hand_side_expression_token);

    token_t *operator_token;
    if (token_match_punctuator_clone(input, position_p, PUNCTUATOR_INCREMENT,
                                     &operator_token)
        || token_match_punctuator_clone(input, position_p, PUNCTUATOR_DECREMENT,
                                        &operator_token)) {

        token_t *postfix_expression_token = token_new_no_data(
                TOKEN_EXPRESSION_POSTFIX_EXPRESSION);
        token_add_child(postfix_expression_token,
                        left_hand_side_expression_token);
        token_add_child(postfix_expression_token, operator_token);
        return postfix_expression_token;

    } else {
        return left_hand_side_expression_token;
    }
}

/*
 * AST:
 * UnaryExpression = PostfixExpression
 * UnaryExpression - (delete|void|typeof|++|--|+|-|~|!) UnaryExpression
 *
 * GRAMMAR:
 * UnaryExpression :
 *     PostfixExpression
 *     delete UnaryExpression
 *     void UnaryExpression
 *     typeof UnaryExpression
 *     ++ UnaryExpression
 *     -- UnaryExpression
 *     + UnaryExpression
 *     - UnaryExpression
 *     ~ UnaryExpression
 *     ! UnaryExpression
 */

token_t *unary_expression(GPtrArray *input, gsize *position_p) {

    token_t *operator_token;
    if (token_match_keyword_clone(input, position_p, KEYWORD_DELETE,
                                  &operator_token)
        || token_match_keyword_clone(input, position_p, KEYWORD_VOID,
                                     &operator_token)
        || token_match_keyword_clone(input, position_p, KEYWORD_TYPEOF,
                                     &operator_token)
        || token_match_punctuator_clone(input, position_p, PUNCTUATOR_INCREMENT,
                                        &operator_token)
        || token_match_punctuator_clone(input, position_p, PUNCTUATOR_DECREMENT,
                                        &operator_token)
        || token_match_punctuator_clone(input, position_p, PUNCTUATOR_PLUS,
                                        &operator_token)
        || token_match_punctuator_clone(input, position_p, PUNCTUATOR_MINUS,
                                        &operator_token)
        || token_match_punctuator_clone(input, position_p, PUNCTUATOR_TILDE,
                                        &operator_token)
        ||token_match_punctuator_clone(input, position_p,
                                       PUNCTUATOR_EXCLAMATION, &operator_token)
            ) {

        token_t *unary_expression_token = token_new_no_data(
                TOKEN_EXPRESSION_UNARY_EXPRESSION);
        token_add_child(unary_expression_token, operator_token);
        tokenize_and_add_child_or_free_parent_and_return_error(input,
                position_p, unary_expression, unary_expression_token)
        return unary_expression_token;

    } else {
        return postfix_expression(input, position_p);
    }
}

/*
 * AST:
 * MultiplicativeExpression = UnaryExpression
 * MultiplicativeExpression - MultiplicativeExpression (*|/|%) UnaryExpression
 *
 * GRAMMAR:
 * MultiplicativeExpression :
 *     UnaryExpression
 *     MultiplicativeExpression * UnaryExpression
 *     MultiplicativeExpression / UnaryExpression
 *     MultiplicativeExpression % UnaryExpression
 */

token_t *multiplicative_expression(GPtrArray *input, gsize *position_p) {

    token_t *unary_or_multiplicative_expression_token = unary_expression(input,
            position_p);
    return_if_error(unary_or_multiplicative_expression_token);

    // LEFT_RECURSION
    token_t *operator_token;
    while (token_match_punctuator_clone(input, position_p, PUNCTUATOR_ASTERISK,
                                        &operator_token)
           || token_match_punctuator_clone(input, position_p, PUNCTUATOR_SLASH,
                                           &operator_token)
           || token_match_punctuator_clone(input, position_p,
                                           PUNCTUATOR_PERCENT,
                                           &operator_token)) {

        token_t *multiplicative_expression_token = token_new_no_data(
                TOKEN_EXPRESSION_MULTIPLICATIVE_EXPRESSION);
        token_add_child(multiplicative_expression_token,
                        unary_or_multiplicative_expression_token);
        token_add_child(multiplicative_expression_token, operator_token);
        tokenize_and_add_child_or_free_parent_and_return_error(input,
                position_p, unary_expression, multiplicative_expression_token)
        unary_or_multiplicative_expression_token =
                multiplicative_expression_token;
    }

    return unary_or_multiplicative_expression_token;
}

/*
 * AST:
 * AdditiveExpression = MultiplicativeExpression
 * AdditiveExpression - AdditiveExpression (+|-) MultiplicativeExpression
 *
 * GRAMMAR:
 * AdditiveExpression :
 *     MultiplicativeExpression
 *     AdditiveExpression + MultiplicativeExpression
 *     AdditiveExpression - MultiplicativeExpression
 */

token_t *additive_expression(GPtrArray *input, gsize *position_p) {

    token_t *multiplicative_or_additive_expression_token =
            multiplicative_expression(input, position_p);
    return_if_error(multiplicative_or_additive_expression_token);

    // LEFT_RECURSION
    token_t *operator_token;
    while (token_match_punctuator_clone(input, position_p, PUNCTUATOR_PLUS,
                                        &operator_token)
           || token_match_punctuator_clone(input, position_p, PUNCTUATOR_MINUS,
                                           &operator_token)) {

        token_t *additive_expression_token = token_new_no_data(
                TOKEN_EXPRESSION_ADDITIVE_EXPRESSION);
        token_add_child(additive_expression_token,
                        multiplicative_or_additive_expression_token);
        token_add_child(additive_expression_token, operator_token);
        tokenize_and_add_child_or_free_parent_and_return_error(input,
                position_p, multiplicative_expression,
                additive_expression_token)
        multiplicative_or_additive_expression_token = additive_expression_token;
    }

    return multiplicative_or_additive_expression_token;
}

/*
 * AST:
 * ShiftExpression = AdditiveExpression
 * ShiftExpression - ShiftExpression (<<|>>|>>>) AdditiveExpression
 *
 * GRAMMAR:
 * ShiftExpression :
 *     AdditiveExpression
 *     ShiftExpression << AdditiveExpression
 *     ShiftExpression >> AdditiveExpression
 *     ShiftExpression >>> AdditiveExpression
 */

token_t *shift_expression(GPtrArray *input, gsize *position_p) {

    token_t *additive_or_shift_expression_token = additive_expression(input,
            position_p);
    return_if_error(additive_or_shift_expression_token);

    // LEFT_RECURSION
    token_t *operator_token;
    while (token_match_punctuator_clone(input, position_p,
                                        PUNCTUATOR_LEFT_SHIFT, &operator_token)
           || token_match_punctuator_clone(input, position_p,
                                           PUNCTUATOR_SIGNED_RIGHT_SHIFT,
                                           &operator_token)
           || token_match_punctuator_clone(input, position_p,
                                           PUNCTUATOR_UNSIGNED_RIGHT_SHIFT,
                                           &operator_token)) {

        token_t *shift_expression_token = token_new_no_data(
                TOKEN_EXPRESSION_SHIFT_EXPRESSION);
        token_add_child(shift_expression_token,
                        additive_or_shift_expression_token);
        token_add_child(shift_expression_token, operator_token);
        tokenize_and_add_child_or_free_parent_and_return_error(input,
                position_p, additive_expression, shift_expression_token)
        additive_or_shift_expression_token = shift_expression_token;
    }

    return additive_or_shift_expression_token;
}

/*
 * AST:
 * RelationalExpression = ShiftExpression
 * RelationalExpression - RelationalExpression (<|>|<=|>=|instanceof)
 *         ShiftExpression
 *
 * GRAMMAR:
 * // NONSTANDARD: No in.
 * RelationalExpression :
 *     ShiftExpression
 *     RelationalExpression < ShiftExpression
 *     RelationalExpression > ShiftExpression
 *     RelationalExpression <= ShiftExpression
 *     RelationalExpression >= ShiftExpression
 *     RelationalExpression instanceof ShiftExpression
 */

token_t *relational_expression(GPtrArray *input, gsize *position_p) {

    token_t *shift_or_relational_expression_token = shift_expression(input,
            position_p);
    return_if_error(shift_or_relational_expression_token);

    // LEFT_RECURSION
    token_t *operator_token;
    while (token_match_punctuator_clone(input, position_p,
                                        PUNCTUATOR_ANGLE_BRACKET_LEFT,
                                        &operator_token)
           || token_match_punctuator_clone(input, position_p,
                                           PUNCTUATOR_ANGLE_BRACKET_RIGHT,
                                           &operator_token)
           || token_match_punctuator_clone(input, position_p,
                                           PUNCTUATOR_LESS_THAN_OR_EQUAL,
                                           &operator_token)
           || token_match_punctuator_clone(input, position_p,
                                           PUNCTUATOR_GREATER_THAN_OR_EQUAL,
                                           &operator_token)
           || token_match_keyword_clone(input, position_p, KEYWORD_INSTANCEOF,
                                        &operator_token)) {

        token_t *relational_expression_token = token_new_no_data(
                TOKEN_EXPRESSION_RELATIONAL_EXPRESSION);
        token_add_child(relational_expression_token,
                        shift_or_relational_expression_token);
        token_add_child(relational_expression_token, operator_token);
        tokenize_and_add_child_or_free_parent_and_return_error(input,
                position_p, shift_expression, relational_expression_token)
        shift_or_relational_expression_token = relational_expression_token;
    }

    return shift_or_relational_expression_token;
}

/*
 * AST:
 * EqualityExpression = RelationalExpression
 * EqualityExpression - EqualityExpression (==|!=|===|!==) RelationalExpression
 *
 * GRAMMAR:
 * EqualityExpression :
 *     RelationalExpression
 *     EqualityExpression == RelationalExpression
 *     EqualityExpression != RelationalExpression
 *     EqualityExpression === RelationalExpression
 *     EqualityExpression !== RelationalExpression
 */

token_t *equality_expression(GPtrArray *input, gsize *position_p) {

    token_t *relational_or_equality_expression_token = relational_expression(
            input, position_p);
    return_if_error(relational_or_equality_expression_token);

    // LEFT_RECURSION
    token_t *operator_token;
    while (token_match_punctuator_clone(input, position_p, PUNCTUATOR_EQUALS,
                                        &operator_token)
           || token_match_punctuator_clone(input, position_p,
                                           PUNCTUATOR_DOES_NOT_EQUAL,
                                           &operator_token)
           || token_match_punctuator_clone(input, position_p,
                                           PUNCTUATOR_STRICT_EQUALS,
                                           &operator_token)
           || token_match_punctuator_clone(input, position_p,
                                           PUNCTUATOR_STRICT_DOES_NOT_EQUAL,
                                           &operator_token)) {

        token_t *equality_expression_token = token_new_no_data(
                TOKEN_EXPRESSION_EQUALITY_EXPRESSION);
        token_add_child(equality_expression_token,
                        relational_or_equality_expression_token);
        token_add_child(equality_expression_token, operator_token);
        tokenize_and_add_child_or_free_parent_and_return_error(input,
                position_p, relational_expression, equality_expression_token)
        relational_or_equality_expression_token = equality_expression_token;
    }

    return relational_or_equality_expression_token;
}

/*
 * AST:
 * BitwiseANDExpression = EqualityExpression
 * BitwiseANDExpression - BitwiseANDExpression EqualityExpression
 *
 * GRAMMAR:
 * BitwiseANDExpression :
 *     EqualityExpression
 *     BitwiseANDExpression & EqualityExpression
 */

token_t *bitwise_and_expression(GPtrArray *input, gsize *position_p) {

    token_t *equality_or_bitwise_and_expression_token = equality_expression(
            input, position_p);
    return_if_error(equality_or_bitwise_and_expression_token);

    // LEFT_RECURSION
    while (token_match_punctuator(input, position_p, PUNCTUATOR_AMPERSAND)) {

        token_t *bitwise_and_expression_token = token_new_no_data(
                TOKEN_EXPRESSION_BITWISE_AND_EXPRESSION);
        token_add_child(bitwise_and_expression_token,
                        equality_or_bitwise_and_expression_token);
        tokenize_and_add_child_or_free_parent_and_return_error(input,
                position_p, equality_expression, bitwise_and_expression_token)
        equality_or_bitwise_and_expression_token = bitwise_and_expression_token;
    }

    return equality_or_bitwise_and_expression_token;
}

/*
 * AST:
 * BitwiseXORExpression = BitwiseANDExpression
 * BitwiseXORExpression - BitwiseXORExpression BitwiseANDExpression
 *
 * GRAMMAR:
 * BitwiseXORExpression :
 *     BitwiseANDExpression
 *     BitwiseXORExpression ^ BitwiseANDExpression
 */

token_t *bitwise_xor_expression(GPtrArray *input, gsize *position_p) {

    token_t *bitwise_and_or_bitwise_xor_expression_token =
            bitwise_and_expression(input, position_p);
    return_if_error(bitwise_and_or_bitwise_xor_expression_token);

    // LEFT_RECURSION
    while (token_match_punctuator(input, position_p, PUNCTUATOR_CARET)) {

        token_t *bitwise_xor_expression_token = token_new_no_data(
                TOKEN_EXPRESSION_BITWISE_XOR_EXPRESSION);
        token_add_child(bitwise_xor_expression_token,
                        bitwise_and_or_bitwise_xor_expression_token);
        tokenize_and_add_child_or_free_parent_and_return_error(input,
                position_p, bitwise_and_expression,
                bitwise_xor_expression_token)
        bitwise_and_or_bitwise_xor_expression_token =
                bitwise_xor_expression_token;
    }

    return bitwise_and_or_bitwise_xor_expression_token;
}

/*
 * AST:
 * BitwiseORExpression = BitwiseXORExpression
 * BitwiseORExpression - BitwiseORExpression BitwiseXORExpression
 *
 * GRAMMAR:
 * BitwiseORExpression :
 *     BitwiseXORExpression
 *     BitwiseORExpression | BitwiseXORExpression
 */

token_t *bitwise_or_expression(GPtrArray *input, gsize *position_p) {

    token_t *bitwise_xor_or_bitwise_or_expression_token =
            bitwise_xor_expression(input, position_p);
    return_if_error(bitwise_xor_or_bitwise_or_expression_token);

    // LEFT_RECURSION
    while (token_match_punctuator(input, position_p, PUNCTUATOR_VERTICAL_BAR)) {

        token_t *bitwise_or_expression_token = token_new_no_data(
                TOKEN_EXPRESSION_BITWISE_OR_EXPRESSION);
        token_add_child(bitwise_or_expression_token,
                        bitwise_xor_or_bitwise_or_expression_token);
        tokenize_and_add_child_or_free_parent_and_return_error(input,
                position_p, bitwise_xor_expression, bitwise_or_expression_token)
        bitwise_xor_or_bitwise_or_expression_token =
                bitwise_or_expression_token;
    }

    return bitwise_xor_or_bitwise_or_expression_token;
}

/*
 * AST:
 * LogicalANDExpression = BitwiseORExpression
 * LogicalANDExpression - LogicalANDExpression BitwiseORExpression
 *
 * GRAMMAR:
 * LogicalANDExpression :
 *     BitwiseORExpression
 *     LogicalANDExpression && BitwiseORExpression
 */

token_t *logical_and_expression(GPtrArray *input, gsize *position_p) {

    token_t *bitwise_or_or_logical_and_expression_token = bitwise_or_expression(
            input, position_p);
    return_if_error(bitwise_or_or_logical_and_expression_token);

    // LEFT_RECURSION
    while (token_match_punctuator(input, position_p, PUNCTUATOR_LOGICAL_AND)) {

        token_t *logical_and_expression_token = token_new_no_data(
                TOKEN_EXPRESSION_LOGICAL_AND_EXPRESSION);
        token_add_child(logical_and_expression_token,
                        bitwise_or_or_logical_and_expression_token);
        tokenize_and_add_child_or_free_parent_and_return_error(input,
                position_p, bitwise_or_expression, logical_and_expression_token)
        bitwise_or_or_logical_and_expression_token =
                logical_and_expression_token;
    }

    return bitwise_or_or_logical_and_expression_token;
}

/*
 * AST:
 * LogicalORExpression = LogicalANDExpression
 * LogicalORExpression - LogicalORExpression LogicalANDExpression
 *
 * GRAMMAR:
 * LogicalORExpression :
 *     LogicalANDExpression
 *     LogicalORExpression || LogicalANDExpression
 */

token_t *logical_or_expression(GPtrArray *input, gsize *position_p) {

    token_t *logical_and_or_logical_or_expression_token =
            logical_and_expression(input, position_p);
    return_if_error(logical_and_or_logical_or_expression_token);

    // LEFT_RECURSION
    while (token_match_punctuator(input, position_p, PUNCTUATOR_LOGICAL_OR)) {

        token_t *logical_or_expression_token = token_new_no_data(
                TOKEN_EXPRESSION_LOGICAL_OR_EXPRESSION);
        token_add_child(logical_or_expression_token,
                        logical_and_or_logical_or_expression_token);
        tokenize_and_add_child_or_free_parent_and_return_error(input,
                position_p, logical_and_expression, logical_or_expression_token)
        logical_and_or_logical_or_expression_token =
                logical_or_expression_token;
    }

    return logical_and_or_logical_or_expression_token;
}

token_t *assignment_expression(GPtrArray *input, gsize *position_p);

gboolean expression_is_first(GPtrArray *input, gsize position) {
    // TODO
    return FALSE;
}

token_t *expression(GPtrArray *input, gsize *position_p);
