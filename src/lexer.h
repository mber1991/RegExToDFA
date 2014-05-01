#ifndef PARSER_H
#define PARSER_H

#include "regex.h"
#include "token.h"


typedef struct Lexer Lexer;

extern Lexer *Lexer_create(void);
extern void Lexer_destroy(Lexer *lexer);

extern Token *Lexer_get_tokens(Lexer *lexer);
extern size_t Lexer_get_token_count(Lexer *lexer);

extern void Lexer_scan_regex(Lexer *lexer, Regex *regex);

#endif
