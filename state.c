#include "state.h"

#include <stdlib.h>


State *State_create(const size_t symbol_count,
                    const unsigned int *symbols,
                    const unsigned int *transitions)
{
    State *state = malloc(sizeof(State));

    state->symbol_count = symbol_count;

    state->symbols = malloc(symbol_count * sizeof(unsigned int));
    state->transitions = malloc(symbol_count * sizeof(unsigned int));
    unsigned int i;
    for (i = 0; i < symbol_count; ++i) {
        state->symbols[i] = symbols[i];
        state->transitions[i] = transitions[i];
    }

    return state;
}

void State_destroy(State *state)
{
    if (state != NULL) {
        if (state->symbols != NULL) {
            free(state->symbols);
            state->symbols = NULL;
        }

        if (state->transitions != NULL) {
            free(state->transitions);
            state->transitions = NULL;
        }

        free(state);
        state = NULL;
    }
}
