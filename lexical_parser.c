//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#include "lexical_parser.h"

#include <string.h>


void lexical_token_init(lexical_token_t *token, lexical_token_id_t id,
                        gchar *text) {
    token->id = id;
    token->text = text;
}

lexical_token_t *lexical_token_new(lexical_token_id_t id, gchar *text) {
    lexical_token_t *token = g_new(lexical_token_t, 1);
    lexical_token_init(token, id, text);
    return token;
}

lexical_token_t *lexical_token_new_strndup(lexical_token_id_t id, gchar *text,
                                           gsize text_length) {
    return lexical_token_new(id, g_strndup(text, text_length));
}

void lexical_token_final(lexical_token_t *token) {
    g_free(token->text);
}

void lexical_token_free(lexical_token_t *token) {
    lexical_token_final(token);
    g_free(token);
}


#define try_match_and_return(input, token_id, token_text) \
    if (g_str_has_prefix(*(input), (token_text))) { \
        gsize token_text_length = strlen((token_text)); \
        *(input) += token_text_length; \
        return lexical_token_new_strndup((token_id), (token_text), \
                token_text_length); \
    }


void normalize_input(gchar **input_p) {

    *input_p = g_utf8_normalize(*input_p, -1, G_NORMALIZE_DEFAULT);

    // TODO: Error!
}

/**
 * InputElementDiv ::
 *     WhiteSpace
 *     LineTerminator
 *     Comment
 *     Token
 *     DivPunctuator
 */
lexical_token_t *input_element_div(gchar **input_p) {
    white_space(input);
    line_terminator(input);
    comment(input);
    token(input);
    div_punctuator(input);
}

/**
 * WhiteSpace ::
 *     <TAB>
 *     <VT>
 *     <FF>
 *     <SP>
 *     <NBSP>
 *     <BOM>
 *     <USP>
 */
lexical_token_t *white_space(gchar **input_p) {

    // <TAB>
    try_match_and_return(input_p, LEXICAL_TOKEN_WHITE_SPACE, "\u0009")
    // <VT>
    try_match_and_return(input_p, LEXICAL_TOKEN_WHITE_SPACE, "\u000B")
    // <FF>
    try_match_and_return(input_p, LEXICAL_TOKEN_WHITE_SPACE, "\u000C")
    // <SP>
    try_match_and_return(input_p, LEXICAL_TOKEN_WHITE_SPACE, "\u0020")
    // <NBSP>
    try_match_and_return(input_p, LEXICAL_TOKEN_WHITE_SPACE, "\u00A0")
    // <BOM>
    try_match_and_return(input_p, LEXICAL_TOKEN_WHITE_SPACE, "\uFEFF")
    // <USP>
    if (g_unichar_isspace(g_utf8_get_char(*input_p))) {
        gchar *input_old = *input_p;
        *input_p = g_utf8_next_char(*input_p);
        return lexical_token_new_strndup(LEXICAL_TOKEN_WHITE_SPACE, input_old,
                                         *input_p - input_old);
    }

    // TODO: Error!
    return NULL;
}

/**
 * LineTerminator ::
 *     <LF>
 *     <CR>
 *     <LS>
 *     <PS>
 */
lexical_token_t *line_terminator(gchar **input_p) {

    // <LF>
    try_match_and_return(input_p, LEXICAL_TOKEN_LINE_TERMINATOR, "\u000A")
    // <CR>
    try_match_and_return(input_p, LEXICAL_TOKEN_LINE_TERMINATOR, "\u000D")
    // <LS>
    try_match_and_return(input_p, LEXICAL_TOKEN_LINE_TERMINATOR, "\u2028")
    // <PS>
    try_match_and_return(input_p, LEXICAL_TOKEN_LINE_TERMINATOR, "\u2029")

    // TODO: Error!
    return NULL;
}

/**
 * LineTerminatorSequence ::
 *     <LF>
 *     <CR> [lookahead ∉ <LF> ]
 *     <LS>
 *     <PS>
 *     <CR> <LF>
 */
lexical_token_t *line_terminator_sequence(gchar **input_p) {

    // <LF>
    try_match_and_return(input_p, LEXICAL_TOKEN_LINE_TERMINATOR, "\u000A")
    // <CR> <LF>
    // NOTE: Promoted over <CR> for the lookahead of <CR>.
    try_match_and_return(input_p, LEXICAL_TOKEN_LINE_TERMINATOR, "\u000D\u000A")
    // <CR>
    try_match_and_return(input_p, LEXICAL_TOKEN_LINE_TERMINATOR, "\u000D")
    // <LS>
    try_match_and_return(input_p, LEXICAL_TOKEN_LINE_TERMINATOR, "\u2028")
    // <PS>
    try_match_and_return(input_p, LEXICAL_TOKEN_LINE_TERMINATOR, "\u2029")

    // TODO: Error!
    return NULL;
}
