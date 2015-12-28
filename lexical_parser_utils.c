//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#include "lexical_parser_utils.h"

#include <string.h>

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
