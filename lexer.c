#include "lexer.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


struct Lexer {
    const Symbol *symbols;
    size_t symbol_count;
};

static void get_delimited_range(const Lexer *lexer,
                                const Token *tokens,
                                const size_t token_count,
                                const char *begin_delimeter,
                                const char *end_delimeter)
{
    if (lexer == NULL || tokens == NULL) {
        return;
    }

    unsigned int *indices;
    indices = malloc(token_count * sizeof(unsigned int));

    if (indices == NULL) {
        return;
    }

    {
        unsigned int i;
        for (i = 0; i < token_count; ++i) {
            indices[i] = 0;
        }
    }

    {
        int open_count, close_count;
        open_count = 0;
        close_count = 0;

        unsigned int i;
        for (i = 0; i < token_count; ++i) {
            if (strcmp(tokens[i].value, begin_delimeter) == 0) {
                indices[i] = 1;
                ++open_count;
            }
            else if (strcmp(tokens[i].value, end_delimeter) == 0) {
                indices[i] = 2;
                ++close_count;
            }
        }

        if (open_count != close_count) {
            printf("Invalid input:\n");
            if (close_count > open_count) {
                printf("Expected open:  \"%s\"\n", begin_delimeter);

                if (indices != NULL) {
                    free(indices);
                    indices = NULL;
                }

                return;
            }
            else if (open_count > close_count) {
                printf("Expected close:  \"%s\"\n", end_delimeter);

                if (indices != NULL) {
                    free(indices);
                    indices = NULL;
                }

                return;
            }
        }
    }

    {
        unsigned int i, open_count, close_count;
        open_count = 0;
        close_count = 0;
        printf("{\n");
        for (i = 0; i < token_count; ++i) {
            if (indices[i] == 1) {
                unsigned int j;
                for (j = i; j < token_count; ++j) {
                    printf("%4s%-4u=> \"%s\"\n",
                           "",
                           j,
                           tokens[j].value);

                    if (indices[j] == 1) {
                        ++open_count;
                    }
                    else if (indices[j] == 2) {
                        ++close_count;
                    }

                    if (open_count == close_count) {
                        printf("\n");
                        break;
                    }
                }
            }
        }
        printf("}\n");
    }

    if (indices != NULL) {
        free(indices);
        indices = NULL;
    }
}

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
        printf("    %-4u => \"%s\"\n",
               tokens[i].index,
               tokens[i].value);
    }
    printf("}\n\n");

    printf("Symbols: \n{\n");
    for (i = 0; i < token_count; ++i) {
        unsigned int j;
        for (j = 0; j < lexer->symbol_count; ++j) {
            if (strcmp(tokens[i].value, lexer->symbols[j].value) == 0) {
                printf("    %-4u => \"%s\"\n",
                       tokens[i].index,
                       tokens[i].value);
            }
        }
    }
    printf("}\n");
}

void Lexer_match_groups(Lexer *lexer, Token *tokens, size_t token_count)
{

    printf("Groups:\n");
    get_delimited_range(lexer,
                        tokens,
                        token_count,
                        lexer->symbols[SYMBOL_GROUP_BEG].value,
                        lexer->symbols[SYMBOL_GROUP_END].value);
    printf("\n");
}

void Lexer_match_ranges(Lexer *lexer, Token *tokens, size_t token_count)
{
    printf("Ranges:\n");
    get_delimited_range(lexer,
                        tokens,
                        token_count,
                        lexer->symbols[SYMBOL_RANGE_BEG].value,
                        lexer->symbols[SYMBOL_RANGE_END].value);
    printf("\n");
}
