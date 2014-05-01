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
    const char **help_messages, **commands;
};

static struct termios orig_term_attr, raw_term_attr;

static const char *default_prompt = "regextodfa> ";

static const char *help_messages[] = {
    [COMMAND_UNKNOWN] = "",
    [COMMAND_DFA]     = "Convert regex to DFA",
    [COMMAND_HELP]    = "Print this help text",
    [COMMAND_QUIT]    = "Quit the program",
};

static const char *commands[] = {
    [COMMAND_UNKNOWN] = "",
    [COMMAND_DFA]     = "dfa",
    [COMMAND_HELP]    = "help",
    [COMMAND_QUIT]    = "quit",
};

static const size_t command_count = 4;

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

static const size_t symbol_count = 10;

static void command_unknown(void)
{
    IO_write("command not found\n");
}

static void command_dfa(const Manager *manager)
{
    if (manager == NULL) {
        return;
    }

    const char *input;
    input = IO_read(manager->io, "enter regex below:\n");

    Regex_set_value(manager->regex, input);

    Lexer_scan_regex(manager->lexer, manager->regex);

    Parser_scan_tokens(manager->parser,
                       Lexer_get_tokens(manager->lexer),
                       Lexer_get_token_count(manager->lexer));

    build_dfa(Parser_get_token_list(manager->parser));
}

static void command_help(const Manager *manager)
{
    if (manager == NULL) {
        return;
    }

    IO_write("Commands:\n");

    unsigned int i;
    for (i = 1; i < command_count; ++i) {
        IO_write("%-5s- %s\n",
                 manager->commands[i],
                 manager->help_messages[i]);
    }
}

static void command_quit(Manager *manager)
{
    Manager_destroy(manager);
    exit(0);
}

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

static COMMAND get_command(const Manager *manager, const char *command_str)
{
    if (manager == NULL || command_str == NULL) {
        return COMMAND_UNKNOWN;
    }

    if (strcmp(command_str, manager->commands[COMMAND_HELP]) == 0) {
        return COMMAND_HELP;
    }
    if (strcmp(command_str, manager->commands[COMMAND_DFA]) == 0) {
        return COMMAND_DFA;
    }
    if (strcmp(command_str, manager->commands[COMMAND_QUIT]) == 0) {
        return COMMAND_QUIT;
    }

    return COMMAND_UNKNOWN;
}

static void run_command(Manager *manager, COMMAND command)
{
    if (manager == NULL) {
        return;
    }

    switch (command) {
        case COMMAND_UNKNOWN:
            command_unknown();
            break;

        case COMMAND_DFA:
            command_dfa(manager);
            break;

        case COMMAND_HELP:
            command_help(manager);
            break;

        case COMMAND_QUIT:
            command_quit(manager);
            break;

        default:
            break;
    }
}

Manager *Manager_create(void)
{
    Manager *manager;
    manager = malloc(sizeof(Manager));

    if (manager != NULL) {
        manager->io = IO_create(default_prompt);
        manager->lexer = Lexer_create();
        manager->parser = Parser_create(symbols, symbol_count);
        manager->regex = Regex_create("");

        manager->help_messages = help_messages;
        manager->commands = commands;

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
    if (manager == NULL) {
        return;
    }

    while (1) {
        const char *input;
        input = IO_read(manager->io, NULL);
        if (input != NULL && input[0] != '\0') {
            COMMAND command;
            command = get_command(manager, input);
            run_command(manager, command);
        }
    }
}
