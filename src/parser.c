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
    return parser_parse_variable_definition(parser);
  } else { 
    return parser_parse_variable(parser);
  }

}

void parser_eat(parser_T* parser, int token_type)
{
  if(parser->current_token->type == token_type)
  {
    printf("[parser_eat - current_token] %s\n", parser->current_token->value);
    parser->current_token = lexer_get_next_token(parser->lexer);
  } else {
    printf(
        "Token não esperado `%s`, com o tipo %d, se esperava %d",
        parser->current_token->value, 
        parser->current_token->type,
        token_type
    );
    exit(1);

  }
}
// Função para pegar a "funcao inicio()"
AST_T* parser_parse_entrypoint(parser_T* parser)
{
  // Aqui, já esperamos ter um node "programa" feito.
  printf("[DEBUG] Encontrando entrypoint\n");
  parser_eat(parser, TOKEN_FUNC);
  // Parseamos para encontrar funções e entrypoint
  parser_parse_variable(parser);
  // skipa os () 
  parser_eat(parser, TOKEN_LPAREN);
  parser_eat(parser, TOKEN_RPAREN);

  parser_eat(parser, TOKEN_OPENINGBRACKET); // "{"

  AST_T* entryPoint_Body = parser_parse_statements(parser);


  parser_eat(parser, TOKEN_CLOSINGBRACKET); // "{"
  AST_T* entrypoint_node = init_ast(AST_INICIO);
  entrypoint_node->entryBody = entryPoint_Body;

  return entrypoint_node;
}

AST_T* parser_parse_programa(parser_T* parser)
{
    parser_eat(parser, TOKEN_PROGRAMA); // "programa"
    parser_eat(parser, TOKEN_OPENINGBRACKET); // "{"

    parser_parse_statements(parser);

    parser_eat(parser, TOKEN_CLOSINGBRACKET); // "}"

    return parser_parse_entrypoint(parser);
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
    case TOKEN_FUNC: return parser_parse_entrypoint(parser);
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


// TODO ESSA BOMBA AQ
AST_T* parser_parse_function_call(parser_T* parser)
{
  // funcao nome(args) { }
  char* function_name = parser->current_token->value;
  printf("[parser_parse_function_call] parsing function %s\n", function_name);
  parser_eat(parser, TOKEN_ID);
  // printf("[parser_parse_function_call - current_token] %s\n", parser->current_token->value); 

// pulamos o primeiro ( 
  parser_eat(parser, TOKEN_LPAREN);

  /*
    Começamos a pegar os argumentos da função 
    char* function_call_name;
    struct AST_STRUCT** function_call_arguments;
    size_t function_call_arguments_size;
  */

  // Loopear até encontrar o )
  AST_T* ast_function = init_ast(AST_FUNCTION_CALL);
  ast_function->function_call_name = function_name;
}

AST_T* parser_parse_variable(parser_T* parser)
{
  char* token_value = parser->current_token->value;
  // verificamos se é o entry point tlg 
  if(strcmp(token_value, "inicio") == 0)
  {
    printf("[DEBUG] encontrado entryPoint\n");
  }

  printf("[parser_parse_variable] parsing %s\n", parser->current_token->value);

  // caso o nosso token for um parenteses, vamos parsear como função
  if (parser->current_token->type == TOKEN_FUNC || strcmp(parser->current_token->value, "inicio") == 0)
  {
    printf("[parser_parse_string] parsing function, %s\n", parser->current_token->value);
    return parser_parse_function_call(parser);
  }
  
  AST_T* ast_variable = init_ast(AST_VARIABLE);
  ast_variable->variable_name = token_value;

  printf("[parser_parse_variable] done parsing ast_variable, %s\n", ast_variable->variable_name);
  return ast_variable;
}

AST_T* parser_parse_string(parser_T* parser)
{
  AST_T* ast_string = init_ast(AST_STRING);
  ast_string->string_value = parser->current_token->value;

  parser_eat(parser, TOKEN_STRING);
  return ast_string;
}

