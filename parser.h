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
    TOKEN_LEXICAL_SINGLE_LINE_COMMENT,
    TOKEN_LEXICAL_IDENTIFIER_NAME,
    TOKEN_LEXICAL_PUNCTUATOR,
    TOKEN_LEXICAL_DIV_PUNCTUATOR,
    TOKEN_LEXICAL_NUMERIC_LITERAL,
    TOKEN_LEXICAL_DECIMAL_LITERAL,
    TOKEN_LEXICAL_DECIMAL_INTEGER_LITERAL,
    TOKEN_LEXICAL_BINARY_INTEGER_LITERAL,
    TOKEN_LEXICAL_OCTAL_INTEGER_LITERAL,
    TOKEN_LEXICAL_HEX_INTEGER_LITERAL
} token_id_t;

typedef void (*token_data_free_func_t)(gpointer *data);

typedef struct {
    token_id_t id;
    gchar *text;
    gpointer data;
    token_data_free_func_t data_free_func;
} token_t;

void token_init(token_t *token, token_id_t id, gchar *text, gpointer data,
                token_data_free_func_t data_free_func);

token_t * token_new(token_id_t id, gchar *text, gpointer data,
                    token_data_free_func_t data_free_func);

token_t *token_new_strndup(token_id_t id, gchar *text, gchar *text_end,
                           gpointer data,
                           token_data_free_func_t data_free_func);

token_t * token_new_no_data(token_id_t id, gchar *text);

token_t * token_new_strndup_no_data(token_id_t id, gchar *text,
                                    gchar *text_end);

void token_final(token_t *token);

void token_free(token_t *token);


void normalize_input(gchar **input_p);

#endif //KISCRIPT_PARSER_H
