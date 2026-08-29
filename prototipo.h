#ifndef PROTOTIPO_H_INCLUDED
#define PROTOTIPO_H_INCLUDED

struct DadosEndereco
{
    /*Estrutura para guardar dados do endereco */
    char CEPRua[9];
    char Cidade[50];
    char Estado[3];
    char Rua[100];
    char Bairro[50];
    int numero;
};


struct DadosCidade
{
    /*Estrura de Dados para guardar informações de uma cidade*/
    char CEP[9];
    char NomeCidade[50];
    char Estado[3];
    struct DadosEndereco enderco;

};

/*typedef struct ArvoreCidade
{
    struct DadosCidade Cidades;
    struct ArvoreCidade *esquerda, *direita;
} ArvoreCidade;
*/

struct Dadocliente
{
    char CPF [12];
    char Nome[100];
    int AnoNascimento;
    int Fone;
    char Genero[50];
    struct DadosEndereco Enderco;
};


/*typedef struct AvoreClientes
{
    struct Dadocliente Cliente;
    struct ArvoreClientes *esquerda, *direita;
};*/

typedef enum {TIPO_CLIENTE, TIPO_CIDADE} TipoDoNo;

union ContemCidadeECliente{
    struct Dadocliente cliente;
    struct DadosCidade cidade;
};

typedef struct ArvoreBinaria{
    TipoDoNo tipoN;
    union ContemCidadeECliente info;
    struct ArvoreBinaria *esquerda, *direita;
}ArvoreBinaria;


/*Criar prototipo de funções*/

int valida_cpf(int tamanho, char *cpf);
int  cepSemZero(int tamanho, char *cep);
int verificaCasaCEP(char *cep, int tamanho);
int ehSoDigito(char *dado);
char *processaDado( char *dado);


struct Dadocliente lerDadoCliente();
struct DadosCidade  lerDadoCidade();
struct DadosEndereco lerDadoEndereco();

void extraiChaveEAlocaNo(ArvoreBinaria *raiz, ArvoreBinaria *NovoNo, char *chaveAtual, char *chaveNova);
ArvoreBinaria *criaNoCliente(struct Dadocliente cliente);
ArvoreBinaria *criaNoCidade(struct DadosCidade cidade);

int inserirArvore(ArvoreBinaria **raiz, ArvoreBinaria *NovoNo );
ArvoreBinaria *buscar(ArvoreBinaria *raiz, char chave[50]);
int removerNo(ArvoreBinaria **raiz, char chave[50]);

void imprimirDadoEndereco(struct DadosEndereco info);
void imprimirDadoCliente(struct Dadocliente info);
void imprimirDadoCidade(struct DadosCidade info);
void imprimirArvore(ArvoreBinaria *raiz);
void liberaNo(ArvoreBinaria *raiz);

#endif  PROTOTIPO_H_INCLUDED

