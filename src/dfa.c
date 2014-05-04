#include "dfa.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


struct DFA {
    size_t symbol_count, state_count, terminal_state_count;
    const char **symbols;
    State *states;
    const unsigned int *terminal_states;
};

State *State_create(const size_t transition_count,
                    const unsigned int *transitions)
{
    State *state = malloc(sizeof(State));

    if (state != NULL) {
        state->transition_count = transition_count;
        state->transitions = malloc(transition_count * sizeof(unsigned int));
        unsigned int i;
        for (i = 0; i < transition_count; ++i) {
            state->transitions[i] = transitions[i];
        }
    }

    return state;
}

void State_destroy(State *state)
{
    if (state != NULL) {
        if (state->transitions != NULL) {
            free(state->transitions);
            state->transitions = NULL;
        }
        free(state);
        state = NULL;
    }
}

DFA *DFA_create(const char **symbols,
                const size_t symbol_count,
                const size_t state_count,
                const unsigned int *terminal_states,
                const size_t terminal_state_count)
{
    DFA *dfa = malloc(sizeof(DFA));

    if (dfa != NULL) {
        dfa->symbols = symbols;
        dfa->symbol_count = symbol_count;

        dfa->state_count = state_count;
        dfa->states = malloc(state_count * sizeof(State));
        unsigned int i;
        for (i = 0; i < state_count; ++i) {
            State state;
            state.transition_count = 0;
            state.transitions = NULL;
            dfa->states[i] = state;
        }

        dfa->terminal_states = terminal_states;
        dfa->terminal_state_count = terminal_state_count;
    }

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
    if (dfa == NULL
        || state_index >= dfa->state_count
        || state == NULL
        || state->transitions == NULL
    ) {
        return;
    }

    dfa->states[state_index].transition_count = state->transition_count;
    dfa->states[state_index].transitions = state->transitions;
}

void DFA_to_string(DFA *dfa)
{
    char temp[128];
    temp[0] = '\0';
    sprintf(temp, "%-2u", dfa->state_count);
    size_t row_length, column_offset;
    row_length = 0;
    column_offset = strlen(temp) + 1;

    printf("%*s| ",
           column_offset,
           "");

    unsigned int i;
    for (i = 0; i < dfa->symbol_count; ++i) {
        printf("%s   ", dfa->symbols[i]);
        size_t padding;
        padding = (i != (dfa->symbol_count - 1)) ? 3 : 1;
        row_length += strlen(dfa->symbols[i]) + padding;
    }
    printf("\n");
    for (i = 0; i < column_offset; ++i) {
        printf("-");
    }
    printf("+-");
    for (i = 0; i < row_length; ++i) {
        printf("-");
    }
    printf("\n");

    if (dfa->states == NULL) {
        return;
    }

    for (i = 0; i < dfa->state_count; ++i) {
        printf("%-*u| ",
               column_offset,
               i);

        unsigned int j;
        for (j = 0; j < dfa->states[i].transition_count; ++j) {
            if (dfa->states[i].transitions != NULL && dfa->symbols[j] != NULL) {
                printf("%u %*s",
                       dfa->states[i].transitions[j],
                       strlen(dfa->symbols[j]) + 1,
                       "");
            }
        }
        printf("\n");
    }
    printf("\n");
}
