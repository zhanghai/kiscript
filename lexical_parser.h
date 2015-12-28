//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#ifndef KISCRIPT_LEXICAL_PARSER_H
#define KISCRIPT_LEXICAL_PARSER_H

#include "parser.h"

GPtrArray *lexical_token_list(gchar **input_p);

token_t *lexical_token(gchar **input_p);

gboolean white_space_is_first(gchar *input);

token_t * white_space(gchar **input_p);

gboolean line_terminator_is_first(gchar *input);

token_t *line_terminator(gchar **input_p);

token_t *line_terminator_sequence(gchar **input_p);

gboolean multi_line_comment_is_first(gchar *input);

token_t *multi_line_comment(gchar **input_p);

gboolean single_line_comment_is_first(gchar *input);

token_t *single_line_comment(gchar **input_p);

gboolean token_is_first(gchar *input);

token_t *token(gchar **input_p);

gboolean identifier_name_is_first(gchar *input);

token_t *identifier_name(gchar **input_p);

gboolean reserved_word_is_match(gchar *input);

typedef enum {
    KEYWORD_BREAK,
    KEYWORD_DO,
    KEYWORD_INSTANCEOF,
    KEYWORD_TYPEOF,
    KEYWORD_CASE,
    KEYWORD_ELSE,
    KEYWORD_NEW,
    KEYWORD_VAR,
    KEYWORD_CATCH,
    KEYWORD_FINALLY,
    KEYWORD_RETURN,
    KEYWORD_VOID,
    KEYWORD_CONTINUE,
    KEYWORD_FOR,
    KEYWORD_SWITCH,
    KEYWORD_WHILE,
    KEYWORD_DEBUGGER,
    KEYWORD_FUNCTION,
    KEYWORD_THIS,
    KEYWORD_WITH,
    KEYWORD_DEFAULT,
    KEYWORD_IF,
    KEYWORD_THROW,
    KEYWORD_DELETE,
    KEYWORD_IN,
    KEYWORD_TRY
} keyword_id_t;

gboolean keyword_is_first(gchar *input);

token_t *keyword(gchar **input_p);

gboolean keyword_is_match(gchar *input);

typedef enum {
    FUTURE_RESERVED_WORD_CLASS,
    FUTURE_RESERVED_WORD_ENUM,
    FUTURE_RESERVED_WORD_EXTENDS,
    FUTURE_RESERVED_WORD_SUPER,
    FUTURE_RESERVED_WORD_CONST,
    FUTURE_RESERVED_WORD_EXPORT,
    FUTURE_RESERVED_WORD_IMPORT,
    // Strictmode
    FUTURE_RESERVED_WORD_IMPLEMENTS,
    FUTURE_RESERVED_WORD_LET,
    FUTURE_RESERVED_WORD_PRIVATE,
    FUTURE_RESERVED_WORD_PUBLIC,
    FUTURE_RESERVED_WORD_YIELD,
    FUTURE_RESERVED_WORD_INTERFACE,
    FUTURE_RESERVED_WORD_PACKAGE,
    FUTURE_RESERVED_WORD_PROTECTED,
    FUTURE_RESERVED_WORD_STATIC
} future_reserved_word_id_t;

gboolean future_reserved_word_is_first(gchar *input);

token_t *future_reserved_word(gchar **input_p);

gboolean future_reserved_word_is_match(gchar *input);

gboolean punctuator_is_first(gchar *input);

token_t *punctuator(gchar **input_p);

gboolean div_punctuator_is_first(gchar *input);

token_t *div_punctuator(gchar **input_p);

gboolean null_literal_is_first(gchar *input);

token_t *null_literal(gchar **input_p);

gboolean null_literal_is_match(gchar *input);

gboolean boolean_literal_is_first(gchar *input);

token_t *boolean_literal(gchar **input_p);

gboolean boolean_literal_is_match(gchar *input);

gboolean numeric_literal_is_first(gchar *input);

token_t *numeric_literal(gchar **input_p);

gdouble *numeric_literal_get_value(token_t *token);

gboolean string_literal_is_first(gchar *input);

token_t *string_literal(gchar **input_p);

gboolean unicode_escape_sequence_match_save_value(gchar **input_p,
                                                  GString *buffer);

#endif //KISCRIPT_LEXICAL_PARSER_H
