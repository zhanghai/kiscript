//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "lexical_parser.h"
#include "syntactic_parser.h"

int main() {

    char *input = NULL;
    size_t input_length = 0;
    getline(&input, &input_length, stdin);
    if (errno) {
        free(input);
        return EXIT_FAILURE;
    } else {
        char *input_malloc = input;
        input = g_strdup(input);
        free(input_malloc);
    }

    if (!lexical_parse_normalize_input(&input)) {
        return EXIT_FAILURE;
    }

    token_t *error = NULL;
    GPtrArray *token_list = lexical_parse(input, &error);
    if (error) {
        token_free(&error);
        return EXIT_FAILURE;
    }

    token_t *program_or_error = syntactic_parse(token_list);
    if (error_is_error(program_or_error)) {
        token_free((&program_or_error));
        return EXIT_FAILURE;
    }

    printf("Hello, KiScript!\n");
    return EXIT_SUCCESS;
}
