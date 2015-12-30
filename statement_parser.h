//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#ifndef KISCRIPT_STATEMENT_PARSER_H
#define KISCRIPT_STATEMENT_PARSER_H

#include "parser.h"

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

#endif //KISCRIPT_STATEMENT_PARSER_H
