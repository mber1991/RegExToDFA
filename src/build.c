#include "build.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "token.h"
#include "dfa.h"


/*
 * Get a Multi-Dimensional List of tokens in each state
 */
static List **get_states(List *tokens,
                         List **groups,
                         size_t group_count,
                         size_t state_count)
{
    List **states;
    states = malloc(256 * sizeof(List *));
    if (states != NULL) {
        unsigned int i;
        for (i = 0; i < state_count; ++i) {
            states[i] = List_create();
        }
    }

    size_t state_index;
    state_index = 0;

    if (group_count == 0) {
        unsigned int i;
        i = 0;

        Token *token;
        token = NULL;

        while ((token = List_get_data(tokens, i)) != NULL) {
            if (token->type == TOKEN_LITERAL
                || token->type == TOKEN_ESCAPE_CHAR
            ) {
                Token *match;
                match = Token_create(token->value,
                                     token->begin, token->end,
                                     token->type);
                List_push_back(states[state_index], match);

                ++state_index;
            }

            ++i;
        }

        return states;
    }

    unsigned int i;
    i = 0;

    Token *token;
    token = NULL;

    unsigned int group_match_count = 0;

    unsigned int group_match;
    group_match = 0;
    while ((token = List_get_data(tokens, i)) != NULL) {
        if (token->type == TOKEN_LITERAL
            || token->type == TOKEN_ESCAPE_CHAR
        ) {
            unsigned int group_index;
            for (group_index = 0; group_index < group_count; ++group_index) {
                unsigned int j;
                j = 0;

                Token *temp;
                temp = NULL;

                while ((temp = List_get_data(groups[group_index], j)) != NULL) {
                    if (token->begin == temp->begin) {
                        group_match = 1;
                        ++group_match_count;
                    }

                    ++j;
                }
            }

            if (group_match) {
                group_match = 0;

                Token *match;
                match = Token_create(token->value,
                                     token->begin, token->end,
                                     token->type);
                List_push_back(states[state_index], match);
            }
            else {
                ++state_index;
                Token *match;
                match = Token_create(token->value,
                                     token->begin, token->end,
                                     token->type);
                List_push_back(states[state_index], match);

                if (group_match_count > 0) {
                    group_match_count = 0;
                    ++state_index;
                }
            }
        }

        ++i;
    }

    return states;
}

/*
 * Get the total number of required DFA states from a list of tokens
 */
static size_t get_state_count(List *tokens, List **groups, size_t group_count)
{
    size_t state_count;
    state_count = 0;

    if (group_count == 0) {
        unsigned int i;
        i = 0;

        Token *token;
        token = NULL;

        while ((token = List_get_data(tokens, i)) != NULL) {
            if ((token->type == TOKEN_LITERAL
                || token->type == TOKEN_ESCAPE_CHAR)
            ) {
                ++state_count;
            }

            ++i;
        }

        return state_count;
    }

    state_count += group_count;

    unsigned int i;
    i = 0;

    Token *token;
    token = NULL;

    while ((token = List_get_data(tokens, i)) != NULL) {
        if ((token->type == TOKEN_LITERAL
            || token->type == TOKEN_ESCAPE_CHAR)
        ) {
            unsigned int group_match;
            group_match = 0;

            unsigned int group_index;
            for (group_index = 0; group_index < group_count; ++group_index) {
                unsigned int j;
                j = 0;

                Token *temp;
                temp = NULL;

                while ((temp = List_get_data(groups[group_index], j)) != NULL) {
                    if (token->begin == temp->begin) {
                        group_match = 1;
                        break;
                    }

                    ++j;
                }

                if (group_match == 1) {
                    break;
                }
            }

            if (group_match == 0) {
                ++state_count;
            }
        }

        ++i;
    }

    return state_count;
}

/*
 * Get the total symbol count for each DFA state from a list of tokens
 */
static size_t get_symbol_count(List *tokens)
{
    size_t symbol_count;
    symbol_count = 0;

    unsigned int i;
    i = 0;

    Token *token;
    token = NULL;

    while ((token = List_get_data(tokens, i)) != NULL) {
        if (token->type == TOKEN_LITERAL
            || token->type == TOKEN_ESCAPE_CHAR
        ) {
            ++symbol_count;
        }

        ++i;
    }

    return symbol_count;
}

/*
 * Construct a DFA using data from the Parser
 */
void build_dfa(List *tokens, List **groups, size_t group_count)
{
    if (tokens == NULL) {
        return;
    }

    size_t symbol_count;
    symbol_count = get_symbol_count(tokens);

    if (symbol_count == 0) {
        return;
    }

    const char *symbols[symbol_count];
    {
        unsigned int i, literal_index;
        i = 0;
        literal_index = 0;

        Token *token;
        token = NULL;

        while ((token = List_get_data(tokens, i)) != NULL) {
            if (token->type == TOKEN_LITERAL
                || token->type == TOKEN_ESCAPE_CHAR
            ) {
                symbols[literal_index] = token->value;
                ++literal_index;
            }

            ++i;
        }
    }

    size_t state_count;
    state_count = get_state_count(tokens, groups, group_count) + 1;

    List **states;
    states = get_states(tokens, groups, group_count, state_count);

    const unsigned int terminal_states[] = {
        (state_count - 1),
    };

    size_t terminal_state_count;
    terminal_state_count = 1;

    DFA *dfa;
    dfa = DFA_create(symbols, symbol_count,
                     state_count,
                     terminal_states, terminal_state_count);

    {
        unsigned int transitions[symbol_count];

        unsigned int i;
        for (i = 0; i < symbol_count; ++i) {
            transitions[i] = 0;
        }

        {
            State *state;
            state = State_create(symbol_count, transitions);

            DFA_set_state(dfa, 0, state);

            State_destroy(state);
        }

        unsigned int state_index;
        for (state_index = 1; state_index < state_count; ++state_index) {
            unsigned int y;
            for (y = 0; y < symbol_count; ++y) {
                transitions[y] = 0;
            }

            unsigned int j;
            j = 0;

            Token *temp;
            temp = NULL;

            while ((temp = List_get_data(states[state_index-1], j)) != NULL) {
                unsigned int k;
                for (k = 0; k < symbol_count; ++k) {
                    if (strcmp(temp->value, symbols[k]) == 0
                        && transitions[k] == 0
                    ) {
                        transitions[k] = state_index;
                    }
                }

                ++j;
            }

            State *state;
            state = State_create(symbol_count, transitions);

            DFA_set_state(dfa, state_index, state);

            State_destroy(state);
        }

        DFA_to_string(dfa);
    }

    {
        unsigned int i;
        for (i = 0; i < state_count; ++i) {
            List_destroy(states[i], (Destructor) Token_destroy);
        }
        free(states);
        states = NULL;
    }

    DFA_destroy(dfa);
}
