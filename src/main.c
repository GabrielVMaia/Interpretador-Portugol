#include <stdio.h>
#include "include/lexer.h"

int main(int argc, char* argv[])
{
  lexer_T* lexer = init_lexer(
  
      "escreva(\"Olá mundo!\")\nescreva(\"Tchau, mundo!\")\n"
      );

  token_T* token = (void*)0;

  while ((token = lexer_get_next_token(lexer)) != NULL)
  {
    printf("TOKEN(%d, %s)\n", token->type, token->value);

  }
  return 0;
}
