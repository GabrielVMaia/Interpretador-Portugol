#include <stdio.h>
#include "include/lexer.h"
#include "include/parser.h"

int main(int argc, char* argv[])
{

  // programa {
  // funcao inicio() {
    // escreva("Olá, mundo!")
 //  }
// }
    lexer_T* lexer = init_lexer( 
        "programa {\n funcao inicio() { \n cadeia nome = \"John lennon\"\n escreva(nome) \n}\n }"
    );

    parser_T* parser = init_parser(lexer);
    AST_T* root = parser_parse(parser);

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
