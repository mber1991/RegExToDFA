#include "parser.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "list.h"


struct Parser {
    const Symbol *symbols;
    size_t symbol_count;
};

static void get_delimited_tokens(const Parser *parser,
                                 const Token *tokens,
                                 const size_t token_count,
                                 const char *begin_delimeter,
                                 const char *end_delimeter)
{
    if (parser == NULL || tokens == NULL) {
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
    }

    if (indices != NULL) {
        free(indices);
        indices = NULL;
    }
}

Parser *Parser_create(const Symbol *symbols, const size_t symbol_count)
{
    Parser *parser = malloc(sizeof(Parser));

    if (parser != NULL) {
        parser->symbols = symbols;
        parser->symbol_count = symbol_count;
    }

    return parser;
}

void Parser_destroy(Parser *parser)
{
    if (parser != NULL) {
        free(parser);
        parser = NULL;
    }
}

void Parser_match_groups(const Parser *parser,
                         const Token *tokens,
                         const size_t token_count)
{
    printf("Groups:\n{\n");
    get_delimited_tokens(parser,
                         tokens,
                         token_count,
                         parser->symbols[SYMBOL_GROUP_BEG].value,
                         parser->symbols[SYMBOL_GROUP_END].value);
    printf("}\n\n");
}

void Parser_match_ranges(const Parser *parser,
                         const Token *tokens,
                         const size_t token_count)
{
    printf("Ranges:\n{\n");
    get_delimited_tokens(parser,
                         tokens,
                         token_count,
                         parser->symbols[SYMBOL_RANGE_BEG].value,
                         parser->symbols[SYMBOL_RANGE_END].value);
    printf("}\n\n");
}

void Parser_scan_tokens(const Parser *parser,
                        const Token *tokens,
                        const size_t token_count)
{
    if (parser == NULL || tokens == NULL) {
        return;
    }

    char escape_chars[parser->symbol_count][3];
    {
        unsigned int i;
        for (i = 0; i < parser->symbol_count; ++i) {
            if (i == 0) {
                escape_chars[i][0] = '\0';
            }
            else {
                escape_chars[i][0] = parser->symbols[SYMBOL_ESCAPE].value[0];
                escape_chars[i][1] = parser->symbols[i].value[0];
                escape_chars[i][2] = '\0';
            }
        }
    }

    unsigned int indices[token_count];
    {
        unsigned int i;
        for (i = 0; i < token_count; ++i) {
            indices[i] = 0;
        }
    }

    char symbol_buffer[token_count + 1];
    char literal_buffer[token_count + 1];
    char symbol_match[1024];
    char literal_match[1024];
    symbol_buffer[0] = '\0';
    literal_buffer[0] = '\0';
    symbol_match[0] = '\0';
    literal_match[0] = '\0';

    unsigned int symbol_match_begin, symbol_match_end;
    unsigned int literal_match_begin, literal_match_end;
    TOKEN_TYPE symbol_match_type, literal_match_type;
    symbol_match_begin = 0;
    symbol_match_end = 0;
    literal_match_begin = 0;
    literal_match_end = 0;
    symbol_match_type = TOKEN_UNKNOWN;
    literal_match_type = TOKEN_UNKNOWN;

    List *list;
    list = List_create();

    if (list == NULL) {
        return;
    }

    unsigned int i;
    for (i = 0; i < token_count; ++i) {
        unsigned int j, increment;
        increment = 0;
        for (j = i; j < token_count; ++j) {
            strcat(symbol_buffer, tokens[j].value);

            unsigned int k;

            if (strlen(symbol_buffer) == 2
                && symbol_buffer[0] == parser->symbols[SYMBOL_ESCAPE].value[0]
            ) {
                unsigned int match;
                match = 0;

                for (k = 1; k < parser->symbol_count; ++k) {
                    if (strcmp(symbol_buffer, escape_chars[k]) == 0) {
                        sprintf(symbol_match, "%s", symbol_buffer);
                        symbol_match_begin = i;
                        symbol_match_end = i + 1;

                        symbol_match_type = TOKEN_ESCAPE_CHAR;

                        indices[i] = 2;
                        if (i < token_count - 1) {
                            indices[i + 1] = 2;
                            increment = 1;
                        }

                        match = 1;
                        break;
                    }
                }

                if (match) {
                    break;
                }
            }

            for (k = 0; k < parser->symbol_count-1; ++k) {
                if (strcmp(symbol_buffer, parser->symbols[k].value) == 0) {
                    sprintf(symbol_match, "%s", symbol_buffer);
                    symbol_match_begin = i;
                    symbol_match_end = i;

                    symbol_match_type = TOKEN_SYMBOL;

                    indices[i] = 1;

                    break;
                }
            }
        }
        symbol_buffer[0] = '\0';

        if (indices[i] != 1 && indices[i] != 2) {
            strcat(literal_buffer, tokens[i].value);
        }
        else {
            if (strlen(literal_buffer) == 1) {
                sprintf(literal_match, "%s", literal_buffer);
                literal_match_begin = i - (unsigned int) strlen(literal_buffer);
                literal_match_end = literal_match_begin;
                literal_match_type = TOKEN_LITERAL;
            }
            else if (strlen(literal_buffer) > 1) {
                sprintf(literal_match, "%s", literal_buffer);
                literal_match_begin = i - (unsigned int) strlen(literal_buffer);
                literal_match_end = i - 1;
                literal_match_type = TOKEN_LITERAL;
            }

            literal_buffer[0] = '\0';
        }

        {
            if (literal_match[0] != '\0') {
                Token *token;
                token = malloc(sizeof(Token));

                if (token != NULL) {
                    token->value = malloc(sizeof(char) * (strlen(literal_match) + 1));
                    if (token->value == NULL) {
                        List_destroy(list);
                        free(token);
                        token = NULL;
                        return;
                    }

                    token->value[0] = '\0';
                    strcat(token->value, literal_match);

                    token->begin = literal_match_begin;
                    token->end = literal_match_end;

                    token->type = literal_match_type;

                    Node *node;
                    node = Node_create(token);
                    if (node != NULL) {
                        List_push_back(list, node);
                    }
                }
            }
        }

        {
            if (symbol_match[0] != '\0') {
                Token *token;
                token = malloc(sizeof(Token));

                if (token != NULL) {
                    token->value = malloc(sizeof(char) * (strlen(symbol_match) + 1));
                    if (token->value == NULL) {
                        List_destroy(list);
                        free(token);
                        token = NULL;
                        return;
                    }

                    token->value[0] = '\0';
                    strcat(token->value, symbol_match);

                    token->begin = symbol_match_begin;
                    token->end = symbol_match_end;

                    token->type = symbol_match_type;

                    Node *node;
                    node = Node_create(token);
                    if (node != NULL) {
                        List_push_back(list, node);
                    }
                }
            }
        }

        symbol_match[0] = '\0';
        literal_match[0] = '\0';

        i += increment;
    }

    List_destroy(list);
}
