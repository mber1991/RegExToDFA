#include <stdio.h>

#include "dfa.h"
#include "state.h"
#include "lexer.h"
#include "parser.h"


int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;

    const unsigned int terminal_states[3] = {
        1,
        2,
        3,
    };

    DFA *dfa = DFA_create(20, terminal_states, 3);

    {
        const char *symbols[] = {
            "dsa2dwad",
            "dsa2dwad",
            "dsa2dwad",
            "dsa2dwad",
            "dsa2dwad",
            "dsa2dwad",
            "dsa2dwad",
            "dsa2dwad",
            "dsa2dwad",
            "dsa2dwad",
            "dsa2dwad",
            "dsa2dwad",
            "dsa2dwad",
            "dsa2dwad",
            "dsa2dwad",
        };

        const unsigned int transitions[] = {
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
        state.symbol_count = 15;
        state.symbols = symbols;
        state.transitions = transitions;

        DFA_set_state(dfa, 0, &state);
    }

    {
        const char *symbols[] = {
            "dsadsadsa",
            "dsadsadsa",
            "dsadsadsa",
            "dsadsadsa",
            "dsadsadsa",
        };

        const unsigned int transitions[] = {
            3,
            0,
            3,
            1,
            2,
        };

        State *state;
        state = State_create(5, symbols, transitions);

        DFA_set_state(dfa, 1, state);

        State_destroy(state);
    }

    DFA_destroy(dfa);


    const char *regex_str;
    regex_str = "(a|b)*c\\*d.\\(\\[eDDDDDDDDDDD(f(s*ssg))i[a-j\\.[o-z[0-7]]]k\\.kk\\)*\\";

    printf("Regex:\n\"%s\"\n\n", regex_str);

    Regex *regex;
    regex = Regex_create(regex_str);

    Lexer *lexer;
    lexer = Lexer_create(regex);

    const Symbol symbols[] = {
        { "",   SYMBOL_UNKNOWN       },
        { "(",  SYMBOL_GROUP_BEG     },
        { ")",  SYMBOL_GROUP_END     },
        { "[",  SYMBOL_RANGE_BEG     },
        { "]",  SYMBOL_RANGE_END     },
        { "-",  SYMBOL_RANGE_THROUGH },
        { ".",  SYMBOL_ANY_CHAR      },
        { "*",  SYMBOL_ZERO_OR_MORE  },
        { "|",  SYMBOL_OR            },
        { "\\", SYMBOL_ESCAPE        },
    };

    Parser *parser;
    parser = Parser_create(symbols, 10);

    Parser_scan_tokens(parser,
                       Lexer_get_tokens(lexer),
                       Lexer_get_token_count(lexer));

    Parser_match_groups(parser);

    Parser_match_ranges(parser);

    Regex_destroy(regex);
    Lexer_destroy(lexer);
    Parser_destroy(parser);


    return 0;
}
