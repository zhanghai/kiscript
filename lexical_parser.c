//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#include "lexical_parser.h"
#include "parser.h"

#include <string.h>

gboolean is_text(gchar *input, gchar *text) {
    return g_strcmp0(input, text) == 0;
}

gboolean char_is_first(gchar *input, gchar char0) {
    return input[0] == char0;
}

void consume_char(gchar **input_p) {
    // Always use g_utf8_next_char for safety.
    *input_p = g_utf8_next_char(*input_p);
}

gboolean match_char(gchar **input_p, gchar char0) {
    if (char_is_first(*input_p, char0)) {
        consume_char(input_p);
        return TRUE;
    } else {
        return FALSE;
    }
}

gboolean match_not_char(gchar **input_p, gchar char0) {
    if (!char_is_first(*input_p, char0)) {
        consume_char(input_p);
        return TRUE;
    } else {
        return FALSE;
    }
}

gboolean match_not_char2(gchar **input_p, gchar char0, gchar char1) {
    if (!char_is_first(*input_p, char0) && !char_is_first(*input_p, char1)) {
        consume_char(input_p);
        return TRUE;
    } else {
        return FALSE;
    }
}

gboolean text_is_first(gchar *input, gchar *text) {
    return g_str_has_prefix(input, text);
}

void consume_text(gchar **input_p, gchar *text) {
    *input_p += strlen(text);
}

gboolean match_text(gchar **input_p, gchar *text) {
    if (text_is_first(*input_p, text)) {
        consume_text(input_p, text);
        return TRUE;
    } else {
        return FALSE;
    }
}

#define try_match_and_return_token(input_p, token_id, match) \
    { \
        char *input_old = *(input_p); \
        if ((match)) { \
            return token_new_strndup_no_data((token_id), input_old, \
                                             *(input_p) - input_old); \
        } \
    }

#define try_match_char_and_return_token(input_p, token_id, token_char) \
    try_match_and_return_token((input_p), (token_id), \
                               match_char((input_p), (token_char)))

#define try_match_text_and_return_token(input_p, token_id, token_text) \
    try_match_and_return_token((input_p), (token_id), \
                               match_text((input_p), (token_text)))


/*
 * InputElementDiv ::
 *     WhiteSpace
 *     LineTerminator
 *     Comment
 *     Token
 *     DivPunctuator
 */
token_t *input_element_div(gchar **input_p) {
    white_space(input_p);
    line_terminator(input_p);
    comment(input_p);
    token(input_p);
    div_punctuator(input_p);
}

static gboolean match_usp(gchar **input_p) {
    if (g_unichar_isspace(g_utf8_get_char(*input_p))) {
        *input_p = g_utf8_next_char(*input_p);
        return TRUE;
    } else {
        return FALSE;
    }
}

// TODO
#define CHARACTER_TAB_CHAR
#define CHARACTER_VT_CHAR
#define CHARACTER_FF_CHAR
#define CHARACTER_SP_CHAR
#define CHARACTER_NBSP_CHAR
#define CHARACTER_BOM_CHAR

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
    try_match_char_and_return_token(input_p, TOKEN_LEXICAL_WHITE_SPACE, '\u0009')
    // <VT>
    try_match_char_and_return_token(input_p, TOKEN_LEXICAL_WHITE_SPACE, '\u000B')
    // <FF>
    try_match_char_and_return_token(input_p, TOKEN_LEXICAL_WHITE_SPACE, '\u000C')
    // <SP>
    try_match_char_and_return_token(input_p, TOKEN_LEXICAL_WHITE_SPACE, '\u0020')
    // <NBSP>
    try_match_char_and_return_token(input_p, TOKEN_LEXICAL_WHITE_SPACE, '\u00A0')
    // <BOM>
    try_match_text_and_return_token(input_p, TOKEN_LEXICAL_WHITE_SPACE, "\uFEFF")
    // <USP>
    try_match_and_return_token(input_p, TOKEN_LEXICAL_WHITE_SPACE, match_usp(input_p))

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
    try_match_char_and_return_token(input_p, TOKEN_LEXICAL_LINE_TERMINATOR,
                                    CHARACTER_LF_CHAR)
    // <CR>
    try_match_char_and_return_token(input_p, TOKEN_LEXICAL_LINE_TERMINATOR,
                                    CHARACTER_CR_CHAR)
    // <LS>
    try_match_text_and_return_token(input_p, TOKEN_LEXICAL_LINE_TERMINATOR,
                                    CHARACTER_LS_TEXT)
    // <PS>
    try_match_text_and_return_token(input_p, TOKEN_LEXICAL_LINE_TERMINATOR,
                                    CHARACTER_PS_TEXT)

    // TODO: Error!
    return NULL;
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
    try_match_char_and_return_token(input_p,
                                    TOKEN_LEXICAL_LINE_TERMINATOR_SEQUENCE,
                                    CHARACTER_LF_CHAR)
    // <CR> <LF>
    // NOTE: Promoted over <CR> for the lookahead of <CR>.
    try_match_text_and_return_token(input_p,
                                    TOKEN_LEXICAL_LINE_TERMINATOR_SEQUENCE,
                                    TEXT_CR_LF)
    // <CR>
    try_match_char_and_return_token(input_p,
                                    TOKEN_LEXICAL_LINE_TERMINATOR_SEQUENCE,
                                    CHARACTER_CR_CHAR)
    // <LS>
    try_match_text_and_return_token(input_p,
                                    TOKEN_LEXICAL_LINE_TERMINATOR_SEQUENCE,
                                    CHARACTER_LS_TEXT)
    // <PS>
    try_match_text_and_return_token(input_p,
                                    TOKEN_LEXICAL_LINE_TERMINATOR_SEQUENCE,
                                    CHARACTER_PS_TEXT)

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
    return g_str_has_prefix(input, "/*");
}

static gboolean match_post_asterisk_comment_chars(gchar **input_p);

static gboolean match_multi_line_comment_chars(gchar **input_p) {
    if (match_not_char(input_p, '*')) {
        // MultiLineNotAsteriskChar MultiLineCommentChars(opt)
        match_multi_line_comment_chars(input_p);
        return TRUE;
    // LOOKAHEAD!
    } else if (!text_is_first(*input_p, "*/")) {
        // * PostAsteriskCommentChars(opt)
        consume_char(input_p);
        match_post_asterisk_comment_chars(input_p);
        return TRUE;
    }
    return FALSE;
}

static gboolean match_post_asterisk_comment_chars(gchar **input_p) {
    if (match_not_char2(input_p, '/', '*')) {
        // MultiLineNotForwardSlashOrAsteriskChar MultiLineCommentChars(opt)
        match_multi_line_comment_chars(input_p);
        return TRUE;
    // LOOKAHEAD!
    } else if (!text_is_first(*input_p, "*/")) {
        // * PostAsteriskCommentChars(opt)
        consume_char(input_p);
        match_post_asterisk_comment_chars(input_p);
        return TRUE;
    }
    return FALSE;
}

/*
 * MultiLineComment ::
 *     /* MultiLineCommentChars(opt) */                                       /*
 * MultiLineCommentChars ::
 *     MultiLineNotAsteriskChar MultiLineCommentChars(opt)
 *     * PostAsteriskCommentChars(opt)
 * PostAsteriskCommentChars ::
 *     MultiLineNotForwardSlashOrAsteriskChar MultiLineCommentChars(opt)
 *     * PostAsteriskCommentChars(opt)
 * MultiLineNotAsteriskChar ::
 *     SourceCharacter but not *
 * MultiLineNotForwardSlashOrAsteriskChar ::
 *     SourceCharacter but not one of / or *
 */
token_t *multi_line_comment(gchar **input_p) {

    gchar *input_old = *input_p;

    // /* MultiLineCommentChars(opt) */
    if (match_text(input_p, "/*")) {
        match_multi_line_comment_chars(input_p);
        if (match_text(input_p, "*/")) {
            return token_new_strndup_no_data(TOKEN_LEXICAL_MULTI_LINE_COMMENT,
                                             input_old, *(input_p) - input_old);
        }
    }

    // TODO: Error!
    return NULL;
}

gboolean single_line_comment_is_first(gchar *input) {
    return g_str_has_prefix(input, "//");
}

static gboolean match_single_line_comment_chars(gchar **input_p) {
    if (!line_terminator_is_first(*input_p)) {
        consume_char(input_p);
        match_single_line_comment_chars(input_p);
        return TRUE;
    } else {
        return FALSE;
    }
}

/*
 * SingleLineComment ::
 *     // SingleLineCommentChars(opt)
 * SingleLineCommentChars ::
 *     SingleLineCommentChar SingleLineCommentChars(opt)
 * SingleLineCommentChar ::
 *     SourceCharacter but not LineTerminator
 */
token_t *single_line_comment(gchar **input_p) {

    gchar *input_old = *input_p;

    // // SingleLineCommentChars(opt)
    if (match_text(input_p, "//")) {
        match_single_line_comment_chars(input_p);
        return token_new_strndup_no_data(TOKEN_LEXICAL_SINGLE_LINE_COMMENT,
                                         input_old, *(input_p) - input_old);
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

static gboolean is_keyword(gchar *input) {
    for (gsize i = 0; i < G_N_ELEMENTS(KEYWORDS); ++i) {
        if (is_text(input, KEYWORDS[i])) {
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

static gboolean is_future_reserved_word(gchar *input) {
    for (gsize i = 0; i < G_N_ELEMENTS(FUTURE_RESERVED_WORDS); ++i) {
        if (is_text(input, FUTURE_RESERVED_WORDS[i])) {
            return TRUE;
        }
    }
    return FALSE;
}

static gboolean is_reserved_word(gchar *input) {
    return is_keyword(input) || is_future_reserved_word(input)
           || is_null_literal(input) || is_boolean_literal(input);
}

/*
 * Identifier ::
 *     IdentifierName but not ReservedWord
 */
token_t *identifier(gchar **input_p) {

    token_t *identifier_name_token = identifier_name(input_p);
    if (!is_reserved_word(identifier_name_token->text)) {
        // TODO: Wrap child.
        return identifier_name_token;
    }

    // TODO: Error!
    return NULL;
}

/*
 * UnicodeLetter ::
 *     any character in the Unicode categories “Uppercase letter (Lu)”,
 *             “Lowercase letter (Ll)”, “Titlecase letter (Lt)”,
 *             “Modifier letter (Lm)”, “Other letter (Lo)”, or
 *             “Letter number (Nl)”.
 */
static gboolean match_unicode_letter(gchar **input_p) {
    switch (g_unichar_type(g_utf8_get_char(*input_p))) {
        case G_UNICODE_UPPERCASE_LETTER:
        case G_UNICODE_LOWERCASE_LETTER:
        case G_UNICODE_TITLECASE_LETTER:
        case G_UNICODE_MODIFIER_LETTER:
        case G_UNICODE_OTHER_LETTER:
        case G_UNICODE_LETTER_NUMBER:
            consume_char(input_p);
            return TRUE;
        default:
            return FALSE;
    }
}

/**
 * IdentifierStart ::
 *     UnicodeLetter
 *     $
 *     _
 *     \ UnicodeEscapeSequence
 */
static gboolean match_identifier_start(gchar **input_p) {
    gboolean matched = match_unicode_letter(input_p) || match_char(input_p, '$')
                       || match_char(input_p, '_');
    if (matched) {
        return TRUE;
    } else {
        // \ UnicodeEscapeSequence
        gchar *input_old = *input_p;
        if (match_char(input_p, '\\')
            && match_unicode_escape_sequence(input_p)) {
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
static gboolean match_unicode_combining_mark(gchar **input_p) {
    switch (g_unichar_type(g_utf8_get_char(*input_p))) {
        case G_UNICODE_NON_SPACING_MARK:
        case G_UNICODE_SPACING_MARK:
            consume_char(input_p);
            return TRUE;
        default:
            return FALSE;
    }
}

/*
 * UnicodeDigit ::
 *     any character in the Unicode category “Decimal number (Nd)”
 */
static gboolean match_unicode_digit(gchar **input_p) {
    if (g_unichar_type(g_utf8_get_char(*input_p)) == G_UNICODE_DECIMAL_NUMBER) {
        consume_char(input_p);
        return TRUE;
    } else {
        return FALSE;
    }
}

/*
 * UnicodeConnectorPunctuation ::
 *     any character in the Unicode category “Connector punctuation (Pc)”
 */
static gboolean match_unicode_connector_punctuation(gchar **input_p) {
    if (g_unichar_type(g_utf8_get_char(*input_p)) ==
            G_UNICODE_CONNECT_PUNCTUATION) {
        consume_char(input_p);
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
static gboolean match_identifier_part(gchar **input_p) {
    return match_identifier_start(input_p)
           || match_unicode_combining_mark(input_p)
           || match_unicode_digit(input_p)
           || match_unicode_connector_punctuation(input_p)
           || match_text(input_p, CHARACTER_ZWNJ_TEXT)
           || match_text(input_p, CHARACTER_ZWJ_TEXT);
}

/*
 * IdentifierName ::
 *     IdentifierStart
 *     IdentifierName IdentifierPart
 */
token_t *identifier_name(gchar **input_p) {

    gchar *input_old = *input_p;

    // IdentifierStart IdentifierPart*
    if (match_identifier_start(input_p)) {
        while (match_identifier_part(input_p));
        return token_new_strndup_no_data(TOKEN_LEXICAL_IDENTIFIER_NAME,
                                         input_old, *(input_p) - input_old);
    }

    // TODO: Error!
    return NULL;
}

gboolean match_unicode_escape_sequence(gchar **input_p) {
    // TODO
    return FALSE;
}

#define TEXT_NULL "null"

/*
 * NullLiteral ::
 *     null
 */
gboolean null_literal(gchar **input_p) {
    // TODO
}

gboolean is_null_literal(gchar *input) {
    return is_text(input, TEXT_NULL);
}

#define TEXT_TRUE "true"
#define TEXT_FALSE "false"

/*
 * BooleanLiteral ::
 *     true
 *     false
 */
gboolean boolean_literal(gchar **input_p) {
    // TODO
}

gboolean is_boolean_literal(gchar *input) {
    return is_text(input, TEXT_TRUE) || is_text(input, TEXT_FALSE);
}
