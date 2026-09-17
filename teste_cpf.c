#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "prototipo.h"

/* ===================================================================
   ITEM (F) - EXPERIMENTO COM 100 CPFs

   Gera 100 CPFs (com digitos verificadores validos), insere os mesmos
   100 CPFs em 4 ordens diferentes e mede quantos nos sao percorridos
   para encontrar 10 CPFs escolhidos (os mesmos nos 4 casos) e mais um
   CPF que nao esta cadastrado.
   =================================================================== */

#define QTD_CPFS   100
#define QTD_BUSCAS  10

static char cpfsOrdenados[QTD_CPFS][12];   /* os 100 CPFs em ordem crescente */
static char cpfAusente[12];                /* CPF que nao sera cadastrado    */

/* ---------- geracao dos CPFs ---------- */

static void geraCPFValido(char *destino){
    /* gera 9 digitos aleatorios e calcula os 2 digitos verificadores */
    int d[11], i, soma, resto;

    for(i = 0; i < 9; i++){
        d[i] = rand() % 10;
    }

    soma = 0;
    for(i = 0; i < 9; i++){
        soma = soma + d[i] * (10 - i);
    }
    resto = soma % 11;
    d[9] = (resto < 2) ? 0 : 11 - resto;

    soma = 0;
    for(i = 0; i < 10; i++){
        soma = soma + d[i] * (11 - i);
    }
    resto = soma % 11;
    d[10] = (resto < 2) ? 0 : 11 - resto;

    for(i = 0; i < 11; i++){
        destino[i] = '0' + d[i];
    }
    destino[11] = '\0';
}

static int jaExiste(char vetor[][12], int qtd, char *cpf){
    int i, achou = 0;
    for(i = 0; i < qtd; i++){
        if(strcmp(vetor[i], cpf) == 0){
            achou = 1;
        }
    }
    return achou;
}

static int comparaCPF(const void *a, const void *b){
    return strcmp((const char*)a, (const char*)b);
}

static void geraVetorCPFs(){
    char novo[12];
    int i;

    for(i = 0; i < QTD_CPFS; i++){
        do{
            geraCPFValido(novo);
        }while(jaExiste(cpfsOrdenados, i, novo));
        strcpy(cpfsOrdenados[i], novo);
    }

    /* ordem crescente: facilita montar os 4 casos de insercao */
    qsort(cpfsOrdenados, QTD_CPFS, 12, comparaCPF);

    /* um CPF que com certeza nao esta na arvore */
    do{
        geraCPFValido(cpfAusente);
    }while(jaExiste(cpfsOrdenados, QTD_CPFS, cpfAusente));
}

/* ---------- montagem das arvores ---------- */

static struct Dadocliente montaCliente(char *cpf, int i){
    struct Dadocliente cliente;

    strcpy(cliente.CPF, cpf);
    sprintf(cliente.Nome, "Cliente_%03d", i);
    cliente.AnoNascimento = 1970 + (i % 40);
    cliente.Fone = 999000000 + i;
    strcpy(cliente.Genero, "NaoInformado");
    cliente.Enderco = enderecoVazio();

    return cliente;
}

static void insereCPF(ArvoreBinaria **raiz, char *cpf, int i){
    ArvoreBinaria *novo = criaNoCliente(montaCliente(cpf, i));
    if(inserirArvore(raiz, novo) == 0){
        free(novo);   /* nao entrou na arvore: devolve a memoria */
    }
}

static void embaralha(int *ordem, int n){
    int i, j, troca;
    for(i = n - 1; i > 0; i--){
        j = rand() % (i + 1);
        troca = ordem[i];
        ordem[i] = ordem[j];
        ordem[j] = troca;
    }
}

static ArvoreBinaria *montaArvore(int caso){
    /*
        caso 1 -> CPFs em ordem crescente
        caso 2 -> CPFs em ordem decrescente
        caso 3 -> o CPF do meio entra primeiro, o resto em ordem aleatoria
        caso 4 -> totalmente aleatorio
    */
    ArvoreBinaria *raiz = CriaArvore();
    int ordem[QTD_CPFS];
    int i, k, meio;

    if(caso == 1){
        for(i = 0; i < QTD_CPFS; i++){
            insereCPF(&raiz, cpfsOrdenados[i], i);
        }
    }else if(caso == 2){
        for(i = QTD_CPFS - 1; i >= 0; i--){
            insereCPF(&raiz, cpfsOrdenados[i], i);
        }
    }else if(caso == 3){
        meio = QTD_CPFS / 2;
        insereCPF(&raiz, cpfsOrdenados[meio], meio);
        /* embaralha os outros CPFs */
        k = 0;
        for(i = 0; i < QTD_CPFS; i++){
            if(i != meio){
                ordem[k] = i;
                k++;
            }
        }
        embaralha(ordem, k);
        for(i = 0; i < k; i++){
            insereCPF(&raiz, cpfsOrdenados[ordem[i]], ordem[i]);
        }
    }else{
        for(i = 0; i < QTD_CPFS; i++){
            ordem[i] = i;
        }
        embaralha(ordem, QTD_CPFS);
        for(i = 0; i < QTD_CPFS; i++){
            insereCPF(&raiz, cpfsOrdenados[ordem[i]], ordem[i]);
        }
    }

    return raiz;
}

/* ---------- execucao do experimento ---------- */

void testeDesempenhoCPF(){
    char *nomeCaso[5] = {
        "",
        "1) CPFs ordenados (crescente)",
        "2) CPFs ordenados (decrescente)",
        "3) CPF do meio primeiro + resto aleatorio",
        "4) Totalmente aleatorio"
    };
    int indiceBusca[QTD_BUSCAS];
    int passosCaso[5][QTD_BUSCAS];
    int passosAusente[5];
    int altura[5];
    int caso, i, passos, total;
    ArvoreBinaria *raiz;
    ArvoreBinaria *achado;

    srand((unsigned)time(NULL));
    geraVetorCPFs();

    /* os 10 CPFs pesquisados sao os mesmos nos 4 casos */
    for(i = 0; i < QTD_BUSCAS; i++){
        indiceBusca[i] = i * (QTD_CPFS / QTD_BUSCAS);   /* 0, 10, 20, ... 90 */
    }

    printf("\n\n================= ITEM (F) - EXPERIMENTO =================\n");
    printf("100 CPFs gerados. CPF nao cadastrado usado nas buscas: %s\n", cpfAusente);

    for(caso = 1; caso <= 4; caso++){
        raiz = montaArvore(caso);
        altura[caso] = alturaArvore(raiz);

        for(i = 0; i < QTD_BUSCAS; i++){
            passos = 0;
            achado = buscarComPassos(raiz, cpfsOrdenados[indiceBusca[i]], &passos);
            if(achado == NULL){
                passos = -1;   /* nao deveria acontecer */
            }
            passosCaso[caso][i] = passos;
        }

        passos = 0;
        buscarComPassos(raiz, cpfAusente, &passos);
        passosAusente[caso] = passos;

        liberaArvore(raiz);
    }

    /* ---- tabela de resultados ---- */
    printf("\nPASSOS (nos percorridos) PARA ENCONTRAR CADA CPF\n");
    printf("-----------------------------------------------------------------------\n");
    printf("%-15s %-8s %-8s %-8s %-8s\n", "CPF buscado", "Caso 1", "Caso 2", "Caso 3", "Caso 4");
    printf("-----------------------------------------------------------------------\n");
    for(i = 0; i < QTD_BUSCAS; i++){
        printf("%-15s %-8d %-8d %-8d %-8d\n",
               cpfsOrdenados[indiceBusca[i]],
               passosCaso[1][i], passosCaso[2][i],
               passosCaso[3][i], passosCaso[4][i]);
    }
    printf("-----------------------------------------------------------------------\n");

    printf("%-15s ", "MEDIA");
    for(caso = 1; caso <= 4; caso++){
        total = 0;
        for(i = 0; i < QTD_BUSCAS; i++){
            total = total + passosCaso[caso][i];
        }
        printf("%-8.1f ", (double)total / QTD_BUSCAS);
    }
    printf("\n");

    printf("%-15s ", "ALTURA");
    for(caso = 1; caso <= 4; caso++){
        printf("%-8d ", altura[caso]);
    }
    printf("\n");

    printf("%-15s ", "AUSENTE");
    for(caso = 1; caso <= 4; caso++){
        printf("%-8d ", passosAusente[caso]);
    }
    printf("\n-----------------------------------------------------------------------\n");

    printf("\nLegenda dos casos:\n");
    for(caso = 1; caso <= 4; caso++){
        printf("  Caso %s\n", nomeCaso[caso]);
    }
    printf("\nAUSENTE = passos gastos ate concluir que o CPF %s nao esta cadastrado.\n", cpfAusente);
    printf("==========================================================\n\n");
}
