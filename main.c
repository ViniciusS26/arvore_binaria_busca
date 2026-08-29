#include <stdio.h>
#include <stdlib.h>

#include "prototipo.h"

int main(){
    int opc, status;

    ArvoreBinaria *arvoreBin;
    arvoreBin = NULL;

    struct Dadocliente dadocli;
    struct DadosCidade dadocid;
    struct DadosEndereco dadoEnd;

    do{
        printf(
          "\n 1 - inserir\n"
          "\n 2 - Buscar\n"
          "\n 3 - Remover\n"
          "\n 4 - Listar\n"
          "\n 0 - Sair\n"
        );

        scanf("%d", &opc);
        switch(opc){
            char opcs[2];
            case 1:
                printf(
                    "\n\t a - Inserir cliente\n"
                    "\n\t b - Inserir Cidade\n"
                    "\n\t c - Inserir Endereco\n"
                );
                scanf("%s", &opcs);
                if(opcs == "a"){
                    dadocli = lerDadoCliente();
                    status = inserirArvore(&arvoreBin, dadocli);
                }else if(opcs == "b"){
                    dadocid = lerDadoCidade();
                    status = inserirArvore(&arvoreBin, dadocid);
                }else if(opcs == "c"){
                    dadoEnd = lerDadoEndereco();
                    //fazer função para endereco
                }
            case 2:
                // fazer função de buscar
        }
    }while(opc != 0);




    return 0;
}
