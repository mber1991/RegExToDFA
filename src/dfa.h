#ifndef DFA_H
#define DFA_H

#include <stddef.h>


typedef struct State {
    size_t symbol_count;
    const char **symbols;
    const unsigned int *transitions;
} State;

typedef struct DFA DFA;

extern State *State_create(const size_t symbol_count,
                           const char **symbols,
                           const unsigned int *transitions);
extern void State_destroy(State *state);

extern DFA *DFA_create(const size_t state_count,
                       const unsigned int *terminal_states,
                       const size_t terminal_state_count);
extern void DFA_destroy(DFA *dfa);

extern void DFA_set_state(DFA *dfa,
                          const unsigned int state_index,
                          const State *state);

#endif
