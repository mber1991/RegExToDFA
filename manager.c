#include "manager.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <termios.h>

#include "lexer.h"
#include "parser.h"
#include "build.h"
#include "io.h"


struct Manager {
    IO *io;
    Lexer *lexer;
    Parser *parser;
    Regex *regex;
};

static const Symbol symbols[] = {
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

Manager *Manager_create(void)
{
    Manager *manager;
    manager = malloc(sizeof(Manager));

    if (manager != NULL) {
        manager->io = IO_create();
        manager->lexer = Lexer_create();
        manager->parser = Parser_create(symbols, 10);
        manager->regex = Regex_create("");

        init_signals();
        init_terminal();
    }

    return manager;
}

void Manager_destroy(Manager *manager)
{
    if (manager == NULL) {
        return;
    }

    Regex_destroy(manager->regex);
    Parser_destroy(manager->parser);
    Lexer_destroy(manager->lexer);
    IO_destroy(manager->io);

    reset_terminal();

    free(manager);
    manager = NULL;
}

void Manager_run(Manager *manager)
{
    while (1) {
        const char *regex_str;
        regex_str = IO_read(manager->io);
        if (regex_str != NULL) {
            if (strcmp(regex_str, "exit") == 0
                || strcmp(regex_str, "quit") == 0
            ) {
                break;
            }

            if (regex_str[0] != '\0') {
                Regex_set_value(manager->regex, regex_str);

                Lexer_scan_regex(manager->lexer, manager->regex);

                Parser_scan_tokens(manager->parser,
                                   Lexer_get_tokens(manager->lexer),
                                   Lexer_get_token_count(manager->lexer));

                build_dfa(Parser_get_token_list(manager->parser));
            }
        }
    }
}
