#include "state.h"

#include <stdlib.h>


State *State_create(const size_t symbol_count,
                    const char **symbols,
                    const unsigned int *transitions)
{
    State *state = malloc(sizeof(State));

    state->symbol_count = symbol_count;
    state->symbols = symbols;
    state->transitions = transitions;

    return state;
}

void State_destroy(State *state)
{
    if (state != NULL) {
        free(state);
        state = NULL;
    }
}
