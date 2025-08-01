#ifndef AST_H
#define AST_H

#include <stdlib.h>

typedef struct AST_STRUCT
{
  enum {
    AST_FUNCTION_CALL,
    AST_VARIABLE_DEFINITION,
    AST_VARIABLE,
    AST_STRING,
    AST_REAL,
    AST_COMPOUND,
    AST_PROGRAMA,
    AST_INICIO
  } type;

  /* AST_VARIABLE_DEFINITION */ 
  char* variable_definition_varname;
  struct AST_STRUCT* variable_definition_value;

  /* AST_VARIABLE */
  char* variable_name;
  
  /* AST_FUNCTION_CALL */ 
  char* function_call_name;
  struct AST_STRUCT** function_call_arguments;
  size_t function_call_arguments_size;

  /* AST_STRING */
  char* string_value;

  /* AST_REAL */
  char* real_value;

  /* AST_COMPOUND */
  struct AST_STRUCT** compound_value;
  size_t compound_size;

  /* AST_PROGRAMA */  
  struct AST_STRUCT* body;

  /* AST_INICIO */  
  struct AST_STRUCT* entryBody;

} AST_T;

AST_T* init_ast(int type);

void ast_print(AST_T* ast);
#endif // !AST_H
