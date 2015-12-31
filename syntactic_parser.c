//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#include "syntactic_parser.h"

#include "program_parser.h"

token_t *syntactic_parse(GPtrArray *input) {
    gsize position = 0;
    return program(input, &position);
}
