//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "lexical_parser.h"
#include "syntactic_parser.h"

int main() {

    char *input = "0x1234567890abcdefABCDEF \tabc\vfor";

    token_t *error = NULL;
    GPtrArray *token_list = lexical_parse(&input, &error);
    if (error) {
        return EXIT_FAILURE;
    }

    token_t *program_or_error = syntactic_parse(token_list);
    if (error_is_error(program_or_error)) {
        return EXIT_FAILURE;
    }

    printf("Hello, World!\n");
    return EXIT_SUCCESS;
}
