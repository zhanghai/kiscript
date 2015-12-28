//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#include "lexical_parser.h"

#include <string.h>
#include <errno.h>

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

gboolean char_match_save_other(gchar **input_p, gchar char0,
                               GString *buffer, gunichar char1) {
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


/*
 * InputElementDiv ::
 *     WhiteSpace
 *     LineTerminator
 *     Comment
 *     Token
 *     DivPunctuator
 */
token_t *input_element_div(gchar **input_p) {
    // TODO
    white_space(input_p);
    line_terminator(input_p);
    comment(input_p);
    token(input_p);
    div_punctuator(input_p);
}

static gboolean usp_is_first(gchar *input) {
    return g_unichar_isspace(g_utf8_get_char(input));
}

static gboolean usp_match(gchar **input_p) {
    if (usp_is_first(*input_p)) {
        char_consume(input_p);
        return TRUE;
    } else {
        return FALSE;
    }
}

#define CHARACTER_TAB_CHAR '\u0009'
#define CHARACTER_VT_CHAR '\u000B'
#define CHARACTER_FF_CHAR '\u000C'
#define CHARACTER_SP_CHAR '\u0020'
// TODO: NO_BREAK SPACE in UTF-8 is 0xc2a0, while the C code is '\u00a0',
// according to http://www.fileformat.info/info/unicode/char/00a0/index.htm .
// Will this give the correct behavior?
#define CHARACTER_NBSP_TEXT "\u00A0"
#define CHARACTER_BOM_TEXT "\uFEFF"

gboolean white_space_is_first(gchar *input) {
    return char_is_first(input, CHARACTER_TAB_CHAR)
           || char_is_first(input, CHARACTER_VT_CHAR)
           || char_is_first(input, CHARACTER_FF_CHAR)
           || char_is_first(input, CHARACTER_SP_CHAR)
           || text_is_first(input, CHARACTER_NBSP_TEXT)
           || text_is_first(input, CHARACTER_BOM_TEXT)
           || usp_is_first(input);
}

/*
 * WhiteSpace ::
 *     <TAB>
 *     <VT>
 *     <FF>
 *     <SP>
 *     <NBSP>
 *     <BOM>
 *     <USP>
 */
token_t *white_space(gchar **input_p) {

    // <TAB>
    try_match_char_and_return_token(input_p, CHARACTER_TAB_CHAR,
                                    TOKEN_LEXICAL_WHITE_SPACE)
    // <VT>
    try_match_char_and_return_token(input_p, CHARACTER_VT_CHAR,
                                    TOKEN_LEXICAL_WHITE_SPACE)
    // <FF>
    try_match_char_and_return_token(input_p, CHARACTER_FF_CHAR,
                                    TOKEN_LEXICAL_WHITE_SPACE)
    // <SP>
    try_match_char_and_return_token(input_p, CHARACTER_SP_CHAR,
                                    TOKEN_LEXICAL_WHITE_SPACE)
    // <NBSP>
    try_match_text_and_return_token(input_p, CHARACTER_NBSP_TEXT,
                                    TOKEN_LEXICAL_WHITE_SPACE)
    // <BOM>
    try_match_text_and_return_token(input_p, CHARACTER_BOM_TEXT,
                                    TOKEN_LEXICAL_WHITE_SPACE)
    // <USP>
    try_match_and_return_token(input_p, usp_match(input_p),
                               TOKEN_LEXICAL_WHITE_SPACE)

    // TODO: Error!
    return NULL;
}

#define CHARACTER_LF_CHAR '\u000A'
#define CHARACTER_CR_CHAR '\u000D'
#define CHARACTER_LS_TEXT "\u2028"
#define CHARACTER_PS_TEXT "\u2029"
#define TEXT_CR_LF "\u000D\u000A"

gboolean line_terminator_is_first(gchar *input) {
    return char_is_first(input, CHARACTER_LF_CHAR)
           || char_is_first(input, CHARACTER_CR_CHAR)
           || text_is_first(input, CHARACTER_LS_TEXT)
           || text_is_first(input, CHARACTER_PS_TEXT);
}

/*
 * LineTerminator ::
 *     <LF>
 *     <CR>
 *     <LS>
 *     <PS>
 */
token_t *line_terminator(gchar **input_p) {

    // <LF>
    try_match_char_and_return_token(input_p, CHARACTER_LF_CHAR,
                                    TOKEN_LEXICAL_LINE_TERMINATOR)
    // <CR>
    try_match_char_and_return_token(input_p, CHARACTER_CR_CHAR,
                                    TOKEN_LEXICAL_LINE_TERMINATOR)
    // <LS>
    try_match_text_and_return_token(input_p, CHARACTER_LS_TEXT,
                                    TOKEN_LEXICAL_LINE_TERMINATOR)
    // <PS>
    try_match_text_and_return_token(input_p, CHARACTER_PS_TEXT,
                                    TOKEN_LEXICAL_LINE_TERMINATOR)

    // TODO: Error!
    return NULL;
}

static gboolean line_terminator_sequence_match(gchar **input_p) {
    return char_match(input_p, CHARACTER_LF_CHAR)
           || text_match(input_p, TEXT_CR_LF)
           || char_match(input_p, CHARACTER_CR_CHAR)
           || text_match(input_p, CHARACTER_LS_TEXT)
           || text_match(input_p, CHARACTER_PS_TEXT);
}

/*
 * LineTerminatorSequence ::
 *     <LF>
 *     <CR> [lookahead ∉ <LF> ]
 *     <LS>
 *     <PS>
 *     <CR> <LF>
 */
token_t *line_terminator_sequence(gchar **input_p) {

    // <LF>
    try_match_char_and_return_token(input_p, CHARACTER_LF_CHAR,
                                    TOKEN_LEXICAL_LINE_TERMINATOR_SEQUENCE)
    // <CR> <LF>
    // NOTE: Promoted over <CR> for the lookahead of <CR>.
    try_match_text_and_return_token(input_p, TEXT_CR_LF,
                                    TOKEN_LEXICAL_LINE_TERMINATOR_SEQUENCE)
    // <CR>
    try_match_char_and_return_token(input_p, CHARACTER_CR_CHAR,
                                    TOKEN_LEXICAL_LINE_TERMINATOR_SEQUENCE)
    // <LS>
    try_match_text_and_return_token(input_p, CHARACTER_LS_TEXT,
                                    TOKEN_LEXICAL_LINE_TERMINATOR_SEQUENCE)
    // <PS>
    try_match_text_and_return_token(input_p, CHARACTER_PS_TEXT,
                                    TOKEN_LEXICAL_LINE_TERMINATOR_SEQUENCE)

    // TODO: Error!
    return NULL;
}

gboolean comment_is_first(gchar *input) {
    return multi_line_comment_is_first(input)
           || single_line_comment_is_first(input);
}

/*
 * Comment ::
 *     MultiLineComment
 *     SingleLineComment
 */
token_t *comment(gchar **input_p) {

    // MultiLineComment
    if (multi_line_comment_is_first(*input_p)) {
        // TODO: Wrap as children!
        return multi_line_comment(input_p);
    }
    // SingleLineComment
    if (single_line_comment_is_first(*input_p)) {
        // TODO: Wrap as children!
        return single_line_comment(input_p);
    }

    // TODO: Error!
    return NULL;
}

gboolean multi_line_comment_is_first(gchar *input) {
    return text_is_first(input, "/*");
}

static gboolean post_asterisk_comment_chars_match(gchar **input_p);

static gboolean multi_line_comment_chars_match(gchar **input_p) {
    if (char_match_not(input_p, '*')) {
        // MultiLineNotAsteriskChar MultiLineCommentChars?
        multi_line_comment_chars_match(input_p);
        return TRUE;
    // LOOKAHEAD!
    } else if (!text_is_first(*input_p, "*/")) {
        // * PostAsteriskCommentChars?
        char_consume(input_p);
        post_asterisk_comment_chars_match(input_p);
        return TRUE;
    }
    return FALSE;
}

static gboolean post_asterisk_comment_chars_match(gchar **input_p) {
    if (char_match_not_2(input_p, '/', '*')) {
        // MultiLineNotForwardSlashOrAsteriskChar MultiLineCommentChars?
        multi_line_comment_chars_match(input_p);
        return TRUE;
    // LOOKAHEAD!
    } else if (!text_is_first(*input_p, "*/")) {
        // * PostAsteriskCommentChars?
        char_consume(input_p);
        post_asterisk_comment_chars_match(input_p);
        return TRUE;
    }
    return FALSE;
}

/*
 * MultiLineComment ::
 *     /* MultiLineCommentChars? */                                       /*
 * MultiLineCommentChars ::
 *     MultiLineNotAsteriskChar MultiLineCommentChars?
 *     * PostAsteriskCommentChars?
 * PostAsteriskCommentChars ::
 *     MultiLineNotForwardSlashOrAsteriskChar MultiLineCommentChars?
 *     * PostAsteriskCommentChars?
 * MultiLineNotAsteriskChar ::
 *     SourceCharacter but not *
 * MultiLineNotForwardSlashOrAsteriskChar ::
 *     SourceCharacter but not one of / or *
 */
token_t *multi_line_comment(gchar **input_p) {

    gchar *input_old = *input_p;

    // /* MultiLineCommentChars? */
    if (text_match(input_p, "/*")) {
        multi_line_comment_chars_match(input_p);
        if (text_match(input_p, "*/")) {
            return token_new_strndup_no_data(TOKEN_LEXICAL_MULTI_LINE_COMMENT,
                                             input_old, *input_p);
        }
    }

    // TODO: Error!
    return NULL;
}

gboolean single_line_comment_is_first(gchar *input) {
    return text_is_first(input, "//");
}

static gboolean single_line_comment_char_match(gchar **input_p) {
    if (!line_terminator_is_first(*input_p)) {
        char_consume(input_p);
        return TRUE;
    } else {
        return FALSE;
    }
}

static DEFINE_MATCH_ANY_FUNC(single_line_comment_char_match)

/*
 * SingleLineComment ::
 *     // SingleLineCommentChar*
 * SingleLineCommentChar ::
 *     SourceCharacter but not LineTerminator
 */
token_t *single_line_comment(gchar **input_p) {

    gchar *input_old = *input_p;

    // // SingleLineCommentChar*
    if (text_match(input_p, "//")) {
        single_line_comment_char_match_any(input_p);
        return token_new_strndup_no_data(TOKEN_LEXICAL_SINGLE_LINE_COMMENT,
                                         input_old, *input_p);
    }

    // TODO: Error!
    return NULL;
}

/*
 * Token ::
 *     IdentifierName
 *     Punctuator
 *     NumericLiteral
 *     StringLiteral
 */
token_t *token(gchar **input_p) {

    // TODO

    // TODO: Error!
    return NULL;
}

static char *KEYWORDS[] = {
        "break",
        "do",
        "instanceof",
        "typeof",
        "case",
        "else",
        "new",
        "var",
        "catch",
        "finally",
        "return",
        "void",
        "continue",
        "for",
        "switch",
        "while",
        "debugger",
        "function",
        "this",
        "with",
        "default",
        "if",
        "throw",
        "delete",
        "in",
        "try"
};

static gboolean keyword_is_match(gchar *input) {
    for (gsize i = 0; i < G_N_ELEMENTS(KEYWORDS); ++i) {
        if (text_is_match(input, KEYWORDS[i])) {
            return TRUE;
        }
    }
    return FALSE;
}

static char *FUTURE_RESERVED_WORDS[] = {
        "class",
        "enum",
        "extends",
        "super",
        "const",
        "export",
        "import",
        // Strict mode
        "implements",
        "let",
        "private",
        "public",
        "yield",
        "interface",
        "package",
        "protected",
        "static"
};

static gboolean future_reserved_word_is_match(gchar *input) {
    for (gsize i = 0; i < G_N_ELEMENTS(FUTURE_RESERVED_WORDS); ++i) {
        if (text_is_match(input, FUTURE_RESERVED_WORDS[i])) {
            return TRUE;
        }
    }
    return FALSE;
}

static gboolean reserved_word_is_match(gchar *input) {
    return keyword_is_match(input) || future_reserved_word_is_match(input)
           || null_literal_is_match(input) || boolean_literal_is_match(input);
}

/*
 * Identifier ::
 *     IdentifierName but not ReservedWord
 */
token_t *identifier(gchar **input_p) {

    token_t *identifier_name_token = identifier_name(input_p);
    if (!reserved_word_is_match(identifier_name_token->text)) {
        // TODO: Wrap child.
        return identifier_name_token;
    }

    // TODO: Error!
    return NULL;
}

static gboolean unicode_letter_is_first(gchar *input) {
    switch (g_unichar_type(g_utf8_get_char(input))) {
        case G_UNICODE_UPPERCASE_LETTER:
        case G_UNICODE_LOWERCASE_LETTER:
        case G_UNICODE_TITLECASE_LETTER:
        case G_UNICODE_MODIFIER_LETTER:
        case G_UNICODE_OTHER_LETTER:
        case G_UNICODE_LETTER_NUMBER:
            return TRUE;
        default:
            return FALSE;
    }
}

/*
 * UnicodeLetter ::
 *     any character in the Unicode categories “Uppercase letter (Lu)”,
 *             “Lowercase letter (Ll)”, “Titlecase letter (Lt)”,
 *             “Modifier letter (Lm)”, “Other letter (Lo)”, or
 *             “Letter number (Nl)”.
 */
static gboolean unicode_letter_match_save(gchar **input_p, GString *buffer) {
    if (unicode_letter_is_first(*input_p)) {
        char_consume_save(input_p, buffer);
        return TRUE;
    } else {
        return FALSE;
    }
}

static gboolean identifier_start_is_first(gchar *input) {
    return unicode_letter_is_first(input) || char_is_first(input, '$')
           || char_is_first(input, '_') || char_is_first(input, '\\');
}

/**
 * IdentifierStart ::
 *     UnicodeLetter
 *     $
 *     _
 *     \ UnicodeEscapeSequence
 */
static gboolean identifier_start_match_save_value(gchar **input_p,
                                                  GString *buffer) {
    gboolean matched = unicode_letter_match_save(input_p, buffer)
                       || char_match_save(input_p, '$', buffer)
                       || char_match_save(input_p, '_', buffer);
    if (matched) {
        return TRUE;
    } else {
        // \ UnicodeEscapeSequence
        gchar *input_old = *input_p;
        if (char_match(input_p, '\\')
            && unicode_escape_sequence_match_save_value(input_p, buffer)) {
            return TRUE;
        } else {
            // BACKTRACK!
            *input_p = input_old;
            return FALSE;
        }
    }
}

/*
 * UnicodeCombiningMark ::
 *     any character in the Unicode categories “Non-spacing mark (Mn)” or
 *     “Combining spacing mark (Mc)”
 */
static gboolean unicode_combining_mark_match_save(gchar **input_p,
                                                  GString *buffer) {
    switch (g_unichar_type(g_utf8_get_char(*input_p))) {
        case G_UNICODE_NON_SPACING_MARK:
        case G_UNICODE_SPACING_MARK:
            char_consume_save(input_p, buffer);
            return TRUE;
        default:
            return FALSE;
    }
}

/*
 * UnicodeDigit ::
 *     any character in the Unicode category “Decimal number (Nd)”
 */
static gboolean unicode_digit_match_save(gchar **input_p, GString *buffer) {
    if (g_unichar_type(g_utf8_get_char(*input_p)) == G_UNICODE_DECIMAL_NUMBER) {
        char_consume_save(input_p, buffer);
        return TRUE;
    } else {
        return FALSE;
    }
}

/*
 * UnicodeConnectorPunctuation ::
 *     any character in the Unicode category “Connector punctuation (Pc)”
 */
static gboolean unicode_connector_punctuation_match_save(gchar **input_p,
                                                         GString *buffer) {
    if (g_unichar_type(g_utf8_get_char(*input_p)) ==
            G_UNICODE_CONNECT_PUNCTUATION) {
        char_consume_save(input_p, buffer);
        return TRUE;
    } else {
        return FALSE;
    }
}

#define CHARACTER_ZWNJ_TEXT "\u200C"
#define CHARACTER_ZWJ_TEXT "\u200D"

/*
 * IdentifierPart ::
 *     IdentifierStart
 *     UnicodeCombiningMark
 *     UnicodeDigit
 *     UnicodeConnectorPunctuation
 *     <ZWNJ>
 *     <ZWJ>
 */
static gboolean identifier_part_match_save_value(gchar **input_p,
                                                 GString *buffer) {
    return identifier_start_match_save_value(input_p, buffer)
           || unicode_combining_mark_match_save(input_p, buffer)
           || unicode_digit_match_save(input_p, buffer)
           || unicode_connector_punctuation_match_save(input_p, buffer)
           || text_match_save(input_p, CHARACTER_ZWNJ_TEXT, buffer)
           || text_match_save(input_p, CHARACTER_ZWJ_TEXT, buffer);
}

static DEFINE_MATCH_ANY_SAVE_FUNC(identifier_part_match_any_save_value,
                                  identifier_part_match_save_value)

/*
 * IdentifierName ::
 *     IdentifierStart IdentifierPart*
 */
token_t *identifier_name(gchar **input_p) {

    gchar *input_old = *input_p;
    GString *string = g_string_new(NULL);
    if (identifier_start_match_save_value(input_p, string)) {
        identifier_part_match_any_save_value(input_p, string);
        return token_new_strndup_gstring(TOKEN_LEXICAL_IDENTIFIER_NAME,
                                         input_old, *input_p, string);
    }

    // TODO: Error!
    g_string_free(string, TRUE);
    return NULL;
}

static char *PUNCTUATORS[] = {
        "{",
        "}",
        "(",
        ")",
        "[",
        "]",
        ".",
        ";",
        ",",
        "<",
        ">",
        "<=",
        ">=",
        "==",
        "!=",
        "===",
        "!==",
        "",
        "+",
        "-",
        "*",
        "%",
        "++",
        "--",
        "<<",
        ">>",
        ">>>",
        "&",
        "|",
        "^",
        "!",
        "~",
        "&&",
        "||",
        "?",
        ":",
        "=",
        "+=",
        "-=",
        "*=",
        "%=",
        "<<=",
        ">>=",
        ">>>=",
        "&=",
        "|=",
        "^="
};

gboolean punctuator_is_first(gchar *input) {
    for (gsize i = 0; i < G_N_ELEMENTS(FUTURE_RESERVED_WORDS); ++i) {
        if (text_is_first(input, FUTURE_RESERVED_WORDS[i])) {
            return TRUE;
        }
    }
    return FALSE;
}

token_t *punctuator(gchar **input_p) {

    for (gsize i = 0; i < G_N_ELEMENTS(PUNCTUATORS); ++i) {
        try_match_text_and_return_token(input_p, PUNCTUATORS[i],
                                        TOKEN_LEXICAL_PUNCTUATOR)
    }

    // TODO: Error!
    return NULL;
}

static char *DIV_PUNCTUATORS[] = {
        "|=",
        "^="
};

gboolean div_punctuator_is_first(gchar *input) {
    for (gsize i = 0; i < G_N_ELEMENTS(DIV_PUNCTUATORS); ++i) {
        if (text_is_first(input, DIV_PUNCTUATORS[i])) {
            return TRUE;
        }
    }
    return FALSE;
}

token_t *div_punctuator(gchar **input_p) {

    for (gsize i = 0; i < G_N_ELEMENTS(DIV_PUNCTUATORS); ++i) {
        try_match_text_and_return_token(input_p, DIV_PUNCTUATORS[i],
                                        TOKEN_LEXICAL_DIV_PUNCTUATOR)
    }

    // TODO: Error!
    return NULL;
}

#define TEXT_NULL "null"

/*
 * NullLiteral ::
 *     null
 */
token_t *null_literal(gchar **input_p) {

    try_match_text_and_return_token(input_p, TEXT_NULL,
                                    TOKEN_LEXICAL_NULL_LITERAL)

    // TODO: Error!
    return NULL;
}

gboolean null_literal_is_match(gchar *input) {
    return text_is_match(input, TEXT_NULL);
}

gboolean *boolean_new(gboolean value) {
    gboolean *value_p = g_new(gboolean, 1);
    *value_p = value;
    return value_p;
}

#define TEXT_TRUE "true"
#define TEXT_FALSE "false"

/*
 * BooleanLiteral ::
 *     true
 *     false
 */
token_t *boolean_literal(gchar **input_p) {

    char *input_old = *(input_p);
    if (text_match(input_p, TEXT_TRUE)) {
        return token_new_strndup(TOKEN_LEXICAL_BOOLEAN_LITERAL, input_old,
                                 *(input_p), boolean_new(TRUE), NULL);
    } else if (text_match(input_p, TEXT_FALSE)) {
        return token_new_strndup(TOKEN_LEXICAL_BOOLEAN_LITERAL, input_old,
                                 *(input_p), boolean_new(FALSE), NULL);
    }

    // TODO: Error!
    return NULL;
}

gboolean boolean_literal_is_match(gchar *input) {
    return text_is_match(input, TEXT_TRUE) || text_is_match(input, TEXT_FALSE);
}

static gdouble *number_new(gdouble value) {
    gdouble *value_p = g_new(gdouble, 1);
    *value_p = value;
    return value_p;
}

static gdouble *number_new_parse(gchar *input, gchar *input_end) {
    gchar *text = g_strndup(input, input_end - input);
    gdouble *value_p = number_new(g_ascii_strtod(text, NULL));
    g_free(text);
    if (errno) {
        // TODO: Handle overflow/underflow.
        errno = 0;
    }
    return value_p;
}

static gdouble *number_new_parse_integer(gchar *input, gchar *input_end,
                                         guint base) {
    gchar *text = g_strndup(input, input_end - input);
    gdouble *value_p = number_new(g_ascii_strtoll(text, NULL, base));
    g_free(text);
    if (errno) {
        // TODO: Handle overflow/underflow.
        errno = 0;
    }
    return value_p;
}

static gboolean decimal_digit_match(gchar **input_p) {
    return chars_match(input_p, "0123456789");
}

static DEFINE_MATCH_MULTIPLE_FUNC(decimal_digit_match)

/*
 * NONSTANDARD:
 * Doesn't allow ".1", or "1.".
 * DecimalLiteral ::
 *     DecimalDigit+ (. DecimalDigit+)? (e|E (+|-)? DecimalDigit+)?
 *
 * STANDARD:
 * DecimalLiteral ::
 *     (0|NonZeroDigit DecimalDigit*) (. DecimalDigit*)? ExponentPart?
 *     . DecimalDigit+ ExponentPart?
 * DecimalDigit :: one of
 *     0 1 2 3 4 5 6 7 8 9
 * NonZeroDigit :: one of
 *     1 2 3 4 5 6 7 8 9
 * ExponentPart ::
 *     ExponentIndicator SignedInteger
 * ExponentIndicator ::
 *     e|E
 * SignedInteger ::
 *     DecimalDigits
 *     + DecimalDigits
 *     - DecimalDigits
 */
static token_t *decimal_literal(gchar **input_p) {
    do {
        gchar *input_old = *input_p;
        // DecimalDigit+
        if (decimal_digit_match_multiple(input_p)) {
            // (. DecimalDigit+)?
            if (char_match(input_p, '.')) {
                if (!decimal_digit_match_multiple(input_p)) {
                    break;
                }
            }
            // (e|E (+|-)? DecimalDigit+)?
            if (chars_match(input_p, "eE")) {
                chars_match(input_p, "+-");
                if (!decimal_digit_match_multiple(input_p)) {
                    break;
                }
            }
            return token_new_strndup(TOKEN_LEXICAL_NUMERIC_LITERAL, input_old,
                                     *input_p, number_new_parse(input_old,
                                                                *input_p),
                                     NULL);
        }
    } while (FALSE);

    // TODO: Error!
    return NULL;
}

#define TEXT_BINARY_INTEGER_PREFIX_1 "0b"
#define TEXT_BINARY_INTEGER_PREFIX_2 "0B"

static gboolean binary_integer_is_first(gchar *input) {
    return text_is_first(input, TEXT_BINARY_INTEGER_PREFIX_1)
           || text_is_first(input, TEXT_BINARY_INTEGER_PREFIX_2);
}

static gboolean binary_digit_match(gchar **input_p) {
    return chars_match(input_p, "01");
}

static DEFINE_MATCH_MULTIPLE_FUNC(binary_digit_match)

/*
 * BinaryIntegerLiteral ::
 *     0b BinaryDigit+
 *     0B BinaryDigit+
 * BinaryDigit :: one of
 *     0 1
 */
static token_t *binary_integer_literal(gchar **input_p) {

    gchar *input_old = *input_p;
    if (text_match(input_p, TEXT_BINARY_INTEGER_PREFIX_1)
        || text_match(input_p, TEXT_BINARY_INTEGER_PREFIX_2)) {
        gchar *input_digit_start = *input_p;
        if (binary_digit_match_multiple(input_p)) {
            return token_new_strndup(TOKEN_LEXICAL_NUMERIC_LITERAL, input_old,
                                     *input_p,
                                     number_new_parse_integer(input_digit_start,
                                                              *input_p, 2),
                                     NULL);
        }
    }

    // TODO: Error!
    return NULL;
}

#define TEXT_OCTAL_INTEGER_PREFIX_1 "0o"
#define TEXT_OCTAL_INTEGER_PREFIX_2 "0O"

static gboolean octal_integer_is_first(gchar *input) {
    return text_is_first(input, TEXT_OCTAL_INTEGER_PREFIX_1)
           || text_is_first(input, TEXT_OCTAL_INTEGER_PREFIX_2);
}

static gboolean octal_digit_match(gchar **input_p) {
    return chars_match(input_p, "01234567");
}

static DEFINE_MATCH_MULTIPLE_FUNC(octal_digit_match)

/*
 * OctalIntegerLiteral ::
 *     0x OctalDigit+
 *     0X OctalDigit+
 * OctalDigit :: one of
 *     0 1 2 3 4 5 6 7
 */
static token_t *octal_integer_literal(gchar **input_p) {

    gchar *input_old = *input_p;
    if (text_match(input_p, TEXT_OCTAL_INTEGER_PREFIX_1)
        || text_match(input_p, TEXT_OCTAL_INTEGER_PREFIX_2)) {
        gchar *input_digit_start = *input_p;
        if (octal_digit_match_multiple(input_p)) {
            return token_new_strndup(TOKEN_LEXICAL_NUMERIC_LITERAL, input_old,
                                     *input_p,
                                     number_new_parse_integer(input_digit_start,
                                                              *input_p, 8),
                                     NULL);
        }
    }

    // TODO: Error!
    return NULL;
}

#define TEXT_HEX_INTEGER_PREFIX_1 "0x"
#define TEXT_HEX_INTEGER_PREFIX_2 "0X"

static gboolean hex_integer_is_first(gchar *input) {
    return text_is_first(input, TEXT_HEX_INTEGER_PREFIX_1)
           || text_is_first(input, TEXT_HEX_INTEGER_PREFIX_2);
}

#define CHARACTERS_HEX_DIGITS_CHARS "0123456789abcdefABCDEF"

static gboolean hex_digit_match(gchar **input_p) {
    return chars_match(input_p, CHARACTERS_HEX_DIGITS_CHARS);
}

static gboolean hex_digit_match_save(gchar **input_p, gunichar *char_p) {
    return chars_match_save_char(input_p, CHARACTERS_HEX_DIGITS_CHARS, char_p);
}

static guint hex_digit_to_value(gunichar char0) {

    g_assert(chars_is_match(char0, CHARACTERS_HEX_DIGITS_CHARS));
    if (char0 >= '0' && char0 <= '9') {
        return (guint) char0 - '0';
    }
    if (char0 >= 'a' && char0 <= 'f') {
        return (guint) char0 - 'a' + 10;
    }
    if (char0 >= 'A' && char0 <= 'F') {
        return (guint) char0 - 'A' + 10;
    }

    // TODO: Error!
    return 16;
}

static gboolean hex_digit_match_save_value(gchar **input_p, guint *value_p) {
    gunichar char0;
    if (hex_digit_match_save(input_p, &char0)) {
        *value_p = hex_digit_to_value(char0);
        return TRUE;
    } else {
        return FALSE;
    }
}

static DEFINE_MATCH_MULTIPLE_FUNC(hex_digit_match)

/*
 * HexIntegerLiteral ::
 *     0x HexDigit+
 *     0X HexDigit+
 * HexDigit :: one of
 *     0 1 2 3 4 5 6 7 8 9 a b c d e f A B C D E F
 */
static token_t *hex_integer_literal(gchar **input_p) {

    gchar *input_old = *input_p;
    if (text_match(input_p, TEXT_HEX_INTEGER_PREFIX_1)
        || text_match(input_p, TEXT_HEX_INTEGER_PREFIX_2)) {
        gchar *input_digit_start = *input_p;
        if (hex_digit_match_multiple(input_p)) {
            return token_new_strndup(TOKEN_LEXICAL_NUMERIC_LITERAL, input_old,
                                     *input_p,
                                     number_new_parse_integer(input_digit_start,
                                                              *input_p, 16),
                                     NULL);
        }
    }

    // TODO: Error!
    return NULL;
}

/*
 * NumericLiteral ::
 *     DecimalLiteral
 *     BinaryIntegerLiteral
 *     OctalIntegerLiteral
 *     HexIntegerLiteral
 * The SourceCharacter immediately following a NumericLiteral must not be an
 * IdentifierStart or DecimalDigit.
 */
token_t *numeric_literal(gchar **input_p) {

    token_t *token;
    if (binary_integer_is_first(*input_p)) {
        token = binary_integer_literal(input_p);
    } else if (octal_integer_is_first(*input_p)) {
        token = octal_integer_literal(input_p);
    } else if (hex_integer_is_first(*input_p)) {
        token = hex_integer_literal(input_p);
    } else {
        token = decimal_literal(input_p);
    }
    if (!identifier_start_is_first(*input_p)) {
        return token;
    }

    // Error!
    return NULL;
}

gdouble *numeric_literal_get_value(token_t *token) {
    g_assert(token->id == TOKEN_LEXICAL_NUMERIC_LITERAL);
    return (gdouble *) token->data;
}

static gboolean hex_escape_sequence_match_save_value(gchar **input_p,
                                                     GString *buffer) {

    gchar *input_old = *input_p;
    guint digit[2];
    if (char_match(input_p, 'x') && hex_digit_match_save(input_p, &digit[0])
        && hex_digit_match_save_value(input_p, &digit[1])) {
        gunichar char0 = 16 * digit[0] + digit[1];
        g_string_append_unichar(buffer, char0);
        return TRUE;
    }

    // BACKTRACK!
    *input_p = input_old;
    return FALSE;
}

gboolean unicode_escape_sequence_match_save_value(gchar **input_p,
                                                  GString *buffer) {

    gchar *input_old = *input_p;
    guint digit[4];
    if (char_match(input_p, 'u') && hex_digit_match_save(input_p, &digit[0])
        && hex_digit_match_save_value(input_p, &digit[1])
        && hex_digit_match_save_value(input_p, &digit[2])
        && hex_digit_match_save_value(input_p, &digit[3])) {
        gunichar char0 = 4096 * digit[0] + 256 * digit[1] + 16 * digit[2]
                         + digit[3];
        g_string_append_unichar(buffer, char0);
        return TRUE;
    }

    // BACKTRACK!
    *input_p = input_old;
    return FALSE;
}

gboolean string_character_match_save(gchar **input_p, gchar quote,
                                     GString *buffer) {
    if (char_is_first(*input_p, quote) || line_terminator_is_first(*input_p)) {
        return FALSE;
    }
    // \ LineTerminatorSequence
    // \ EscapeSequence
    if (char_match(input_p, '\\')) {
        // LineTerminatorSequence
        if (line_terminator_sequence_match(input_p)) {
            return TRUE;
        } else {
            // SingleEscapeCharacter
            // 0
            // HexEscapeSequence
            // UnicodeEscapeSequence
            if (char_match_save(input_p, '\'', buffer)
                || char_match_save(input_p, '\"', buffer)
                || char_match_save(input_p, '\\', buffer)
                || char_match_save_other(input_p, 'b', buffer, '\u0008')
                || char_match_save_other(input_p, 'f', buffer, '\u000C')
                || char_match_save_other(input_p, 'n', buffer, '\u000A')
                || char_match_save_other(input_p, 'r', buffer, '\u000D')
                || char_match_save_other(input_p, 't', buffer, '\u0009')
                || char_match_save_other(input_p, 'v', buffer, '\u000B')
                || char_match_save_other(input_p, '0', buffer, '\0')
                || hex_escape_sequence_match_save_value(input_p, buffer)
                || unicode_escape_sequence_match_save_value(input_p, buffer)) {
                return TRUE;
            // NonEscapeCharacter
            } else {
                char_consume_save(input_p, buffer);
                return TRUE;
            }
        }
    } else {
        // SourceCharacter but not one of `quote` or \ or LineTerminator
        char_consume_save(input_p, buffer);
        return TRUE;
    }
}

/*
 * StringLiteral ::
 *     " DoubleStringCharacter+ "
 *     ' SingleStringCharacter+ '
 * DoubleStringCharacter ::
 *     SourceCharacter but not one of " or \ or LineTerminator
 *     \ EscapeSequence
 *     \ LineTerminatorSequence
 * SingleStringCharacter ::
 *     SourceCharacter but not one of ' or \ or LineTerminator
 *     \ EscapeSequence
 *     \ LineTerminatorSequence
 * EscapeSequence ::
 *     CharacterEscapeSequence
 * // NONSTANDARD
 *     0
 * // STANDARD
 *     0 [lookahead ∉ DecimalDigit]
 *     x HexDigit HexDigit
 *     u HexDigit HexDigit HexDigit HexDigit
 * CharacterEscapeSequence ::
 *     SingleEscapeCharacter
 *     NonEscapeCharacter
 * SingleEscapeCharacter :: one of
 *     ' " \ b f n r t v
 * NonEscapeCharacter ::
 *     SourceCharacter but not one of EscapeCharacter or LineTerminator
 * EscapeCharacter ::
 *     SingleEscapeCharacter
 * // NONSTANDARD
 *     0
 * // STANDARD
 *     DecimalDigit
 *     x
 *     u
 */
token_t *string_literal(gchar **input_p) {

    do {

        gchar *input_old = *input_p;

        gchar quote;
        if (char_match(input_p, '\'')) {
            quote = '\'';
        } else if (char_match(input_p, '\"')) {
            quote = '\"';
        } else {
            break;
        }

        GString *string = g_string_new(NULL);
        if (string_character_match_save(input_p, quote, string)
            && char_match(input_p, quote)) {
            return token_new_strndup_gstring(TOKEN_LEXICAL_STRING_LITERAL,
                                             input_old, *input_p, string);
        }
        g_string_free(string, TRUE);
    } while (FALSE);

    // TODO: Error!
    return NULL;
}
