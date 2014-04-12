#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>

#include "token.h"


typedef enum SYMBOL_TYPE {
    SYMBOL_UNKNOWN,
    SYMBOL_GROUP_BEG,
    SYMBOL_GROUP_END,
    SYMBOL_RANGE_BEG,
    SYMBOL_RANGE_END,
    SYMBOL_ANY_CHAR,
    SYMBOL_ZERO_OR_MORE,
    SYMBOL_OR,
    SYMBOL_ESCAPE,
} SYMBOL_TYPE;

typedef struct Symbol {
    const char *value;
    SYMBOL_TYPE type;
} Symbol;

typedef struct Lexer Lexer;

extern Lexer *Lexer_create(const Symbol *symbols, const size_t symbol_count);
extern void Lexer_destroy(Lexer *lexer);

extern void Lexer_match_groups(const Lexer *lexer,
                               const Token *tokens,
                               const size_t token_count);

extern void Lexer_match_ranges(const Lexer *lexer,
                               const Token *tokens,
                               const size_t token_count);

extern void Lexer_match_escapes(const Lexer *lexer,
                                const Token *tokens,
                                const size_t token_count);

#endif
