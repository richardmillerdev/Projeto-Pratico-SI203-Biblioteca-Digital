#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "biblioteca.h"


//funcao menu para aprensentação das opções
void menu(){
    printf("\n----------------------------------------");
    printf("\n[1] Cadastrar Usuario");
    printf("\n[2] Cadastrar Livro");
    printf("\n[3] Consultar historico de emprestimos ");
    printf("\n[4] Listar Acervo");
    printf("\n[5] Emprestar livro");
    printf("\n[6] Devolver livro");
    printf("\n[7] Excluir livro");
    printf("\n[8] Sair");
    printf("\n----------------------------------------");
}


void cadastrarUsuario(){
    //Abertura e verificacao do arquivo que armazena os usuarios
    FILE *arq = fopen("usuarios.txt", "a");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Usuario usuario;

    getchar();

    //captaçao dos dados do usuario
    printf("Nome: ");
    fgets(usuario.nome, sizeof(usuario.nome), stdin);
    usuario.nome[strcspn(usuario.nome, "\n")] = '\0';

    printf("CPF: ");
    scanf("%lld", &usuario.cpf);
    getchar();

    //Gravacao no arquivo usuarios.txt
    fprintf(arq, "%s|%lld\n", usuario.nome, usuario.cpf);

    fclose(arq);
    printf("Usuario cadastrado com sucesso!\n");
}

void cadastrarLivro(){
    //Abertura e verificacao do arquivo que armazena os usuarios
    FILE *arq = fopen("acervo.txt", "a");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Livro livro;

    getchar();
    
    //captaçao dos dados do Livro
    printf("Titulo: ");
    fgets(livro.titulo, sizeof(livro.titulo), stdin);
    livro.titulo[strcspn(livro.titulo, "\n")] = '\0';

    printf("codigo: ");
    scanf("%d", &livro.codigo);
    getchar();

    printf("Autor: ");
    fgets(livro.autor, sizeof(livro.autor), stdin);
    livro.autor[strcspn(livro.autor, "\n")] = '\0';

    printf("Ano: ");
    scanf("%d", &livro.ano);
    getchar();

    printf("Genero: ");
    fgets(livro.genero, sizeof(livro.genero), stdin);
    livro.genero[strcspn(livro.genero, "\n")] = '\0';

    printf("Quantidade: ");
    scanf("%d", &livro.quantidade);
    getchar();

    //Gravaçao dos dados no arquivo acervo.txt
    fprintf(arq, "%s|%d|%s|%d|%s|%d\n",
            livro.titulo,
            livro.codigo,
            livro.autor,
            livro.ano,
            livro.genero,
            livro.quantidade
    );

    fclose(arq);
    printf("Livro cadastrado com sucesso!\n");
}

void consultarHistorico(){
    int qnt_emprestados = 0;
    long long int cpf;
    long long int comparador;
    int codigo;
    int situacao;

    //Captacao dos dados do usuario
    printf("Digite o CPF: ");
    scanf("%lld",&cpf);
    printf("\n");

    
    //Abertura e verificacao do arquivo que armazena os emprestivos dos livros pelos usuarios
    FILE *file = fopen("emprestimos.txt", "r");
    if (file == NULL) {
        printf("Nenhum livro emprestado.\n");
        return ;
    }

    //leitura do arquivo e verificacao da situacao do emprestimo do usuaio
    while (fscanf(file, "%lld %d %d", &comparador, &codigo, &situacao ) != EOF) {
        if(cpf == comparador){
            printf("codigo: %d | ",codigo);
            if(situacao == 1){
                printf("situacao: emprestado");
                qnt_emprestados +=1;
            }
            else{
                printf("situacao: devolvido");
            }
            printf("\n");
        }
    }

    
    printf("\nQuantidade de livros emprestados: %d",qnt_emprestados);
    fclose(file);
    return ;
}

void listarAcervo(){
    //Abertura e verificacao do arquivo que armazena o estoque da biblioteca
    FILE *arq = fopen("acervo.txt", "r");
    if (arq == NULL) {
        printf("Nenhum livro cadastrado ainda.\n");
        return;
    }

    Livro livro;
    char linha[256];

    printf("\n===== LISTA DE LIVROS =====\n");

    //Print de todos os dados de cada livro presente no acervo
    while (fgets(linha, sizeof(linha), arq)) {
        linha[strcspn(linha, "\n")] = '\0';
        sscanf(linha, "%[^|]|%d|%[^|]|%d|%[^|]|%d",
               livro.titulo,
               &livro.codigo,
               livro.autor,
               &livro.ano,
               livro.genero,
               &livro.quantidade
        );
        printf("\nTitulo: %s\n", livro.titulo);
        printf("Codigo: %d\n", livro.codigo);
        printf("Autor: %s\n", livro.autor);
        printf("Ano: %d\n", livro.ano);
        printf("Genero: %s\n", livro.genero);
        printf("Quantidade: %d\n\n", livro.quantidade);
    }

    fclose(arq);
}

void emprestarLivro(){
    Livro livro;
    char linha[256];
    long long int cpf;
    int codigo;
    int situacao = 1, disponivel = 0, encontrado = 0;

    //Captacao da identificacao do usuario e do livro
    printf("Digite o cpf: ");
    scanf("%lld",&cpf);
    printf("Digite o codigo do livro: ");
    scanf("%d",&codigo);
    printf("\n");

    //Abertura e verificacao do arquivo que armazena o estoque da biblioteca
    FILE *acervo = fopen("acervo.txt", "r");
    if (acervo == NULL) {
        printf("Nenhum livro cadastrado ainda.\n");
        return;
    }

    //Criacao e verificacao do arquivo que armazenara temporariamente o estoque da biblioteca
    FILE *temp_acervo = fopen("temp_acervo.txt", "w");
    if (temp_acervo == NULL) {
        fclose(acervo);
        printf("Erro ao criar arquivo temporario.\n");
        return;
    }

    //verificacao se o livro pode ser emprestado ou nao, dependendo do estoque. Atualizando o acervo (-1 na qtde) quando possivel realizar o emprestimo
    while (fgets(linha, sizeof(linha), acervo)) {
        linha[strcspn(linha, "\n")] = '\0';
        sscanf(linha, "%[^|]|%d|%[^|]|%d|%[^|]|%d",
               livro.titulo,
               &livro.codigo,
               livro.autor,
               &livro.ano,
               livro.genero,
               &livro.quantidade);

        if (livro.codigo == codigo) {
            encontrado = 1;
            if (livro.quantidade > 0) {
                livro.quantidade -= 1;
                disponivel = 1;
                printf("Emprestimo possivel: %s (codigo %d)\n", livro.titulo, livro.codigo);
            } else {
                printf("Livro sem exemplares disponiveis.\n");
            }
        }
        fprintf(temp_acervo, "%s|%d|%s|%d|%s|%d\n",
                livro.titulo, livro.codigo, livro.autor, livro.ano, livro.genero, livro.quantidade);
    }

    fclose(acervo);
    fclose(temp_acervo);

    //Verificacao se o livro existe no banco
    if (!encontrado) {
        remove("temp_acervo.txt");
        printf("Livro com codigo %d nao encontrado.\n", codigo);
        return;
    }
    //Caso o emprestimo nao seja possivel, o arquivo e temporario, pois nao sera utilizado
    if (!disponivel) {
        remove("temp_acervo.txt");
        return;
    }

    //Substituicao do arquivo antigo pelo arquivo que era temporario
    remove("acervo.txt");
    rename("temp_acervo.txt", "acervo.txt");

    //Abertura e verificacao do arquivo que armazena os emprestimos de cada usuario
    FILE *arqEmprestimo = fopen("emprestimos.txt","a");
    if (arqEmprestimo==NULL){
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    
    //Adicao do emprestimo no arquivo de emprestimos
    fprintf(arqEmprestimo,"%lld %d %d\n", cpf,codigo,situacao);
    fclose(arqEmprestimo);
    
    
    printf("Emprestimo registrado com sucesso!\n");
}

void excluirLivro(){
    printf("Qual o codigo do livro que voce gostaria de excluir?\n");
    int codLivroExcluir;
    scanf("%d", &codLivroExcluir);
    
    
    
    //Abertura e verificacao do arquivo que armazena o estoque da biblioteca
    FILE *acervo = fopen("acervo.txt", "r");
    if (acervo == NULL) {
        printf("Nenhum livro cadastrado ainda.\n");
        return;
    }

    //Criacao e verificacao do arquivo que armazenara temporariamente o estoque da biblioteca
    FILE *temp_acervo = fopen("temp_acervo.txt", "w");
    if (temp_acervo == NULL) {
        fclose(acervo);
        printf("Erro ao criar arquivo temporario.\n");
        return;
    }
    int tem_emprestimo = 1;
    //Abertura e verificacao do arquivo que armazena os emprestivos dos livros pelos usuarios
    FILE *file = fopen("emprestimos.txt", "r");
    if (file == NULL) {
        printf("Nenhum emprestimo realizado.\n");
        tem_emprestimo = 0;
       
    }
    
    if(tem_emprestimo){
        //Criacao e verificacao do arquivo que armazenara temporariamente os emprestivos dos livros pelos usuarios
        FILE *temp = fopen("temp_emprestimo.txt", "w");
        if (!temp) {
            fclose(file);
            printf("Erro ao criar arquivo temporario do emprestimo.\n");
            return;
        }
    
        long long int cpf_lido;
        int codigo_lido, situacao;
        
        //Encontra o emprestimo
        while (fscanf(file, "%lld %d %d", &cpf_lido, &codigo_lido, &situacao) != EOF) {
            if (codigo_lido != codLivroExcluir) {
                fprintf(temp, "%lld %d %d\n", cpf_lido, codigo_lido, situacao);
            }else{
                if(situacao == 1){
                    printf("Voce nao pode deletar um livro que esta emprestado,\no livro esta emprestado pelo cpf: %lld, codigo do livro: %d", cpf_lido, codigo_lido);
                
                    return;
                }
            }
        }
    
        fclose(file);
        fclose(temp);
    
    
        //Substituicao do arquivo antigo pelo arquivo temporario
        remove("emprestimos.txt");
        rename("temp_emprestimos.txt", "emprestimos.txt");

    }
    
    
    char linha[512];
    Livro livro;
    int acervo_encontrado = 0;

    //deleta no acervo
    while (fgets(linha, sizeof(linha), acervo)) {
        linha[strcspn(linha, "\n")] = '\0';
        sscanf(linha, "%[^|]|%d|%[^|]|%d|%[^|]|%d",
               livro.titulo,
               &livro.codigo,
               livro.autor,
               &livro.ano,
               livro.genero,
               &livro.quantidade);

        if (livro.codigo != codLivroExcluir) {
            fprintf(temp_acervo, "%s|%d|%s|%d|%s|%d\n",
                    livro.titulo, livro.codigo, livro.autor, livro.ano, livro.genero, livro.quantidade);
        }
        
    }

    fclose(acervo);
    fclose(temp_acervo);
        //Substituicao do arquivo antigo pelo arquivo temporario acervo
    remove("acervo.txt");
    rename("temp_acervo.txt", "acervo.txt");
    
    printf("Seu livro foi excluido com sucesso!\nJunto com todo o histórico de emprestimos dele.\n");
    
    
}
void devolverLivro(){
    long long int cpf;
    int codigo;
    //Captacao da identificacao do usuario e do livro
    printf("Digite o CPF: ");
    scanf("%lld",&cpf);
    printf("Digite o codigo do livro:");
    scanf("%d",&codigo);
    printf("\n");
    
    //Abertura e verificacao do arquivo que armazena os emprestimos
    FILE *file = fopen("emprestimos.txt", "r");
    if (!file) {
        printf("Nenhum emprestimo registrado.\n");
        return;
    }
    
    //Criacao e verificacao do arquivo que armazenara temporariamente os emprestimos
    FILE *temp = fopen("temp_emprestimos.txt", "w");
    if (!temp) {
        fclose(file);
        printf("Erro ao criar arquivo temporario.\n");
        return;
    }

    long long int cpf_lido;
    int codigo_lido, situacao;
    int encontrado = 0;

    //Encontra o emprestimo
    while (fscanf(file, "%lld %d %d", &cpf_lido, &codigo_lido, &situacao) != EOF) {
        if (!encontrado && cpf_lido == cpf && codigo_lido == codigo && situacao == 1) {
            fprintf(temp, "%lld %d %d\n", cpf_lido, codigo_lido, 0);
            encontrado = 1;
        } else {
            fprintf(temp, "%lld %d %d\n", cpf_lido, codigo_lido, situacao);
        }
    }

    fclose(file);
    fclose(temp);

    if (!encontrado) {
        remove("temp_emprestimos.txt");
        printf("Emprestimo nao encontrado ou ja devolvido.\n");
        return;
    }

    //Substituicao do arquivo antigo "emprestimos" pelo arquivo temporario
    remove("emprestimos.txt");
    rename("temp_emprestimos.txt", "emprestimos.txt");

    //Abertura e verificacao do arquivo que armazena o acervo(estoque) dos livros
    FILE *acervo = fopen("acervo.txt", "r");
    if (!acervo) {
        printf("Erro: acervo nao encontrado para atualizar quantidade.\n");
        return;
    }
    
    //Criacao e verificacao do arquivo que armazenara temporariamente o acervo
    FILE *temp_acervo = fopen("temp_acervo.txt", "w");
    if (!temp_acervo) {
        fclose(acervo);
        printf("Erro ao criar arquivo temporario do acervo.\n");
        return;
    }

    char linha[512];
    Livro livro;
    int acervo_encontrado = 0;

    //aumenta o estoque do livro devolvido
    while (fgets(linha, sizeof(linha), acervo)) {
        linha[strcspn(linha, "\n")] = '\0';
        sscanf(linha, "%[^|]|%d|%[^|]|%d|%[^|]|%d",
               livro.titulo,
               &livro.codigo,
               livro.autor,
               &livro.ano,
               livro.genero,
               &livro.quantidade);

        if (livro.codigo == codigo) {
            livro.quantidade += 1;
            acervo_encontrado = 1;
        }
        fprintf(temp_acervo, "%s|%d|%s|%d|%s|%d\n",
                livro.titulo, livro.codigo, livro.autor, livro.ano, livro.genero, livro.quantidade);
    }

    fclose(acervo);
    fclose(temp_acervo);

    
    if (!acervo_encontrado) {
        printf("Aviso: livro (codigo %d) nao encontrado no acervo para atualizar quantidade.\n", codigo);
        remove("temp_acervo.txt");
        return;
    }

    //Substituicao do arquivo antigo pelo arquivo de acervo novo
    remove("acervo.txt");
    rename("temp_acervo.txt", "acervo.txt");

    printf("Livro devolvido com sucesso!\n");
}

void sair(){
    printf("\nsaindo...");
    exit(0);
}


