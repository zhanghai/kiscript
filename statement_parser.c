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
    token_t *token;
    return token_get(input, position, &token)
           && punctuator_is_punctuator_with_id(token,
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

    if (!token_match_free_punctuator(input, position_p,
                                    PUNCTUATOR_CURLY_BRACE_LEFT)) {
        return error_new_syntactic(ERROR_STATEMENT_BLOCK_CURLY_BRACE_LEFT,
                                   *position_p);
    }

    token_t *block_token = token_new_no_data(TOKEN_STATEMENT_BLOCK);
    while (!token_match_free_punctuator(input, position_p,
                                        PUNCTUATOR_CURLY_BRACE_RIGHT)) {

        token_t *statement_token;
        if (!token_tokenize(input, position_p, statement, &statement_token)) {
            token_free(&block_token);
            return error_new_syntactic(ERROR_STATEMENT_BLOCK_STATEMENT,
                                       *position_p);
        }
        token_add_child(block_token, statement_token);
    }

    return block_token;
}
