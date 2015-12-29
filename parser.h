//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#ifndef KISCRIPT_PARSER_H
#define KISCRIPT_PARSER_H

#include "glib_utils.h"
#include "utils.h"

typedef enum {
    TOKEN_ERROR,
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
    TOKEN_STATEMENT_BLOCK,
} token_id_t;

typedef struct {
    token_id_t id;
    gchar *text;
    gpointer data;
    clone_func_t data_clone_func;
    free_func_t data_free_func;
    GPtrArray *children;
} token_t;

token_t *token_init(token_t *token, token_id_t id, gchar *text, gpointer data,
                    clone_func_t data_clone_func, free_func_t data_free_func);

token_t * token_new(token_id_t id, gchar *text, gpointer data,
                    clone_func_t data_clone_func, free_func_t data_free_func);

token_t *token_new_strndup(token_id_t id, gchar *text, gchar *text_end,
                           gpointer data, clone_func_t data_clone_func,
                           free_func_t data_free_func);

token_t *token_new_strndup_gstring(token_id_t id, gchar *text, gchar *text_end,
                                   GString *string);

token_t *token_new_no_data(token_id_t id, gchar *text);

token_t *token_new_strndup_no_data(token_id_t id, gchar *text, gchar *text_end);

token_t *token_new_no_text(token_id_t id, gpointer data,
                           clone_func_t data_clone_func,
                           free_func_t data_free_func);

token_t *token_new_no_text_no_data(token_id_t id);

token_t *token_clone(token_t *token);

void token_final(token_t *token);

void token_free_no_nullify(token_t *token_p);

void token_free(token_t **token_p);

void token_add_child(token_t *token, token_t *child);


#define DECLARE_TOKEN_IS_TOKEN_FUNC(token_name) \
    gboolean token_name##_is_##token_name(token_t *token);

#define DEFINE_TOKEN_IS_TOKEN_FUNC(token_name, token_id) \
    gboolean token_name##_is_##token_name(token_t *token) { \
        return token->id == (token_id); \
    }

#define DECLARE_TOKEN_GET_ID_FUNC(token_name) \
    token_name##_id_t *token_name##_get_id(token_t *token);

#define DEFINE_TOKEN_GET_ID_FUNC(token_name) \
    token_name##_id_t *token_name##_get_id(token_t *token) { \
            g_assert(token_name##_is_##token_name(token)); \
            return (token_name##_id_t *) token->data; \
    }

#define DECLARE_TOKEN_IS_TOKEN_WITH_ID_FUNC(token_name) \
    gboolean token_name##_is_##token_name##_with_id(token_t *token, \
            token_name##_id_t token_name##_id);

#define DEFINE_TOKEN_IS_TOKEN_WITH_ID_FUNC(token_name) \
    gboolean token_name##_is_##token_name##_with_id(token_t *token, \
            token_name##_id_t token_name##_id) { \
        return token_name##_is_##token_name(token) \
                && *token_name##_get_id(token) == token_name##_id; \
    }


typedef enum {
    ERROR_STATEMENT_BLOCK_CURLY_BRACE_LEFT,
    ERROR_STATEMENT_BLOCK_STATEMENT
} error_id_t;

typedef struct {
    error_id_t id;
    gboolean is_lexical;
    union {
        gchar *lexical_input;
        GPtrArray *syntactic_input;
    };
    gsize position;
} error_info_t;

error_info_t *error_info_init_lexical(error_info_t *error_info, error_id_t id,
                                      gchar *input, gsize position) {
    error_info->id = id;
    error_info->is_lexical = TRUE;
    error_info->lexical_input = input;
    error_info->position = position;
    return error_info;
}

error_info_t *error_info_init_syntactic(error_info_t *error_info, error_id_t id,
                                        GPtrArray *input, gsize position) {
    error_info->id = id;
    error_info->is_lexical = FALSE;
    error_info->syntactic_input = input;
    error_info->position = position;
    return error_info;
}

static error_info_t *error_info_alloc() {
    return g_new(error_info_t, 1);
}

error_info_t *error_info_new_lexical(error_id_t id, gchar *input,
                                     gsize position) {
    return error_info_init_lexical(error_info_alloc(), id, input, position);
}

error_info_t *error_info_new_syntactic(error_id_t id, GPtrArray *input,
                                       gsize position) {
    return error_info_init_syntactic(error_info_alloc(), id, input, position);
}

void error_info_final(error_info_t *error_info) {
    if (error_info->is_lexical) {
        g_free(error_info->lexical_input);
    } else {
        // STOPSHIP: FIXME: Write token_data_clone_func_t
        for (gsize i = 0; i < error_info->syntactic_input->len; ++i) {

        }
    }
}

void error_info_free

token_t *error_new(error_id_t error_id, gchar *text);

token_t *error_new_strndup(error_id_t error_id, gchar *text,
                           gchar *text_end);

token_t *error_new_no_text(error_id_t error_id);

DECLARE_TOKEN_IS_TOKEN_FUNC(error)

DECLARE_TOKEN_GET_ID_FUNC(error)

DECLARE_TOKEN_IS_TOKEN_WITH_ID_FUNC(error)


void normalize_input(gchar **input_p);

#endif //KISCRIPT_PARSER_H
