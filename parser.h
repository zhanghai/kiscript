//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#ifndef KISCRIPT_PARSER_H
#define KISCRIPT_PARSER_H

#include "glib_utils.h"
#include "utils.h"

typedef enum {
    TOKEN_ERROR,
    TOKEN_LEXICAL_WHITE_SPACE,
    TOKEN_LEXICAL_LINE_TERMINATOR,
    TOKEN_LEXICAL_LINE_TERMINATOR_SEQUENCE,
    TOKEN_LEXICAL_MULTI_LINE_COMMENT,
    TOKEN_LEXICAL_SINGLE_LINE_COMMENT,
    TOKEN_LEXICAL_IDENTIFIER,
    TOKEN_LEXICAL_KEYWORD,
    TOKEN_LEXICAL_FUTURE_RESERVED_WORD,
    TOKEN_LEXICAL_PUNCTUATOR,
    TOKEN_LEXICAL_DIV_PUNCTUATOR,
    TOKEN_LEXICAL_NULL_LITERAL,
    TOKEN_LEXICAL_BOOLEAN_LITERAL,
    TOKEN_LEXICAL_NUMERIC_LITERAL,
    TOKEN_LEXICAL_STRING_LITERAL,
    TOKEN_STATEMENT_BLOCK,
    TOKEN_STATEMENT_VARIABLE_STATEMENT,
    TOKEN_STATEMENT_VARIABLE_DECLARATION
} token_id_t;

typedef struct {
    token_id_t id;
    gpointer data;
    clone_func_t data_clone_func;
    free_func_t data_free_func;
    GPtrArray *children;
} token_t;

token_t * token_new(token_id_t id, gpointer data, clone_func_t data_clone_func,
                    free_func_t data_free_func);

token_t *token_new_gstring(token_id_t id, GString *string);

token_t *token_new_no_data(token_id_t id);

token_t *token_clone(token_t *token);

DECLARE_FREE_FUNCS(token)

void token_add_child(token_t *token, token_t *child);


GPtrArray *token_list_new();

GPtrArray *token_list_clone(GPtrArray *token_list);

DECLARE_FREE_FUNCS_WITH_TYPE(token_list, GPtrArray);


#define DECLARE_TOKEN_IS_TOKEN_FUNC(token_name) \
    gboolean token_name##_is_##token_name(token_t *token);

#define DEFINE_TOKEN_IS_TOKEN_FUNC(token_name, token_id) \
    gboolean token_name##_is_##token_name(token_t *token) { \
        return token->id == (token_id); \
    }

#define DECLARE_TOKEN_GET_ID_FUNC(token_name) \
    token_name##_id_t *token_name##_get_id(token_t *token);

#define DEFINE_TOKEN_GET_ID_FUNC(token_name) \
    token_name##_id_t *token_name##_get_id(token_t *token) { \
            g_assert(token_name##_is_##token_name(token)); \
            return (token_name##_id_t *) token->data; \
    }

#define DECLARE_TOKEN_IS_TOKEN_WITH_ID_FUNC(token_name) \
    gboolean token_name##_is_##token_name##_with_id(token_t *token, \
            token_name##_id_t token_name##_id);

#define DEFINE_TOKEN_IS_TOKEN_WITH_ID_FUNC(token_name) \
    gboolean token_name##_is_##token_name##_with_id(token_t *token, \
            token_name##_id_t token_name##_id) { \
        return token_name##_is_##token_name(token) \
                && *token_name##_get_id(token) == token_name##_id; \
    }


typedef enum {
    ERROR_LEXICAL_LEXICAL_TOKEN,
    ERROR_LEXICAL_WHITE_SPACE,
    ERROR_LEXICAL_LINE_TERMINATOR,
    ERROR_LEXICAL_LINE_TERMINATOR_SEQUENCE,
    ERROR_LEXICAL_MULTI_LINE_COMMENT,
    ERROR_LEXICAL_SINGLE_LINE_COMMENT,
    ERROR_LEXICAL_KEYWORD,
    ERROR_LEXICAL_FUTURE_RESERVED_WORD,
    ERROR_LEXICAL_IDENTIFIER,
    ERROR_LEXICAL_PUNCTUATOR,
    ERROR_LEXICAL_DIV_PUNCTUATOR,
    ERROR_LEXICAL_NULL_LITERAL,
    ERROR_LEXICAL_BOOLEAN_LITERAL,
    ERROR_LEXICAL_NUMERIC_LITERAL_DECIMAL,
    ERROR_LEXICAL_NUMERIC_LITERAL_BINARY,
    ERROR_LEXICAL_NUMERIC_LITERAL_OCTAL,
    ERROR_LEXICAL_NUMERIC_LITERAL_HEX,
    ERROR_LEXICAL_NUMERIC_LITERAL_FOLLOWED_BY_IDENTIFIER,
    ERROR_LEXICAL_STRING_LITERAL,
    ERROR_EXPRESSION_PRIMARY_EXPRESSION_PARENTHESIS_LEFT,
    ERROR_EXPRESSION_PRIMARY_EXPRESSION,
    ERROR_STATEMENT_BLOCK_CURLY_BRACE_LEFT,
    ERROR_STATEMENT_VARIABLE_STATEMENT_VAR,
    ERROR_STATEMENT_VARIABLE_STATEMENT_SEMICOLON,
    ERROR_STATEMENT_VARIABLE_DECLARATION_IDENTIFIER,
    ERROR_STATEMENT_INITIALIZER_EQUALS_SIGN
} error_id_t;

typedef struct {
    error_id_t id;
    gboolean is_lexical;
    union {
        gchar *lexical;
        gsize syntactic;
    } position;
} error_info_t;

error_info_t *error_info_new_lexical(error_id_t id, gchar *position);

error_info_t *error_info_new_syntactic(error_id_t id, gsize position);

DECLARE_FREE_FUNCS(error_info)


token_t *error_new_lexical(error_id_t id, gchar *position);

token_t *error_new_syntactic(error_id_t id, gsize position);

DECLARE_TOKEN_IS_TOKEN_FUNC(error)

DECLARE_TOKEN_GET_ID_FUNC(error)

DECLARE_TOKEN_IS_TOKEN_WITH_ID_FUNC(error)


void normalize_input(gchar **input_p);

#endif //KISCRIPT_PARSER_H
