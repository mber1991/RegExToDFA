#include <stdio.h>

#include "dfa.h"
#include "state.h"
#include "parser.h"


int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;

    unsigned int terminal_states[3] = {
        1,
        2,
        3,
    };

    DFA *dfa = DFA_create(20, terminal_states, 3);


    {
        unsigned int symbols[15] = {
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
            'a',
            'k',
            'l',
            'd',
            'p',
        };

        unsigned int transitions[15] = {
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
            9,
            2,
            6,
            5,
            1,
        };

        State state;
        state.symbol_count = 0;
        state.symbols = symbols;
        state.transitions = transitions;

        DFA_set_state(dfa, 0, &state);
    }

    {
        unsigned int symbols[5] = {
            'p',
            'j',
            's',
            'z',
            'n',
        };

        unsigned int transitions[5] = {
            3,
            0,
            3,
            1,
            2,
        };

        State *state = State_create(5, symbols, transitions);

        DFA_set_state(dfa, 1, state);

        State_destroy(state);
    }

    DFA_destroy(dfa);



    const char *regex_str = "(a|b)*cd.efg\\hi[j]";

    printf("Regex:\n\"%s\"\n\n", regex_str);

    Regex *regex;
    regex = Regex_create(regex_str);

    Parser *parser;
    parser = Parser_create(regex);

    Regex_destroy(regex);

    Parser_match_symbols(parser);

    Parser_destroy(parser);


    return 0;
}
