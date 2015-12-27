//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#ifndef KISCRIPT_LEXICAL_PARSER_H
#define KISCRIPT_LEXICAL_PARSER_H

#include <glib.h>

typedef enum {
    LEXICAL_TOKEN_INPUT_ELEMENT_DIV,
    LEXICAL_TOKEN_WHITE_SPACE,
    LEXICAL_TOKEN_LINE_TERMINATOR,
    LEXICAL_TOKEN_LINE_TERMINATOR_SEQUENCE
} lexical_token_id_t;

typedef struct {
    lexical_token_id_t id;
    gchar *text;
} lexical_token_t;

#endif //KISCRIPT_LEXICAL_PARSER_H
