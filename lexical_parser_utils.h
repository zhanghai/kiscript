//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#ifndef KISCRIPT_LEXICAL_PARSER_UTILS_H
#define KISCRIPT_LEXICAL_PARSER_UTILS_H

#include "parser.h"

gboolean char_is_first(gchar *input, gchar char0);

gboolean char_is_match(gunichar input, gchar char0);

void char_consume(gchar **input_p);

void char_consume_save_char(gchar **input_p, gunichar *char_p);

void char_consume_save_other(gchar **input_p, GString *buffer, gunichar char0);

void char_consume_save(gchar **input_p, GString *buffer);

gboolean char_match(gchar **input_p, gchar char0);

gboolean char_match_save_char(gchar **input_p, gchar char0, gunichar *char_p);

gboolean char_match_save_other(gchar **input_p, gchar char0, GString *buffer,
                               gunichar char1);

gboolean char_match_save(gchar **input_p, gchar char0, GString *buffer);

gboolean char_match_not(gchar **input_p, gchar char0);

gboolean char_match_not_2(gchar **input_p, gchar char0, gchar char1);

gboolean chars_is_first(gchar *input, gchar *chars);

gboolean chars_match(gchar **input_p, gchar *chars);

gboolean chars_match_save_char(gchar **input_p, gchar *chars, gunichar *char_p);

gboolean chars_is_match(gunichar input, gchar *chars);

gboolean text_is_first(gchar *input, gchar *text);

void text_consume(gchar **input_p, gchar *text);

void text_consume_save(gchar **input_p, gchar *text, GString *buffer);

gboolean text_match(gchar **input_p, gchar *text);

gboolean text_match_save(gchar **input_p, gchar *text, GString *buffer);

gboolean text_is_match(gchar *input, gchar *text);

gboolean text_array_is_first(gchar *input, gchar **text_array,
                             gsize text_array_length);

gboolean text_array_match(gchar **input_p, gchar **text_array,
                          gsize text_array_length);

gboolean text_array_match_save_index(gchar **input_p, gchar **text_array,
                                     gsize text_array_length, gsize *index_p);

gboolean text_array_is_match(gchar *input, gchar **text_array,
                             gsize text_array_length);


#define return_token_if_is_first(input_p, token_name) \
    if (token_name##_is_first(*(input_p))) { \
        return token_name((input_p));\
    }

#define try_match_and_return_token(input_p, match, token_id) \
    { \
        char *input_old = *(input_p); \
        if ((match)) { \
            return token_new_strndup_no_data((token_id), input_old, \
                                             *(input_p)); \
        } \
    }

#define try_match_char_and_return_token(input_p, token_char, token_id) \
    try_match_and_return_token((input_p), char_match((input_p), (token_char)), \
                               (token_id))

#define try_match_text_and_return_token(input_p, token_text, token_id) \
    try_match_and_return_token((input_p), text_match((input_p), (token_text)), \
                               (token_id))


#define DEFINE_MATCH_ANY_FUNC(match_func) \
    void match_func##_any(gchar **input_p) { \
        while (match_func(input_p)) {} \
    }

#define DEFINE_MATCH_ANY_SAVE_FUNC(match_func_name, match_func) \
    void match_func_name(gchar **input_p, GString *buffer) { \
        while (match_func(input_p, buffer)) {} \
    }

#define DEFINE_MATCH_MULTIPLE_FUNC(match_func) \
    gboolean match_func##_multiple(gchar **input_p) { \
        if (match_func(input_p)) { \
            while (match_func(input_p)) {} \
            return TRUE; \
        } else { \
            return FALSE; \
        } \
    }

#define DEFINE_MATCH_MULTIPLE_SAVE_FUNC(match_func_name, match_func) \
    gboolean match_func_name(gchar **input_p, GString *buffer) { \
        if (match_func(input_p, buffer)) { \
            while (match_func(input_p, buffer)) {} \
            return TRUE; \
        } else { \
            return FALSE; \
        } \
    }

#define DEFINE_ENUM_NEW(enum_name) \
    enum_name##_t *enum_name##_new(enum_name##_t value) { \
        enum_name##_t *value_p = g_new(enum_name##_t, 1); \
        *value_p = value; \
        return value_p; \
    }

#endif //KISCRIPT_LEXICAL_PARSER_UTILS_H