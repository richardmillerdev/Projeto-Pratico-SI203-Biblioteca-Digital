#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

typedef struct {
    char titulo[100];
    char autor[50];
    int ano;
    char genero[30];
    int quantidade;
    int codigo;
} Livro;

typedef struct {
    char nome[50];
    long long int cpf;
} Usuario;

void menu();
void cadastrarUsuario();
void cadastrarLivro();
void consultarHistorico();
void listarAcervo();
void emprestarLivro();
void devolverLivro();
void excluirLivro();
void sair();

#endif

