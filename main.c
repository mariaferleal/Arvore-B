#include "arvoreB.h"

#include <stdio.h>

int main()
{
    // Crie e teste a árvore B aqui.
    ArvoreB* arvore = criaArvore(3);

    adicionaChave(arvore, 10);
    adicionaChave(arvore, 20);
    adicionaChave(arvore, 5);
    adicionaChave(arvore, 15);

    removeChave(arvore, 10);

    // Mais operações de teste e impressões aqui, se necessário.
    
    return 0;
}
