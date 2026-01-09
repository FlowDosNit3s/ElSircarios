clear
clc
// =====================================================================
//  Problema 1 – Operações com Conjuntos
//  Script em Scilab
//  Autor: Diogo Costa, Vasco Leal, João Oliveira, Guilherme Gomes
//  Grupo: W
// =====================================================================
// NOTA: CTRL + E EM CIMA DE CADA CLC(DIVIDIDO POR ALINEAS)!!!

// ------------------------- DEFINIÇÕES -------------------------------
U = 1:20;                       // Universo com 20 elementos
A = [1 3 5 7 9 11 13];          // #A = 7   (5 ≤ #A <10)
B = 5:20;                       // #B = 16  (>15)

clc
// ------------------------- a) CARDINALIDADES ------------------------
printf("\n(a) #A = %d, #B = %d\n", size(A, "*"), size(B, "*"));

clc
// ------------------------- b) COMPLEMENTO de B ----------------------
Bbar = setdiff(U, B);
printf("\n(b) Complemento de B: "); disp(Bbar);

clc
// ------------------------- c) UNIÃO ---------------------------------
AuB = union(A, B);
printf("\n(c) A ∪ B: "); disp(AuB);
printf("|A ∪ B| = %d\n", size(AuB, "*"));

clc
// ------------------------- d) INTERSEÇÃO ----------------------------
AnB = intersect(A, B);
printf("\n(d) A ∩ B: "); disp(AnB);
printf("|A ∩ B| = %d\n", size(AnB, "*"));

clc
// ------------------------- e) DIFERENÇA B − A -----------------------
BminusA = setdiff(B, A);
printf("\n(e) B − A: "); disp(BminusA);

clc
// ------------------------- f) DIFERENÇA SIMÉTRICA -------------------
AminusB = setdiff(A, B);
symDiff = union(AminusB, BminusA);
printf("\n(f) A ⊕ B: "); disp(symDiff);
printf("|A ⊕ B| = %d\n", size(symDiff, "*"));

clc
// ------------------------- g)  COMPLEMENTO, DIFERENÇA SIMÉTRICA E DIFERENÇA --------------------
//COMO o complemento de (A ⊕ B) é igual 
//ao complemento de (A - B) ∪ (B - A);

// Diferença simétrica: (A - B) ∪ (B - A)
A_sym_B = union(setdiff(A, B), setdiff(B, A));

// Complemento da diferença simétrica
comp_A_sym_B = setdiff(U, A_sym_B);

// Diferença A - B
AminusB = setdiff(A, B);

// Resultado final
resultado = union(comp_A_sym_B, AminusB);

// Exibir resultado
disp("Resultado final: complemento de (A ⊕ B) unido com (A - B)");
disp(resultado);

clc
// ------------------------- h) PRODUTO CARTESIANO B × A --------------
printf("\n(h) B × A (mostrando os 10 primeiros pares):\n");
cart_BA = [];
for b = B
    for a = A
        cart_BA = [cart_BA; b a];
    end
end
disp(cart_BA(1:10, :));   // lista enorme; exibimos só 10 pares
printf("|B × A| = %d\n", size(cart_BA,1));

clc
// ------------------------- i) PRODUTO CARTESIANO A³ ------------------
printf("\n(i) A³ (número total de trios): |A|³ = %d\n", size(A,"*")^3);
// Se quiser listar alguns trios:
printf("   Exemplos de trios (primeiros 5):\n");
count = 0;
for a1 = A
    for a2 = A
        for a3 = A
            count = count + 1;
            if count <= 5 then
                printf("   (%d,%d,%d)\n", a1, a2, a3);
            end
        end
    end
end

printf("\n==== Fim do script ====\n");

clc
