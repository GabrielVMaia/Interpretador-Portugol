#ifndef TOKEN_H
#define TOKEN_H


typedef enum {
    TOKEN_ID,
    TOKEN_FUNC,
    TOKEN_ENTRY,
    TOKEN_END,
    TOKEN_PROGRAMA,
    TOKEN_EQUALS,
    TOKEN_STRING,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_OPENINGBRACKET,
    TOKEN_CLOSINGBRACKET
} TokenType;

typedef struct TOKEN_STRUCT 
{
    TokenType type;
    char* value;
} token_T;

token_T* init_token(TokenType type, char* value);

#endif // !TOKEN_H
