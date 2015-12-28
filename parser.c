//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#include "parser.h"

#include <string.h>

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

static void token_data_free_gstring(gpointer string) {
    g_string_free((GString *) string, TRUE);
}

token_t *token_new_strndup_gstring(token_id_t id, gchar *text, gchar *text_end,
                                   GString *string) {
    return token_new_strndup(id, text, text_end, string,
                             token_data_free_gstring);
}

token_t *token_new_no_data(token_id_t id, gchar *text) {
    return token_new(id, text, NULL, NULL);
}

token_t *token_new_strndup_no_data(token_id_t id, gchar *text,
                                   gchar *text_end) {
    return token_new_no_data(id, g_strndup(text, text_end - text));
}

static void token_free_func(gpointer token) {
    token_free_recursive((token_t *) token);
}

static void token_final_internal(token_t *token, gboolean recursive) {
    g_free(token->text);
    if (token->data != NULL) {
        if (token->data_free_func != NULL) {
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

void token_append_child(token_t *token, token_t *child) {
    g_ptr_array_add(token->children, child);
}


gboolean char_is_first(gchar *input, gchar char0) {
    return input[0] == char0;
}

gboolean char_is_match(gunichar input, gchar char0) {
    return input == char0;
}

void char_consume(gchar **input_p) {
    // Always use g_utf8_next_char for safety.
    *input_p = g_utf8_next_char(*input_p);
}

void char_consume_save_char(gchar **input_p, gunichar *char_p) {
    *char_p = g_utf8_get_char(*input_p);
    char_consume(input_p);
}

void char_consume_save_other(gchar **input_p, GString *buffer, gunichar char0) {
    g_string_append_unichar(buffer, char0);
    char_consume(input_p);
}

void char_consume_save(gchar **input_p, GString *buffer) {
    g_string_append_unichar(buffer, g_utf8_get_char(*input_p));
    char_consume(input_p);
}

gboolean char_match(gchar **input_p, gchar char0) {
    if (char_is_first(*input_p, char0)) {
        char_consume(input_p);
        return TRUE;
    } else {
        return FALSE;
    }
}

gboolean char_match_save_char(gchar **input_p, gchar char0, gunichar *char_p) {
    if (char_is_first(*input_p, char0)) {
        char_consume_save_char(input_p, char_p);
        return TRUE;
    } else {
        return FALSE;
    }
}

gboolean char_match_save_other(gchar **input_p, gchar char0, GString *buffer,
                               gunichar char1) {
    if (char_is_first(*input_p, char0)) {
        char_consume_save_other(input_p, buffer, char1);
        return TRUE;
    } else {
        return FALSE;
    }
}

gboolean char_match_save(gchar **input_p, gchar char0, GString *buffer) {
    if (char_is_first(*input_p, char0)) {
        char_consume_save(input_p, buffer);
        return TRUE;
    } else {
        return FALSE;
    }
}

gboolean char_match_not(gchar **input_p, gchar char0) {
    if (!char_is_first(*input_p, char0)) {
        char_consume(input_p);
        return TRUE;
    } else {
        return FALSE;
    }
}

gboolean char_match_not_2(gchar **input_p, gchar char0, gchar char1) {
    if (!char_is_first(*input_p, char0) && !char_is_first(*input_p, char1)) {
        char_consume(input_p);
        return TRUE;
    } else {
        return FALSE;
    }
}

gboolean chars_is_first(gchar *input, gchar *chars) {
    for (gchar *char_p = chars; *char_p; ++char_p) {
        if (char_is_first(input, *char_p)) {
            return TRUE;
        }
    }
    return FALSE;
}

gboolean chars_match(gchar **input_p, gchar *chars) {
    if (chars_is_first(*input_p, chars)) {
        char_consume(input_p);
        return TRUE;
    } else {
        return FALSE;
    }
}

gboolean chars_match_save_char(gchar **input_p, gchar *chars,
                               gunichar *char_p) {
    if (chars_is_first(*input_p, chars)) {
        char_consume_save_char(input_p, char_p);
        return TRUE;
    } else {
        return FALSE;
    }
}

gboolean chars_is_match(gunichar input, gchar *chars) {
    for (gchar *char_p = chars; *char_p; ++char_p) {
        if (char_is_match(input, *char_p)) {
            return TRUE;
        }
    }
    return FALSE;
}

gboolean text_is_first(gchar *input, gchar *text) {
    return g_str_has_prefix(input, text);
}

void text_consume(gchar **input_p, gchar *text) {
    *input_p += strlen(text);
}

void text_consume_save(gchar **input_p, gchar *text, GString *buffer) {
    gsize text_length = strlen(text);
    g_string_append_len(buffer, *input_p, text_length);
    *input_p += text_length;
}

gboolean text_match(gchar **input_p, gchar *text) {
    if (text_is_first(*input_p, text)) {
        text_consume(input_p, text);
        return TRUE;
    } else {
        return FALSE;
    }
}

gboolean text_match_save(gchar **input_p, gchar *text, GString *buffer) {
    if (text_is_first(*input_p, text)) {
        text_consume_save(input_p, text, buffer);
        return TRUE;
    } else {
        return FALSE;
    }
}

gboolean text_is_match(gchar *input, gchar *text) {
    return g_strcmp0(input, text) == 0;
}

gboolean text_array_is_first(gchar *input, gchar **text_array,
                             gsize text_array_length) {
    for (gsize i = 0; i < text_array_length; ++i) {
        if (text_is_first(input, text_array[i])) {
            return TRUE;
        }
    }
    return FALSE;
}

gboolean text_array_match(gchar **input_p, gchar **text_array,
                          gsize text_array_length) {
    for (gsize i = 0; i < text_array_length; ++i) {
        if (text_match(input_p, text_array[i])) {
            return TRUE;
        }
    }
    return FALSE;
}

gboolean text_array_match_save_index(gchar **input_p, gchar **text_array,
                                     gsize text_array_length, gsize *index_p) {
    for (gsize i = 0; i < text_array_length; ++i) {
        if (text_match(input_p, text_array[i])) {
            *index_p = i;
            return TRUE;
        }
    }
    return FALSE;
}

gboolean text_array_is_match(gchar *input, gchar **text_array,
                             gsize text_array_length) {
    for (gsize i = 0; i < text_array_length; ++i) {
        if (text_is_match(input, text_array[i])) {
            return TRUE;
        }
    }
    return FALSE;
}


void normalize_input(gchar **input_p) {

    *input_p = g_utf8_normalize(*input_p, -1, G_NORMALIZE_DEFAULT);

    // TODO: Error!
}
