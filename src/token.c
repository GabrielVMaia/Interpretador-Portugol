#include "include/token.h"
#include <stdlib.h>

token_T* init_token(TokenType type, char* value)
{
  token_T* token = calloc(1, sizeof(struct TOKEN_STRUCT));
  token->type = type; 
  token->value = value;

  return token; 
}
