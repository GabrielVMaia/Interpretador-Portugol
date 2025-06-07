#include "include/parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 1


// TODO LIST
// - Fazer funções incompletas

parser_T* init_parser(lexer_T* lexer)
{
    parser_T* parser = calloc(1, sizeof(struct PARSER_STRUCT));
    parser->lexer = lexer;
    parser->current_token = lexer_get_next_token(lexer);

    printf("[DEBUG] Primeiro token: '%s' (tipo %d)\n",
           parser->current_token->value,
           parser->current_token->type);

    return parser;
}


AST_T* parser_parse_variable_definition(parser_T* parser)
{
  // tipo nome = valor 
  // inteiro numero = 10

  // pegamos o TIPO da variavel 
  char* variable_type = parser->current_token->value;
  parser_eat(parser, TOKEN_ID);

  // comemos o NOME IDENTIFICADOR da variavel 
  char* variable_name = parser->current_token->value; 
  parser_eat(parser, TOKEN_ID);

  // come o "="
  parser_eat(parser, TOKEN_EQUALS);

  // parsea o valor 
  AST_T* variable_value = parser_parse_expr(parser);

  #ifdef DEBUG
    printf("[PARSER - variable_definition] type: %s, name: %s, value: ", variable_type, variable_name);
    ast_print(variable_value);
    printf("\n");
  #endif /* ifdef DEBUG */


  // Declaramos a definição da variavel e então retornamos ela 
  
  AST_T* variable_definition = init_ast(AST_VARIABLE_DEFINITION);
  variable_definition->variable_definition_varname = variable_name;
  variable_definition->variable_definition_value = variable_value;

  return variable_definition;
}


// TODO MELHORAR ISSO
int isReserved(const char* value)
{
    
    static const char* reservedTypes[] = {"inteiro", "real", "logico", "cadeia"};

    int count = sizeof(reservedTypes) / sizeof(reservedTypes[0]);

    for (int i = 0; i < count; i++)
    {
        if (strcmp(value, reservedTypes[i]) == 0)
            return 1;
    }
    return 0;
}

AST_T* parser_parse_id(parser_T* parser)
{

  /*
  if (strcmp(parser->current_token->value, "inteiro") == 0)
  {
    return parser_parse_variable_definition(parser);
  } else {
    return parser_parse_variable(parser);
  }
  */ 
  if (isReserved(parser->current_token->value)) {
    printf("[PARSER] Parseando variable definition\n");
    return parser_parse_variable_definition(parser);
  } else { 
    printf("[Parser] Parseando variable\n");
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

AST_T* parser_parse_programa(parser_T* parser)
{
    parser_eat(parser, TOKEN_ID); // "programa"
    parser_eat(parser, TOKEN_OPENINGBRACKET); // "{"

    AST_T* body = parser_parse_statements(parser);

    parser_eat(parser, TOKEN_CLOSINGBRACKET); // "}"

    AST_T* programa_node = init_ast(AST_PROGRAMA);
    programa_node->body = body;

    return programa_node;
}

AST_T* parser_parse(parser_T* parser)
{
    if (strcmp(parser->current_token->value, "programa") == 0)
        return parser_parse_programa(parser);

    printf("Erro: programa deve começar com 'programa'\n");
    exit(1);
}


AST_T* parser_parse_statement(parser_T* parser)
{
  switch (parser->current_token->type) 
  {
    case TOKEN_ID: return parser_parse_id(parser);
    default: return NULL;
  }
}


AST_T* parser_parse_statements(parser_T* parser)
{
    AST_T* compound = init_ast(AST_COMPOUND);
    compound->compound_value = calloc(1, sizeof(struct AST_STRUCT*));
    compound->compound_size = 0;

    while (
        parser->current_token->type != TOKEN_CLOSINGBRACKET &&
        parser->current_token->type != TOKEN_END
    )
    {
        AST_T* statement = parser_parse_statement(parser);
        if (!statement)
            break;

        compound->compound_size += 1;
        compound->compound_value = realloc(
            compound->compound_value,
            compound->compound_size * sizeof(struct AST_STRUCT*)
        );

        compound->compound_value[compound->compound_size - 1] = statement;
    }

    return compound;
}

AST_T* parser_parse_expr(parser_T* parser)
{
  printf("%s\n", parser->current_token->value);
  switch(parser->current_token->type)
  {
    case TOKEN_STRING: return parser_parse_string(parser);
  }

  printf("Erro: expressão inesperada `%s`\n", parser->current_token->value);
  exit(1);
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
  char* token_value = parser->current_token->value;
  // esperamos o nome da variavel ou nome da função
  parser_eat(parser, TOKEN_ID);

  // caso o nosso token for um parenteses, vamos parsear como função
  if (parser->current_token->type == TOKEN_LPAREN)
    return parser_parse_function_call(parser);
  
  AST_T* ast_variable = init_ast(AST_VARIABLE);
  ast_variable->variable_name = token_value;

  return ast_variable;
}

AST_T* parser_parse_string(parser_T* parser)
{
  AST_T* ast_string = init_ast(AST_STRING);
  ast_string->string_value = parser->current_token->value;

  parser_eat(parser, TOKEN_STRING);
  return ast_string;
}

