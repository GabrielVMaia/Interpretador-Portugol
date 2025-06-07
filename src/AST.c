#include "include/AST.H"
#include <stdio.h>

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

void ast_print(AST_T* ast) {
    if (!ast) {
        printf("NULL");
        return;
    }

    switch (ast->type) {
        case AST_VARIABLE_DEFINITION:
            printf("AST_VARIABLE_DEFINITION(varname: %s, value: ", ast->variable_definition_varname);
            ast_print(ast->variable_definition_value);
            printf(")");
            break;

        case AST_VARIABLE:
            printf("AST_VARIABLE(name: %s)", ast->variable_name);
            break;

        case AST_STRING:
            printf("AST_STRING(value: \"%s\")", ast->string_value);
            break;

        case AST_FUNCTION_CALL:
            printf("AST_FUNCTION_CALL(name: %s, args: [", ast->function_call_name);
            for (size_t i = 0; i < ast->function_call_arguments_size; i++) {
                ast_print(ast->function_call_arguments[i]);
                if (i + 1 < ast->function_call_arguments_size)
                    printf(", ");
            }
            printf("])");
            break;

        case AST_COMPOUND:
            printf("AST_COMPOUND([\n");
            for (size_t i = 0; i < ast->compound_size; i++) {
                printf("  ");
                ast_print(ast->compound_value[i]);
                printf("\n");
            }
            printf("])");
            break;

        default:
            printf("AST_UNKNOWN");
            break;
    }
}

