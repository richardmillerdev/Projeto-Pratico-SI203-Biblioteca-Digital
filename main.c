#include <stdio.h>
#include "biblioteca.h"

void (*functions[])() = {
    NULL,
    cadastrarUsuario,
    cadastrarLivro,
    consultarHistorico,
    listarAcervo,
    emprestarLivro,
    devolverLivro,
    excluirLivro,
    sair,
};

int main(){
    int escolha;
    while(1){
        menu();
        printf("\nDigite o que deseja fazer: ");
        scanf("%d",&escolha);
        printf("\n");
        if (escolha >= 1 && escolha <= 8)
            functions[escolha]();
        else
            printf("Opcao invalida.\n");
    }
}

