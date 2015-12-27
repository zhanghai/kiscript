//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#include "parser.h"

void token_init(token_t *token, token_id_t id, gchar *text, gpointer data,
                token_data_free_func_t data_free_func) {
    token->id = id;
    token->text = text;
    token->data = data;
    token->data_free_func = data_free_func;
}

token_t *token_new(token_id_t id, gchar *text, gpointer data,
                   token_data_free_func_t data_free_func) {
    token_t *token = g_new(token_t, 1);
    token_init(token, id, text, data, data_free_func);
    return token;
}

token_t *token_new_strndup(token_id_t id, gchar *text, gchar *text_end,
                           gpointer data,
                           token_data_free_func_t data_free_func) {
    return token_new(id, g_strndup(text, text_end - text), data,
                     data_free_func);
}

token_t *token_new_no_data(token_id_t id, gchar *text) {
    return token_new(id, text, NULL, NULL);
}

token_t *token_new_strndup_no_data(token_id_t id, gchar *text,
                                   gchar *text_end) {
    return token_new_no_data(id, g_strndup(text, text_end - text));
}

void token_final(token_t *token) {
    g_free(token->text);
    if (token->data != NULL) {
        if (token->data_free_func != NULL) {
            token->data_free_func(token->data);
        } else {
            g_free(token->data);
        }
    }
}

void token_free(token_t *token) {
    token_final(token);
    g_free(token);
}


void normalize_input(gchar **input_p) {

    *input_p = g_utf8_normalize(*input_p, -1, G_NORMALIZE_DEFAULT);

    // TODO: Error!
}
