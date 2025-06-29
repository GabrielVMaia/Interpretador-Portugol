#ifndef TOKEN_H
#define TOKEN_H


typedef enum {
    TOKEN_ID,// 0
    TOKEN_FUNC,// 1
    TOKEN_ENTRY, // 2
    TOKEN_END, // 3
    TOKEN_PROGRAMA, // 4
    TOKEN_EQUALS, // 5
    TOKEN_STRING, // 6
    TOKEN_REAL,
    TOKEN_LPAREN, // 7
    TOKEN_RPAREN, // 8
    TOKEN_OPENINGBRACKET, // 9
    TOKEN_CLOSINGBRACKET // 10
} TokenType;

typedef struct TOKEN_STRUCT 
{
    TokenType type;
    char* value;
} token_T;

token_T* init_token(TokenType type, char* value);

#endif // !TOKEN_H
