//
// Copyright (c) 2015 Zhang Hai <Dreaming.in.Code.ZH@Gmail.com>
// All Rights Reserved.
//

#ifndef KISCRIPT_STATEMENT_PARSER_H
#define KISCRIPT_STATEMENT_PARSER_H

#include "parser.h"

token_t *statement(GPtrArray *input, gsize *position_p);

#endif //KISCRIPT_STATEMENT_PARSER_H
