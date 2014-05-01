#include "lexer.h"

#include <stdlib.h>


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

            tokens[i].begin = i;
            tokens[i].end = i;

            tokens[i].type = TOKEN_UNKNOWN;
        }
    }

    return tokens;
}

static void clear_data(Lexer *lexer)
{
    if (lexer == NULL || lexer->tokens == NULL) {
        return;
    }

    unsigned int i;
    for (i = 0; i < lexer->token_count; ++i) {
        if (lexer->tokens[i].value != NULL) {
            free(lexer->tokens[i].value);
            lexer->tokens[i].value = NULL;
        }
    }
    free(lexer->tokens);
    lexer->tokens = NULL;

    lexer->token_count = 0;
}

Lexer *Lexer_create(void)
{
    Lexer *lexer = malloc(sizeof(Lexer));

    if (lexer != NULL) {
        lexer->tokens = NULL;
        lexer->token_count = 0;
    }

    return lexer;
}

void Lexer_destroy(Lexer *lexer)
{
    if (lexer == NULL) {
        return;
    }

    clear_data(lexer);

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

void Lexer_scan_regex(Lexer *lexer, Regex *regex)
{
    if (lexer == NULL || regex == NULL) {
        return;
    }

    clear_data(lexer);

    lexer->tokens = init_tokens(regex);
    lexer->token_count = Regex_get_length(regex);
}
