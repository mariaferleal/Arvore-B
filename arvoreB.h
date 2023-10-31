#ifndef ARVOREB_H
#define ARVOREB_H

typedef struct no No;
typedef struct arvoreB ArvoreB;

ArvoreB* criaArvore(int ordem);
void adicionaChave(ArvoreB* arvore, int chave);
void removeChave(ArvoreB* arvore, int chave);
No* divideNo(ArvoreB* arvore, No* no);
void adicionaChaveNo(No* no, No* novo, int chave);
int transbordo(ArvoreB* arvore, No* no);
void adicionaChaveRecursivo(ArvoreB* arvore, No* no, No* novo, int chave);
void removeChaveNo(No* no, int chave);
void mergeNos(ArvoreB* arvore, No* no, No* irmao, No* pai, int pos);
void removeChaveRecursivo(ArvoreB* arvore, No* no, int chave);
No* localizaNo(ArvoreB* arvore, int chave);

#endif

