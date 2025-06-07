#include "include/lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

lexer_T* init_lexer(char* contents)
{
  lexer_T* lexer = calloc(1, sizeof(struct LEXER_STRUCT));
  lexer->contents = contents;
  lexer->index = 0;
  lexer->currentChar = contents[lexer->index];
  return lexer;
}

void lexer_advance(lexer_T* lexer)
{
  if (lexer->currentChar != '\0' && lexer->index < strlen(lexer->contents))
  {
    lexer->index += 1;
    lexer->currentChar = lexer->contents[lexer->index];
  }
}

void lexer_skip_whitespace(lexer_T* lexer)
{
  while (lexer->currentChar == ' ' || lexer->currentChar == 10) {
    lexer_advance(lexer);
  }
}

token_T* lexer_advance_with_token(lexer_T* lexer, token_T* token)
{
  lexer_advance(lexer);

  return token;
}

char* lexer_get_current_char_as_string(lexer_T* lexer)
{
  char* str = calloc(2, sizeof(char));
  str[0] = lexer->currentChar;
  str[1] = '\0';
  
  return str;
}

token_T* lexer_collect_string(lexer_T* lexer)
{
  lexer_advance(lexer);

  char* value = calloc(1, sizeof(char));
  value[0] = '\0';
  while (lexer->currentChar != '"')
  {
    char *s = lexer_get_current_char_as_string(lexer);
    value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
    strcat(value, s);

    lexer_advance(lexer);
  }

  lexer_advance(lexer);

  return init_token(TOKEN_STRING, value);
}

token_T* lexer_collect_id(lexer_T* lexer)
{
  char* value = calloc(1, sizeof(char));
  value[0] = '\0';

  while (isalnum(lexer->currentChar))
  {
    char *s = lexer_get_current_char_as_string(lexer);
    value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
    strcat(value, s);

    lexer_advance(lexer);
  }

  return init_token(TOKEN_ID, value);
}

token_T* lexer_get_next_token(lexer_T* lexer)
{
  while (lexer->currentChar != '\0' && lexer->index < strlen(lexer->contents))
  {
    if (isspace(lexer->currentChar))
      lexer_skip_whitespace(lexer);

    if (isalnum(lexer->currentChar))
      return lexer_collect_id(lexer);

    if (lexer->currentChar == '"')
      return lexer_collect_string(lexer);

    switch (lexer->currentChar)
    {
      case '=': return lexer_advance_with_token(lexer, init_token(TOKEN_EQUALS, lexer_get_current_char_as_string(lexer)));
      case '{': return lexer_advance_with_token(lexer, init_token(TOKEN_OPENINGBRACKET, lexer_get_current_char_as_string(lexer)));
      case '}': return lexer_advance_with_token(lexer, init_token(TOKEN_CLOSINGBRACKET, lexer_get_current_char_as_string(lexer)));
      case '(': return lexer_advance_with_token(lexer, init_token(TOKEN_LPAREN, lexer_get_current_char_as_string(lexer)));
      case ')': return lexer_advance_with_token(lexer, init_token(TOKEN_RPAREN, lexer_get_current_char_as_string(lexer)));
    }

    lexer_advance(lexer);
  }

  return NULL; // fim dos tokens
}

