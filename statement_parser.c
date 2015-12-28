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
 * Block -> StatementList?
 *
 * GRAMMAR:
 * Block :
 *     { StatementList? }
 */
token_t *block(GPtrArray *input, gsize *position_p) {

    token_t *token;
    do {
        if (token_get_consume_free_punctuator_with_id(input, position_p,
                PUNCTUATOR_CURLY_BRACE_LEFT)) {
            token_t *statement_list_token = NULL;
            if (!token_get_consume_free_punctuator_with_id(input,
                    position_p, PUNCTUATOR_CURLY_BRACE_RIGHT)) {
                statement_list_token = statement_list(input, position_p);
                if (!token_get_consume_free_punctuator_with_id(input,
                        position_p, PUNCTUATOR_CURLY_BRACE_RIGHT)) {
                    break;
                }
            }
            token = token_new_no_text_no_data(TOKEN_STATEMENT_BLOCK);
            if (statement_list_token) {
                token_add_child(token, statement_list_token);
            }
            return token;
        }
    } while (FALSE);

    // TODO: Error!
    return NULL;
}

token_t *statement_list(GPtrArray *input, gsize *position_p) {

    // TODO

    // TODO: Error!
    return NULL;
}
