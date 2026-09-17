#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "prototipo.h"

int ehSoDigito(char *dado)
{
    /*
        Esta funcao verifica se o dado (CPF ou CEP) contem algum caractere
        que NAO seja digito (separadores como - _ . , ou letras).

        RETORNO: 1 -> encontrou caractere invalido (nao e' so digito)
                 0 -> o dado e' formado somente por digitos

        CORRECAO: antes so olhava a lista "-_.," e deixava passar letras.
        Agora qualquer caractere fora da faixa '0'..'9' invalida o dado.
    */
    int encontrou = 0;
    int tamanhoDado = strlen(dado);
    int i;

    for(i = 0; i < tamanhoDado; i++)
    {
        if(dado[i] < '0' || dado[i] > '9')
        {
            encontrou = 1;
        }
    }
    return encontrou;
}


int valida_cpf(int tamanho, char *cpf)
{
    /*
        Validacao simples: CPF deve ter 11 posicoes e conter apenas numeros.

        RETORNO: 1 -> CPF valido   |   0 -> CPF invalido

        CORRECOES:
        - a variavel "valido" nao era inicializada (lixo de memoria);
        - a logica estava invertida: ao achar um caractere que nao era
          digito a funcao fazia valido = 1 (dizia que era valido).
    */
    int valido = 1;
    int i;

    if(tamanho != TAM_CPF)
    {
        valido = 0;
    }
    if(ehSoDigito(cpf))
    {
        /* ehSoDigito retorna 1 se encontrou caractere que nao e' digito */
        valido = 0;
    }
    for(i = 0; i < tamanho; i++)
    {
        if(cpf[i] < '0' || cpf[i] > '9')
        {
            valido = 0;
        }
    }
    return valido;
}

int cepSemZero(int tamanho, char *cep)
{
    /*
        Verifica a regra adotada no trabalho: o prefixo do CEP (3 primeiros
        digitos) nao pode ser "000".

        RETORNO: 1 -> o prefixo e' 000 (CEP recusado)
                 0 -> prefixo aceitavel

        CORRECOES:
        - havia um malloc que nunca era liberado (vazamento de memoria) e
          que nao era usado para nada;
        - antes bastava UM zero entre os 3 primeiros para recusar o CEP,
          o que rejeitava CEPs reais como 01310000 (Sao Paulo).
          Agora so recusa quando os tres primeiros sao zero.
    */
    int temZero = 0;
    int i;
    int zeros = 0;

    if(tamanho >= 3)
    {
        for(i = 0; i < 3; i++)
        {
            if(cep[i] == '0')
            {
                zeros++;
            }
        }
        if(zeros == 3)
        {
            temZero = 1;
        }
    }
    return temZero;
}

int verificaCasaCEP(char *cep, int tamanho)
{
    /*
        Verifica se o numero de CADA posicao do CEP esta no range '0' a '9'.

        RETORNO: 1 -> todas as casas sao digitos   |   0 -> alguma casa invalida

        CORRECAO: antes retornava 1 se UMA unica casa fosse digito, ou seja,
        "1ABCDEFG" era considerado valido. Agora basta uma casa errada para
        a funcao retornar 0.
    */
    int estaNoRange = 1;
    int i;

    for(i = 0; i < tamanho; i++)
    {
        if(cep[i] < '0' || cep[i] > '9')
        {
            estaNoRange = 0;
        }
    }
    return estaNoRange;
}

int valida_cep(int tamanho, char *cep)
{
    /*
        Junta as regras do CEP em um unico lugar.
        RETORNO: 1 -> CEP valido   |   0 -> CEP invalido
    */
    int valido = 1;

    if(tamanho != TAM_CEP)
    {
        valido = 0;
    }
    else
    {
        if(verificaCasaCEP(cep, tamanho) == 0)
        {
            valido = 0;
        }
        if(cepSemZero(tamanho, cep) == 1)
        {
            valido = 0;
        }
    }
    return valido;
}


char *processaDado(char *dado)
{
    /*
        Esta funcao processa o dado informado pelo usuario. Pelo tamanho ela
        decide se o que foi digitado e' um CPF (11) ou um CEP (8) e aplica as
        validacoes:
        - CPF: 11 posicoes, somente digitos;
        - CEP: 8 posicoes, todas as casas entre 0 e 9 e prefixo diferente de 000.

        RETORNO: o proprio dado, quando valido;
                 NULL, quando invalido.

        CORRECAO: antes a funcao SEMPRE terminava com "divDado = dado; return
        divDado;", isto e', devolvia um ponteiro valido mesmo quando o dado
        estava errado. Por isso o teste feito na main (if checaCPFeCEP != NULL)
        nunca reprovava nada.
    */
    int tamanho = strlen(dado);
    char *divDado = NULL;

    if(tamanho == TAM_CPF)
    {
        if(valida_cpf(tamanho, dado))
        {
            printf("\nO numero que voce digitou eh um CPF valido.\n");
            divDado = dado;
        }
        else
        {
            printf("\nCPF invalido: deve ter 11 digitos numericos.\n");
        }
    }
    else if(tamanho == TAM_CEP)
    {
        if(ehSoDigito(dado))
        {
            printf("\nCEP invalido: use somente digitos.\n");
        }
        else if(cepSemZero(tamanho, dado))
        {
            printf("\nCEP invalido: o prefixo nao pode ser 000.\n");
        }
        else if(verificaCasaCEP(dado, tamanho) == 0)
        {
            printf("\nCEP invalido: os numeros das casas devem estar entre 0 e 9.\n");
        }
        else
        {
            printf("\nO numero que voce digitou eh um CEP valido.\n");
            divDado = dado;
        }
    }
    else
    {
        printf("\nTamanho invalido: informe 11 digitos (CPF) ou 8 digitos (CEP).\n");
    }

    return divDado;
}


/* ==================== ITEM (C) ====================
   Funcao generica de leitura: le tanto CPF quanto CEP e so aceita digitos.
   O que muda de uma chave para outra e' apenas o parametro "tamanho"
   (11 para CPF, 8 para CEP) e o rotulo mostrado na tela.
   ================================================== */

void limpaBuffer()
{
    /* descarta o que sobrou no buffer do teclado (inclusive o '\n') */
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

void leChaveSoDigitos(char *destino, int tamanho, char *rotulo)
{
    /*
        destino -> vetor onde a chave sera gravada (precisa ter tamanho+1 posicoes)
        tamanho -> quantidade exata de digitos esperada (TAM_CPF ou TAM_CEP)
        rotulo  -> texto exibido na tela ("CPF", "CEP", "CEP da rua"...)

        Logica: le a entrada como texto, elimina os separadores de formatacao
        mais comuns ( . - / e espaco ), recusa qualquer outro caractere que nao
        seja digito e so devolve o controle quando a quantidade de digitos for
        exatamente a esperada. Assim nenhum dado sujo entra na arvore.
    */
    char entrada[64];
    char limpo[64];
    int i, j, valido;

    do
    {
        valido = 1;
        j = 0;

        printf("\nDigite o %s (%d digitos, somente numeros): ", rotulo, tamanho);
        if(scanf("%63s", entrada) != 1)
        {
            limpaBuffer();
            valido = 0;
        }
        else
        {
            limpaBuffer();
            for(i = 0; entrada[i] != '\0'; i++)
            {
                if(entrada[i] >= '0' && entrada[i] <= '9')
                {
                    if(j < 63)
                    {
                        limpo[j] = entrada[i];
                        j++;
                    }
                }
                else if(entrada[i] != '.' && entrada[i] != '-' &&
                        entrada[i] != '/' && entrada[i] != ' ')
                {
                    /* caractere que nao e' digito nem separador conhecido */
                    valido = 0;
                }
            }
            limpo[j] = '\0';

            if(j != tamanho)
            {
                valido = 0;
            }
        }

        if(valido)
        {
            strcpy(destino, limpo);
        }
        else
        {
            printf("\nEntrada invalida! Informe exatamente %d digitos.\n", tamanho);
        }

    }while(valido == 0);
}
