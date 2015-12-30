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
 *     // Moved down for ambiguity
 *     ExpressionStatement
 */

token_t *statement(GPtrArray *input, gsize *position_p) {

    tokenize_and_return_if_is_first(input, position_p, block)
    tokenize_and_return_if_is_first(input, position_p, variable_statement)
    // NONSTANDARD: No EmptyStatement.
    //tokenize_and_return_if_is_first(input, position_p, empty_statement)
    // TODO

    // TODO: Error!
    return NULL;
}

/*
 * AST:
 * Block - Statement*
 *
 * GRAMMAR:
 * Block :
 *     { Statement* }
 */

gboolean block_is_first(GPtrArray *input, gsize position) {
    return token_is_first_punctuator(input, position,
                                     PUNCTUATOR_CURLY_BRACE_LEFT);
}

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

/*
 * AST:
 * VariableStatement - VariableDeclaration+
 *
 * GRAMMAR:
 * VariableStatement :
 *     var VariableDeclaration (, VariableDeclaration)* ;
 */

gboolean variable_statement_is_first(GPtrArray *input, gsize position) {
    return token_is_first_keyword(input, position, KEYWORD_VAR);
}

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

/*
 * AST:
 * Initialiser = AssignmentExpression
 *
 * GRAMMAR:
 * Initialiser :
 *     = AssignmentExpression
 */

gboolean initializer_is_first(GPtrArray *input, gsize position) {
    return token_is_first_punctuator(input, position, PUNCTUATOR_EQUALS_SIGN);
}

token_t *initializer(GPtrArray *input, gsize *position_p) {

    if (!token_match_punctuator(input, position_p, PUNCTUATOR_EQUALS_SIGN)) {
        return error_new_syntactic(ERROR_STATEMENT_INITIALIZER_EQUALS_SIGN,
                                   *position_p);
    }

    return assignment_expression(input, position_p);
}

/*
 * AST:
 * EmptyStatement = EmptyStatement
 *
 * GRAMMAR:
 * EmptyStatement :
 * ;
 */

gboolean empty_statement_is_first(GPtrArray *input, gsize position) {
    return token_is_first_punctuator(input, position, PUNCTUATOR_SEMICOLON);
}

token_t *empty_statement(GPtrArray *input, gsize *position_p) {

    if (!token_match_punctuator(input, position_p, PUNCTUATOR_SEMICOLON)) {
        return error_new_syntactic(ERROR_STATEMENT_EMPTY_STATEMENT_SEMICOLON,
                                   *position_p);
    }

    return token_new_no_data(TOKEN_STATEMENT_EMPTY_STATEMENT);
}

/*
 * AST:
 * IfStatement - Expression Block Block?
 *
 * GRAMMAR:
 * NONSTANDARD:
 * Force Block instead of Statement.
 * IfStatement :
 *     if ( Expression ) Block (else Block)?
 * STANDARD:
 * IfStatement :
 *     if ( Expression ) Statement (else Statement)?
 */

gboolean if_statement_is_first(GPtrArray *input, gsize position) {
    return token_match_keyword(input, &position,KEYWORD_IF)
           && token_is_first_punctuator(input, position,
                                        PUNCTUATOR_PARENTHESIS_LEFT);
}

token_t *if_statement(GPtrArray *input, gsize *position_p) {

    if (!token_match_keyword(input, position_p, KEYWORD_IF)) {
        return error_new_syntactic(ERROR_STATEMENT_IF_STATEMENT_IF,
                                   *position_p);
    }

    if (!token_match_keyword(input, position_p, KEYWORD_IF)) {
        return error_new_syntactic(
                ERROR_STATEMENT_IF_STATEMENT_PARENTHESIS_LEFT, *position_p);
    }

    token_t *if_statement_token =
            token_new_no_data(TOKEN_STATEMENT_IF_STATEMENT);

    token_t *expression_token_or_error = expression(input, position_p);
    if (error_is_error(expression_token_or_error)) {
        token_free(&if_statement_token);
        return expression_token_or_error;
    }
    token_add_child(if_statement_token, expression_token_or_error);

    if (!token_match_punctuator(input, position_p,
                                PUNCTUATOR_PARENTHESIS_RIGHT)) {
        token_free(&if_statement_token);
        return error_new_syntactic(
                ERROR_STATEMENT_IF_STATEMENT_PARENTHESIS_RIGHT, *position_p);
    }

    token_t *block_token_or_error = block(input, position_p);
    if (error_is_error(block_token_or_error)) {
        token_free(&if_statement_token);
        return block_token_or_error;
    }
    token_add_child(if_statement_token, block_token_or_error);

    if (token_match_keyword(input, position_p, KEYWORD_ELSE)) {
        token_t *block_else_token_or_error = block(input, position_p);
        if (error_is_error(block_else_token_or_error)) {
            token_free(&if_statement_token);
            return block_else_token_or_error;
        }
        token_add_child(if_statement_token, block_else_token_or_error);
    }

    return if_statement_token;
}
