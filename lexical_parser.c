//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#include "lexical_parser.h"


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


void source_character(gchar *input) {
    if (g_utf8_validate(input)) {
        //match();
    } else {
        //error();
    }
}

/**
 * InputElementDiv ::
 *     WhiteSpace
 *     LineTerminator
 *     Comment
 *     Token
 *     DivPunctuator
 */
lexical_token_t *input_element_div(gchar *input) {
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
lexical_token_t *white_space(gchar *input) {
    if (g_str_has_prefix(input, "\u000A")) { // <TAB>
        return lexical_token_new_strndup(LEXICAL_TOKEN_WHITE_SPACE, input, );
    }
    gunichar char0 = g_utf8_get_char(input);
    switch (char0) {
        case :
        case '\u000B': // <VT>
        case '\u000D': // <FF>
        case '\u2028': // <SP>
        case '\u00A0': // <NBSP>
        case '\uFEFF': // <BOM>
            //match();
            break;
        default:
            if (g_unichar_isspace(char0)) { // <USP>
                //match();
            } else {
                //error();
            }
    }
}

/**
 * LineTerminator ::
 *     <LF>
 *     <CR>
 *     <LS>
 *     <PS>
 */
void line_terminator(gchar *input) {
    switch (input[0]) {
        case '\u000A': // <LF>
        case '\u000D': // <CR>
        case '\u2028': // <LS>
        case '\u2029': // <PS>
            //match()
            break;
        default:
            //error()
    }
}

/**
 * LineTerminatorSequence ::
 *     <LF>
 *     <CR> [lookahead ∉ <LF> ]
 *     <LS>
 *     <PS>
 *     <CR> <LF>
 */
void line_terminator_sequence(gchar *input) {
    switch (input[0]) {
        case '\u000A': // <LF>
        case '\u000D': // <CR>
        case '\u2028': // <LS>
        case '\u2029': // <PS>
            //match()
            break;
        default:
            //error()
    }
}
