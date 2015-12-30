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
    tokenize_and_return_if_is_first(input, position_p, if_statement)
    tokenize_and_return_if_is_first(input, position_p, do_while_statement)
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

    match_punctuator_or_return_error(input, position_p,
                                     PUNCTUATOR_CURLY_BRACE_LEFT,
                                     ERROR_STATEMENT_BLOCK_CURLY_BRACE_LEFT)

    token_t *block_token = token_new_no_data(TOKEN_STATEMENT_BLOCK);
    while (!token_match_punctuator(input, position_p,
                                   PUNCTUATOR_CURLY_BRACE_RIGHT)) {

        tokenize_and_add_child_or_free_parent_and_return_error(input, position_p,
                                                               statement,
                                                               block_token)
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

    match_keyword_or_return_error(input, position_p, KEYWORD_VAR,
                                  ERROR_STATEMENT_VARIABLE_STATEMENT_VAR);

    token_t *variable_statement_token =
            token_new_no_data(TOKEN_STATEMENT_VARIABLE_STATEMENT);

    do {
        tokenize_and_add_child_or_free_parent_and_return_error(input,
                position_p, variable_declaration, variable_statement_token)

    } while (token_match_punctuator(input, position_p, PUNCTUATOR_COMMA));

    match_punctuator_or_free_and_return_error(input, position_p,
            PUNCTUATOR_SEMICOLON, variable_statement_token,
            ERROR_STATEMENT_VARIABLE_STATEMENT_SEMICOLON)

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

    match_token_id_clone_and_add_child_or_free_parent_and_return_error(input,
            position_p, TOKEN_LEXICAL_IDENTIFIER, variable_declaration_token,
            ERROR_STATEMENT_VARIABLE_DECLARATION_IDENTIFIER)

    if (initializer_is_first(input, *position_p)) {
        tokenize_and_add_child_or_free_parent_and_return_error(input,
                position_p, initializer, variable_declaration_token)
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

    match_punctuator_or_return_error(input, position_p, PUNCTUATOR_EQUALS_SIGN,
                                     ERROR_STATEMENT_INITIALIZER_EQUALS_SIGN)

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

    match_punctuator_or_return_error(input, position_p, PUNCTUATOR_SEMICOLON,
                                     ERROR_STATEMENT_EMPTY_STATEMENT_SEMICOLON)

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
    return token_is_first_keyword(input, position, KEYWORD_IF);
}

token_t *if_statement(GPtrArray *input, gsize *position_p) {

    match_keyword_or_return_error(input, position_p, KEYWORD_IF,
                                  ERROR_STATEMENT_IF_STATEMENT_IF)

    match_punctuator_or_return_error(input, position_p,
            PUNCTUATOR_PARENTHESIS_LEFT,
            ERROR_STATEMENT_IF_STATEMENT_PARENTHESIS_LEFT)

    token_t *if_statement_token =
            token_new_no_data(TOKEN_STATEMENT_IF_STATEMENT);

    tokenize_and_add_child_or_free_parent_and_return_error(input, position_p,
                                                           expression,
                                                           if_statement_token)

    match_punctuator_or_free_and_return_error(input, position_p,
            PUNCTUATOR_PARENTHESIS_RIGHT, if_statement_token,
            ERROR_STATEMENT_IF_STATEMENT_PARENTHESIS_RIGHT)

    tokenize_and_add_child_or_free_parent_and_return_error(input, position_p,
                                                           block,
                                                           if_statement_token)

    if (token_match_keyword(input, position_p, KEYWORD_ELSE)) {
        tokenize_and_add_child_or_free_parent_and_return_error(input,
                position_p, block, if_statement_token)
    }

    return if_statement_token;
}

/*
 * AST:
 * DoWhileStatement - Block Expression
 *
 * GRAMMAR:
 * NONSTANDARD:
 * Force Block instead of Statement; No trailing semicolon.
 * DoWhileStatement :
 *     do Block while ( Expression )
 * STANDARD:
 * DoWhileStatement :
 *     do Statement while ( Expression );
 */

gboolean do_while_statement_is_first(GPtrArray *input, gsize position) {
    return token_is_first_keyword(input, position, KEYWORD_DO);
}

token_t *do_while_statement(GPtrArray *input, gsize *position_p) {

    match_keyword_or_return_error(input, position_p, KEYWORD_DO,
                                  ERROR_STATEMENT_DO_WHILE_STATEMENT_DO)

    token_t *do_while_statement_token =
            token_new_no_data(TOKEN_STATEMENT_DO_WHILE_STATEMENT);

    tokenize_and_add_child_or_free_parent_and_return_error(input, position_p,
            block, do_while_statement_token)

    match_keyword_or_free_and_return_error(input, position_p, KEYWORD_WHILE,
            do_while_statement_token, ERROR_STATEMENT_DO_WHILE_STATEMENT_WHILE)

    match_punctuator_or_free_and_return_error(input, position_p,
            PUNCTUATOR_PARENTHESIS_LEFT, do_while_statement_token,
            ERROR_STATEMENT_DO_WHILE_STATEMENT_PARENTHESIS_LEFT)

    tokenize_and_add_child_or_free_parent_and_return_error(input, position_p,
            expression, do_while_statement_token)

    match_punctuator_or_free_and_return_error(input, position_p,
            PUNCTUATOR_PARENTHESIS_RIGHT, do_while_statement_token,
            ERROR_STATEMENT_DO_WHILE_STATEMENT_PARENTHESIS_RIGHT)

    return do_while_statement_token;
}
