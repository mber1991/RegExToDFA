#ifndef STATE_H
#define STATE_H

#include <stddef.h>


typedef struct State {
    size_t symbol_count;
    const char **symbols;
    const unsigned int *transitions;
} State;

extern State *State_create(const size_t symbol_count,
                           const char **symbols,
                           const unsigned int *transitions);
extern void State_destroy(State *state);

#endif