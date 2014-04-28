#include "parser.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "list.h"
#include "token.h"


struct Parser {
    List *token_list;
    List **groups, **ranges;
    List **zero_or_more_groups, **zero_or_more_ranges;
    size_t group_count, range_count;
    size_t zero_or_more_group_count, zero_or_more_range_count;
    const Symbol *symbols;
    size_t symbol_count;
};

static size_t get_delimeted_group_count(const Parser *parser,
                                        const Symbol beg,
                                        const Symbol end)
{
    if (parser == NULL) {
        return 0;
    }

    unsigned int i, open_count, close_count;
    i = 0;
    open_count = 0;
    close_count = 0;

    size_t group_count;
    group_count = 0;

    Token *token;
    token = NULL;

    while ((token = List_get_data(parser->token_list, i)) != NULL) {
        if (strcmp(token->value, beg.value) == 0) {
            unsigned int j;
            j = i;
            Token *temp;
            temp = NULL;
            while ((temp = List_get_data(parser->token_list, j)) != NULL) {
                if (strcmp(temp->value, beg.value) == 0) {
                    ++open_count;
                }
                if (strcmp(temp->value, end.value) == 0) {
                    ++close_count;
                }
                if (open_count == close_count) {
                    ++group_count;
                    break;
                }

                ++j;
            }
        }

        ++i;
    }

    return group_count;
}

static void get_delimited_tokens(const Parser *parser,
                                 const Symbol beg,
                                 const Symbol end,
                                 List **out)
{
    if (parser == NULL || out == NULL) {
        return;
    }

    Token *token;
    token = NULL;

    unsigned int i, open_count, close_count, group_index;
    i = 0;
    open_count = 0;
    close_count = 0;
    group_index = 0;

    while ((token = List_get_data(parser->token_list, i)) != NULL) {
        if (strcmp(token->value, beg.value) == 0) {
            unsigned int j;
            j = i;
            Token *temp;
            temp = NULL;
            while ((temp = List_get_data(parser->token_list, j)) != NULL) {
                Token *match;
                match = Token_create(temp->value,
                                     temp->begin, temp->end,
                                     temp->type);

                List_push_back(out[group_index], match);

                // printf("%4s%-4u=> \"%s\"\n",
                //        "",
                //        j,
                //        match->value);

                if (strcmp(temp->value, beg.value) == 0) {
                    ++open_count;
                }
                if (strcmp(temp->value, end.value) == 0) {
                    ++close_count;
                }

                if (open_count == close_count) {
                    printf("\n");

                    ++group_index;
                    break;
                }

                ++j;
            }
        }

        ++i;
    }
}

static void get_modified_tokens(const Parser *parser,
                                const Symbol beg, const Symbol end,
                                const Symbol modifier,
                                List ***out, size_t *out_size)
{
    if (parser == NULL || out == NULL || out_size == NULL) {
        return;
    }

    get_delimited_tokens(parser,
                         beg,
                         modifier,
                         (*out));

    unsigned int indices[(*out_size)];
    {
        unsigned int i;
        for (i = 0; i < (*out_size); ++i) {
            indices[i] = 0;
        }
    }

    unsigned int match_count;
    match_count = 0;

    unsigned int k;
    for (k = 0; k < (*out_size); ++k) {
        size_t size = List_get_size((*out)[k]);
        if (size > 0) {
            Token *token;
            token = List_get_data((*out)[k], size - 1);
            if (strcmp(token->value, end.value) == 0) {
                indices[k] = 1;
                ++match_count;
            }
        }
    }

    List **temp_list;
    temp_list = malloc(match_count * sizeof(List *));
    if (temp_list != NULL) {
        unsigned int i;
        for (i = 0; i < match_count; ++i) {
            temp_list[i] = List_create();
        }
    }

    unsigned int group_index;
    group_index = 0;

    for (k = 0; k < (*out_size); ++k) {
        if (indices[k] == 1) {
            Token *token;
            token = NULL;

            unsigned int i;
            i = 0;

            while ((token = List_get_data((*out)[k], i)) != NULL) {
                Token *temp;
                temp = Token_create(token->value,
                                    token->begin, token->end,
                                    token->type);
                if (temp != NULL) {
                    List_push_back(temp_list[group_index], temp);
                }

                ++i;
            }

            ++group_index;
        }
    }

    if ((*out) != NULL) {
        unsigned int i;
        for (i = 0; i < (*out_size); ++i) {
            List_destroy((*out)[i],
                         (Destructor) Token_destroy);
        }
        free((*out));
        (*out) = NULL;
    }

    (*out_size) = match_count;
    (*out) = temp_list;
}

static void init_groups(Parser *parser)
{
    if (parser == NULL) {
        return;
    }

    if (parser->groups == NULL) {
        parser->group_count = get_delimeted_group_count(
            parser,
            parser->symbols[SYMBOL_GROUP_BEG],
            parser->symbols[SYMBOL_GROUP_END]);

        parser->groups = malloc(parser->group_count * sizeof(List *));
        if (parser->groups != NULL) {
            unsigned int i;
            for (i = 0; i < parser->group_count; ++i) {
                parser->groups[i] = List_create();
            }
        }

        get_delimited_tokens(parser,
                             parser->symbols[SYMBOL_GROUP_BEG],
                             parser->symbols[SYMBOL_GROUP_END],
                             parser->groups);
    }
}

static void init_zero_or_more_groups(Parser *parser)
{
    if (parser == NULL) {
        return;
    }

    if (parser->zero_or_more_groups == NULL) {
        parser->zero_or_more_group_count = get_delimeted_group_count(
            parser,
            parser->symbols[SYMBOL_GROUP_BEG],
            parser->symbols[SYMBOL_ZERO_OR_MORE]);

        parser->zero_or_more_groups = malloc(parser->zero_or_more_group_count * sizeof(List *));
        if (parser->zero_or_more_groups != NULL) {
            unsigned int i;
            for (i = 0; i < parser->zero_or_more_group_count; ++i) {
                parser->zero_or_more_groups[i] = List_create();
            }
        }

        get_modified_tokens(parser,
                            parser->symbols[SYMBOL_GROUP_BEG],
                            parser->symbols[SYMBOL_GROUP_END],
                            parser->symbols[SYMBOL_ZERO_OR_MORE],
                            &parser->zero_or_more_groups,
                            &parser->zero_or_more_group_count);

        // unsigned int indices[parser->zero_or_more_group_count];
        // {
        //     unsigned int i;
        //     for (i = 0; i < parser->zero_or_more_group_count; ++i) {
        //         indices[i] = 0;
        //     }
        // }

        // unsigned int match_count;
        // match_count = 0;

        // unsigned int k;
        // for (k = 0; k < parser->zero_or_more_group_count; ++k) {
        //     size_t size = List_get_size(parser->zero_or_more_groups[k]);
        //     if (size > 0) {
        //         Token *token;
        //         token = List_get_data(parser->zero_or_more_groups[k], size - 1);
        //         if (strcmp(token->value, parser->symbols[SYMBOL_GROUP_END].value) == 0) {
        //             indices[k] = 1;
        //             ++match_count;
        //         }
        //     }
        // }

        // List **temp_list;
        // temp_list = malloc(match_count * sizeof(List *));
        // if (temp_list != NULL) {
        //     unsigned int i;
        //     for (i = 0; i < match_count; ++i) {
        //         temp_list[i] = List_create();
        //     }
        // }

        // unsigned int group_index;
        // group_index = 0;

        // for (k = 0; k < parser->zero_or_more_group_count; ++k) {
        //     if (indices[k] == 1) {
        //         Token *token;
        //         token = NULL;

        //         unsigned int i;
        //         i = 0;

        //         while ((token = List_get_data(parser->zero_or_more_groups[k], i)) != NULL) {
        //             Token *temp;
        //             temp = Token_create(token->value,
        //                                 token->begin, token->end,
        //                                 token->type);
        //             if (temp != NULL) {
        //                 List_push_back(temp_list[group_index], temp);
        //             }

        //             ++i;
        //         }

        //         ++group_index;
        //     }
        // }

        // if (parser->zero_or_more_groups != NULL) {
        //     unsigned int i;
        //     for (i = 0; i < parser->zero_or_more_group_count; ++i) {
        //         List_destroy(parser->zero_or_more_groups[i],
        //                      (Destructor) Token_destroy);
        //     }
        //     free(parser->zero_or_more_groups);
        //     parser->zero_or_more_groups = NULL;
        // }

        // parser->zero_or_more_group_count = match_count;
        // parser->zero_or_more_groups = temp_list;
    }
}

static void init_ranges(Parser *parser)
{
    if (parser == NULL) {
        return;
    }

    if (parser->ranges == NULL) {
        parser->range_count = get_delimeted_group_count(
            parser,
            parser->symbols[SYMBOL_RANGE_BEG],
            parser->symbols[SYMBOL_RANGE_END]);

        parser->ranges = malloc(parser->range_count * sizeof(List *));
        if (parser->ranges != NULL) {
            unsigned int i;
            for (i = 0; i < parser->range_count; ++i) {
                parser->ranges[i] = List_create();
            }
        }

        get_delimited_tokens(parser,
                             parser->symbols[SYMBOL_RANGE_BEG],
                             parser->symbols[SYMBOL_RANGE_END],
                             parser->ranges);

    }
}

static void init_tokens(Parser *parser,
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

            for (k = 1; k < parser->symbol_count; ++k) {
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
                token = Token_create(literal_match,
                                     literal_match_begin, literal_match_end,
                                     literal_match_type);

                List_push_back(parser->token_list, token);
            }
        }

        {
            if (symbol_match[0] != '\0') {
                Token *token;
                token = Token_create(symbol_match,
                                     symbol_match_begin, symbol_match_end,
                                     symbol_match_type);

                List_push_back(parser->token_list, token);
            }
        }

        symbol_match[0] = '\0';
        literal_match[0] = '\0';

        i += increment;
    }
}

Parser *Parser_create(const Symbol *symbols, const size_t symbol_count)
{
    Parser *parser = malloc(sizeof(Parser));

    if (parser != NULL) {
        parser->symbols = symbols;
        parser->symbol_count = symbol_count;
        parser->token_list = List_create();

        parser->groups = NULL;
        parser->ranges = NULL;
        parser->zero_or_more_groups = NULL;
        parser->zero_or_more_ranges = NULL;

        parser->group_count = 0;
        parser->range_count = 0;
        parser->zero_or_more_group_count = 0;
        parser->zero_or_more_range_count = 0;
    }

    return parser;
}

void Parser_destroy(Parser *parser)
{
    if (parser != NULL) {
        List_destroy(parser->token_list, (Destructor) Token_destroy);

        if (parser->groups != NULL) {
            unsigned int i;
            for (i = 0; i < parser->group_count; ++i) {
                List_destroy(parser->groups[i], (Destructor) Token_destroy);
            }
            free(parser->groups);
            parser->groups = NULL;
        }

        if (parser->zero_or_more_groups != NULL) {
            unsigned int i;
            for (i = 0; i < parser->zero_or_more_group_count; ++i) {
                List_destroy(parser->zero_or_more_groups[i], (Destructor) Token_destroy);
            }
            free(parser->zero_or_more_groups);
            parser->zero_or_more_groups = NULL;
        }

        if (parser->ranges != NULL) {
            unsigned int i;
            for (i = 0; i < parser->range_count; ++i) {
                List_destroy(parser->ranges[i], (Destructor) Token_destroy);
            }
            free(parser->ranges);
            parser->ranges = NULL;
        }

        free(parser);
        parser = NULL;
    }
}

void Parser_scan_tokens(Parser *parser,
                        const Token *tokens,
                        const size_t token_count)
{
    init_tokens(parser, tokens, token_count);

    init_groups(parser);
    init_zero_or_more_groups(parser);

    unsigned int k;
    for (k = 0; k < parser->zero_or_more_group_count; ++k) {
        List_to_string(parser->zero_or_more_groups[k]);
    }

    init_ranges(parser);
}
