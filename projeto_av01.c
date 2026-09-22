/* 
INTEGRANTES DO GRUPO:
1) Davi Barcia
2) Daniel Rozendo
3) Slann Antonio
4) Renan Marinho
5) Ismael Victor

Observacao: todas as alocacoes sao dinamicas (malloc/free), portanto o
programa funciona para qualquer dimensao informada pelo usuario, inclusive
as dimensoes pedidas no "Calculo de Tempo" de cada questao.
*/

/* Faz o mingw aceitar os formatos %lld / %llu*/
#define __USE_MINGW_ANSI_STDIO 1

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Quantidade maxima de elementos impressos sem pedir confirmacao ao usuario */
#define MAX_IMPRESSAO 200

/*
 SECAO 1 - FUNCOES AUXILIARES (NAO AVALIADAS / SEM ANALISE DE COMPLEXIDADE)
 */

/* Limpa a tela do terminal (cls no Windows, clear no Linux/macOS) */
void limparTela(void)
{
    fflush(stdout); /* garante que nada pendente seja impresso depois da limpeza */
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/* Espera o usuario pressionar ENTER antes de voltar ao menu */
void pausar(void)
{
    int c;

    printf("\nPressione ENTER para voltar ao menu...");
    fflush(stdout);
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

/* Le um inteiro garantindo que ele seja >= minimo */
int lerInteiro(const char *mensagem, int minimo)
{
    int valor;
    int ok;

    do {
        printf("%s", mensagem);
        ok = scanf("%d", &valor);
        while (getchar() != '\n' && !feof(stdin)) { /* limpa o buffer */ }
        if (ok != 1 || valor < minimo) {
            printf("Valor invalido. Informe um inteiro maior ou igual a %d.\n", minimo);
            ok = 0;
        }
    } while (ok != 1);

    return valor;
}

/* Pergunta como o arranjo deve ser preenchido: 1 = manual, 2 = aleatorio */
int escolherPreenchimento(void)
{
    int opcao;

    printf("\nPreenchimento do arranjo\n");
    printf("1 - Preencher MANUALMENTE (digitando os valores)\n");
    printf("2 - Preencher AUTOMATICAMENTE com valores aleatorios\n");
    opcao = lerInteiro("Escolha: ", 1);
    while (opcao != 1 && opcao != 2) {
        printf("Opcao invalida.\n");
        opcao = lerInteiro("Escolha: ", 1);
    }

    return opcao;
}

/* Decide se o arranjo deve mesmo ser impresso (arranjos enormes travam a tela) */
int confirmarImpressao(long long totalElementos)
{
    int opcao;

    if (totalElementos <= MAX_IMPRESSAO)
        return 1;

    printf("\nO arranjo possui %lld elementos (muito grande para a tela).\n", totalElementos);
    printf("1 - Imprimir mesmo assim\n");
    printf("0 - Nao imprimir\n");
    opcao = lerInteiro("Escolha: ", 0);

    return (opcao == 1);
}

/* VETORES */

int *criarVetor(int n)
{
    int *V = (int *) malloc((size_t) n * sizeof(int));
    if (V == NULL) {
        printf("\nERRO: memoria insuficiente para um vetor de %d elementos.\n", n);
        exit(1);
    }
    return V;
}

/* modo: 1 = manual, 2 = aleatorio (valores de 0 ate limite-1) */
void preencherVetor(int *V, int n, int modo, int limite, const char *nome)
{
    int i;

    if (modo == 1) {
        printf("\nDigite os %d valores do vetor %s:\n", n, nome);
        for (i = 0; i < n; i++) {
            printf("  %s[%d] = ", nome, i);
            if (scanf("%d", &V[i]) != 1) {
                V[i] = 0;
                while (getchar() != '\n' && !feof(stdin)) { }
            }
        }
        while (getchar() != '\n' && !feof(stdin)) { }
    } else {
        for (i = 0; i < n; i++)
            V[i] = rand() % limite;
    }
}

void imprimirVetor(const char *nome, int *V, int n)
{
    int i;

    printf("\nVetor %s (%d elementos):\n", nome, n);
    if (!confirmarImpressao((long long) n))
        return;

    printf("[ ");
    for (i = 0; i < n; i++) {
        printf("%d", V[i]);
        if (i < n - 1) printf(", ");
        if ((i + 1) % 20 == 0 && i < n - 1) printf("\n  ");
    }
    printf(" ]\n");
}

/* Comparador e ordenacao usados apenas para preparar o vetor B da Funcao 5 */
int comparaInt(const void *a, const void *b)
{
    int x = *(const int *) a;
    int y = *(const int *) b;
    return (x > y) - (x < y);
}

void ordenarVetor(int *V, int n)
{
    qsort(V, (size_t) n, sizeof(int), comparaInt);
}

/* MATRIZES */

/* Matriz dinamica (ponteiro para ponteiros) com "linhas" x "colunas" */
int **criarMatriz(int linhas, int colunas)
{
    int i, j;
    int **M = (int **) malloc((size_t) linhas * sizeof(int *));

    if (M == NULL) {
        printf("\nERRO: memoria insuficiente para a matriz.\n");
        exit(1);
    }
    for (i = 0; i < linhas; i++) {
        M[i] = (int *) malloc((size_t) colunas * sizeof(int));
        if (M[i] == NULL) {
            printf("\nERRO: memoria insuficiente na linha %d da matriz.\n", i);
            for (j = 0; j < i; j++) free(M[j]);
            free(M);
            exit(1);
        }
    }
    return M;
}

void liberarMatriz(int **M, int linhas)
{
    int i;
    for (i = 0; i < linhas; i++)
        free(M[i]);
    free(M);
}

void preencherMatriz(int **M, int linhas, int colunas, int modo, int limite, const char *nome)
{
    int i, j;

    if (modo == 1) {
        printf("\nDigite os %d valores da matriz %s:\n", linhas * colunas, nome);
        for (i = 0; i < linhas; i++) {
            for (j = 0; j < colunas; j++) {
                printf("  %s[%d][%d] = ", nome, i, j);
                if (scanf("%d", &M[i][j]) != 1) {
                    M[i][j] = 0;
                    while (getchar() != '\n' && !feof(stdin)) { }
                }
            }
        }
        while (getchar() != '\n' && !feof(stdin)) { }
    } else {
        for (i = 0; i < linhas; i++)
            for (j = 0; j < colunas; j++)
                M[i][j] = rand() % limite;
    }
}

void imprimirMatriz(const char *nome, int **M, int linhas, int colunas)
{
    int i, j;

    printf("\nMatriz %s (%d x %d):\n", nome, linhas, colunas);
    if (!confirmarImpressao((long long) linhas * colunas))
        return;

    for (i = 0; i < linhas; i++) {
        printf("  ");
        for (j = 0; j < colunas; j++)
            printf("%6d", M[i][j]);
        printf("\n");
    }
}

/* ARRANJOS TRIDIMENSIONAIS */

int ***criarCubo(int n)
{
    int i, j, k;
    int ***C = (int ***) malloc((size_t) n * sizeof(int **));

    if (C == NULL) {
        printf("\nERRO: memoria insuficiente para o arranjo 3D.\n");
        exit(1);
    }
    for (i = 0; i < n; i++) {
        C[i] = (int **) malloc((size_t) n * sizeof(int *));
        if (C[i] == NULL) {
            printf("\nERRO: memoria insuficiente no arranjo 3D.\n");
            exit(1);
        }
        for (j = 0; j < n; j++) {
            C[i][j] = (int *) malloc((size_t) n * sizeof(int));
            if (C[i][j] == NULL) {
                printf("\nERRO: memoria insuficiente no arranjo 3D.\n");
                exit(1);
            }
            for (k = 0; k < n; k++)
                C[i][j][k] = 0;
        }
    }
    return C;
}

void liberarCubo(int ***C, int n)
{
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++)
            free(C[i][j]);
        free(C[i]);
    }
    free(C);
}

void preencherCubo(int ***C, int n, int modo, int limite, const char *nome)
{
    int i, j, k;

    if (modo == 1) {
        printf("\nDigite os %d valores do arranjo %s:\n", n * n * n, nome);
        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++)
                for (k = 0; k < n; k++) {
                    printf("  %s[%d][%d][%d] = ", nome, i, j, k);
                    if (scanf("%d", &C[i][j][k]) != 1) {
                        C[i][j][k] = 0;
                        while (getchar() != '\n' && !feof(stdin)) { }
                    }
                }
        while (getchar() != '\n' && !feof(stdin)) { }
    } else {
        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++)
                for (k = 0; k < n; k++)
                    C[i][j][k] = rand() % limite;
    }
}

void imprimirCubo(const char *nome, int ***C, int n)
{
    int i, j, k;

    printf("\nArranjo tridimensional %s (%d x %d x %d):\n", nome, n, n, n);
    if (!confirmarImpressao((long long) n * n * n))
        return;

    for (i = 0; i < n; i++) {
        printf("  Camada %s[%d]:\n", nome, i);
        for (j = 0; j < n; j++) {
            printf("    ");
            for (k = 0; k < n; k++)
                printf("%6d", C[i][j][k]);
            printf("\n");
        }
    }
}

/*
 *  SECAO 2 - AS 5 FUNCOES AVALIADAS (analise de complexidade no arquivo .txt)
 */

/*
 FUNCAO 1 - Contagem de Ocorrencias Distintas
 Percorre a lista de k elementos buscados e, para cada um, conta quantas
 vezes ele aparece no vetor V de n elementos. Retorna a soma das aparicoes.
 Complexidade: f(n,k) = 3kn + 4k + 3  ->  O(n * k)
 */
long long contarOcorrencias(int *V, int n, int *B, int k)
{
    long long soma = 0;                          /* 1 */
    int i, j, cont;

    for (i = 0; i < k; i++) {                    /* 2 */
        cont = 0;                                /* 3 */
        for (j = 0; j < n; j++) {                /* 4 */
            if (V[j] == B[i])                    /* 5 */
                cont = cont + 1;                 /* 6 */
        }
        soma = soma + cont;                      /* 7 */
    }
    return soma;                                 /* 8 */
}

/*
 FUNCAO 2 - Analise de Pares em Matriz Triangular
 Para cada elemento da diagonal principal e da metade SUPERIOR, testa se
 A[i][j] + A[j][i] e multiplo de 5. Retorna o total de ocorrencias.
 Complexidade: f(n) = (3/2)n^2 + (7/2)n + 3  ->  O(n^2)
 */
long long analisarPares(int **A, int n)
{
    long long cont = 0;                          /* 1 */
    int i, j;

    for (i = 0; i < n; i++) {                    /* 2 */
        for (j = i; j < n; j++) {                /* 3 */
            if ((A[i][j] + A[j][i]) % 5 == 0)    /* 4 */
                cont = cont + 1;                 /* 5 */
        }
    }
    return cont;                                 /* 6 */
}

/*
 FUNCAO 3 - Comparacao de Arranjos Tridimensionais
 Soma todos os elementos de A, soma todos os elementos de B e retorna
 1 se soma(A) >= soma(B), ou 0 caso contrario.
 Complexidade: f(n) = 4n^3 + 4n^2 + 4n + 6  ->  O(n^3)
 */
int compararArranjos(int ***A, int ***B, int n)
{
    long long somaA = 0;                         /* 1 */
    long long somaB;
    int i, j, k;

    for (i = 0; i < n; i++)                      /* 2 */
        for (j = 0; j < n; j++)                  /* 3 */
            for (k = 0; k < n; k++)              /* 4 */
                somaA = somaA + A[i][j][k];      /* 5 */

    somaB = 0;                                   /* 6 */
    for (i = 0; i < n; i++)                      /* 7 */
        for (j = 0; j < n; j++)                  /* 8 */
            for (k = 0; k < n; k++)              /* 9 */
                somaB = somaB + B[i][j][k];      /* 10 */

    printf("\n  >> Soma do arranjo A = %lld\n", somaA);
    printf("  >> Soma do arranjo B = %lld\n", somaB);

    if (somaA >= somaB)                          /* 11 */
        return 1;                                /* 12 */
    else
        return 0;                                /* 13 */
}

/*
 FUNCAO AUXILIAR DA FUNCAO 4 - Fatorial iterativo
 Complexidade: f(x) = 2x + 1  ->  O(x)
 */
unsigned long long fatorial(int x)
{
    unsigned long long f = 1;                    /* 1 */
    int i;

    for (i = 2; i <= x; i++)                     /* 2 */
        f = f * (unsigned long long) i;          /* 3 */

    return f;                                    /* 4 */
}

/*
 FUNCAO 4 - Analise de Casos Assimetricos no Condicional
 Elemento PAR   -> soma o proprio valor ao somatorio (custo constante).
 Elemento IMPAR -> soma o fatorial do valor (custo proporcional ao valor).
 Pior caso (todos impares): f(n) = 2mn + 4n + 3, sendo m o maior valor
 armazenado no vetor. Adotando m da mesma ordem de n  ->  O(n^2)
 */
unsigned long long processar_vetor(int *V, int n)
{
    unsigned long long soma = 0;                 /* 1 */
    int i;

    for (i = 0; i < n; i++) {                    /* 2 */
        if (V[i] % 2 == 0)                       /* 3 */
            soma = soma + (unsigned long long) V[i];   /* 4 */
        else
            soma = soma + fatorial(V[i]);        /* 5 */
    }
    return soma;                                 /* 6 */
}

/*
 FUNCAO AUXILIAR DA FUNCAO 5 - Busca Binaria
 Retorna 1 se x existe no vetor ORDENADO B, ou 0 caso contrario.
 Complexidade (pior caso): f(n) = 5*log2(n) + 4  ->  O(log n)
 */
int buscaBinaria(int *B, int n, int x)
{
    int ini = 0;                                 /* 1 */
    int fim = n - 1;                             /* 2 */
    int meio;

    while (ini <= fim) {                         /* 3 */
        meio = (ini + fim) / 2;                  /* 4 */
        if (B[meio] == x)                        /* 5 */
            return 1;                            /* 6 */
        else if (B[meio] < x)                    /* 7 */
            ini = meio + 1;                      /* 8 */
        else
            fim = meio - 1;                      /* 9 */
    }
    return 0;                                    /* 10 */
}

/*
 FUNCAO 5 - Contagem de Elementos Presentes em Vetor Ordenado
 Para cada elemento do vetor A (nao ordenado), faz uma busca binaria no
 vetor B (ordenado). Retorna quantos elementos de A existem em B.
 Complexidade: f(n) = 5n*log2(n) + 6n + 3  ->  O(n log n)
 */
long long contarPresentes(int *A, int *B, int n)
{
    long long total = 0;                          /* 1 */
    int i;

    for (i = 0; i < n; i++)                       /* 2 */
        total = total + buscaBinaria(B, n, A[i]); /* 3 */

    return total;                                 /* 4 */
}

/*
 SECAO 3 - FUNCAO EXIGIDA NO TOPICO "ATENCAO"
 Pergunta quantas linhas e quantas colunas o arranjo deve ter, cria o
 arranjo usando MATRIZ DINAMICA e preenche tudo com valores aleatorios.
 */
int **criarEPreencherArranjoAleatorio(int *linhas, int *colunas)
{
    int **M;

    printf("\nCriacao de arranjo dinamico com valores aleatorios\n");
    *linhas  = lerInteiro("Quantas linhas o arranjo deve ter?  ", 1);
    *colunas = lerInteiro("Quantas colunas o arranjo deve ter? ", 1);

    M = criarMatriz(*linhas, *colunas);
    preencherMatriz(M, *linhas, *colunas, 2, 100, "M");

    return M;
}

/*
 SECAO 4 - ROTINAS DE MENU (montagem dos dados + chamada de cada funcao)
 */

void mostrarIntegrantes(void)
{
    printf("INTEGRANTES DO GRUPO:\n");
    printf("1) Davi Barcia\n");
    printf("2) Daniel Rozendo\n");
    printf("3) Slann Antonio\n");
    printf("4) Renan Marinho\n");
    printf("5) Ismael Victor\n");
}

void executarFuncao1(void)
{
    int n, k, modo;
    int *V, *B;
    long long resultado;
    clock_t t0, t1;

    printf("\nFUNCAO 1 - Contagem de Ocorrencias Distintas\n");
    printf("(Calculo de tempo do projeto: n = 50000 e k = 4000)\n");
    n = lerInteiro("Tamanho n do vetor principal: ", 1);
    k = lerInteiro("Tamanho k do vetor de buscados: ", 1);

    modo = escolherPreenchimento();

    V = criarVetor(n);
    B = criarVetor(k);
    preencherVetor(V, n, modo, 50, "V");
    preencherVetor(B, k, modo, 50, "B");

    imprimirVetor("V (principal)", V, n);
    imprimirVetor("B (buscados)", B, k);

    t0 = clock();
    resultado = contarOcorrencias(V, n, B, k);
    t1 = clock();

    printf("\nRESULTADO: total de aparicoes = %lld\n", resultado);
    printf("Tempo real de execucao: %.6f s\n", (double)(t1 - t0) / CLOCKS_PER_SEC);

    free(V);
    free(B);
}

void executarFuncao2(void)
{
    int n, modo;
    int **A;
    long long resultado;
    clock_t t0, t1;

    printf("\nFUNCAO 2 - Analise de Pares em Matriz Triangular\n");
    printf("(Calculo de tempo do projeto: n = 500)\n");

    modo = escolherPreenchimento();

    if (modo == 2) {
        /* Preenchimento automatico: usa a funcao exigida no topico ATENCAO 1.
           A Funcao 2 exige matriz quadrada, entao linhas deve ser igual a colunas. */
        int linhas, colunas;
        A = criarEPreencherArranjoAleatorio(&linhas, &colunas);
        while (linhas != colunas) {
            printf("A Funcao 2 exige matriz quadrada (linhas = colunas). Tente novamente.\n");
            liberarMatriz(A, linhas);
            A = criarEPreencherArranjoAleatorio(&linhas, &colunas);
        }
        n = linhas;
    } else {
        n = lerInteiro("Ordem n da matriz (n x n): ", 1);
        A = criarMatriz(n, n);
        preencherMatriz(A, n, n, modo, 100, "A");
    }

    imprimirMatriz("A", A, n, n);

    t0 = clock();
    resultado = analisarPares(A, n);
    t1 = clock();

    printf("\nRESULTADO: %lld pares A[i][j] + A[j][i] multiplos de 5\n", resultado);
    printf("Tempo real de execucao: %.6f s\n", (double)(t1 - t0) / CLOCKS_PER_SEC);

    liberarMatriz(A, n);
}

void executarFuncao3(void)
{
    int n, modo, resultado;
    int ***A, ***B;
    clock_t t0, t1;

    printf("\nFUNCAO 3 - Comparacao de Arranjos Tridimensionais\n");
    printf("(Calculo de tempo do projeto: n = 300)\n");
    printf("ATENCAO: n = 300 consome cerca de 216 MB de memoria (dois arranjos).\n");
    n = lerInteiro("Ordem n dos arranjos (n x n x n): ", 1);

    modo = escolherPreenchimento();

    A = criarCubo(n);
    B = criarCubo(n);
    preencherCubo(A, n, modo, 100, "A");
    preencherCubo(B, n, modo, 100, "B");

    imprimirCubo("A", A, n);
    imprimirCubo("B", B, n);

    t0 = clock();
    resultado = compararArranjos(A, B, n);
    t1 = clock();

    printf("\nRESULTADO: %d  (1 = soma de A >= soma de B / 0 = caso contrario)\n", resultado);
    printf("Tempo real de execucao: %.6f s\n", (double)(t1 - t0) / CLOCKS_PER_SEC);

    liberarCubo(A, n);
    liberarCubo(B, n);
}

void executarFuncao4(void)
{
    int n, modo;
    int *V;
    unsigned long long resultado;
    clock_t t0, t1;

    printf("\nFUNCAO 4 - Analise de Casos Assimetricos no Condicional\n");
    printf("(Calculo de tempo do projeto: n = 50000 no pior caso)\n");
    printf("OBS: no modo aleatorio os valores ficam entre 0 e 20 para que o\n");
    printf("     fatorial nao estoure a capacidade do tipo numerico.\n");
    n = lerInteiro("Tamanho n do vetor: ", 1);

    modo = escolherPreenchimento();

    V = criarVetor(n);
    preencherVetor(V, n, modo, 21, "V");

    imprimirVetor("V", V, n);

    t0 = clock();
    resultado = processar_vetor(V, n);
    t1 = clock();

    printf("\nRESULTADO: somatorio = %llu\n", resultado);
    printf("Tempo real de execucao: %.6f s\n", (double)(t1 - t0) / CLOCKS_PER_SEC);

    free(V);
}

void executarFuncao5(void)
{
    int n, modo;
    int *A, *B;
    long long resultado;
    clock_t t0, t1;

    printf("\nFUNCAO 5 - Contagem de Elementos Presentes em Vetor Ordenado\n");
    printf("(Calculo de tempo do projeto: n = 10000000)\n");
    n = lerInteiro("Tamanho n dos vetores A e B: ", 1);

    modo = escolherPreenchimento();

    A = criarVetor(n);
    B = criarVetor(n);
    preencherVetor(A, n, modo, 1000, "A");
    preencherVetor(B, n, modo, 1000, "B");

    /* O vetor B PRECISA estar ordenado para que a busca binaria funcione. */
    ordenarVetor(B, n);
    printf("\nO vetor B foi ordenado automaticamente (pre-requisito da busca binaria).\n");

    imprimirVetor("A (nao ordenado)", A, n);
    imprimirVetor("B (ordenado)", B, n);

    t0 = clock();
    resultado = contarPresentes(A, B, n);
    t1 = clock();

    printf("\nRESULTADO: %lld elementos de A foram encontrados em B\n", resultado);
    printf("Tempo real de execucao: %.6f s\n", (double)(t1 - t0) / CLOCKS_PER_SEC);

    free(A);
    free(B);
}

void executarCriacaoArranjo(void)
{
    int linhas, colunas;
    int **M;

    M = criarEPreencherArranjoAleatorio(&linhas, &colunas);
    imprimirMatriz("M", M, linhas, colunas);
    liberarMatriz(M, linhas);
}

/*
 SECAO 5 - PROGRAMA PRINCIPAL
 */
int main(void)
{
    int opcao;

    srand((unsigned int) time(NULL));

    do {
        limparTela();
        printf("MENU PRINCIPAL\n");
        printf("1 - Funcao 1: Contagem de Ocorrencias Distintas\n");
        printf("2 - Funcao 2: Analise de Pares em Matriz Triangular\n");
        printf("3 - Funcao 3: Comparacao de Arranjos Tridimensionais\n");
        printf("4 - Funcao 4: Analise de Casos Assimetricos no Condicional\n");
        printf("5 - Funcao 5: Contagem de Elementos em Vetor Ordenado\n");
        printf("6 - Criar arranjo dinamico (linhas x colunas) aleatorio\n");
        printf("7 - Mostrar os integrantes do grupo\n");
        printf("0 - Sair\n");
        opcao = lerInteiro("Opcao: ", 0);

        /* Apaga o menu (e a saida anterior) antes de executar a opcao */
        limparTela();

        switch (opcao) {
            case 1: executarFuncao1(); break;
            case 2: executarFuncao2(); break;
            case 3: executarFuncao3(); break;
            case 4: executarFuncao4(); break;
            case 5: executarFuncao5(); break;
            case 6: executarCriacaoArranjo(); break;
            case 7: mostrarIntegrantes(); break;
            case 0: printf("\nPrograma encerrado.\n"); break;
            default: printf("\nOpcao inexistente. Tente novamente.\n"); break;
        }

        /* Mantem a saida na tela ate o usuario pressionar ENTER */
        if (opcao != 0)
            pausar();
    } while (opcao != 0);

    return 0;
}
