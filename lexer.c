#include "lexer.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


struct Lexer {
    const Symbol *symbols;
    size_t symbol_count;
};

Lexer *Lexer_create(const Symbol *symbols, size_t symbol_count)
{
    Lexer *lexer = malloc(sizeof(Lexer));

    if (lexer != NULL) {
        lexer->symbols = symbols;
        lexer->symbol_count = symbol_count;
    }

    return lexer;
}

void Lexer_destroy(Lexer *lexer)
{
    if (lexer != NULL) {
        free(lexer);
        lexer = NULL;
    }
}

void Lexer_match_symbols(Lexer *lexer, Token *tokens, size_t token_count)
{
    if (lexer == NULL || tokens == NULL) {
        return;
    }

    printf("Tokens: \n{\n");
    unsigned int i;
    for (i = 0; i < token_count; ++i) {
        printf("    %u => \"%s\"\n",
               tokens[i].index,
               tokens[i].value);
    }
    printf("}\n\n");

    printf("Symbols: \n{\n");
    for (i = 0; i < token_count; ++i) {
        unsigned int j;
        for (j = 0; j < lexer->symbol_count; ++j) {
            if (strcmp(tokens[i].value, lexer->symbols[j].value) == 0) {
                printf("    %u => \"%s\"\n",
                       tokens[i].index,
                       tokens[i].value);
            }
        }
    }
    printf("}\n");
}
