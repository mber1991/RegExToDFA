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

#endif
