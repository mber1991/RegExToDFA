#include "dfa.h"

#include <stdlib.h>


struct DFA {
    size_t state_count, terminal_state_count;
    State *states;
    const unsigned int *terminal_states;
};

DFA *DFA_create(const size_t state_count,
                const unsigned int *terminal_states,
                const size_t terminal_state_count)
{
    DFA *dfa = malloc(sizeof(DFA));

    dfa->state_count = state_count;
    dfa->states = malloc(state_count * sizeof(State));

    dfa->terminal_states = terminal_states;
    dfa->terminal_state_count = terminal_state_count;

    return dfa;
}

void DFA_destroy(DFA *dfa)
{
    if (dfa != NULL) {
        if (dfa->states != NULL) {
            free(dfa->states);
            dfa->states = NULL;
        }

        free(dfa);
        dfa = NULL;
    }
}

void DFA_set_state(DFA *dfa,
                    const unsigned int state_index,
                    const State *state)
{
    if (state_index >= dfa->state_count
        || state == NULL
        || state->symbols == NULL
        || state->transitions == NULL
    ) {
        return;
    }

    dfa->states[state_index].symbol_count = state->symbol_count;
    dfa->states[state_index].symbols = state->symbols;
    dfa->states[state_index].transitions = state->transitions;
}
