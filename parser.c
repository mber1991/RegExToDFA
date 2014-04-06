#include "parser.h"

#include <stdlib.h>
#include <stdio.h>


struct Parser {
    Token *tokens;
    size_t token_count;
};

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

Token *Parser_get_tokens(Parser *parser)
{
    if (parser == NULL) {
        return NULL;
    }

    return parser->tokens;
}

size_t Parser_get_token_count(Parser *parser)
{
    if (parser == NULL) {
        return 0;
    }

    return parser->token_count;
}
