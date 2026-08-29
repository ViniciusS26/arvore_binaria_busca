#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prototipo.h"

struct Dadocliente lerDadoCliente(){
    struct Dadocliente dado;

    printf("Digite o cPF: ");
    scanf("%s", &dado.CPF);

    printf("\nDigite o nome da cidade: ");
    scanf("%s", &dado.Nome);

    printf("\nDigite o ano de nascimento: ");
    scanf("%d", &dado.AnoNascimento);

    printf("\nDigite o telefone: ");
    scanf("%d",  &dado.Fone);

    printf("\nDigite o genero: ");
    scanf("%s", dado.Genero);

    return dado;


}


ArvoreBinaria *criaNoCliente(struct Dadocliente cliente){
    ArvoreBinaria *NovoCliente;
    NovoCliente = (ArvoreBinaria*)malloc(sizeof(ArvoreBinaria*));
    if(NovoCliente != NULL){
        NovoCliente->tipoN = TIPO_CLIENTE;
        NovoCliente->info.cliente = cliente;
        NovoCliente->esquerda = NULL;
        NovoCliente->direita = NULL;
    }else{
        printf("Erro ao alocar cliente");
    }
    return NovoCliente;
}



ArvoreBinaria *criaNoCidade(struct DadosCidade cidade){
    ArvoreBinaria *NovaCidade;
    NovaCidade = (ArvoreBinaria*)malloc(sizeof(ArvoreBinaria*));
    if(NovaCidade != NULL){
        NovaCidade->tipoN = TIPO_CIDADE;
        NovaCidade->info.cidade = cidade;
        NovaCidade->esquerda = NULL;
        NovaCidade->direita = NULL;
    }else{
        printf("Erro ao alocar cidade");
    }
    return NovaCidade;
}



void extraiChaveEAlocaNo(ArvoreBinaria *raiz, char *chaveAtual, char *chaveNova){

    if(raiz->tipoN == TIPO_CLIENTE){
        strcpy(chaveAtual, raiz->info.cliente.CPF);// pega a info que ja tem cadastrada na arvore
        strcpy(chaveNova, NovoNo->info.cliente.CPF);// pega a chave nova que será cadastrada na arvore
    }else if(raiz->tipoN == TIPO_CIDADE){
        strcpy(chaveAtual, raiz->info.cidade.CEP);
        strcpy(chaveNova, NovoNo->info.cidade.CEP);
    }
}

int inserirArvore(ArvoreBinaria **raiz, ArvoreBinaria *NovoNo ){
    int inseriu = 0;

    char *chaveAtual;
    char *chaveNova;

    if(*raiz == NULL){
        extraiChaveEAlocaNo(*raiz, *NovoNo, chaveAtual, chaveNova);
        inseriu = 1;
    }else if(strcmp(chaveNova, chaveAtual)< 0){
        inseriu = inserirArvore(&((*raiz))->esquerda,NovoNo);
    }else if(chaveNova, chaveAtual)> 0){
        inseriu = inserirArvore(&((*raiz))->direita,NovoNo);
    }

    return inseriu;

}
void imprimirDadoEndereco(DadosEndereco info){
    printf(
        "\nCEP: %s\n"
        "\nCidade: %s\n"
        "\nRua: %s"
        "\nBairro: %s"
        "\nNumero da Rua: %d", info.CEPRua, info.Cidade, info.Rua, info.Bairro, info.numero
    );
}



void imprimirDadoCliente(Dadocliente info){
    printf(
        "\nCPF: %s \n"
        "\nNome: %s \n"
        "\nAno de Nascimento: %d\n"
        "\nTelefone: %d\n"
        "\nGenero %s", info.CPF, info.Nome, info.AnoNascimento,info.Fone,info.Genero
    );
    if(info.Enderco != NULL){
        imprimirDadoEndereco(info.Enderco);
    }
}
void imprimirArvore(ArvoreBinaria *raiz){
    if(raiz != NULL){
        if(raiz->tipoN == TIPO_CLIENTE){
            imprimirDadoCliente(raiz->info.cliente);
        }
        imprimirArvore(raiz->esquerda);
        imprimirArvore(raiz->direita);
    }
}
