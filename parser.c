#include "parser.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


struct Token
{
    char *value;
    unsigned int index;
};

struct Parser
{
    Token *tokens;
    size_t token_count;
};

static const char *symbols[] = {
    [SYMBOL_UNKNOWN]      = "",
    [SYMBOL_GROUP_BEG]    = "(",
    [SYMBOL_GROUP_END]    = ")",
    [SYMBOL_RANGE_BEG]    = "[",
    [SYMBOL_RANGE_END]    = "]",
    [SYMBOL_ANY_CHAR]     = ".",
    [SYMBOL_ZERO_OR_MORE] = "*",
    [SYMBOL_OR]           = "|",
    [SYMBOL_ESCAPE]       = "\\",
};

static const size_t symbol_count = 9;

static Token *init_tokens(Regex *regex)
{
    if (regex == NULL) {
        return NULL;
    }

    Token *tokens = malloc(sizeof(Token) * regex->length);

    if (tokens != NULL) {
        unsigned int i;
        for (i = 0; i < regex->length; ++i) {
            char *temp = malloc(sizeof(char) * 2);
            temp[0] = regex->value[i];
            temp[1] = '\0';
            tokens[i].value = temp;

            tokens[i].index = i;
        }
    }

    return tokens;
}

Parser *Parser_create(Regex *regex)
{
    if (regex == NULL) {
        return NULL;
    }

    Parser *parser = malloc(sizeof(Parser));

    if (parser != NULL) {
        parser->tokens = init_tokens(regex);
        parser->token_count = regex->length;
    }

    return parser;
}

void Parser_destroy(Parser *parser)
{
    if (parser == NULL) {
        return;
    }

    if (parser->tokens != NULL) {
        unsigned int i;
        for (i = 0; i < parser->token_count; ++i) {
            if (parser->tokens[i].value != NULL) {
                free(parser->tokens[i].value);
                parser->tokens[i].value = NULL;
            }
        }
        free(parser->tokens);
        parser->tokens = NULL;
    }

    free(parser);
    parser = NULL;
}

void Parser_match_symbols(Parser *parser)
{
    if (parser == NULL) {
        return;
    }

    printf("Tokens: \n{\n");
    unsigned int i;
    for (i = 0; i < parser->token_count; ++i) {
        printf("    %u => \"%s\"\n",
               parser->tokens[i].index,
               parser->tokens[i].value);
    }
    printf("}\n\n");

    printf("Symbols: \n{\n");
    for (i = 0; i < parser->token_count; ++i) {
        unsigned int j;
        for (j = 0; j < symbol_count; ++j) {
            if (strcmp(parser->tokens[i].value, symbols[j]) == 0) {
                printf("    %u => \"%s\"\n",
                       parser->tokens[i].index,
                       parser->tokens[i].value);
            }
        }
    }
    printf("}\n");
}

