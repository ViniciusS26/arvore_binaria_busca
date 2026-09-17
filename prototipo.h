#ifndef PROTOTIPO_H_INCLUDED
#define PROTOTIPO_H_INCLUDED

/* Tamanhos das chaves (sem contar o '\0') */
#define TAM_CPF 11
#define TAM_CEP 8

struct DadosEndereco
{
    /* Estrutura para guardar dados do endereco */
    char CEPRua[9];
    char Cidade[50];
    char Estado[3];
    char Rua[100];
    char Bairro[50];
    int numero;
};


struct DadosCidade
{
    /* Estrutura de dados para guardar informacoes de uma cidade */
    char CEP[9];
    char NomeCidade[50];
    char Estado[3];
    struct DadosEndereco enderco;
};


struct Dadocliente
{
    char CPF[12];
    char Nome[100];
    int AnoNascimento;
    int Fone;
    char Genero[50];
    struct DadosEndereco Enderco;
};

/* Usamos o union para armazenar dados de cliente ou cidade */
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


/* ==================== PROTOTIPOS ==================== */


int ehSoDigito(char *dado);
int valida_cpf(int tamanho, char *cpf);
int valida_cep(int tamanho, char *cep);
int cepSemZero(int tamanho, char *cep);
int verificaCasaCEP(char *cep, int tamanho);
char *processaDado(char *dado);

void limpaBuffer();
void leChaveSoDigitos(char *destino, int tamanho, char *rotulo);

struct DadosEndereco enderecoVazio();
struct Dadocliente lerDadoCliente();
struct DadosCidade lerDadoCidade();
struct DadosEndereco lerDadoEndereco();

ArvoreBinaria *CriaArvore();
char *pegaChave(ArvoreBinaria *no);
ArvoreBinaria *criaNoCliente(struct Dadocliente cliente);
ArvoreBinaria *criaNoCidade(struct DadosCidade cidade);

int inserirArvore(ArvoreBinaria **raiz, ArvoreBinaria *NovoNo);
ArvoreBinaria *buscar(ArvoreBinaria *raiz, char *chave);
ArvoreBinaria *buscarComPassos(ArvoreBinaria *raiz, char *chave, int *passos);

int ehFolha(ArvoreBinaria *raiz);
ArvoreBinaria *temSoUmFilho(ArvoreBinaria *raiz);
int removerNo(ArvoreBinaria **raiz, char *chave);

int alturaArvore(ArvoreBinaria *raiz);
int contaNos(ArvoreBinaria *raiz);

void imprimirDadoEndereco(struct DadosEndereco info);
void imprimirDadoCliente(struct Dadocliente info);
void imprimirDadoCidade(struct DadosCidade info);
void imprimirArvore(ArvoreBinaria *raiz);
void liberaArvore(ArvoreBinaria *raiz);

/* experimento do item (f)*/
void testeDesempenhoCPF();

#endif // PROTOTIPO_H_INCLUDED
