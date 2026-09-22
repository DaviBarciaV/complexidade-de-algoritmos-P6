# Complexidade de Algoritmos — Projeto AV01 (P6)

Programa em C com cinco funções analisadas quanto à complexidade de tempo. Para cada função, o programa monta os dados de entrada (manualmente ou com valores aleatórios), mostra os arranjos na tela, executa a função e informa o resultado e o tempo real de execução.

## Integrantes

1. Davi Barcia
2. Daniel Rozendo
3. Slann Antonio
4. Renan Marinho
5. Ismael Victor

## Arquivos

| Arquivo | Conteúdo |
|---|---|
| `projeto_av01.c` | Código-fonte do programa |
| `projeto_av01.exe` | Executável para Windows |
| `pseudocodigos_e_complexidade (1).txt` | Pseudocódigo, contagem de instruções linha a linha, expressão de complexidade, Big O e cálculo de tempo de cada função |
| `Projeto AV01 - complexidade.pdf` / `Projeto AV01.docx` | Enunciado e documentação do projeto |

## Como compilar e executar

É preciso ter o `gcc` instalado (no Windows, via MinGW).

```bash
gcc -Wall -Wextra -o projeto_av01 projeto_av01.c
```

No Windows:

```bash
projeto_av01.exe
```

No Linux/macOS:

```bash
./projeto_av01
```

## Menu principal

```
MENU PRINCIPAL
1 - Funcao 1: Contagem de Ocorrencias Distintas
2 - Funcao 2: Analise de Pares em Matriz Triangular
3 - Funcao 3: Comparacao de Arranjos Tridimensionais
4 - Funcao 4: Analise de Casos Assimetricos no Condicional
5 - Funcao 5: Contagem de Elementos em Vetor Ordenado
6 - Criar arranjo dinamico (linhas x colunas) aleatorio
7 - Mostrar os integrantes do grupo
0 - Sair
```

Ao escolher uma opção, a tela é limpa e só a saída daquela opção aparece. No final, o programa mostra "Pressione ENTER para voltar ao menu..."; depois do ENTER, a tela é limpa de novo e o menu volta.

Em cada uma das cinco funções, o programa:

1. pede as dimensões da entrada;
2. pergunta se o preenchimento será **manual** (digitando os valores) ou **automático** (valores aleatórios);
3. imprime os arranjos criados, para conferir o resultado (acima de 200 elementos, pergunta antes se deve imprimir);
4. executa a função e mostra o resultado e o tempo real medido com `clock()`.

## As cinco funções

### Função 1 — Contagem de Ocorrências Distintas

`contarOcorrencias(V, n, B, k)`: para cada um dos `k` elementos de `B`, conta quantas vezes ele aparece no vetor `V` de `n` elementos e retorna a soma dessas contagens.

- f(n, k) = 3kn + 4k + 3
- **O(n · k)**

### Função 2 — Análise de Pares em Matriz Triangular

`analisarPares(A, n)`: percorre a diagonal principal e a metade superior de uma matriz `n × n` e conta quantos pares `A[i][j] + A[j][i]` são múltiplos de 5. O laço interno começa em `j = i`, então o total de repetições é a soma n + (n−1) + … + 1 = n(n+1)/2.

- f(n) = 1,5n² + 3,5n + 3
- **O(n²)**

### Função 3 — Comparação de Arranjos Tridimensionais

`compararArranjos(A, B, n)`: soma todos os elementos de dois arranjos `n × n × n` e retorna 1 se soma(A) ≥ soma(B), ou 0 caso contrário. Os dois blocos de laços são sequenciais, então os custos se somam.

- f(n) = 4n³ + 4n² + 4n + 6
- **O(n³)**

### Função 4 — Análise de Casos Assimétricos no Condicional

`processar_vetor(V, n)`: se o elemento é **par**, soma o próprio valor (custo constante); se é **ímpar**, soma o fatorial do valor (custo proporcional ao valor, via `fatorial(x)`, que é O(x)). No pior caso, todos os elementos são ímpares; tomando o maior valor `m` da mesma ordem de `n`:

- f(n, m) = 2mn + 4n + 3 → com m = n: f(n) = 2n² + 4n + 3
- **O(n²)**

No modo aleatório os valores ficam entre 0 e 20, porque `unsigned long long` só comporta até 20!.

### Função 5 — Contagem de Elementos Presentes em Vetor Ordenado

`contarPresentes(A, B, n)`: para cada elemento do vetor `A`, faz uma busca binária no vetor ordenado `B` (`buscaBinaria`, O(log n)) e retorna quantos elementos de `A` existem em `B`. O programa ordena `B` antes de executar, como preparação dos dados de teste; essa ordenação não entra na análise.

- f(n) = 5n · log₂(n) + 6n + 3
- **O(n log n)**

## Resumo da análise

Tempos teóricos para um computador que executa 10⁸ instruções por segundo:

| Função | Expressão de complexidade | Big O | Entrada | Tempo |
|:---:|---|---|---|---|
| 1 | 3kn + 4k + 3 | O(n · k) | n = 50.000, k = 4.000 | 6,00016 s |
| 2 | 1,5n² + 3,5n + 3 | O(n²) | n = 500 | 0,0037675 s |
| 3 | 4n³ + 4n² + 4n + 6 | O(n³) | n = 300 | 1,08361 s |
| 4 | 2n² + 4n + 3 (pior caso) | O(n²) | n = 50.000 | 50,002 s |
| 5 | 5n · log₂(n) + 6n + 3 | O(n log n) | n = 10.000.000 | 12,2267 s |

A contagem linha a linha de cada função está em `pseudocodigos_e_complexidade (1).txt`.

## Observações

- **Alocação dinâmica:** todos os arranjos são criados com `malloc()` e liberados com `free()`, então o programa aceita qualquer dimensão, inclusive os tamanhos do cálculo de tempo. Não foram usadas VLAs porque elas ficam na pilha (cerca de 1 MB no Windows e 8 MB no Linux), e os tamanhos pedidos não caberiam: na Função 3 com n = 300, cada arranjo ocupa 108 MB.
- **Criação de arranjo aleatório (opção 6):** `criarEPreencherArranjoAleatorio()` pergunta o número de linhas e de colunas, cria uma matriz dinâmica e a preenche com valores aleatórios. A Função 2 também a usa no preenchimento automático e pede os valores de novo se linhas ≠ colunas.
- **Memória:** a Função 3 com n = 300 usa cerca de 216 MB (dois arranjos).
- **Tempo medido:** o tempo real usa `clock()`, que no Windows tem resolução de cerca de 1 ms. Com entradas pequenas a função termina antes disso e o tempo aparece como `0.000000 s`. Com os tamanhos do cálculo de tempo, o valor medido aparece normalmente.
