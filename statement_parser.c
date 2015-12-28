//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#include "statement_parser.h"

#include "lexical_parser.h"
#include "syntactic_parser_utils.h"

/*
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

token_t *block(GPtrArray *input, gsize *position_p) {

    token_t *token;
    if (token_get(input, *position_p, &token)
        && punctuator_is_punctuator_with_id(token,
                                            PUNCTUATOR_CURLY_BRACE_LEFT)) {
        token_consume_free(input, position_p);
        
    }

    // TODO: Error!
    return NULL;
}
