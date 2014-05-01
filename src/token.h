#ifndef TOKEN_H
#define TOKEN_H


typedef enum TOKEN_TYPE {
    TOKEN_UNKNOWN,
    TOKEN_SYMBOL,
    TOKEN_LITERAL,
    TOKEN_ESCAPE_CHAR,
} TOKEN_TYPE;

typedef struct Token {
    char *value;
    unsigned int begin, end;
    TOKEN_TYPE type;
} Token;


extern Token *Token_create(const char *value,
                           const unsigned int begin, const unsigned int end,
                           const TOKEN_TYPE type);
extern void Token_destroy(Token *token);

#endif
