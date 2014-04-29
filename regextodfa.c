#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <termios.h>


#include "lexer.h"
#include "parser.h"
#include "build.h"
#include "io.h"


static struct termios orig_term_attr, raw_term_attr;

static void init_terminal(void)
{
    tcgetattr(fileno(stdin), &orig_term_attr);

    raw_term_attr = orig_term_attr;

    raw_term_attr.c_lflag &= (unsigned int) ~(ECHOCTL);

    tcsetattr(fileno(stdin), TCSANOW, &raw_term_attr);
}

static void reset_terminal(void)
{
    tcsetattr(fileno(stdin), TCSANOW, &orig_term_attr);
}

static int init_signals(void)
{
    if (signal(SIGINT, SIG_IGN) == SIG_ERR) {
        return 0;
    }

    return 1;
}

int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;

    init_signals();
    init_terminal();

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

    IO *io;
    Lexer *lexer;
    Parser *parser;
    Regex *regex;

    io = IO_create();
    lexer = Lexer_create();
    parser = Parser_create(symbols, 10);
    regex = Regex_create("");

    while (1) {
        const char *regex_str;
        regex_str = IO_read(io);
        if (regex_str != NULL) {
            if (strcmp(regex_str, "exit") == 0
                || strcmp(regex_str, "quit") == 0
            ) {
                break;
            }

            if (regex_str[0] != '\0') {
                Regex_set_value(regex, regex_str);

                Lexer_scan_regex(lexer, regex);

                Parser_scan_tokens(parser,
                                   Lexer_get_tokens(lexer),
                                   Lexer_get_token_count(lexer));

                build_dfa(Parser_get_token_list(parser));
            }
        }
    }

    Regex_destroy(regex);
    Parser_destroy(parser);
    Lexer_destroy(lexer);
    IO_destroy(io);

    reset_terminal();

    return 0;
}
