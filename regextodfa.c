#include "lexer.h"
#include "parser.h"
#include "build.h"


int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;

    const char *regex_str;
    regex_str = "(a|b)*c\\*d.\\(\\[eDDDDDDDDDDD(f(s*ssg))i[a-j\\.[o-z[0-7]]]k\\.kk\\)*\\";

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

    build_dfa(Parser_get_token_list(parser));

    Regex_destroy(regex);
    Lexer_destroy(lexer);
    Parser_destroy(parser);


    return 0;
}
