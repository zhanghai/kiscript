//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#include "parser.h"

static token_t *token_init(token_t *token, token_id_t id, gpointer data,
                           clone_func_t data_clone_func,
                           free_func_t data_free_func, GPtrArray *children) {
    token->id = id;
    token->data = data;
    token->data_clone_func = data_clone_func;
    token->data_free_func = data_free_func;
    token->children = children;
    return token;
}

static token_t *token_new_with_children(token_id_t id, gpointer data,
                                        clone_func_t data_clone_func,
                                        free_func_t data_free_func,
                                        GPtrArray *children) {
    return token_init(g_new(token_t, 1), id, data, data_clone_func,
                      data_free_func, children);
}

token_t *token_new(token_id_t id, gpointer data, clone_func_t data_clone_func,
                   free_func_t data_free_func) {
    return token_new_with_children(id, data, data_clone_func, data_free_func,
                                   token_list_new());
}

static gpointer token_data_gstring_clone_func(gpointer string) {
    return g_string_clone(string);
}

static void token_data_gstring_free_func(gpointer string) {
    g_string_free(string, TRUE);
}

token_t *token_new_gstring(token_id_t id, GString *string) {
    return token_new(id, string, token_data_gstring_clone_func,
                     token_data_gstring_free_func);
}

token_t *token_new_no_data(token_id_t id) {
    return token_new(id, NULL, NULL, NULL);
}

token_t *token_clone(token_t *token) {
    gpointer data = NULL;
    if (token->data) {
        if (token->data_clone_func) {
            data = token->data_clone_func(token->data);
        } else {
            g_error("data_clone_func is NULL");
        }
    }
    return token_new_with_children(token->id, data, token->data_clone_func,
                                   token->data_free_func,
                                   token_list_clone(token->children));
}

static void token_final(token_t *token) {
    if (token->data) {
        if (token->data_free_func) {
            token->data_free_func(token->data);
        } else {
            g_free(token->data);
        }
        token->data = NULL;
    }
    token_list_free(&token->children);
    token->children = NULL;
}

DEFINE_FREE_FUNCS(token)

void token_add_child(token_t *token, token_t *child) {
    g_ptr_array_add(token->children, child);
}


static void token_free_func(gpointer token) {
    token_free(token);
}

GPtrArray *token_list_new() {
    return g_ptr_array_new_with_free_func(token_free_func);
}

GPtrArray *token_list_clone(GPtrArray *token_list) {
    GPtrArray *token_list_clone = g_ptr_array_new_full(token_list->len,
                                                       token_free_func);
    for (gsize i = 0; i < token_list->len; ++i) {
        g_ptr_array_add(token_list_clone,
                        token_clone(g_ptr_array_index(token_list, i)));
    }
    return token_list_clone;
}

static void token_list_final(GPtrArray *token_list) {
    g_ptr_array_free(token_list, TRUE);
}

DEFINE_FREE_FUNCS_WITH_TYPE(token_list, GPtrArray)


static error_info_t *error_info_init_lexical(error_info_t *error_info,
                                             error_id_t id, gchar *input,
                                             gsize position) {
    error_info->id = id;
    error_info->is_lexical = TRUE;
    error_info->input.lexical = input;
    error_info->position = position;
    return error_info;
}

static error_info_t *error_info_init_syntactic(error_info_t *error_info,
                                               error_id_t id, GPtrArray *input,
                                               gsize position) {
    error_info->id = id;
    error_info->is_lexical = FALSE;
    error_info->input.syntactic = input;
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

static void error_info_final(error_info_t *error_info) {
    if (error_info->is_lexical) {
        g_free(error_info->input.lexical);
        error_info->input.lexical = NULL;
    } else {
        token_list_free(&error_info->input.syntactic);
    }
}

DEFINE_FREE_FUNCS(error_info)


static void error_info_free_func(gpointer error_info) {
    error_info_free(error_info);
}

token_t *error_new_lexical(error_id_t id, gchar *input, gchar *position) {
    return token_new(TOKEN_ERROR, error_info_new_lexical(id, input,
                                                         position - input),
                     NULL, error_info_free_func);
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
