//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#ifndef KISCRIPT_PARSER_H
#define KISCRIPT_PARSER_H

#include <glib.h>

gboolean g_ptr_array_in_range(GPtrArray *array, gsize position);


typedef enum {
    TOKEN_LEXICAL_WHITE_SPACE,
    TOKEN_LEXICAL_LINE_TERMINATOR,
    TOKEN_LEXICAL_LINE_TERMINATOR_SEQUENCE,
    TOKEN_LEXICAL_MULTI_LINE_COMMENT,
    TOKEN_LEXICAL_SINGLE_LINE_COMMENT,
    TOKEN_LEXICAL_IDENTIFIER,
    TOKEN_LEXICAL_KEYWORD,
    TOKEN_LEXICAL_FUTURE_RESERVED_WORD,
    TOKEN_LEXICAL_PUNCTUATOR,
    TOKEN_LEXICAL_DIV_PUNCTUATOR,
    TOKEN_LEXICAL_NULL_LITERAL,
    TOKEN_LEXICAL_BOOLEAN_LITERAL,
    TOKEN_LEXICAL_NUMERIC_LITERAL,
    TOKEN_LEXICAL_STRING_LITERAL,
} token_id_t;

typedef struct {
    token_id_t id;
    gchar *text;
    gpointer data;
    GDestroyNotify data_free_func;
    GPtrArray *children;
} token_t;

void token_init(token_t *token, token_id_t id, gchar *text, gpointer data,
                GDestroyNotify data_free_func);

token_t * token_new(token_id_t id, gchar *text, gpointer data,
                    GDestroyNotify data_free_func);

token_t *token_new_strndup(token_id_t id, gchar *text, gchar *text_end,
                           gpointer data, GDestroyNotify data_free_func);

token_t *token_new_strndup_gstring(token_id_t id, gchar *text, gchar *text_end,
                                   GString *string);

token_t * token_new_no_data(token_id_t id, gchar *text);

token_t * token_new_strndup_no_data(token_id_t id, gchar *text,
                                    gchar *text_end);

void token_final(token_t *token);

void token_final_recursive(token_t *token);

void token_free(token_t *token);

void token_free_recursive(token_t *token);

void token_append_child(token_t *token, token_t *child);


void normalize_input(gchar **input_p);

#endif //KISCRIPT_PARSER_H
