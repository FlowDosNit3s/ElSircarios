#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_PONTOS 20
#define LIMIAR 5.0
#define LIMITE 50.0
#define MAX_ARESTAS 100
#define MAX_CAMINHOS 20

typedef struct {
    float x, y, z;
} Ponto3D;

typedef struct {
    int origem, destino;
    float peso;
} Aresta;

typedef struct {
    int pontos[NUM_PONTOS];
    int tamanho;
    double comprimento_total;
} CaminhoEncontrado;

typedef struct {
    int destino;
    float peso;
} ArestaGrafo;

typedef struct {
    ArestaGrafo arestas[NUM_PONTOS];
    int num_arestas;
} ListaAdj;

Ponto3D pontos[NUM_PONTOS] = {
    {0, 4, 0}, {1, 4, 0}, {2, 4, 0}, {4, 2, 0}, {1, 2, 1},
    {2, 1, 0}, {3, 0, 0}, {0, 3, 1}, {1, 1, 1}, {2, 2, 0},
    {3, 1, 1}, {4, 3, 0}, {2, 2, 2}, {3, 3, 1}, {0, 0, 3},
    {1, 1, 3}, {2, 0, 4}, {0, 4, 3}, {3, 0, 3}, {3, 3, 3}
};

ListaAdj grafo[NUM_PONTOS];
Aresta arestas[MAX_ARESTAS];
int total_arestas = 0;

CaminhoEncontrado caminhos[MAX_CAMINHOS];
int total_caminhos = 0;

// Função para calcular distância
float distancia(Ponto3D a, Ponto3D b) {
    return sqrtf((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y) + (a.z - b.z)*(a.z - b.z));
}

// Adicionar aresta
void adicionar_aresta(int origem, int destino, float peso) {
    if (grafo[origem].num_arestas < NUM_PONTOS)
        grafo[origem].arestas[grafo[origem].num_arestas++] = (ArestaGrafo){destino, peso};
    if (grafo[destino].num_arestas < NUM_PONTOS)
        grafo[destino].arestas[grafo[destino].num_arestas++] = (ArestaGrafo){origem, peso};
}

// Salvar caminho válido
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

// Busca recursiva
void buscar_caminhos(int atual, int destino, int caminho[], int profundidade, double soma, int visitados[]) {
    caminho[profundidade] = atual;

    if (atual == destino) {
        salvar_caminho(caminho, profundidade, soma);
        return;
    }

    visitados[atual] = 1;

    for (int i = 0; i < grafo[atual].num_arestas; i++) {
        int vizinho = grafo[atual].arestas[i].destino;
        double peso = grafo[atual].arestas[i].peso;

        if (!visitados[vizinho]) {
            buscar_caminhos(vizinho, destino, caminho, profundidade + 1, soma + peso, visitados);
        }
    }

    visitados[atual] = 0; // backtracking
}

// Comparar caminhos (para qsort)
int comparar_caminhos(const void *a, const void *b) {
    CaminhoEncontrado *c1 = (CaminhoEncontrado *)a;
    CaminhoEncontrado *c2 = (CaminhoEncontrado *)b;
    if (c1->comprimento_total < c2->comprimento_total) return -1;
    else if (c1->comprimento_total > c2->comprimento_total) return 1;
    else return 0;
}

// Comparar arestas (para qsort)
int comparar_arestas(const void *a, const void *b) {
    Aresta *ar1 = (Aresta *)a;
    Aresta *ar2 = (Aresta *)b;
    if (ar1->peso < ar2->peso) return -1;
    else if (ar1->peso > ar2->peso) return 1;
    else return 0;
}

int main() {
    int matriz[NUM_PONTOS][NUM_PONTOS] = {0};

    // Construir o grafo
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

    // Ordenar arestas antes de mostrar
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

    buscar_caminhos(origem, destino, caminho, 0, 0.0, visitados);

    if (total_caminhos == 0) {
        printf("Nenhum caminho encontrado com comprimento suficiente.\n");
    } else {
        // Ordenar caminhos
        qsort(caminhos, total_caminhos, sizeof(CaminhoEncontrado), comparar_caminhos);

        printf("Total de caminhos encontrados: %d\n\n", total_caminhos);

        // Mostrar caminhos
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
