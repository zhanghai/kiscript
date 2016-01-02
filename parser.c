//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#include "parser.h"

static void token_init(token_t *token, token_id_t id, gpointer data,
                       clone_func_t data_clone_func,
                       free_func_t data_free_func, GPtrArray *children) {
    token->id = id;
    token->data = data;
    token->data_clone_func = data_clone_func;
    token->data_free_func = data_free_func;
    token->children = children;
}

static token_t *token_new_with_children(token_id_t id, gpointer data,
                                        clone_func_t data_clone_func,
                                        free_func_t data_free_func,
                                        GPtrArray *children) {
    token_t *token = g_new_1(token_t);
    token_init(token, id, data, data_clone_func, data_free_func, children);
    return token;
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
}

DEFINE_FREE_FUNCS(token)

void token_add_child(token_t *token, token_t *child) {
    g_ptr_array_add(token->children, child);
}

gboolean token_has_child(token_t *token, gsize position) {
    return position < token->children->len;
}

token_t *token_get_child(token_t *token, gsize position) {
    g_assert(token_has_child(token, position));
    return g_ptr_array_index(token->children, position);
}

token_t *token_get_last_child(token_t *token) {
    g_assert(token->children->len > 0);
    return token_get_child(token, token->children->len - 1);
}


static void token_free_func(gpointer token) {
    if (token) {
        token_free(token);
    }
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

void token_list_free_no_nullify(GPtrArray *token_list) {
    g_ptr_array_free(token_list, TRUE);
}

void token_list_free(GPtrArray **token_list_p) {
    token_list_free_no_nullify(*token_list_p);
    *token_list_p = NULL;
}


static void error_info_init_lexical(error_info_t *error_info, error_id_t id,
                                    gchar *position) {
    error_info->id = id;
    error_info->is_lexical = TRUE;
    error_info->position.lexical = position;
}

static void error_info_init_syntactic(error_info_t *error_info, error_id_t id,
                                      gsize position) {
    error_info->id = id;
    error_info->is_lexical = FALSE;
    error_info->position.syntactic = position;
}

error_info_t *error_info_new_lexical(error_id_t id, gchar *position) {
    error_info_t *error_info = g_new_1(error_info_t);
    error_info_init_lexical(error_info, id, position);
    return error_info;
}

error_info_t *error_info_new_syntactic(error_id_t id, gsize position) {
    error_info_t *error_info = g_new_1(error_info_t);
    error_info_init_syntactic(error_info, id, position);
    return error_info;
}

static void error_info_final(error_info_t *error_info) {
    if (error_info->is_lexical) {
        error_info->position.lexical = NULL;
    }
}

DEFINE_FREE_FUNCS(error_info)


static void error_info_free_func(gpointer error_info) {
    error_info_free(error_info);
}

static token_t *error_new(error_info_t *info) {
    return token_new(TOKEN_ERROR, info, NULL, error_info_free_func);
}

token_t *error_new_lexical(error_id_t id, gchar *position) {
    return error_new(error_info_new_lexical(id, position));
}

token_t *error_new_syntactic(error_id_t id, gsize position) {
    return error_new(error_info_new_syntactic(id, position));
}

DEFINE_TOKEN_IS_TOKEN_FUNC(error, TOKEN_ERROR)

DEFINE_TOKEN_GET_ID_FUNC(error)

DEFINE_TOKEN_IS_TOKEN_WITH_ID_FUNC(error)
