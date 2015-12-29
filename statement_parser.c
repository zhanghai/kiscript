//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#include "statement_parser.h"

#include "expression_parser.h"
#include "lexical_parser.h"
#include "syntactic_parser_utils.h"

/*
 * AST:
 * TODO
 *
 * GRAMMAR:
 * Statement :
 *     Block
 *     VariableStatement
 *     EmptyStatement
 *     ExpressionStatement
 *     IfStatement
 *     IterationStatement
 *     ContinueStatement
 *     BreakStatement
 *     ReturnStatement
 *     WithStatement
 *     LabelledStatement
 *     SwitchStatement
 *     ThrowStatement
 *     TryStatement
 *     DebuggerStatement
 */
token_t *statement(GPtrArray *input, gsize *position_p) {

    tokenize_and_return_if_is_first(input, position_p, block)
    tokenize_and_return_if_is_first(input, position_p, variable_statement)
    // TODO

    // TODO: Error!
    return NULL;
}

gboolean block_is_first(GPtrArray *input, gsize position) {
    return token_is_first_punctuator(input, position,
                                     PUNCTUATOR_CURLY_BRACE_LEFT);
}

/*
 * AST:
 * Block - Statement*
 *
 * GRAMMAR:
 * Block :
 *     { Statement* }
 */
token_t *block(GPtrArray *input, gsize *position_p) {

    if (!token_match_punctuator(input, position_p,
                                PUNCTUATOR_CURLY_BRACE_LEFT)) {
        return error_new_syntactic(ERROR_STATEMENT_BLOCK_CURLY_BRACE_LEFT,
                                   *position_p);
    }

    token_t *block_token = token_new_no_data(TOKEN_STATEMENT_BLOCK);
    while (!token_match_punctuator(input, position_p,
                                   PUNCTUATOR_CURLY_BRACE_RIGHT)) {

        token_t *statement_token_or_error = statement(input, position_p);
        if (error_is_error(statement_token_or_error)) {
            token_free(&block_token);
            return statement_token_or_error;
        }
        token_add_child(block_token, statement_token_or_error);
    }

    return block_token;
}

gboolean variable_statement_is_first(GPtrArray *input, gsize position) {
    return token_is_first_keyword(input, position, KEYWORD_VAR);
}

/*
 * AST:
 * VariableStatement - VariableDeclaration+
 *
 * GRAMMAR:
 * VariableStatement :
 *     var VariableDeclaration (, VariableDeclaration)* ;
 */
token_t *variable_statement(GPtrArray *input, gsize *position_p) {

    if (!token_match_keyword(input, position_p, KEYWORD_VAR)) {
        return error_new_syntactic(ERROR_STATEMENT_VARIABLE_STATEMENT_VAR,
                                   *position_p);
    }

    token_t *variable_statement_token =
            token_new_no_data(TOKEN_STATEMENT_VARIABLE_STATEMENT);

    do {
        token_t *variable_declaration_token_or_error =
                variable_declaration(input, position_p);
        if (error_is_error(variable_declaration_token_or_error)) {
            token_free(&variable_statement_token);
            return variable_declaration_token_or_error;
        }
        token_add_child(variable_statement_token,
                        variable_declaration_token_or_error);

    } while (token_match_punctuator(input, position_p, PUNCTUATOR_COMMA));

    if (!token_match_punctuator(input, position_p, PUNCTUATOR_SEMICOLON)) {
        return error_new_syntactic(ERROR_STATEMENT_VARIABLE_STATEMENT_SEMICOLON,
                                   *position_p);
    }

    return variable_statement_token;
}

/*
 * AST:
 * VariableDeclaration - Identifier Initializer?
 *
 * GRAMMAR:
 * VariableDeclaration :
 *     Identifier Initializer?
 */
token_t *variable_declaration(GPtrArray *input, gsize *position_p) {

    token_t *variable_declaration_token =
            token_new_no_data(TOKEN_STATEMENT_VARIABLE_DECLARATION);

    token_t *identifier_token;
    if (!token_match_id_clone(input, position_p, TOKEN_LEXICAL_IDENTIFIER,
                             &identifier_token)) {
        token_free(&variable_declaration_token);
        return error_new_syntactic(
                ERROR_STATEMENT_VARIABLE_DECLARATION_IDENTIFIER, *position_p);
    }
    token_add_child(variable_declaration_token, identifier_token);

    if (initializer_is_first(input, *position_p)) {
        token_t *initializer_token_or_error = initializer(input, position_p);
        if (error_is_error(initializer_token_or_error)) {
            token_free(&variable_declaration_token);
            return initializer_token_or_error;
        }
        token_add_child(variable_declaration_token, initializer_token_or_error);
    }

    return variable_declaration_token;
}

gboolean initializer_is_first(GPtrArray *input, gsize position) {
    return token_is_first_punctuator(input, position, PUNCTUATOR_EQUALS_SIGN);
}

/*
 * AST:
 * Initialiser = AssignmentExpression
 *
 * GRAMMAR:
 * Initialiser :
 *     = AssignmentExpression
 */
token_t *initializer(GPtrArray *input, gsize *position_p) {

    if (!token_match_punctuator(input, position_p, PUNCTUATOR_EQUALS_SIGN)) {
        return error_new_syntactic(ERROR_STATEMENT_INITIALIZER_EQUALS_SIGN,
                                   *position_p);
    }

    return assignment_expression(input, position_p);
}
