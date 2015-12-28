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

gboolean identifier_part_is_first(gchar *input);

gboolean identifier_is_first(gchar *input);

token_t *identifier(gchar **input_p);

typedef enum {
    KEYWORD_WITH,
    KEYWORD_WHILE,
    KEYWORD_VOID,
    KEYWORD_VAR,
    KEYWORD_TYPEOF,
    KEYWORD_TRY,
    KEYWORD_THROW,
    KEYWORD_THIS,
    KEYWORD_SWITCH,
    KEYWORD_RETURN,
    KEYWORD_NEW,
    KEYWORD_INSTANCEOF,
    KEYWORD_IN,
    KEYWORD_IF,
    KEYWORD_FUNCTION,
    KEYWORD_FOR,
    KEYWORD_FINALLY,
    KEYWORD_ELSE,
    KEYWORD_DO,
    KEYWORD_DELETE,
    KEYWORD_DEFAULT,
    KEYWORD_DEBUGGER,
    KEYWORD_CONTINUE,
    KEYWORD_CATCH,
    KEYWORD_CASE,
    KEYWORD_BREAK
} keyword_id_t;

gboolean keyword_is_first(gchar *input);

token_t *keyword(gchar **input_p);

keyword_id_t *keyword_get_id(token_t *token);

gboolean keyword_is_keyword_with_id(token_t *token, keyword_id_t keyword_id);

typedef enum {
    FUTURE_RESERVED_WORD_YIELD,
    FUTURE_RESERVED_WORD_SUPER,
    FUTURE_RESERVED_WORD_STATIC,
    FUTURE_RESERVED_WORD_PUBLIC,
    FUTURE_RESERVED_WORD_PROTECTED,
    FUTURE_RESERVED_WORD_PRIVATE,
    FUTURE_RESERVED_WORD_PACKAGE,
    FUTURE_RESERVED_WORD_LET,
    FUTURE_RESERVED_WORD_INTERFACE,
    FUTURE_RESERVED_WORD_IMPORT,
    FUTURE_RESERVED_WORD_IMPLEMENTS,
    FUTURE_RESERVED_WORD_EXTENDS,
    FUTURE_RESERVED_WORD_EXPORT,
    FUTURE_RESERVED_WORD_ENUM,
    FUTURE_RESERVED_WORD_CONST,
    FUTURE_RESERVED_WORD_CLASS
} future_reserved_word_id_t;

gboolean future_reserved_word_is_first(gchar *input);

token_t *future_reserved_word(gchar **input_p);

future_reserved_word_id_t *future_reserved_word_get_id(token_t *token);

gboolean future_reserved_word_is_future_reserved_word_with_id(token_t *token,
        future_reserved_word_id_t future_reserved_word_id);

typedef enum {
    PUNCTUATOR_INCREMENT,
    PUNCTUATOR_ADD_ASSIGNMENT,
    PUNCTUATOR_PLUS,
    PUNCTUATOR_MODULO_ASSIGNMENT,
    PUNCTUATOR_PERCENT,
    PUNCTUATOR_LOGICAL_AND,
    PUNCTUATOR_AND_ASSIGNMENT,
    PUNCTUATOR_AMPERSAND,
    PUNCTUATOR_MULTIPLY_ASSIGNMENT,
    PUNCTUATOR_ASTERISK,
    PUNCTUATOR_CURLY_BRACE_RIGHT,
    PUNCTUATOR_CURLY_BRACE_LEFT,
    PUNCTUATOR_SQUARE_BRACKET_RIGHT,
    PUNCTUATOR_SQUARE_BRACKET_LEFT,
    PUNCTUATOR_PARENTHESIS_RIGHT,
    PUNCTUATOR_PARENTHESIS_LEFT,
    PUNCTUATOR_DOT,
    PUNCTUATOR_QUESTION_MARK,
    PUNCTUATOR_STRICT_DOES_NOT_EQUAL,
    PUNCTUATOR_DOES_NOT_EQUAL,
    PUNCTUATOR_EXCLAMATION,
    PUNCTUATOR_COLON,
    PUNCTUATOR_SEMICOLON,
    PUNCTUATOR_COMMA,
    PUNCTUATOR_DECREMENT,
    PUNCTUATOR_SUBTRACT_ASSIGNMENT,
    PUNCTUATOR_MINUS,
    PUNCTUATOR_LOGICAL_OR,
    PUNCTUATOR_OR_ASSIGNMENT,
    PUNCTUATOR_VERTICAL_BAR,
    PUNCTUATOR_UNSIGNED_RIGHT_SHIFT_ASSIGNMENT,
    PUNCTUATOR_UNSIGED_RIGHT_SHIFT,
    PUNCTUATOR_SIGNED_RIGHT_SHIFT_ASSIGNMENT,
    PUNCTUATOR_SIGNED_RIGHT_SHIFT,
    PUNCTUATOR_GREATER_THAN_OR_EQUAL,
    PUNCTUATOR_ANGLE_BRACKET_RIGHT,
    PUNCTUATOR_STRICT_EQUALS,
    PUNCTUATOR_EQUALS,
    PUNCTUATOR_EQUALS_SIGN,
    PUNCTUATOR_LESS_THAN_OR_EQUAL,
    PUNCTUATOR_LEFT_SHIFT_ASSIGNMENT,
    PUNCTUATOR_LEFT_SHIFT,
    PUNCTUATOR_ANGLE_BRACKET_LEFT,
    PUNCTUATOR_TILDE,
    PUNCTUATOR_XOR_ASSIGNMENT,
    PUNCTUATOR_CARET
} punctuator_id_t;

gboolean punctuator_is_first(gchar *input);

token_t *punctuator(gchar **input_p);

punctuator_id_t *punctuator_get_id(token_t *token);

gboolean punctuator_is_punctuator_with_id(token_t *token,
                                          punctuator_id_t punctuator_id);

gboolean div_punctuator_is_first(gchar *input);

token_t *div_punctuator(gchar **input_p);

gboolean null_literal_is_first(gchar *input);

token_t *null_literal(gchar **input_p);

gboolean boolean_literal_is_first(gchar *input);

token_t *boolean_literal(gchar **input_p);

gboolean numeric_literal_is_first(gchar *input);

token_t *numeric_literal(gchar **input_p);

gdouble *numeric_literal_get_value(token_t *token);

gboolean string_literal_is_first(gchar *input);

token_t *string_literal(gchar **input_p);

gboolean unicode_escape_sequence_match_save_value(gchar **input_p,
                                                  GString *buffer);

//token_t *primary_expression(gchar **input_p);
//
//token_t *expression(gchar **input_p);

#endif //KISCRIPT_LEXICAL_PARSER_H
