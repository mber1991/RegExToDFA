#ifndef PARSER_H
#define PARSER_H

#include "regex.h"
#include "token.h"


typedef struct Parser Parser;

extern Parser *Parser_create(Regex *regex);
extern void Parser_destroy(Parser *parser);

extern Token *Parser_get_tokens(Parser *parser);
extern size_t Parser_get_token_count(Parser *parser);

#endif
