//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#include "function_program_parser.h"

#include "lexical_parser.h"
#include "syntactic_parser_utils.h"

/*
 * AST：
 * FunctionDeclaration - Identifier FormalParameterList FunctionBody
 * FunctionExpression - (Identifier|null) FormalParameterList FunctionBody
 *
 * GRAMMAR:
 * FunctionDeclaration :
 *     function Identifier FormalParameterList SourceElementList
 * FunctionExpression :
 *     function Identifier? FormalParameterList SourceElementList
 */

static gboolean function_is_first(GPtrArray *input, gsize position) {
    return token_is_first_keyword(input, position, KEYWORD_FUNCTION);
}

static token_t *function(GPtrArray *input, gsize *position_p,
                         gboolean is_identifier_required, token_id_t token_id,
                         error_id_t function_error_id,
                         error_id_t identifier_error_id) {

    match_keyword_or_return_error(input, position_p, KEYWORD_FUNCTION,
                                  function_error_id)

    token_t *function_token = token_new_no_data(token_id);

    if (!token_is_first_punctuator(input, *position_p,
                                   PUNCTUATOR_PARENTHESIS_LEFT)) {
        match_token_id_clone_and_add_child_or_free_parent_and_return_error(
                input, position_p, TOKEN_LEXICAL_IDENTIFIER, function_token,
                identifier_error_id)
    } else if (is_identifier_required) {
        token_free(&function_token);
        return error_new_syntactic(identifier_error_id, *position_p);
    } else {
        token_add_child(function_token, NULL);
    }

    tokenize_and_add_child_or_free_parent_and_return_error(input, position_p,
            formal_parameter_list, function_token)

    tokenize_and_add_child_or_free_parent_and_return_error(input, position_p,
                                                           function_body,
                                                           function_token)

    return function_token;
}

gboolean function_declaration_is_first(GPtrArray *input, gsize position) {
    return function_is_first(input, position);
}

token_t *function_declaration(GPtrArray *input, gsize *position_p) {
    return function(input, position_p, TRUE,
                    TOKEN_FUNCTION_FUNCTION_DECLARATION,
                    ERROR_FUNCTION_FUNCTION_DECLARATION_FUNCTION,
                    ERROR_FUNCTION_FUNCTION_DECLARATION_IDENTIFIER);
}

gboolean function_expression_is_first(GPtrArray *input, gsize position) {
    return function_is_first(input, position);
}

token_t *function_expression(GPtrArray *input, gsize *position_p) {
    return function(input, position_p, FALSE,
                    TOKEN_FUNCTION_FUNCTION_EXPRESSION,
                    ERROR_FUNCTION_FUNCTION_EXPRESSION_FUNCTION,
                    ERROR_FUNCTION_FUNCTION_EXPRESSION_IDENTIFIER);
}

/*
 * GRAMMAR:
 * FormalParameterList :
 *     ( Identifier?|(Identifier (, Identifier)*) )
 */

token_t *formal_parameter_list(GPtrArray *input, gsize *position_p) {

    match_punctuator_or_return_error(input, position_p,
            PUNCTUATOR_PARENTHESIS_LEFT,
            ERROR_FUNCTION_FORMAL_PARAMETER_LIST_PARENTHESIS_LEFT)

    token_t *formal_parameter_list_token = token_new_no_data(
            TOKEN_FUNCTION_FORMAL_PARAMETER_LIST);

    if (!token_is_first_punctuator(input, *position_p,
                                   PUNCTUATOR_PARENTHESIS_RIGHT)) {
        do {
            match_token_id_clone_and_add_child_or_free_parent_and_return_error(
                    input, position_p, TOKEN_LEXICAL_IDENTIFIER,
                    formal_parameter_list_token,
                    ERROR_FUNCTION_FORMAL_PARAMETER_LIST_IDENTIFIER)
        } while (token_match_punctuator(input, position_p, PUNCTUATOR_COMMA));
    }

    match_punctuator_or_free_and_return_error(input, position_p,
                PUNCTUATOR_PARENTHESIS_RIGHT, formal_parameter_list_token,
                ERROR_FUNCTION_FORMAL_PARAMETER_LIST_PARENTHESIS_RIGHT)

    return formal_parameter_list_token;
}

/*
 * GRAMMAR:
 * FunctionBody :
 *     { SourceElement* }
 */

token_t *function_body(GPtrArray *input, gsize *position_p) {

    match_punctuator_or_return_error(input, position_p,
            PUNCTUATOR_CURLY_BRACE_LEFT,
            ERROR_FUNCTION_FUNCTION_BODY_CURLY_BRACE_LEFT)

    token_t *function_body_token = token_new_no_data(
            TOKEN_FUNCTION_FUNCTION_BODY);

    while (!token_is_first_punctuator(input, *position_p,
                                      PUNCTUATOR_CURLY_BRACE_RIGHT)) {
        tokenize_and_add_child_or_free_parent_and_return_error(input,
                position_p, source_element, function_body_token)
    }

    match_punctuator_or_free_and_return_error(input, position_p,
            PUNCTUATOR_CURLY_BRACE_RIGHT, function_body_token,
            ERROR_FUNCTION_FUNCTION_BODY_CURLY_BRACE_RIGHT)

    return function_body_token;
}
