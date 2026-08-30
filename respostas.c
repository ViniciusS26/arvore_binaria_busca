#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prototipo.h"

/* ==================== LEITURA DE DADOS ==================== */

struct DadosEndereco lerDadoEndereco(){
    struct DadosEndereco dado;

    printf("\nDigite o CEP da rua: ");
    scanf("%8s", dado.CEPRua);

    printf("\nDigite a cidade: ");
    scanf("%49s", dado.Cidade);

    printf("\nDigite o estado (sigla): ");
    scanf("%2s", dado.Estado);

    printf("\nDigite a rua: ");
    scanf("%99s", dado.Rua);

    printf("\nDigite o bairro: ");
    scanf("%49s", dado.Bairro);

    printf("\nDigite o numero: ");
    scanf("%d", &dado.numero);

    return dado;
}

struct Dadocliente lerDadoCliente(){
    struct Dadocliente dado;

    printf("Digite o CPF (somente numeros): ");
    scanf("%11s", dado.CPF);

    printf("\nDigite o nome do cliente: ");
    scanf("%99s", dado.Nome);

    printf("\nDigite o ano de nascimento: ");
    scanf("%d", &dado.AnoNascimento);

    printf("\nDigite o telefone: ");
    scanf("%d", &dado.Fone);

    printf("\nDigite o genero: ");
    scanf("%49s", dado.Genero);


    return dado;
}

struct DadosCidade lerDadoCidade(){
    struct DadosCidade dado;

    printf("Digite o CEP da cidade: ");
    scanf("%8s", dado.CEP);

    printf("\nDigite o nome da cidade: ");
    scanf("%49s", dado.NomeCidade);

    printf("\nDigite o estado (sigla): ");
    scanf("%2s", dado.Estado);

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
        printf("Erro ao alocar cliente");
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
        printf("Erro ao alocar cidade");
    }
    return NovaCidade;
}


char *pegaChave(ArvoreBinaria *no){
    char *chave;
    if(no->tipoN == TIPO_CLIENTE){
        strcpy(chave,no->info.cliente.CPF);
    }else{
        strcpy(chave, no->info.cidade.CEP);
    }
    return chave;
}

/* ==================== INSERCAO ==================== */

int inserirArvore(ArvoreBinaria **raiz, ArvoreBinaria *NovoNo){
    int inseriu = 0;
    int comparaChave;

    if(*raiz == NULL){
        *raiz =  NovoNo;
        inseriu = 1;
    }
    comparaChave = strcmp(pegaChave(NovoNo), pegaChave(*raiz));
    if(comparaChave < 0){
        inseriu = inserirArvore(&((*raiz)->esquerda), NovoNo);
    }else if(comparaChave > 0){
        inseriu = inserirArvore(&((*raiz)->direita), NovoNo);
    }else{
        /* chave duplicada: nao insere de novo */
        printf("\nJa existe um registro com essa chave!\n");
    }
    return inseriu;

}

/* ==================== BUSCA ==================== */

ArvoreBinaria *buscar(ArvoreBinaria *raiz, char *chave){
    int cmp;
    ArvoreBinaria *encontrou;

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

/* ==================== REMOCAO ==================== */

int ehFolha(ArvoreBinaria *raiz){
    int ehfolha;
    ehfolha = 0;
    if(raiz->esquerda == NULL && raiz->direita == NULL){
        ehfolha = 1;
    }
    return ehfolha;
}

ArvoreBinaria *temSoUmFilho(ArvoreBinaria *raiz){
    ArvoreBinaria *tem;
    tem = NULL;
    if(raiz->esquerda == NULL){
        tem = raiz->direita;
    }else if(raiz->direita == NULL){
        tem = raiz->esquerda;
    }
    return tem;
}

int removerNo(ArvoreBinaria **raiz, char *chave){
    int comparaStrings, removeu;
    removeu = 0;
    ArvoreBinaria *aux, *endFilho;

    if(*raiz != NULL){
        comparaStrings = strcmp(chave, pegaChave(*raiz));
        if(comparaStrings == 0){
            if(ehFolha((*raiz))== 1){// é no flho
                aux = *raiz;
                *raiz = NULL;
                free(aux);
            }else if((endFilho = temSoUmFilho((*raiz))) != NULL){ // tem só um filho
                aux  = *raiz;
                *raiz = endFilho;
                free(aux);
            }else{
                 /* dois filhos substitui pelo menor da subarvore direita */
                ArvoreBinaria *menor = (*raiz)->direita;
                char chaveMenor[15];

                while(menor->esquerda != NULL){
                    menor = menor->esquerda;
                }

                (*raiz)->info = menor->info; /* copia os dados do menor encontrado  para a raiz*/
                strcpy(chaveMenor, pegaChave(menor));
                removerNo(&((*raiz)->direita), chaveMenor);
            }
            removeu = 1;

        }else if(comparaStrings < 0){
            removeu = removerNo(&((*raiz)->esquerda), chave);
        }else if(comparaStrings > 0){
            removeu = removerNo(&((*raiz)->direita), chave);
        }
    }
    return removeu;

}

/* ==================== IMPRESSAO ==================== */

void imprimirDadoEndereco(struct DadosEndereco info){
    printf(
        "\n   CEP: %s"
        "\n   Cidade: %s"
        "\n   Estado: %s"
        "\n   Rua: %s"
        "\n   Bairro: %s"
        "\n   Numero: %d\n", info.CEPRua, info.Cidade, info.Estado, info.Rua, info.Bairro, info.numero
    );
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
