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
    TOKEN_EXPRESSION_ARRAY_LITERAL,
    TOKEN_EXPRESSION_OBJECT_LITERAL,
    TOKEN_EXPRESSION_PROPERTY_ASSIGNMENT_PLAIN,
    TOKEN_EXPRESSION_PROPERTY_ASSIGNMENT_GET,
    TOKEN_EXPRESSION_PROPERTY_ASSIGNMENT_SET,
    TOKEN_EXPRESSION_PROPERTY_ACCESSOR,
    TOKEN_EXPRESSION_CALL_EXPRESSION,
    TOKEN_EXPRESSION_ARGUMENT_LIST,
    TOKEN_EXPRESSION_NEW_EXPRESSION,
    TOKEN_EXPRESSION_POSTFIX_EXPRESSION,
    TOKEN_EXPRESSION_UNARY_EXPRESSION,
    TOKEN_EXPRESSION_MULTIPLICATIVE_EXPRESSION,
    TOKEN_EXPRESSION_ADDITIVE_EXPRESSION,
    TOKEN_EXPRESSION_SHIFT_EXPRESSION,
    TOKEN_EXPRESSION_RELATIONAL_EXPRESSION,
    TOKEN_EXPRESSION_EQUALITY_EXPRESSION,
    TOKEN_EXPRESSION_BITWISE_AND_EXPRESSION,
    TOKEN_STATEMENT_BLOCK,
    TOKEN_STATEMENT_VARIABLE_STATEMENT,
    TOKEN_STATEMENT_VARIABLE_DECLARATION,
    TOKEN_STATEMENT_EMPTY_STATEMENT,
    TOKEN_STATEMENT_IF_STATEMENT,
    TOKEN_STATEMENT_DO_WHILE_STATEMENT,
    TOKEN_STATEMENT_WHILE_STATEMENT,
    TOKEN_STATEMENT_VARIABLE_DECLARATION_LIST,
    TOKEN_STATEMENT_FOR_STATEMENT,
    TOKEN_STATEMENT_FOR_EACH_STATEMENT,
    TOKEN_STATEMENT_CONTINUE_STATEMENT,
    TOKEN_STATEMENT_BREAK_STATEMENT,
    TOKEN_STATEMENT_RETURN_STATEMENT,
    TOKEN_STATEMENT_LABELED_STATEMENT,
    TOKEN_STATEMENT_SWITCH_STATEMENT,
    TOKEN_STATEMENT_CASE_CLAUSE,
    TOKEN_STATEMENT_DEFAULT_CLAUSE,
    TOKEN_STATEMENT_THROW_STATEMENT,
    TOKEN_STATEMENT_TRY_STATEMENT,
    TOKEN_STATEMENT_DEBUGGER_STATEMENT,
    TOKEN_STATEMENT_EXPRESSION_STATEMENT,
    TOKEN_FUNCTION_FUNCTION_DECLARATION,
    TOKEN_FUNCTION_FUNCTION_EXPRESSION,
    TOKEN_FUNCTION_FORMAL_PARAMETER_LIST,
    TOKEN_FUNCTION_FUNCTION_BODY,
    TOKEN_PROGRAM_PROGRAM
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

gboolean token_has_child(token_t *token, gsize position);

token_t *token_get_child(token_t *token, gsize position);

token_t *token_get_last_child(token_t *token);


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
    ERROR_EXPRESSION_ARRAY_LITERAL_SQUARE_BRACKET_LEFT,
    ERROR_EXPRESSION_ARRAY_LITERAL_COMMA_OR_SQUARE_BRACKET_RIGHT,
    ERROR_EXPRESSION_OBJECT_LITERAL_CURLY_BRACE_LEFT,
    ERROR_EXPRESSION_OBJECT_LITERAL_COMMA_OR_CURLY_BRACE_RIGHT,
    ERROR_EXPRESSION_PROPERTY_ASSIGNMENT,
    ERROR_EXPRESSION_PROPERTY_ASSIGNMENT_PLAIN_COLON,
    ERROR_EXPRESSION_PROPERTY_ASSIGNMENT_GET_GET,
    ERROR_EXPRESSION_PROPERTY_ASSIGNMENT_GET_PARENTHESIS_LEFT,
    ERROR_EXPRESSION_PROPERTY_ASSIGNMENT_GET_PARENTHESIS_RIGHT,
    ERROR_EXPRESSION_PROPERTY_ASSIGNMENT_GET_CURLY_BRACE_LEFT,
    ERROR_EXPRESSION_PROPERTY_ASSIGNMENT_GET_CURLY_BRACE_RIGHT,
    ERROR_EXPRESSION_PROPERTY_ASSIGNMENT_SET_SET,
    ERROR_EXPRESSION_PROPERTY_ASSIGNMENT_SET_PARENTHESIS_LEFT,
    ERROR_EXPRESSION_PROPERTY_ASSIGNMENT_SET_PARENTHESIS_RIGHT,
    ERROR_EXPRESSION_PROPERTY_ASSIGNMENT_SET_IDENTIFIER,
    ERROR_EXPRESSION_PROPERTY_ASSIGNMENT_SET_CURLY_BRACE_LEFT,
    ERROR_EXPRESSION_PROPERTY_ASSIGNMENT_SET_CURLY_BRACE_RIGHT,
    ERROR_EXPRESSION_PROPERTY_NAME,
    ERROR_EXPRESSION_PROPERTY_NAME_SQUARE_BRACKET_RIGHT,
    ERROR_EXPRESSION_CALLABLE_EXPRESSION_CALLABLE_EXPRESSION,
    ERROR_EXPRESSION_PROPERTY_ACCESSOR_IDENTIFIER,
    ERROR_EXPRESSION_PROPERTY_ACCESSOR_SQUARE_BRACKET_RIGHT,
    ERROR_EXPRESSION_ARGUMENT_LIST_PARENTHESIS_LEFT,
    ERROR_EXPRESSION_ARGUMENT_LIST_COMMA_OR_PARENTHESIS_RIGHT,
    ERROR_EXPRESSION_NEW_EXPRESSION_NEW,
    ERROR_STATEMENT_BLOCK_CURLY_BRACE_LEFT,
    ERROR_STATEMENT_VARIABLE_STATEMENT_VAR,
    ERROR_STATEMENT_VARIABLE_STATEMENT_SEMICOLON,
    ERROR_STATEMENT_VARIABLE_DECLARATION_IDENTIFIER,
    ERROR_STATEMENT_INITIALIZER_EQUALS_SIGN,
    ERROR_STATEMENT_EMPTY_STATEMENT_SEMICOLON,
    ERROR_STATEMENT_IF_STATEMENT_IF,
    ERROR_STATEMENT_IF_STATEMENT_PARENTHESIS_LEFT,
    ERROR_STATEMENT_IF_STATEMENT_PARENTHESIS_RIGHT,
    ERROR_STATEMENT_DO_WHILE_STATEMENT_DO,
    ERROR_STATEMENT_DO_WHILE_STATEMENT_WHILE,
    ERROR_STATEMENT_DO_WHILE_STATEMENT_PARENTHESIS_LEFT,
    ERROR_STATEMENT_DO_WHILE_STATEMENT_PARENTHESIS_RIGHT,
    ERROR_STATEMENT_WHILE_STATEMENT_WHILE,
    ERROR_STATEMENT_WHILE_STATEMENT_PARENTHESIS_LEFT,
    ERROR_STATEMENT_WHILE_STATEMENT_PARENTHESIS_RIGHT,
    ERROR_STATEMENT_FOR_STATEMENT_FOR,
    ERROR_STATEMENT_FOR_STATEMENT_PARENTHESIS_LEFT,
    ERROR_STATEMENT_FOR_EACH_STATEMENT_PARENTHESIS_RIGHT,
    ERROR_STATEMENT_FOR_STATEMENT_FIRST_SEMICOLON,
    ERROR_STATEMENT_FOR_STATEMENT_SECOND_SEMICOLON,
    ERROR_STATEMENT_FOR_STATEMENT_PARENTHESIS_RIGHT,
    ERROR_STATEMENT_CONTINUE_STATEMENT_CONTINUE,
    ERROR_STATEMENT_CONTINUE_STATEMENT_IDENTIFIER,
    ERROR_STATEMENT_CONTINUE_STATEMENT_SEMICOLON,
    ERROR_STATEMENT_BREAK_STATEMENT_BREAK,
    ERROR_STATEMENT_BREAK_STATEMENT_IDENTIFIER,
    ERROR_STATEMENT_BREAK_STATEMENT_SEMICOLON,
    ERROR_STATEMENT_RETURN_STATEMENT_RETURN,
    ERROR_STATEMENT_RETURN_STATEMENT_IDENTIFIER,
    ERROR_STATEMENT_RETURN_STATEMENT_SEMICOLON,
    ERROR_STATEMENT_LABELED_STATEMENT_IDENTIFIER,
    ERROR_STATEMENT_LABELED_STATEMENT_COLON,
    ERROR_STATEMENT_SWITCH_STATEMENT_SWITCH,
    ERROR_STATEMENT_SWITCH_STATEMENT_PARENTHESIS_LEFT,
    ERROR_STATEMENT_SWITCH_STATEMENT_PARENTHESIS_RIGHT,
    ERROR_STATEMENT_SWITCH_STATEMENT_CURLY_BRACE_LEFT,
    ERROR_STATEMENT_SWITCH_STATEMENT_CURLY_BRACE_RIGHT,
    ERROR_STATEMENT_CASE_CLAUSE_CASE,
    ERROR_STATEMENT_CASE_CLAUSE_COLON,
    ERROR_STATEMENT_DEFAULT_CLAUSE_CASE,
    ERROR_STATEMENT_DEFAULT_CLAUSE_COLON,
    ERROR_STATEMENT_THROW_STATEMENT_THROW,
    ERROR_STATEMENT_THROW_STATEMENT_IDENTIFIER,
    ERROR_STATEMENT_THROW_STATEMENT_SEMICOLON,
    ERROR_STATEMENT_TRY_STATEMENT_TRY,
    ERROR_STATEMENT_TRY_STATEMENT_CATCH_PARENTHESIS_LEFT,
    ERROR_STATEMENT_TRY_STATEMENT_CATCH_IDENTIFIER,
    ERROR_STATEMENT_TRY_STATEMENT_CATCH_PARENTHESIS_RIGHT,
    ERROR_STATEMENT_TRY_STATEMENT_CATCH_OR_FINALLY,
    ERROR_STATEMENT_DEBUGGER_STATEMENT_DEBUGGER,
    ERROR_STATEMENT_DEBUGGER_STATEMENT_SEMICOLON,
    ERROR_STATEMENT_EXPRESSION_STATEMENT_CURLY_BRACE_LEFT_OR_FUNCTION,
    ERROR_STATEMENT_EXPRESSION_STATEMENT_SEMICOLON,
    ERROR_STATEMENT_STATEMENT_STATEMENT,
    ERROR_FUNCTION_FUNCTION_DECLARATION_FUNCTION,
    ERROR_FUNCTION_FUNCTION_DECLARATION_IDENTIFIER,
    ERROR_FUNCTION_FUNCTION_EXPRESSION_FUNCTION,
    ERROR_FUNCTION_FUNCTION_EXPRESSION_IDENTIFIER,
    ERROR_FUNCTION_FORMAL_PARAMETER_LIST_PARENTHESIS_LEFT,
    ERROR_FUNCTION_FORMAL_PARAMETER_LIST_IDENTIFIER,
    ERROR_FUNCTION_FORMAL_PARAMETER_LIST_PARENTHESIS_RIGHT,
    ERROR_FUNCTION_FUNCTION_BODY_CURLY_BRACE_LEFT,
    ERROR_FUNCTION_FUNCTION_BODY_CURLY_BRACE_RIGHT
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
