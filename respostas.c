#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prototipo.h"

/* ==================== LEITURA DE DADOS ==================== */

struct DadosEndereco enderecoVazio(){
    /*
        Devolve um endereco "em branco".
        CORRECAO: antes o campo Enderco do cliente ficava com lixo de memoria
        quando o endereco ainda nao tinha sido cadastrado, e a impressao
        mostrava caracteres aleatorios na tela.
    */
    struct DadosEndereco dado;

    dado.CEPRua[0] = '\0';
    dado.Cidade[0] = '\0';
    dado.Estado[0] = '\0';
    dado.Rua[0]    = '\0';
    dado.Bairro[0] = '\0';
    dado.numero    = 0;

    return dado;
}

struct DadosEndereco lerDadoEndereco(){
    struct DadosEndereco dado;

    /* item (c): a mesma funcao generica le o CEP do endereco */
    leChaveSoDigitos(dado.CEPRua, TAM_CEP, "CEP da rua");

    printf("\nDigite a cidade: ");
    scanf("%49s", dado.Cidade);

    printf("\nDigite o estado (sigla): ");
    scanf("%2s", dado.Estado);

    printf("\nDigite a rua: ");
    scanf("%99s", dado.Rua);

    printf("\nDigite o bairro: ");
    scanf("%49s", dado.Bairro);

    printf("\nDigite o numero: ");
    if(scanf("%d", &dado.numero) != 1){
        dado.numero = 0;
    }
    limpaBuffer();

    return dado;
}

struct Dadocliente lerDadoCliente(){
    struct Dadocliente dado;

    /* item (c): leitura generica, so aceita digitos */
    leChaveSoDigitos(dado.CPF, TAM_CPF, "CPF");

    printf("\nDigite o nome do cliente: ");
    scanf("%99s", dado.Nome);

    printf("\nDigite o ano de nascimento: ");
    if(scanf("%d", &dado.AnoNascimento) != 1){
        dado.AnoNascimento = 0;
    }
    limpaBuffer();

    printf("\nDigite o telefone (somente numeros): ");
    if(scanf("%d", &dado.Fone) != 1){
        dado.Fone = 0;
    }
    limpaBuffer();

    printf("\nDigite o genero: ");
    scanf("%49s", dado.Genero);
    limpaBuffer();

    /* endereco comeca vazio: e' cadastrado depois pela opcao 1-c */
    dado.Enderco = enderecoVazio();

    return dado;
}

struct DadosCidade lerDadoCidade(){
    struct DadosCidade dado;

    leChaveSoDigitos(dado.CEP, TAM_CEP, "CEP da cidade");

    printf("\nDigite o nome da cidade: ");
    scanf("%49s", dado.NomeCidade);

    printf("\nDigite o estado (sigla): ");
    scanf("%2s", dado.Estado);
    limpaBuffer();

    dado.enderco = lerDadoEndereco();

    return dado;
}

/* ==================== CRIACAO DE NOS/ARVORE ==================== */

ArvoreBinaria *CriaArvore(){
    return NULL;
}

ArvoreBinaria *criaNoCliente(struct Dadocliente cliente){
    ArvoreBinaria *NovoCliente;
    NovoCliente = (ArvoreBinaria*)malloc(sizeof(ArvoreBinaria));
    if(NovoCliente != NULL){
        NovoCliente->tipoN = TIPO_CLIENTE;
        NovoCliente->info.cliente = cliente;
        NovoCliente->esquerda = NULL;
        NovoCliente->direita = NULL;
    }else{
        printf("Erro ao alocar cliente\n");
    }
    return NovoCliente;
}

ArvoreBinaria *criaNoCidade(struct DadosCidade cidade){
    ArvoreBinaria *NovaCidade;
    NovaCidade = (ArvoreBinaria*)malloc(sizeof(ArvoreBinaria));
    if(NovaCidade != NULL){
        NovaCidade->tipoN = TIPO_CIDADE;
        NovaCidade->info.cidade = cidade;
        NovaCidade->esquerda = NULL;
        NovaCidade->direita = NULL;
    }else{
        printf("Erro ao alocar cidade\n");
    }
    return NovaCidade;
}


char *pegaChave(ArvoreBinaria *no){
    /*
        Devolve a chave do no: CPF se for cliente, CEP se for cidade.
        E' essa funcao que torna inserir/buscar/remover genericas: elas nunca
        precisam saber o que o no guarda, so comparam a string devolvida aqui.
    */
    char *chave = NULL;

    if(no != NULL){
        if(no->tipoN == TIPO_CLIENTE){
            chave = no->info.cliente.CPF;
        }else{
            chave = no->info.cidade.CEP;
        }
    }
    return chave;
}

/* ==================== INSERCAO ==================== */

int inserirArvore(ArvoreBinaria **raiz, ArvoreBinaria *NovoNo){
    /*
        RETORNO: 1 -> inseriu   |   0 -> nao inseriu (chave duplicada)
    */
    int inseriu = 0;
    int comparaChave;

    if(NovoNo != NULL){
        if(*raiz == NULL){
            *raiz = NovoNo;
            inseriu = 1;
        }else{
            comparaChave = strcmp(pegaChave(NovoNo), pegaChave(*raiz));

            if(comparaChave < 0){
                inseriu = inserirArvore(&((*raiz)->esquerda), NovoNo);
            }else if(comparaChave > 0){
                inseriu = inserirArvore(&((*raiz)->direita), NovoNo);
            }else{
                /* chave duplicada: nao insere de novo */
                printf("\nJa existe um registro com essa chave!\n");
            }
        }
    }
    return inseriu;
}

/* ==================== BUSCA ==================== */

ArvoreBinaria *buscar(ArvoreBinaria *raiz, char *chave){
    /*
        RETORNO: endereco do no encontrado   |   NULL se nao encontrou
    */
    int cmp;
    ArvoreBinaria *encontrou = NULL;

    if(raiz != NULL){
        cmp = strcmp(chave, pegaChave(raiz));
        if(cmp == 0){
            encontrou = raiz;
        }else if(cmp < 0){
            encontrou = buscar(raiz->esquerda, chave);
        }else{
            encontrou = buscar(raiz->direita, chave);
        }
    }

    return encontrou;
}

ArvoreBinaria *buscarComPassos(ArvoreBinaria *raiz, char *chave, int *passos){
    /*
        Igual a buscar(), mas conta quantos nos foram visitados ate achar a
        chave (ou ate concluir que ela nao esta cadastrada).
        Usada no experimento do item (f).
    */
    int cmp;
    ArvoreBinaria *encontrou = NULL;

    if(raiz != NULL){
        (*passos)++;
        cmp = strcmp(chave, pegaChave(raiz));
        if(cmp == 0){
            encontrou = raiz;
        }else if(cmp < 0){
            encontrou = buscarComPassos(raiz->esquerda, chave, passos);
        }else{
            encontrou = buscarComPassos(raiz->direita, chave, passos);
        }
    }

    return encontrou;
}

/* ==================== REMOCAO ==================== */

int ehFolha(ArvoreBinaria *raiz){
    /* Funcao que verifica se o no e' folha */
    int ehfolha;
    ehfolha = 0;
    if(raiz != NULL && raiz->esquerda == NULL && raiz->direita == NULL){
        ehfolha = 1;
    }
    return ehfolha;
}

ArvoreBinaria *temSoUmFilho(ArvoreBinaria *raiz){
    /* devolve o endereco do unico filho; NULL se tiver dois filhos ou nenhum */
    ArvoreBinaria *tem;
    tem = NULL;
    if(raiz != NULL){
        if(raiz->esquerda == NULL){
            tem = raiz->direita;
        }else if(raiz->direita == NULL){
            tem = raiz->esquerda;
        }
    }
    return tem;
}

int removerNo(ArvoreBinaria **raiz, char *chave){
    /*
        RETORNO: 1 -> removeu   |   0 -> chave nao encontrada
    */
    int comparaStrings, removeu;
    ArvoreBinaria *aux, *endFilho;

    removeu = 0;

    if(*raiz != NULL){
        comparaStrings = strcmp(chave, pegaChave(*raiz));

        if(comparaStrings == 0){

            if(ehFolha(*raiz) == 1){                             /* e' no folha */
                aux = *raiz;
                *raiz = NULL;
                free(aux);
                removeu = 1;

            }else if((endFilho = temSoUmFilho(*raiz)) != NULL){   /* tem so um filho */
                aux = *raiz;
                *raiz = endFilho;
                free(aux);
                removeu = 1;

            }else{
                /* dois filhos: substitui pelo menor da subarvore direita */
                ArvoreBinaria *menor = (*raiz)->direita;
                char chaveMenor[12];

                while(menor->esquerda != NULL){
                    menor = menor->esquerda;
                }

                strcpy(chaveMenor, pegaChave(menor));   /* guarda a chave antes */
                (*raiz)->tipoN = menor->tipoN;          /* copia o tipo do no   */
                (*raiz)->info  = menor->info;           /* copia os dados       */

                removeu = removerNo(&((*raiz)->direita), chaveMenor);
            }

        }else if(comparaStrings < 0){
            removeu = removerNo(&((*raiz)->esquerda), chave);
        }else{
            removeu = removerNo(&((*raiz)->direita), chave);
        }
    }
    return removeu;
}

/* ==================== MEDIDAS DA ARVORE ==================== */

int alturaArvore(ArvoreBinaria *raiz){
    int alturaEsq, alturaDir, altura;

    altura = 0;
    if(raiz != NULL){
       
        if(alturaEsq > alturaDir){
            altura = alturaEsq + 1;
        }else{
            altura = alturaDir + 1;
        }
        alturaEsq = alturaArvore(raiz->esquerda);
        alturaDir = alturaArvore(raiz->direita);
    }
    return altura;
}

int contaNos(ArvoreBinaria *raiz){
    int total = 0;
    if(raiz != NULL){
        total = 1 + contaNos(raiz->esquerda) + contaNos(raiz->direita);
    }
    return total;
}

/* ==================== IMPRESSAO ==================== */

void imprimirDadoEndereco(struct DadosEndereco info){
    if(info.CEPRua[0] == '\0'){
        printf("\n   Endereco: (nao cadastrado)\n");
    }else{
        printf(
            "\n   CEP: %s"
            "\n   Cidade: %s"
            "\n   Estado: %s"
            "\n   Rua: %s"
            "\n   Bairro: %s"
            "\n   Numero: %d\n", info.CEPRua, info.Cidade, info.Estado, info.Rua, info.Bairro, info.numero
        );
    }
}

void imprimirDadoCliente(struct Dadocliente info){
    printf(
        "\nCPF: %s"
        "\nNome: %s"
        "\nAno de Nascimento: %d"
        "\nTelefone: %d"
        "\nGenero: %s\n", info.CPF, info.Nome, info.AnoNascimento, info.Fone, info.Genero
    );
    imprimirDadoEndereco(info.Enderco);
}

void imprimirDadoCidade(struct DadosCidade info){
    printf(
        "\nCEP: %s"
        "\nNome da Cidade: %s"
        "\nEstado: %s\n", info.CEP, info.NomeCidade, info.Estado
    );
    imprimirDadoEndereco(info.enderco);
}

void imprimirArvore(ArvoreBinaria *raiz){
    /* percurso em ordem: sai ordenado pela chave (CPF ou CEP) */
    if(raiz != NULL){
        imprimirArvore(raiz->esquerda);
        if(raiz->tipoN == TIPO_CLIENTE){
            imprimirDadoCliente(raiz->info.cliente);
        }else{
            imprimirDadoCidade(raiz->info.cidade);
        }
        printf("--------------------------------\n");
        imprimirArvore(raiz->direita);
    }
}

/* ==================== LIBERACAO DE MEMORIA ==================== */

void liberaArvore(ArvoreBinaria *raiz){
    if(raiz != NULL){
        liberaArvore(raiz->esquerda);
        liberaArvore(raiz->direita);
        free(raiz);
    }
}
