#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "prototipo.h"

int main(){
    int opc;
    char opcs[10];
    char chaveBusca[100];

    struct Dadocliente dadocli;
    struct DadosCidade dadocid;

    ArvoreBinaria *arvoreCliente = CriaArvore();
    ArvoreBinaria *arvoreCidade  = CriaArvore();
    ArvoreBinaria *novoNo;
    ArvoreBinaria *encontrado;

    do{
        printf(
          "\n============================\n"
          " 1 - Inserir\n"
          " 2 - Buscar\n"
          " 3 - Remover\n"
          " 4 - Listar\n"
          " 0 - Sair\n"
          "============================\n"
          "Escolha: "
        );
        scanf("%d", &opc);

        switch(opc){
            case 1:
                printf(
                    "\n a - Inserir cliente\n"
                    "\n b - Inserir cidade\n"
                    "Escolha: "
                );
                scanf("%9s", opcs);

                if(strcmp(opcs, "a") == 0){
                    dadocli = lerDadoCliente();
                    novoNo = criaNoCliente(dadocli);
                    if(inserirArvore(&arvoreCliente, novoNo)){
                        printf("\nCliente inserido com sucesso!\n");
                    }
                }else if(strcmp(opcs, "b") == 0){
                    dadocid = lerDadoCidade();
                    novoNo = criaNoCidade(dadocid);
                    if(inserirArvore(&arvoreCidade, novoNo)){
                        printf("\nCidade inserida com sucesso!\n");
                    }
                }
                break;

            case 2:
                printf(
                    "\n a - Buscar cliente (por CPF)\n"
                    " b - Buscar cidade (por CEP)\n"
                    "Escolha: "
                );
                scanf("%9s", opcs);

                if(strcmp(opcs, "a") == 0){
                    printf("Digite o CPF: ");
                    scanf("%99s", chaveBusca);
                    encontrado = buscar(arvoreCliente, chaveBusca);
                    if(encontrado != NULL){
                        imprimirDadoCliente(encontrado->info.cliente);
                    }else{
                        printf("\nCliente nao encontrado!\n");
                    }
                }else if(strcmp(opcs, "b") == 0){
                    printf("Digite o CEP: ");
                    scanf("%99s", chaveBusca);
                    encontrado = buscar(arvoreCidade, chaveBusca);
                    if(encontrado != NULL){
                        imprimirDadoCidade(encontrado->info.cidade);
                    }else{
                        printf("\nCidade nao encontrada!\n");
                    }
                }
                break;

            case 3:
                printf(
                    "\n a - Remover cliente (por CPF)\n"
                    " b - Remover cidade (por CEP)\n"
                    "Escolha: "
                );
                scanf("%9s", opcs);

                if(strcmp(opcs, "a") == 0){
                    printf("Digite o CPF: ");
                    scanf("%99s", chaveBusca);
                    if(removerNo(&arvoreCliente, chaveBusca)){
                        printf("\nCliente removido com sucesso!\n");
                    }
                }else if(strcmp(opcs, "b") == 0){
                    printf("Digite o CEP: ");
                    scanf("%99s", chaveBusca);
                    if(removerNo(&arvoreCidade, chaveBusca)){
                        printf("\nCidade removida com sucesso!\n");
                    }
                }
                break;

            case 4:
                printf("\n===== CLIENTES =====\n");
                imprimirArvore(arvoreCliente);
                printf("\n===== CIDADES =====\n");
                imprimirArvore(arvoreCidade);
                break;

            case 0:
                printf("\nSaindo...\n");
                break;

            default:
                printf("\nOpcao invalida!\n");
        }

    }while(opc != 0);

    /* libera toda a memoria alocada nas duas arvores */
    liberaArvore(arvoreCliente);
    liberaArvore(arvoreCidade);

    return 0;
}
