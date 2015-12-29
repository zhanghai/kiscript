//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#include "statement_parser.h"

#include "lexical_parser.h"
#include "syntactic_parser_utils.h"

/*
 * TODO: AST
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

    return_token_if_is_first(input, position_p, block)
    // TODO

    // TODO: Error!
    return NULL;
}

gboolean block_is_first(GPtrArray *input, gsize position) {
    return token_is_first_punctuator(input, position,
                                     PUNCTUATOR_CURLY_BRACE_LEFT);
}

/*
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

        token_t *statement_token;
        if (!token_tokenize(input, position_p, statement, &statement_token)) {
            token_free(&block_token);
            return statement_token;
        }
        token_add_child(block_token, statement_token);
    }

    return block_token;
}

gboolean variable_statement_is_first(GPtrArray *input, gsize position) {
    token_t *token;
    return token_get(input, position, &token)
           && keyword_is_keyword_with_id(token, KEYWORD_VAR);
}

/*
 * VariableStatement - VariableDeclaration+
 *
 * GRAMMAR:
 * VariableStatement :
 *     var VariableDeclaration+ ;
 */
token_t *variable_statement(GPtrArray *input, gsize *position_p) {

    if (!token_match_keyword(input, position_p, KEYWORD_VAR)) {
        return error_new_syntactic(ERROR_STATEMENT_VARIABLE_STATEMENT_VAR,
                                   *position_p);
    }

    token_t *variable_statement_token =
            token_new_no_data(TOKEN_STATEMENT_BLOCK);
    while (!token_match_punctuator(input, position_p, PUNCTUATOR_SEMICOLON)) {

        token_t *variable_declaration_token;
        if (!token_tokenize(input, position_p, variable_declaration,
                            &variable_declaration_token)) {
            token_free(&variable_statement_token);
            return variable_declaration_token;
        }
        token_add_child(variable_statement_token, variable_declaration_token);
    }

    return variable_statement_token;
}
