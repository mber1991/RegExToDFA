#include "build.h"

#include <stdlib.h>
#include <stdio.h>

#include "token.h"
#include "dfa.h"


void build_dfa(List *list)
{
    if (list == NULL) {
        return;
    }

    size_t symbol_count;
    symbol_count = 0;
    {
        unsigned int i;
        i = 0;

        Token *token;
        token = NULL;

        while ((token = List_get_data(list, i)) != NULL) {
            if (token->type == TOKEN_LITERAL) {
                ++symbol_count;
            }

            ++i;
        }
    }

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

        while ((token = List_get_data(list, i)) != NULL) {
            if (token->type == TOKEN_LITERAL) {
                symbols[literal_index] = token->value;
                ++literal_index;
            }

            ++i;
        }
    }

    const unsigned int terminal_states[3] = {
        1,
        2,
        3,
    };

    DFA *dfa = DFA_create(10, terminal_states, 3);

    const unsigned int transitions[symbol_count];

    {
        State state;
        state.symbols = symbols;
        state.symbol_count = symbol_count;
        state.transitions = transitions;

        DFA_set_state(dfa, 0, &state);
        DFA_set_state(dfa, 1, &state);
        DFA_set_state(dfa, 2, &state);
        DFA_set_state(dfa, 3, &state);
        DFA_set_state(dfa, 4, &state);
    }

    {
        State *state;
        state = State_create(symbol_count, symbols, transitions);

        DFA_set_state(dfa, 0, state);

        State_destroy(state);
    }

    DFA_destroy(dfa);
}
