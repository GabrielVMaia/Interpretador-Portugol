#include "include/AST.H"

AST_T* init_ast(int type)
{
    AST_T* ast = calloc(1, sizeof(AST_T));
    ast->type = type;

    /* AST_VARIABLE_DEFINITION */
    ast->variable_definition_varname = NULL;
    ast->variable_definition_value = NULL;

    /* AST_VARIABLE */
    ast->variable_name = NULL;

    /* AST_FUNCTION_CALL */
    ast->function_call_name = NULL;
    ast->function_call_arguments = NULL;
    ast->function_call_arguments_size = 0;

    /* AST_STRING */
    ast->string_value = NULL;

    /* AST_COMPOUND */
    ast->compound_value = NULL;
    ast->compound_size = 0;

    return ast;
}
