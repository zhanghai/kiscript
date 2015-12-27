//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#ifndef KISCRIPT_LEXICAL_PARSER_H
#define KISCRIPT_LEXICAL_PARSER_H

#include "parser.h"

void token_init(token_t *token, token_id_t id, gchar *text);

token_t * token_new(token_id_t id, gchar *text);

token_t * token_new_no_data(token_id_t id, gchar *text);

token_t * token_new_strndup_no_data(token_id_t id, gchar *text,
                                    gsize text_length);

void token_final(token_t *token);

void token_free(token_t *token);

#endif //KISCRIPT_LEXICAL_PARSER_H
