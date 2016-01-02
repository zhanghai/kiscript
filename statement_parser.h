//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#ifndef KISCRIPT_STATEMENT_PARSER_H
#define KISCRIPT_STATEMENT_PARSER_H

#include "parser.h"

gboolean statement_is_first(GPtrArray *input, gsize position);

token_t *statement(GPtrArray *input, gsize *position_p);

gboolean block_is_first(GPtrArray *input, gsize position);

token_t *block(GPtrArray *input, gsize *position_p);

gboolean variable_statement_is_first(GPtrArray *input, gsize position);

token_t *variable_statement(GPtrArray *input, gsize *position_p);

token_t *variable_declaration(GPtrArray *input, gsize *position_p);

gboolean initializer_is_first(GPtrArray *input, gsize position);

token_t *initializer(GPtrArray *input, gsize *position_p);

gboolean empty_statement_is_first(GPtrArray *input, gsize position);

token_t *empty_statement(GPtrArray *input, gsize *position_p);

gboolean if_statement_is_first(GPtrArray *input, gsize position);

token_t *if_statement(GPtrArray *input, gsize *position_p);

gboolean do_while_statement_is_first(GPtrArray *input, gsize position);

token_t *do_while_statement(GPtrArray *input, gsize *position_p);

gboolean while_statement_is_first(GPtrArray *input, gsize position);

token_t *while_statement(GPtrArray *input, gsize *position_p);

gboolean for_statement_is_first(GPtrArray *input, gsize position);

token_t *for_statement(GPtrArray *input, gsize *position_p);

gboolean continue_statement_is_first(GPtrArray *input, gsize position);

token_t *continue_statement(GPtrArray *input, gsize *position_p);

gboolean break_statement_is_first(GPtrArray *input, gsize position);

token_t *break_statement(GPtrArray *input, gsize *position_p);

gboolean return_statement_is_first(GPtrArray *input, gsize position);

token_t *return_statement(GPtrArray *input, gsize *position_p);

gboolean labeled_statement_is_first(GPtrArray *input, gsize position);

token_t *labeled_statement(GPtrArray *input, gsize *position_p);

gboolean switch_statement_is_first(GPtrArray *input, gsize position);

token_t *switch_statement(GPtrArray *input, gsize *position_p);

gboolean case_clause_is_first(GPtrArray *input, gsize position);

token_t *case_clause(GPtrArray *input, gsize *position_p);

gboolean default_clause_is_first(GPtrArray *input, gsize position);

token_t *default_clause(GPtrArray *input, gsize *position_p);

gboolean throw_statement_is_first(GPtrArray *input, gsize position);

token_t *throw_statement(GPtrArray *input, gsize *position_p);

gboolean try_statement_is_first(GPtrArray *input, gsize position);

token_t *try_statement(GPtrArray *input, gsize *position_p);

gboolean debugger_statement_is_first(GPtrArray *input, gsize position);

token_t *debugger_statement(GPtrArray *input, gsize *position_p);

gboolean expression_statement_is_first(GPtrArray *input, gsize position);

token_t *expression_statement(GPtrArray *input, gsize *position_p);

#endif //KISCRIPT_STATEMENT_PARSER_H
