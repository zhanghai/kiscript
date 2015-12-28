//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#include "parser.h"

gboolean g_ptr_array_in_range(GPtrArray *array, gsize position) {
    return position < array->len;
}


void token_init(token_t *token, token_id_t id, gchar *text, gpointer data,
                GDestroyNotify data_free_func) {
    token->id = id;
    token->text = text;
    token->data = data;
    token->data_free_func = data_free_func;
    token->children = g_ptr_array_new();
}

token_t *token_new(token_id_t id, gchar *text, gpointer data,
                   GDestroyNotify data_free_func) {
    token_t *token = g_new(token_t, 1);
    token_init(token, id, text, data, data_free_func);
    return token;
}

token_t *token_new_strndup(token_id_t id, gchar *text, gchar *text_end,
                           gpointer data, GDestroyNotify data_free_func) {
    return token_new(id, g_strndup(text, text_end - text), data,
                     data_free_func);
}

static void token_data_gstring_free_func(gpointer string) {
    g_string_free((GString *) string, TRUE);
}

token_t *token_new_strndup_gstring(token_id_t id, gchar *text, gchar *text_end,
                                   GString *string) {
    return token_new_strndup(id, text, text_end, string,
                             token_data_gstring_free_func);
}

token_t *token_new_no_data(token_id_t id, gchar *text) {
    return token_new(id, text, NULL, NULL);
}

token_t *token_new_strndup_no_data(token_id_t id, gchar *text,
                                   gchar *text_end) {
    return token_new_no_data(id, g_strndup(text, text_end - text));
}

token_t *token_new_no_text(token_id_t id, gpointer data,
                           GDestroyNotify data_free_func) {
    return token_new(id, NULL, data, data_free_func);
}

token_t *token_new_no_text_no_data(token_id_t id) {
    return token_new_no_text(id, NULL, NULL);
}

static void token_free_func(gpointer token) {
    token_free_recursive((token_t *) token);
}

static void token_final_internal(token_t *token, gboolean recursive) {
    if (token->text) {
        g_free(token->text);
    }
    if (token->data) {
        if (token->data_free_func) {
            token->data_free_func(token->data);
        } else {
            g_free(token->data);
        }
    }
    if (recursive) {
        g_ptr_array_set_free_func(token->children, token_free_func);
    }
    g_ptr_array_free(token->children, recursive);
}

void token_final(token_t *token) {
    token_final_internal(token, FALSE);
}

void token_final_recursive(token_t *token) {
    token_final_internal(token, TRUE);
}

void token_free(token_t *token) {
    token_final(token);
    g_free(token);
}

void token_free_recursive(token_t *token) {
    token_final_recursive(token);
    g_free(token);
}

void token_add_child(token_t *token, token_t *child) {
    g_ptr_array_add(token->children, child);
}


void normalize_input(gchar **input_p) {

    *input_p = g_utf8_normalize(*input_p, -1, G_NORMALIZE_DEFAULT);

    // TODO: Error!
}
