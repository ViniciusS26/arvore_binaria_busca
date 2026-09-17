#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "prototipo.h"

int main(){
    int opc;
    char opcs[10];
    char chaveBusca[100];
    char *checaCPFeCEP;

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
          " 5 - Teste de desempenho com 100 CPFs (item f)\n"
          " 0 - Sair\n"
          "============================\n"
          "Escolha: "
        );


        if(scanf("%d", &opc) != 1){
            limpaBuffer();
            printf("\nOpcao invalida!\n");
            opc = -1;
            continue;
        }
        limpaBuffer();

        switch(opc){
            case 1:
                printf(
                    "\n a - Inserir cliente\n"
                    "\n b - Inserir cidade\n"
                    "\n c - Inserir Endereco para um Cliente ja Cadastrado\n"
                    "Escolha: "
                );
                scanf("%9s", opcs);
                limpaBuffer();

                if(strcmp(opcs, "a") == 0){
                    dadocli = lerDadoCliente();
                    checaCPFeCEP = processaDado(dadocli.CPF);
                    if(checaCPFeCEP != NULL){
                        novoNo = criaNoCliente(dadocli);
                        if(inserirArvore(&arvoreCliente, novoNo)){
                            printf("\nCliente inserido com sucesso!\n");
                        }else{
                            free(novoNo);
                            printf("\nCliente nao inserido.\n");
                        }
                    }else{
                        printf("\nO CPF digitado eh invalido!\nCliente nao inserido.\n");
                    }

                }else if(strcmp(opcs, "b") == 0){
                    dadocid = lerDadoCidade();
                    checaCPFeCEP = processaDado(dadocid.CEP);
                    if(checaCPFeCEP != NULL){
                        novoNo = criaNoCidade(dadocid);
                        if(inserirArvore(&arvoreCidade, novoNo)){
                            printf("\nCidade inserida com sucesso!\n");
                        }else{
                            free(novoNo);
                            printf("\nCidade nao inserida.\n");
                        }
                    }else{
                        printf("\nO CEP digitado eh invalido!\nCidade nao inserida.\n");
                    }

                }else if(strcmp(opcs, "c") == 0){
                    printf("Informe o CPF do cliente para adicionar o endereco.\n");
                    leChaveSoDigitos(chaveBusca, TAM_CPF, "CPF");
                    encontrado = buscar(arvoreCliente, chaveBusca);
                    if(encontrado != NULL){
                        struct DadosEndereco novoEndereco = lerDadoEndereco();
                        encontrado->info.cliente.Enderco = novoEndereco;
                        printf("\nEndereco adicionado com sucesso!\n");
                    }else{
                        printf("\nCliente nao encontrado!\n");
                    }
                }else{
                    printf("\nOpcao invalida!\n");
                }
                break;

            case 2:
                printf(
                    "\n a - Buscar cliente (por CPF)\n"
                    "\n b - Buscar cidade (por CEP)\n"
                    "\nEscolha: "
                );
                scanf("%9s", opcs);
                limpaBuffer();

                if(strcmp(opcs, "a") == 0){
                    leChaveSoDigitos(chaveBusca, TAM_CPF, "CPF");
                    encontrado = buscar(arvoreCliente, chaveBusca);
                    if(encontrado != NULL){
                        imprimirDadoCliente(encontrado->info.cliente);
                    }else{
                        printf("\nCliente nao encontrado!\n");
                    }
                }else if(strcmp(opcs, "b") == 0){
                    leChaveSoDigitos(chaveBusca, TAM_CEP, "CEP");
                    encontrado = buscar(arvoreCidade, chaveBusca);
                    if(encontrado != NULL){
                        imprimirDadoCidade(encontrado->info.cidade);
                    }else{
                        printf("\nCidade nao encontrada!\n");
                    }
                }else{
                    printf("\nOpcao invalida!\n");
                }
                break;

            case 3:
                printf(
                    "\n a - Remover cliente (por CPF)\n"
                    "\n b - Remover cidade (por CEP)\n"
                    "\nEscolha: "
                );
                scanf("%9s", opcs);
                limpaBuffer();

                if(strcmp(opcs, "a") == 0){
                    leChaveSoDigitos(chaveBusca, TAM_CPF, "CPF");
                    if(removerNo(&arvoreCliente, chaveBusca)){
                        printf("\nCliente removido com sucesso!\n");
                    }else{
                        /* CORRECAO: antes nao havia aviso quando nao removia */
                        printf("\nCliente nao encontrado!\n");
                    }
                }else if(strcmp(opcs, "b") == 0){
                    leChaveSoDigitos(chaveBusca, TAM_CEP, "CEP");
                    if(removerNo(&arvoreCidade, chaveBusca)){
                        printf("\nCidade removida com sucesso!\n");
                    }else{
                        printf("\nCidade nao encontrada!\n");
                    }
                }else{
                    printf("\nOpcao invalida!\n");
                }
                break;

            case 4:
                printf("\n===== CLIENTES =====\n");
                imprimirArvore(arvoreCliente);
                printf("Total de clientes: %d | Altura da arvore: %d\n",
                       contaNos(arvoreCliente), alturaArvore(arvoreCliente));

                printf("\n===== CIDADES =====\n");
                imprimirArvore(arvoreCidade);
                printf("Total de cidades: %d | Altura da arvore: %d\n",
                       contaNos(arvoreCidade), alturaArvore(arvoreCidade));
                break;

            case 5:
                /* item (f) */
                testeDesempenhoCPF();
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
