#include "lexer.h"

#include <stdlib.h>
#include <stdio.h>


struct Lexer {
    Token *tokens;
    size_t token_count;
};

static Token *init_tokens(Regex *regex)
{
    if (regex == NULL) {
        return NULL;
    }

    Token *tokens = malloc(sizeof(Token) * Regex_get_length(regex));

    if (tokens != NULL) {
        unsigned int i;
        for (i = 0; i < Regex_get_length(regex); ++i) {
            char *temp = malloc(sizeof(char) * 2);
            temp[0] = Regex_get_value(regex)[i];
            temp[1] = '\0';
            tokens[i].value = temp;

            tokens[i].index = i;
        }
    }

    return tokens;
}

Lexer *Lexer_create(Regex *regex)
{
    if (regex == NULL) {
        return NULL;
    }

    Lexer *lexer = malloc(sizeof(Lexer));

    if (lexer != NULL) {
        lexer->tokens = init_tokens(regex);
        lexer->token_count = Regex_get_length(regex);
    }

    return lexer;
}

void Lexer_destroy(Lexer *lexer)
{
    if (lexer == NULL) {
        return;
    }

    if (lexer->tokens != NULL) {
        unsigned int i;
        for (i = 0; i < lexer->token_count; ++i) {
            if (lexer->tokens[i].value != NULL) {
                free(lexer->tokens[i].value);
                lexer->tokens[i].value = NULL;
            }
        }
        free(lexer->tokens);
        lexer->tokens = NULL;
    }

    free(lexer);
    lexer = NULL;
}

Token *Lexer_get_tokens(Lexer *lexer)
{
    if (lexer == NULL) {
        return NULL;
    }

    return lexer->tokens;
}

size_t Lexer_get_token_count(Lexer *lexer)
{
    if (lexer == NULL) {
        return 0;
    }

    return lexer->token_count;
}
