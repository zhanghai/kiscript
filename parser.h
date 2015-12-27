//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#ifndef KISCRIPT_PARSER_H
#define KISCRIPT_PARSER_H

#include <glib.h>

typedef enum {
    TOKEN_LEXICAL_INPUT_ELEMENT_DIV,
    TOKEN_LEXICAL_WHITE_SPACE,
    TOKEN_LEXICAL_LINE_TERMINATOR,
    TOKEN_LEXICAL_LINE_TERMINATOR_SEQUENCE,
    TOKEN_LEXICAL_COMMENT,
    TOKEN_LEXICAL_MULTI_LINE_COMMENT,
    TOKEN_LEXICAL_MULTI_LINE_COMMENT_CHARS,
    TOKEN_LEXICAL_POST_ASTERISK_COMMENT_CHARS,
    TOKEN_LEXICAL_MULTI_LINE_NOT_ASTERISK_CHAR,
    TOKEN_LEXICAL_MULTI_LINE_NOT_FORWARD_SLASH_OR_ASTERISK_CHAR,
    TOKEN_LEXICAL_SINGLE_LINE_COMMENT,
    TOKEN_LEXICAL_SINGLE_LINE_COMMENT_CHARS,
    TOKEN_LEXICAL_SINGLE_LINE_COMMENT_CHAR
} token_id_t;

typedef void (*token_data_free_func_t)(gpointer *data);

typedef struct {
    token_id_t id;
    gchar *text;
    gpointer data;
    token_data_free_func_t data_free;
} token_t;

#endif //KISCRIPT_PARSER_H
