#include "include/parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO LIST
// - Consertar o parsing de variaveis 
// - Fazer funções incompletas


parser_T* init_parser(lexer_T* lexer)
{
  parser_T* parser = calloc(1, sizeof(struct PARSER_STRUCT));
  parser->lexer = lexer;
  parser->current_token = lexer_get_next_token(lexer);
  
  return parser;
};

AST_T* parser_parse_variable_definition(parser_T* parser)
{
  // tipo nome = valor 

  parser_eat(parser, TOKEN_ID); // var  
  char* variable_definition_variable_name = parser->current_token->value;

  parser_eat(parser, TOKEN_ID);
  parser_eat(parser, TOKEN_EQUALS);

  AST_T* variable_value = parser_parse_expr(parser);

  AST_T* variable_definition = init_ast(AST_VARIABLE_DEFINITION);
  variable_definition->variable_definition_varname = variable_definition_variable_name;
  variable_definition->variable_definition_value = variable_value;

  return variable_definition;
}

AST_T* parser_parse_id(parser_T* parser)
{
  // inteiro nome = valor 
  // func in TODO  list
  if (strcmp(parser->current_token->value, "inteiro") == 0)
  {
    return parser_parse_variable_definition(parser);
  } else {
    return parser_parse_variable(parser);
  }
}

void parser_eat(parser_T* parser, int token_type)
{
  if(parser->current_token->type == token_type)
  {
    parser->current_token = lexer_get_next_token(parser->lexer);
  } else {
    printf(
        "Token não esperado `%s`, com o tipo %d",
        parser->current_token->value, 
        parser->current_token->type
    );
    exit(1);

  }
}

AST_T* parser_parse(parser_T* parser)
{
  return parser_parse_statements(parser);
}

AST_T* parser_parse_statement(parser_T* parser)
{
  switch (parser->current_token->type) 
  {
    case TOKEN_ID: return parser_parse_id(parser);
  }
}
AST_T* parser_parse_statements(parser_T* parser)
{
  AST_T* compound = init_ast(AST_COMPOUND);
  compound->compound_value = calloc(1, sizeof(struct AST_STRUCT*));
  

  AST_T* ast_statement = parser_parse_statement(parser);
  compound->compound_value[0] = ast_statement;


  while (parser->current_token->type == TOKEN_OPENINGBRACKET)
  { 
    parser_eat(parser, TOKEN_OPENINGBRACKET);

    AST_T* ast_statement = parser_parse_statement(parser);
    compound->compound_size += 1;
    compound->compound_value = realloc(
        compound->compound_value, 
        compound->compound_size * sizeof(struct AST_STRUCT*)
    );
    compound->compound_value[compound->compound_size - 1] = ast_statement;

  
  }

  return compound;
}
AST_T* parser_parse_expr(parser_T* parser)
{

}
AST_T* parser_parse_factor(parser_T* parser)
{

}
AST_T* parser_parse_term(parser_T* parser)
{

};
AST_T* parser_parse_function_call(parser_T* parser)
{

}
AST_T* parser_parse_variable(parser_T* parser)
{

}
AST_T* parser_parse_string(parser_T* parser)
{

}

