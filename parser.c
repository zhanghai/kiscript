//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#include "parser.h"

static void token_free_func(gpointer token) {
    token_free(token);
}

token_t *token_init(token_t *token, token_id_t id, gchar *text, gpointer data,
                    clone_func_t data_clone_func, free_func_t data_free_func) {
    token->id = id;
    token->text = text;
    token->data = data;
    token->data_clone_func = data_clone_func;
    token->data_free_func = data_free_func;
    g_ptr_array_set_free_func(token->children, token_free_func);
    token->children = g_ptr_array_new();
    return token;
}

token_t *token_new(token_id_t id, gchar *text, gpointer data,
                   clone_func_t data_clone_func, free_func_t data_free_func) {
    return token_init(g_new(token_t, 1), id, text, data, data_clone_func,
                      data_free_func);
}

token_t *token_new_strndup(token_id_t id, gchar *text, gchar *text_end,
                           gpointer data, clone_func_t data_clone_func,
                           free_func_t data_free_func) {
    return token_new(id, g_strndup(text, text_end - text), data,
                     data_clone_func, data_free_func);
}

static gpointer token_data_gstring_clone_func(gpointer string) {
    return g_string_clone(string);
}

static void token_data_gstring_free_func(gpointer string) {
    g_string_free(string, TRUE);
}

token_t *token_new_strndup_gstring(token_id_t id, gchar *text, gchar *text_end,
                                   GString *string) {
    return token_new_strndup(id, text, text_end, string,
                             token_data_gstring_clone_func,
                             token_data_gstring_free_func);
}

token_t *token_new_no_data(token_id_t id, gchar *text) {
    return token_new(id, text, NULL, NULL, NULL);
}

token_t *token_new_strndup_no_data(token_id_t id, gchar *text,
                                   gchar *text_end) {
    return token_new_no_data(id, g_strndup(text, text_end - text));
}

token_t *token_new_no_text(token_id_t id, gpointer data,
                           clone_func_t data_clone_func,
                           free_func_t data_free_func) {
    return token_new(id, NULL, data, data_clone_func, data_free_func);
}

token_t *token_new_no_text_no_data(token_id_t id) {
    return token_new_no_text(id, NULL, NULL, NULL);
}

static void token_final(token_t *token) {
    if (token->text) {
        g_free(token->text);
        token->text = NULL;
    }
    if (token->data) {
        if (token->data_free_func) {
            token->data_free_func(token->data);
        } else {
            g_free(token->data);
        }
        token->data = NULL;
    }
    g_ptr_array_free(token->children, TRUE);
    token->children = NULL;
}

void token_free_no_nullify(token_t *token_p) {
    token_final(token_p);
    g_free(token_p);
}

void token_free(token_t **token_p) {
    token_free_no_nullify(*token_p);
    *token_p = NULL;
}

void token_add_child(token_t *token, token_t *child) {
    g_ptr_array_add(token->children, child);
}


static DEFINE_ENUM_NEW(error_id)

token_t *error_new(error_id_t error_id, gchar *text) {
    return token_new(TOKEN_ERROR, text, error_id_new(error_id), NULL);
}

token_t *error_new_strndup(error_id_t error_id, gchar *text,
                           gchar *text_end) {
    return error_new(error_id, g_strndup(text, text_end - text));
}

token_t *error_new_token_list_position(error_id_t error_id,
                                       GPtrArray *token_list, gsize position) {
    return error_new(error_id, NULL);
}

DEFINE_TOKEN_IS_TOKEN_FUNC(error, TOKEN_ERROR)

DEFINE_TOKEN_GET_ID_FUNC(error)

DEFINE_TOKEN_IS_TOKEN_WITH_ID_FUNC(error)


void normalize_input(gchar **input_p) {

    *input_p = g_utf8_normalize(*input_p, -1, G_NORMALIZE_DEFAULT);

    // TODO: Error!
}
