#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "prototipo.h"



int valida_cpf(int tamanho, char *cpf)
{
    /* Validacao simples: CPF deve ter 11 digitos e conter apenas numeros */
    if(tamanho != 11)
    {
        return 0;
    }
    if(ehSoDigito(cpf))
    {
        /* ehSoDigito retorna 1 se encontrou caractere especial */
        return 0;
    }
    for(int i = 0; i < tamanho; i++)
    {
        if(cpf[i] < '0' || cpf[i] > '9')
        {
            return 0;
        }
    }
    return 1;
}

int  cepSemZero(int tamanho, char *cep)
{
    // verifica se os 3 primeiros digitos começam em zero
    // regra do cep os 3 primeiros numero não podem começar com 0
    char *resultado = malloc(tamanho + 1);
    int temZero = 0;
    for(int i = 0; i < tamanho - 5; i++)
    {
        if(cep[i] == '0')
        {
            resultado[i] = cep[i];
            temZero = 1;
        }
    }

    return temZero;
}

int verificaCasaCEP(char *cep, int tamanho)
{
    // verifico se os numeros de cada posição no cep estão dentro do range 0 a 9
    int estaNoRange = 0;

    for(int i = 0; i < tamanho; i++)
    {
        if(cep[i] >= '0' && cep[i] <= '9')
        {
            estaNoRange = 1;
        }
    }
    return estaNoRange;
}

int ehSoDigito(char *dado)
{
    /*
        Esta função é para verificar se tanto o CPF quanto o CEP contem caraceteres especiais
        A função é chamada na main para fazer a verificação no momento que o usuário digitar
    */
    char especiais[] = "-_.,";
    int encontrou = 0;
    int tamanhoDado = strlen(dado);
    int tamanhoCarcEspecial = strlen(especiais);

    for(int i = 0; i < tamanhoDado; i++)
    {
        for(int j = 0; j < tamanhoCarcEspecial; j++)
        {
            if(especiais[j] == dado[i])
            {
                encontrou = 1;
            }
        }
    }
    return encontrou;
}

char *processaDado( char *dado)
{
    /*
        Esta função serve para processar o dado informado pelo usuário
        No momento que o usuário digita o dados, esta função verifica se
        o que o usuário digitou é um CPF ou um CEP.

        Faz algumas validações:
        - Não permitir que o CEP comece com 000;
        - verifica se cada digito do cep está dentro do range 0 a 9;


    */
    int tamanho = strlen(dado);
    char *divDado = NULL;
    int sodigito = 0;

    if (tamanho == 11)
    {
        printf(" O numero que voce digitou eh um cpf \n");

    }
    if (tamanho == 8)
    {
        printf("O numero que voce digitou eh um cep \n");
        if(cepSemZero(tamanho, dado))
        {
            printf("Nao pode iniciar com 0 \n");
            divDado = dado;
        }
        if(verificaCasaCEP(dado, tamanho))
        {
            printf("Os numeros das casas devem estar entre 0 e 9 \n");
            divDado = dado;
        }

    }
    divDado = dado;

    return divDado;
}


