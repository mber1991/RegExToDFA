#include "dfa.h"
#include "state.h"

#define STATE_COUNT 20
#define SYMBOL_COUNT 10


int main(int argc, char **argv)
{
    unsigned int terminal_states[3] = {
        1,
        2,
        3,
    };

    DFA *dfa = DFA_create(STATE_COUNT, terminal_states, 3);


    {
        unsigned int symbols[SYMBOL_COUNT] = {
            'a',
            'e',
            'd',
            'c',
            'z',
            'g',
            'j',
            's',
            'p',
            'l',
        };

        unsigned int transitions[SYMBOL_COUNT] = {
            1,
            3,
            8,
            0,
            5,
            6,
            3,
            11,
            2,
            5,
        };

        State *state = State_create(SYMBOL_COUNT, symbols, transitions);

        DFA_set_state(dfa, 0, state);

        State_destroy(state);
    }

    {
        unsigned int symbols[SYMBOL_COUNT] = {
            'c',
            'l',
            'e',
            'g',
            'a',
            'd',
            'z',
            'p',
            'j',
            's',
        };

        unsigned int transitions[SYMBOL_COUNT] = {
            1,
            8,
            2,
            0,
            3,
            6,
            5,
            11,
            5,
            3,
        };

        State *state = State_create(SYMBOL_COUNT, symbols, transitions);

        DFA_set_state(dfa, 1, state);

        State_destroy(state);
    }

    DFA_destroy(dfa);


    return 0;
}
