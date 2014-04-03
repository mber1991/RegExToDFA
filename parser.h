#ifndef PARSER_H
#define PARSER_H

#include "regex.h"


typedef enum SYMBOL {
    SYMBOL_UNKNOWN,
    SYMBOL_GROUP_BEG,
    SYMBOL_GROUP_END,
    SYMBOL_RANGE_BEG,
    SYMBOL_RANGE_END,
    SYMBOL_ANY_CHAR,
    SYMBOL_ZERO_OR_MORE,
    SYMBOL_OR,
    SYMBOL_ESCAPE,
} SYMBOL;

typedef struct Token Token;
typedef struct Parser Parser;

extern Parser *Parser_create(Regex *regex);
extern void Parser_destroy(Parser *parser);

extern void Parser_match_symbols(Parser *parser);

#endif
