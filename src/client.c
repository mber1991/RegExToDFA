#include "client.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <termios.h>

#include "lexer.h"
#include "parser.h"
#include "build.h"
#include "io.h"


struct Client {
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

static void command_dfa(const Client *client)
{
    if (client == NULL) {
        return;
    }

    const char *input;
    input = IO_read(client->io, "enter regex below:\n");

    Regex_set_value(client->regex, input);

    Lexer_scan_regex(client->lexer, client->regex);

    Parser_scan_tokens(client->parser,
                       Lexer_get_tokens(client->lexer),
                       Lexer_get_token_count(client->lexer));

    IO_write("\n\n");
    build_dfa(Parser_get_token_list(client->parser),
              Parser_get_groups(client->parser),
              Parser_get_group_count(client->parser));

}

static void command_help(const Client *client)
{
    if (client == NULL) {
        return;
    }

    IO_write("Commands:\n");

    unsigned int i;
    for (i = 1; i < command_count; ++i) {
        IO_write("%-5s- %s\n",
                 client->commands[i],
                 client->help_messages[i]);
    }
}

static void command_quit(Client *client)
{
    Client_destroy(client);
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

static COMMAND get_command(const Client *client, const char *command_str)
{
    if (client == NULL || command_str == NULL) {
        return COMMAND_UNKNOWN;
    }

    if (strcmp(command_str, client->commands[COMMAND_HELP]) == 0) {
        return COMMAND_HELP;
    }
    if (strcmp(command_str, client->commands[COMMAND_DFA]) == 0) {
        return COMMAND_DFA;
    }
    if (strcmp(command_str, client->commands[COMMAND_QUIT]) == 0) {
        return COMMAND_QUIT;
    }

    return COMMAND_UNKNOWN;
}

static void run_command(Client *client, COMMAND command)
{
    if (client == NULL) {
        return;
    }

    switch (command) {
        case COMMAND_UNKNOWN:
            command_unknown();
            break;

        case COMMAND_DFA:
            command_dfa(client);
            break;

        case COMMAND_HELP:
            command_help(client);
            break;

        case COMMAND_QUIT:
            command_quit(client);
            break;

        default:
            break;
    }
}

Client *Client_create(void)
{
    Client *client;
    client = malloc(sizeof(Client));

    if (client != NULL) {
        client->io = IO_create(default_prompt);
        client->lexer = Lexer_create();
        client->parser = Parser_create(symbols, symbol_count);
        client->regex = Regex_create("");

        client->help_messages = help_messages;
        client->commands = commands;

        init_signals();
        init_terminal();
    }

    return client;
}

void Client_destroy(Client *client)
{
    if (client == NULL) {
        return;
    }

    Regex_destroy(client->regex);
    Parser_destroy(client->parser);
    Lexer_destroy(client->lexer);
    IO_destroy(client->io);

    reset_terminal();

    free(client);
    client = NULL;
}

void Client_run(Client *client)
{
    if (client == NULL) {
        return;
    }

    while (1) {
        const char *input;
        input = IO_read(client->io, NULL);
        if (input != NULL && input[0] != '\0') {
            COMMAND command;
            command = get_command(client, input);
            run_command(client, command);
        }
    }
}
