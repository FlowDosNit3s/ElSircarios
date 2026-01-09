#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Definindo constantes
#define NUM_PONTOS 20      // Total de pontos no espaço 3D
#define LIMIAR 5.0         // Distância máxima para criar uma aresta entre dois pontos
#define LIMITE 50.0        // Comprimento mínimo exigido para que um caminho seja considerado válido
#define MAX_ARESTAS 100    // Máximo de arestas permitidas
#define MAX_CAMINHOS 20    // Máximo de caminhos válidos a serem armazenados

// Estrutura para armazenar as coordenadas de um ponto no espaço 3D
typedef struct {
    float x, y, z;
} Ponto3D;

// Estrutura para armazenar informações sobre uma aresta: origem, destino e peso
typedef struct {
    int origem, destino;
    float peso;
} Aresta;

// Estrutura para armazenar um caminho encontrado entre dois pontos
typedef struct {
    int pontos[NUM_PONTOS];    // Lista de pontos no caminho
    int tamanho;               // Número de pontos no caminho
    double comprimento_total;  // Comprimento total do caminho
} CaminhoEncontrado;

// Estrutura para representar uma aresta no grafo (apenas destino e peso)
typedef struct {
    int destino;
    float peso;
} ArestaGrafo;

// Estrutura para representar a lista de adjacência de um grafo
typedef struct {
    ArestaGrafo arestas[NUM_PONTOS]; // Arestas do ponto
    int num_arestas;                 // Número de arestas do ponto
} ListaAdj;

// Inicialização dos pontos no espaço 3D
Ponto3D pontos[NUM_PONTOS] = {
    {0, 4, 0}, {1, 4, 0}, {2, 4, 0}, {4, 2, 0}, {1, 2, 1},
    {2, 1, 0}, {3, 0, 0}, {0, 3, 1}, {1, 1, 1}, {2, 2, 0},
    {3, 1, 1}, {4, 3, 0}, {2, 2, 2}, {3, 3, 1}, {0, 0, 3},
    {1, 1, 3}, {2, 0, 4}, {0, 4, 3}, {3, 0, 3}, {3, 3, 3}
};

// Grafo representado por listas de adjacência
ListaAdj grafo[NUM_PONTOS];

// Array para armazenar as arestas criadas
Aresta arestas[MAX_ARESTAS];
int total_arestas = 0;

// Array para armazenar os caminhos encontrados
CaminhoEncontrado caminhos[MAX_CAMINHOS];
int total_caminhos = 0;

// Função para calcular a distância Euclidiana entre dois pontos no espaço 3D
float distancia(Ponto3D a, Ponto3D b) {
    return sqrtf((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y) + (a.z - b.z)*(a.z - b.z));
}

// Função para adicionar uma aresta entre dois pontos no grafo
void adicionar_aresta(int origem, int destino, float peso) {
    if (grafo[origem].num_arestas < NUM_PONTOS)
        grafo[origem].arestas[grafo[origem].num_arestas++] = (ArestaGrafo){destino, peso};
    if (grafo[destino].num_arestas < NUM_PONTOS)
        grafo[destino].arestas[grafo[destino].num_arestas++] = (ArestaGrafo){origem, peso};
}

// Função para salvar um caminho válido que tenha comprimento >= LIMITE
void salvar_caminho(int caminho[], int profundidade, double soma) {
    if (soma >= LIMITE && total_caminhos < MAX_CAMINHOS) {
        CaminhoEncontrado *c = &caminhos[total_caminhos++];
        c->tamanho = profundidade + 1;
        c->comprimento_total = soma;
        for (int i = 0; i <= profundidade; i++) {
            c->pontos[i] = caminho[i];
        }
    }
}

// Função de busca recursiva para encontrar caminhos entre dois pontos
void buscar_caminhos(int atual, int destino, int caminho[], int profundidade, double soma, int visitados[]) {
    caminho[profundidade] = atual;

    // Se o ponto atual for o destino, salvamos o caminho encontrado
    if (atual == destino) {
        salvar_caminho(caminho, profundidade, soma);
        return;
    }

    visitados[atual] = 1;

    // Explorar vizinhos do ponto atual
    for (int i = 0; i < grafo[atual].num_arestas; i++) {
        int vizinho = grafo[atual].arestas[i].destino;
        double peso = grafo[atual].arestas[i].peso;

        // Se o vizinho não foi visitado, fazemos a recursão
        if (!visitados[vizinho]) {
            buscar_caminhos(vizinho, destino, caminho, profundidade + 1, soma + peso, visitados);
        }
    }

    visitados[atual] = 0; // Backtracking
}

// Função de comparação usada pelo qsort para ordenar caminhos por comprimento total
int comparar_caminhos(const void *a, const void *b) {
    CaminhoEncontrado *c1 = (CaminhoEncontrado *)a;
    CaminhoEncontrado *c2 = (CaminhoEncontrado *)b;
    if (c1->comprimento_total < c2->comprimento_total) return -1;
    else if (c1->comprimento_total > c2->comprimento_total) return 1;
    else return 0;
}

// Função de comparação usada pelo qsort para ordenar arestas por peso
int comparar_arestas(const void *a, const void *b) {
    Aresta *ar1 = (Aresta *)a;
    Aresta *ar2 = (Aresta *)b;
    if (ar1->peso < ar2->peso) return -1;
    else if (ar1->peso > ar2->peso) return 1;
    else return 0;
}

int main() {
    int matriz[NUM_PONTOS][NUM_PONTOS] = {0};

    // Construir o grafo e a matriz de adjacência
    for (int i = 0; i < NUM_PONTOS - 1; i++) {
        for (int j = i + 1; j < NUM_PONTOS; j++) {
            if (total_arestas >= MAX_ARESTAS) break;

            float d = distancia(pontos[i], pontos[j]);
            if (d <= LIMIAR) {
                matriz[i][j] = matriz[j][i] = 1;
                adicionar_aresta(i, j, d);
                arestas[total_arestas++] = (Aresta){i, j, d};
            }
        }
    }

    // Mostrar a matriz de adjacência
    printf("\nMatriz de Adjacência:\n\n    ");
    for (int j = 0; j < NUM_PONTOS; j++) printf("%2d ", j + 1);
    printf("\n   +");
    for (int j = 0; j < NUM_PONTOS; j++) printf("---");
    printf("\n");
    for (int i = 0; i < NUM_PONTOS; i++) {
        printf("%2d |", i + 1);
        for (int j = 0; j < NUM_PONTOS; j++) {
            printf(" %d ", matriz[i][j]);
        }
        printf("\n");
    }

    // Ordenar as arestas por peso (distância)
    qsort(arestas, total_arestas, sizeof(Aresta), comparar_arestas);

    // Mostrar as arestas ordenadas
    printf("\nArestas criadas (ordenadas pelo comprimento):\n");
    for (int i = 0; i < total_arestas; i++) {
        printf("Ponto %2d <-> Ponto %2d (%.2f cm)\n", arestas[i].origem + 1, arestas[i].destino + 1, arestas[i].peso);
    }

    // Definir origem e destino
    int origem = 5;  // ponto 6
    int destino = 9; // ponto 10

    printf("\nBuscando caminhos de %d até %d com no mínimo %.1f cm...\n\n", origem + 1, destino + 1, LIMITE);

    int caminho[NUM_PONTOS];
    int visitados[NUM_PONTOS] = {0};

    // Realizar a busca de caminhos
    buscar_caminhos(origem, destino, caminho, 0, 0.0, visitados);

    // Se houveram caminhos válidos encontrados, ordenar e exibir
    if (total_caminhos == 0) {
        printf("Nenhum caminho encontrado com comprimento suficiente.\n");
    } else {
        qsort(caminhos, total_caminhos, sizeof(CaminhoEncontrado), comparar_caminhos);

        printf("Total de caminhos encontrados: %d\n\n", total_caminhos);

        // Mostrar os caminhos encontrados
        for (int i = 0; i < total_caminhos; i++) {
            printf("Caminho %d (%.2f cm): ", i + 1, caminhos[i].comprimento_total);
            for (int j = 0; j < caminhos[i].tamanho; j++) {
                printf("%d", caminhos[i].pontos[j] + 1);
                if (j < caminhos[i].tamanho - 1) printf(" -> ");
            }
            printf("\n");
        }
    }

    printf("\nBusca finalizada.\n");

    return 0;
}
