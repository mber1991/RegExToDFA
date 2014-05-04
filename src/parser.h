#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>

#include "list.h"
#include "token.h"


typedef enum SYMBOL_TYPE {
    SYMBOL_UNKNOWN,
    SYMBOL_GROUP_BEG,
    SYMBOL_GROUP_END,
    SYMBOL_RANGE_BEG,
    SYMBOL_RANGE_END,
    SYMBOL_RANGE_THROUGH,
    SYMBOL_ANY_CHAR,
    SYMBOL_ZERO_OR_MORE,
    SYMBOL_OR,
    SYMBOL_ESCAPE,
} SYMBOL_TYPE;

typedef struct Symbol {
    const char *value;
    SYMBOL_TYPE type;
} Symbol;

typedef struct Parser Parser;

extern Parser *Parser_create(const Symbol *symbols, const size_t symbol_count);
extern void Parser_destroy(Parser *parser);

extern void Parser_scan_tokens(Parser *parser,
                               const Token *tokens,
                               const size_t token_count);

extern List *Parser_get_token_list(Parser *parser);
extern List **Parser_get_groups(Parser *parser);
extern size_t Parser_get_group_count(Parser *parser);

#endif
