#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "include/lexer.h"
#include "include/parser.h"

int main(int argc, char* argv[])
{
    const char *filepath = "examples/hello_world.por";

    FILE *fp = fopen(filepath, "rb");
    if (fp == NULL)
    {
        printf("[Main.c] Falha ao abrir arquivo: %s \n", strerror(errno));
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    rewind(fp);

    char *buffer = malloc(fileSize + 1);
    if (buffer == NULL)
    {
        printf("[Main.c] Falha ao alocar memória\n");
        fclose(fp);
        return -1;
    }

    size_t read_size = fread(buffer, 1, fileSize, fp);
    buffer[read_size] = '\0';
    fclose(fp);

    lexer_T* lexer = init_lexer(buffer);
    parser_T* parser = init_parser(lexer);
    AST_T* root = parser_parse(parser);

    free(buffer); 
    if (root->type == AST_PROGRAMA) {
        printf("Root type -> AST_PROGRAMA (%d)\n", root->type);

        if (root->body && root->body->type == AST_COMPOUND) {
            printf("Compound block size -> %lu\n", root->body->compound_size);
        } else {
            printf("Erro: programa.body não é um bloco composto\n");
        }
    } else {
        printf("Erro: AST raiz não é do tipo AST_PROGRAMA\n");
    }

    return 0;
}
