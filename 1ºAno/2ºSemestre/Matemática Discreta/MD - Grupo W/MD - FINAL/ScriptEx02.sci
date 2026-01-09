clear
clc
// =====================================================================
//  Problema 2 – Produtórios e Somatórios
//  Script em Scilab
//  Autor: Diogo Costa, Vasco Leal, João Oliveira, Guilherme Gomes
//  Grupo: W
// =====================================================================
// NOTA: CRTL + E NO FIM!!
beta = 8;
n = 32;

// Verificação da condição
if (2*n > 50 + beta & 2*n < 100 - beta) then

    printf("\n============= RESULTADOS =============\n");

    // -------------------------
    // Parte (a) — Somatório
    // -------------------------
    base = (-2*beta - 1)/5;
    S = 0;
    for j = beta + 2 : n
        S = S + base^j;
    end
    printf("\nSomatório (a): %e\n", S);

    // -------------------------
    // Parte (b) — Produtório
    // -------------------------
    M = min(5 + beta, ceil(100 / (beta + 1)));
    C = 5 * (1:M);
    P = 1;
    for i = 1:length(C)
        termo = ((beta + 1) / C(i) - 1)^4;
        P = P * termo;
    end
    printf("Produtório (b): %e\n", P);

    // -------------------------
    // Parte (c) — Produtório complexo
    // -------------------------
    fat6 = prod(1:6);  // 6! = 720
    divF = floor(fat6 / (beta + 1));
    PROD = 1;
    for k = 1 : n - 15
        somaInterna = 0;
        for j = n - 5 : n
            termo = floor(1 + (j + k)/200) - divF;
            somaInterna = somaInterna + termo;
        end
        PROD = PROD * (3 * somaInterna);
    end
    printf("Produtório (c): %e\n", PROD);

    printf("\n=======================================\n");
    printf("==== Fim do script ====\n");

else
    printf("Valor de n inválido para o beta escolhido.\n");
end
