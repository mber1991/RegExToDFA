#include "token.h"

#include <stdlib.h>
#include <string.h>


Token *Token_create(const char *value,
                    const unsigned int begin, const unsigned int end,
                    const TOKEN_TYPE type)
{
    Token *token;
    token = malloc(sizeof(Token));

    if (token != NULL) {
        token->value = malloc(sizeof(char) * (strlen(value) + 1));
        if (token->value != NULL) {
            token->value[0] = '\0';
            strcat(token->value, value);
        }

        token->begin = begin;
        token->end = end;

        token->type = type;
    }

    return token;
}

void Token_destroy(Token *token)
{
    if (token != NULL) {
        if (token->value != NULL) {
            free(token->value);
            token->value = NULL;
        }
        free(token);
        token = NULL;
    }
}
