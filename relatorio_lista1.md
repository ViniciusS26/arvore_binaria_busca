# Lista 1 — Unidade I — Árvore Binária de Busca
**Disciplina:** Estruturas de Dados II — UFPI/CSHNB
**Arquivos do código:** `prototipo.h`, `valida_dados.c`, `respostas.c`, `teste_cpf.c`, `main.c`

---

## (a) Desenho das estruturas de dados e as ligações entre elas

```
   struct DadosEndereco                 struct DadosCidade
  +----------------------+            +--------------------------+
  | CEPRua  [9]          |            | CEP        [9]  <= CHAVE |
  | Cidade  [50]         |            | NomeCidade [50]          |
  | Estado  [3]          |            | Estado     [3]           |
  | Rua     [100]        |<-----------| enderco : DadosEndereco  |  (composicao)
  | Bairro  [50]         |    (1)     +--------------------------+
  | numero  int          |
  +----------------------+            struct Dadocliente
             ^                       +---------------------------+
             |                       | CPF     [12]    <= CHAVE  |
             +-----------------------| Nome    [100]             |
                      (1)            | AnoNascimento int         |
                                     | Fone    int               |
                                     | Genero  [50]              |
                                     | Enderco : DadosEndereco   |  (composicao)
                                     +---------------------------+
```

**Ligação (1) — composição, não ponteiro.** `DadosEndereco` está *dentro* de `Dadocliente` e de `DadosCidade` como campo, e não como ponteiro. A motivação é que um endereço não tem existência independente do dono: ele nasce e morre com o cliente (ou com a cidade). Guardando por valor, não existe risco de ponteiro pendurado, não é preciso um `malloc`/`free` extra por endereço, e a cópia do registro (que acontece na remoção com dois filhos, por exemplo) já carrega o endereço junto.

**Nó genérico da árvore (item d aplicado ao desenho):**

```
                    ArvoreBinaria (no generico)
                 +---------------------------------+
                 | tipoN : TIPO_CLIENTE|TIPO_CIDADE|  <- etiqueta (enum)
                 | info  : union                   |
                 |    +--------------------------+ |
                 |    | Dadocliente cliente      | |  <- os dois ocupam
                 |    |        OU                | |     o MESMO espaco
                 |    | DadosCidade cidade       | |
                 |    +--------------------------+ |
                 | esquerda -----+   direita ---+  |
                 +---------------|--------------|--+
                                 v              v
                         ArvoreBinaria   ArvoreBinaria
```

Duas árvores independentes usam esse mesmo nó:

```
   arvoreCliente (chave = CPF)            arvoreCidade (chave = CEP)
            [50000000000]                        [64600000]
             /         \                          /       \
     [30000000000]  [70000000000]          [64000000]  [64800000]
```

**Motivação do desenho:** a `union` garante que um nó só gasta memória para um dos registros (o maior dos dois), e o `enum tipoN` diz, em tempo de execução, qual dos dois campos está válido — sem ele a `union` seria insegura, porque C não guarda essa informação sozinho. Os ponteiros `esquerda`/`direita` são o que dá a estrutura de árvore; tudo o que as funções de inserir/buscar/remover enxergam é *chave + ponteiros*, e por isso elas servem para as duas árvores.

---

## (b) Fluxograma: cadastrar o endereço de um cliente

```
                     ( INICIO )
                          |
                          v
          +---------------------------------+
          | Ler CPF do cliente              |
          | (leChaveSoDigitos, 11 digitos)  |
          +---------------------------------+
                          |
                          v
          +---------------------------------+
          | no = buscar(arvoreCliente, CPF) |
          +---------------------------------+
                          |
                          v
                   /-------------\
                  /  no != NULL ? \
                  \               /
                   \-------------/
                   NAO |      | SIM
                       |      |
      +----------------+      v
      |            +-------------------------------+
      |            | Ler dados do endereco:        |
      |            | CEP, cidade, estado, rua,     |
      |            | bairro, numero                |
      |            | (lerDadoEndereco)             |
      |            +-------------------------------+
      |                          |
      |                          v
      |                   /-------------\
      |                  /  CEP valido ? \
      |                  \               /
      |                   \-------------/
      |                   NAO |      | SIM
      |                       |      v
      |                       |  +-----------------------------------+
      |                       |  | no->info.cliente.Enderco = novo   |
      |                       |  +-----------------------------------+
      |                       |      |
      v                       v      v
  +----------------+   +--------------+   +-----------------------------+
  | "Cliente nao   |   | "CEP         |   | "Endereco adicionado com    |
  |  encontrado!"  |   |  invalido!"  |   |  sucesso!"                  |
  +----------------+   +--------------+   +-----------------------------+
                          |   |   |
                          v   v   v
                        (  FIM  )
```

**Observação sobre a estrutura de dados:** como o endereço está *dentro* do nó do cliente, o cadastro não cria nó novo nem realoca nada — a busca devolve o ponteiro para o nó e a atribuição `encontrado->info.cliente.Enderco = novoEndereco` grava o registro no lugar. É exatamente o que a opção **1-c** da `main` faz.

---

## (c) Função genérica para ler CPF e CEP aceitando apenas dígitos

Implementada em `valida_dados.c`:

```c
void leChaveSoDigitos(char *destino, int tamanho, char *rotulo);
```

- `leChaveSoDigitos(dado.CPF, TAM_CPF, "CPF")` → lê 11 dígitos
- `leChaveSoDigitos(dado.CEP, TAM_CEP, "CEP da cidade")` → lê 8 dígitos

**Escolhas de lógica:**

1. **O que muda entre CPF e CEP é só um parâmetro.** Em vez de duas funções quase iguais, o tamanho esperado (`TAM_CPF = 11`, `TAM_CEP = 8`) e o rótulo mostrado na tela viram argumentos. Uma única função atende os três pontos de leitura (CPF do cliente, CEP da cidade, CEP da rua).
2. **Ler como texto, não como número.** CPF e CEP não são quantidades: não se somam, podem começar com zero e `int` estoura com 11 dígitos. Lendo com `scanf("%63s", ...)` em um buffer folgado, nenhum zero à esquerda se perde e não há overflow.
3. **Filtrar em vez de rejeitar formatação.** Os separadores usuais (`.`, `-`, `/`, espaço) são descartados, de modo que `111.222.333-44` e `11122233344` produzem a mesma chave. Qualquer outro caractere fora de `'0'..'9'` invalida a entrada.
4. **Laço até a entrada ficar correta.** A função só retorna quando a quantidade de dígitos é exatamente a esperada, então nenhuma chave malformada chega à árvore — o que importa porque a comparação de chaves é feita com `strcmp`.
5. **`limpaBuffer()` após cada leitura.** Evita que o `'\n'` restante seja consumido pelo próximo `scanf`, que era uma fonte de leituras "puladas" no código original.

**Funções usadas:** `scanf` (leitura em string), `strlen`/`strcpy` (`string.h`), `getchar` (limpeza de buffer) e a comparação direta de `char` para testar a faixa de dígitos — mais barata que chamar `isdigit()` e sem depender de locale.

A validação **semântica** ficou separada da leitura, em `processaDado()`: pelo tamanho (11 ou 8) ela decide se é CPF ou CEP e aplica `valida_cpf()` ou as regras de CEP (`verificaCasaCEP`, `cepSemZero`), devolvendo `NULL` quando o dado é inválido.

---

## (d) O que precisou ser acrescentado/modificado nas estruturas

Nas estruturas originais do enunciado havia **dois tipos de nó** (`arbbCl` e `arbbCi`), cada um com um `Info` de tipo diferente. Como C é estaticamente tipado, uma função `inserir(struct arbbCl**, ...)` nunca aceitaria um `struct arbbCi*` — por isso seriam necessárias duas versões de cada função.

**Acrescentar:**

| Estrutura nova | Papel |
|---|---|
| `typedef enum {TIPO_CLIENTE, TIPO_CIDADE} TipoDoNo;` | etiqueta que registra o que o nó guarda |
| `union ContemCidadeECliente { struct Dadocliente cliente; struct DadosCidade cidade; };` | permite que um mesmo campo guarde cliente **ou** cidade |
| `typedef struct ArvoreBinaria { TipoDoNo tipoN; union ContemCidadeECliente info; struct ArvoreBinaria *esquerda, *direita; } ArvoreBinaria;` | nó único da árvore |

**Modificar/remover:** as duas estruturas de nó originais (`arbbCl` e `arbbCi`) deixam de existir e são substituídas por `ArvoreBinaria`. `struct cliente` e `struct cidades` permanecem como estão — elas só passam a morar dentro da `union`. (No `.h` original havia um erro clássico nessas estruturas: `struct arbbCi` declarava os ponteiros como `struct arvbbCi *Esq, *Dir` — nome trocado —, o que criaria um tipo incompleto diferente do pretendido.)

**Função-chave que fecha a generalização:**

```c
char *pegaChave(ArvoreBinaria *no);   /* CPF se cliente, CEP se cidade */
```

É ela que isola a única diferença que resta entre as duas árvores. Todo o resto do algoritmo trabalha sobre `strcmp(pegaChave(a), pegaChave(b))`.

**Por que `union` e não duas árvores?** Um `struct` com os dois registros gastaria a soma dos tamanhos em todo nó; a `union` gasta o maior dos dois. O custo é ter de manter o `tipoN` coerente — se alguém gravar em `info.cliente` e ler `info.cidade`, o resultado é lixo. Por isso `tipoN` é atribuído em `criaNoCliente`/`criaNoCidade` e copiado junto com `info` na remoção.

---

## (e) Principais funções utilizadas

As funções abaixo são implementadas principalmente em `respostas.c` e trabalham com as árvores de clientes e de cidades:

- `ArvoreBinaria *CriaArvore()` — não recebe parâmetros e retorna uma árvore vazia (`NULL`), usada para inicializar a raiz.
- `ArvoreBinaria *criaNoCliente(struct Dadocliente cliente)` — recebe os dados de um cliente, cria e inicializa um nó do tipo cliente; retorna o endereço do novo nó ou `NULL` se não houver memória disponível.
- `ArvoreBinaria *criaNoCidade(struct DadosCidade cidade)` — recebe os dados de uma cidade, cria e inicializa um nó do tipo cidade; retorna o endereço do novo nó ou `NULL` se não houver memória disponível.
- `char *pegaChave(ArvoreBinaria *no)` — recebe um nó e retorna sua chave: o CPF, quando é um cliente, ou o CEP, quando é uma cidade.
- `int inserirArvore(ArvoreBinaria **raiz, ArvoreBinaria *NovoNo)` — recebe o endereço da raiz e o nó a ser inserido. Compara as chaves e insere recursivamente na posição correta da árvore; retorna `1` quando insere e `0` quando a chave já existe ou o nó é inválido.
- `ArvoreBinaria *buscar(ArvoreBinaria *raiz, char *chave)` — recebe a raiz e uma chave, percorre a árvore comparando os valores e retorna o endereço do nó encontrado ou `NULL` caso a chave não exista.
- `ArvoreBinaria *buscarComPassos(ArvoreBinaria *raiz, char *chave, int *passos)` — realiza a mesma busca, atualizando a quantidade de nós visitados; retorna o nó encontrado ou `NULL`. É utilizada no experimento de desempenho.
- `int removerNo(ArvoreBinaria **raiz, char *chave)` — recebe o endereço da raiz e a chave do registro. Remove o nó tratando os casos de folha, um filho ou dois filhos; retorna `1` quando remove e `0` quando a chave não é encontrada.
- `int alturaArvore(ArvoreBinaria *raiz)` — recebe a raiz e retorna a altura da árvore, calculada a partir das subárvores esquerda e direita.
- `int contaNos(ArvoreBinaria *raiz)` — recebe a raiz e retorna a quantidade total de nós da árvore.
- `void imprimirArvore(ArvoreBinaria *raiz)` — recebe a raiz e percorre a árvore para exibir os registros armazenados.
- `void liberaArvore(ArvoreBinaria *raiz)` — recebe a raiz e libera recursivamente a memória de todos os nós da árvore.

As funções de validação e leitura também apoiam o programa, como `leChaveSoDigitos(char *destino, int tamanho, char *rotulo)`, que lê CPF ou CEP, e `processaDado(char *dado)`, que valida o dado informado.

No `main.c`, as **mesmas** funções são chamadas para `arvoreCliente` e `arvoreCidade`:

```c
inserirArvore(&arvoreCliente, criaNoCliente(dadocli));
inserirArvore(&arvoreCidade,  criaNoCidade(dadocid));
buscar(arvoreCliente, cpf);        buscar(arvoreCidade, cep);
removerNo(&arvoreCliente, cpf);    removerNo(&arvoreCidade, cep);
imprimirArvore(arvoreCliente);     imprimirArvore(arvoreCidade);
```

**Funcionou?** Sim. O motivo é que todo o algoritmo de árvore binária de busca depende de apenas duas coisas: uma **chave comparável** e os **ponteiros dos filhos**. Ambas ficam no nó genérico, e a obtenção da chave foi empurrada para `pegaChave()`. Como CPF e CEP são strings de dígitos de tamanho fixo, `strcmp` dá a mesma ordem que a ordem numérica — não seria verdade se as chaves tivessem comprimentos variáveis (aí `"9"` viria depois de `"10"`).

**Isso facilita o desenvolvimento?** Facilita, com ressalvas:

- *A favor:* um único código de inserção/busca/remoção para manter e testar — um bug corrigido vale para as duas árvores; acrescentar um terceiro tipo (fornecedor, produto) custa só um valor novo no `enum`, um campo na `union` e um `case` em `pegaChave`; menos código duplicado significa menos chance de as duas versões divergirem com o tempo.
- *Contra:* perde-se verificação de tipo em tempo de compilação — nada impede, para o compilador, inserir um nó de cidade na árvore de clientes; o programa aceitaria e a árvore ficaria ordenada por chaves de naturezas diferentes. A responsabilidade passa a ser do programador (e do `tipoN`). Além disso, todo nó ocupa `sizeof` do maior registro, mesmo guardando o menor.

Em C, o passo seguinte dessa mesma ideia seria a genericidade por `void*` + ponteiro de função de comparação (como faz o `qsort`), que remove até a dependência do `enum` — ao custo de menos legibilidade.

---

## (f) Experimento com 100 CPFs

Implementado em `teste_cpf.c`, acessível pela **opção 5** do menu. O programa gera 100 CPFs distintos com dígitos verificadores válidos, ordena o vetor, monta as quatro árvores com os **mesmos** 100 CPFs e busca os **mesmos** 10 CPFs (índices 0, 10, 20, …, 90 do vetor ordenado) em todos os casos, contando os nós percorridos. Por fim busca um CPF não cadastrado.

### Resultado de uma execução

| CPF buscado | Caso 1 crescente | Caso 2 decrescente | Caso 3 meio primeiro | Caso 4 aleatório |
|---|---|---|---|---|
| 01286898625 | 1 | 100 | 4 | 10 |
| 15680926873 | 11 | 90 | 8 | 5 |
| 25491598100 | 21 | 80 | 8 | 10 |
| 31668836904 | 31 | 70 | 8 | 11 |
| 38584239391 | 41 | 60 | 9 | 8 |
| 49693661109 | 51 | 50 | 1 | 9 |
| 57134192407 | 61 | 40 | 6 | 6 |
| 68638383622 | 71 | 30 | 9 | 7 |
| 77715130508 | 81 | 20 | 9 | 5 |
| 90747286698 | 91 | 10 | 5 | 5 |
| **Média** | **46,0** | **55,0** | **6,7** | **7,6** |
| **Altura da árvore** | **100** | **100** | **12** | **13** |
| **CPF não cadastrado** | **16** | **86** | **7** | **10** |

Repetindo a execução, os casos 1 e 2 dão sempre 46,0 e 55,0 (são determinísticos); os casos 3 e 4 oscilaram entre 6,4 e 7,7 passos, com altura entre 10 e 15.

### Análise

**Casos 1 e 2 (entrada ordenada) — o pior caso.** Inserindo em ordem crescente, cada CPF novo é maior que todos os anteriores e desce sempre pela direita: a árvore vira uma **lista encadeada** de altura 100. Em ordem decrescente acontece o espelho, degenerando à esquerda. A busca deixa de ser binária e passa a ser sequencial, **O(n)**: o k-ésimo menor CPF custa exatamente k passos no caso crescente e 101−k no decrescente — o que explica as colunas serem progressões aritméticas perfeitas (1, 11, 21, … e 100, 90, 80, …) e as médias somarem 46 + 55 ≈ 101. É o resultado mais importante do experimento: **uma ABB sem balanceamento não dá nenhuma garantia de desempenho; a ordem de inserção decide tudo.**

**Casos 3 e 4 (ordem embaralhada) — comportamento logarítmico.** Com inserções em ordem aleatória, a altura esperada de uma ABB é Θ(log n) (≈ 4,3·log₂n na média teórica), e foi o que apareceu: alturas de 10 a 15 contra o mínimo teórico de ⌈log₂101⌉ = 7, e médias de menos de 8 passos. **A busca ficou cerca de 6 a 8 vezes mais rápida que nos casos ordenados**, pois cada comparação descarta aproximadamente metade dos registros restantes.

**Caso 3 × caso 4.** Inserir o CPF mediano primeiro garante que a raiz divide o conjunto em duas metades de 50 elementos, eliminando o risco de uma raiz muito desbalanceada. Como o restante entra aleatoriamente, o ganho sobre o caso 4 é pequeno e nem sempre aparece — o caso 4 já costuma produzir uma raiz razoável por acaso. Se a regra da mediana fosse aplicada **recursivamente** (mediana da metade esquerda, mediana da metade direita, e assim por diante), a árvore ficaria perfeitamente balanceada com altura 7 e nenhuma busca passaria de 7 passos: é o melhor caso possível.

**Busca por CPF não cadastrado.** Termina quando se chega a uma subárvore vazia, então o custo é o comprimento do caminho até a folha correspondente — limitado pela **altura**. Nos casos ordenados isso chegou a 86 nós percorridos (e pode chegar a 100, quando o CPF procurado é maior que todos); nos casos embaralhados ficou em 7 a 10. Note que a busca malsucedida costuma ser mais cara que a bem-sucedida, porque não tem como parar no meio do caminho.

**Conclusão.** Dados reais raramente chegam embaralhados — cadastros são frequentemente importados em ordem de CPF, de matrícula ou de data —, que é justamente o padrão que degenera a ABB. Para uso em produção, ou se aplica alguma estratégia de inserção que evite a ordenação (como a mediana recursiva, quando o conjunto é conhecido de antemão), ou se usa uma árvore auto-balanceada (AVL, rubro-negra), que mantém a altura em O(log n) independentemente da ordem de entrada.

---
