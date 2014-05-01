#ifndef DFA_H
#define DFA_H

#include <stddef.h>

#include "state.h"


typedef struct DFA DFA;

extern DFA *DFA_create(const size_t state_count,
                       const unsigned int *terminal_states,
                       const size_t terminal_state_count);
extern void DFA_destroy(DFA *dfa);

extern void DFA_set_state(DFA *dfa,
                          const unsigned int state_index,
                          const State *state);

#endif
