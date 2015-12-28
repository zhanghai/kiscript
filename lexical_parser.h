//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#ifndef KISCRIPT_LEXICAL_PARSER_H
#define KISCRIPT_LEXICAL_PARSER_H

#include "parser.h"

token_t * input_element_div(gchar **input_p);

gboolean white_space_is_first(gchar *input);

token_t * white_space(gchar **input_p);

gboolean line_terminator_is_first(gchar *input);

token_t *line_terminator(gchar **input_p);

token_t *line_terminator_sequence(gchar **input_p);

gboolean comment_is_first(gchar *input);

token_t *comment(gchar **input_p);

gboolean multi_line_comment_is_first(gchar *input);

token_t *multi_line_comment(gchar **input_p);

gboolean single_line_comment_is_first(gchar *input);

token_t *single_line_comment(gchar **input_p);

gboolean token_is_first(gchar *input);

token_t *token(gchar **input_p);

token_t *identifier(gchar **input_p);

gboolean identifier_name_is_first(gchar *input);

token_t *identifier_name(gchar **input_p);

gboolean punctuator_is_first(gchar *input);

token_t *punctuator(gchar **input_p);

gboolean div_punctuator_is_first(gchar *input);

token_t *div_punctuator(gchar **input_p);

token_t *null_literal(gchar **input_p);

gboolean null_literal_is_match(gchar *input);

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
