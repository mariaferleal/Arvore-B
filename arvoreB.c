#include<stdio.h>
#include<stdlib.h>

typedef struct no
{
  int total;
  int*chaves;
  struct no** filhos;
  struct no* pai;

} No;

typedef struct arvoreB
{
  No* raiz;
  int ordem;
}ArvoreB;

No* criaNo(ArvoreB* arvore)
{
  int max = arvore->ordem * 2;
  No* no = malloc(sizeof(No));

  no->pai = NULL;

  no->chaves = malloc(sizeof(int) * (max + 1));
  no->filhos = malloc(sizeof(No) * (max + 2));
  no-> total = 0;

  for(int i = 0; i < max + 1; i++)
  {
    no->filhos[i] = NULL;    
  }
  return no;
}

ArvoreB* criaArvore(int ordem)
{
  ArvoreB* a = malloc(sizeof(ArvoreB));
  a->ordem = ordem;
  a->raiz = criaNo(a);
  return a;
}
No* divideNo(ArvoreB* arvore, No* no)
{
  int meio = no->total / 2;
  No* novo = criaNo(arvore);
  novo->pai = no->pai;

  for(int i = meio + 1; i < no->total; i++)
    {
      novo->filhos[novo->total] = no->filhos[i];
      novo->chaves[novo->total] = no->chaves[i];
      if(novo->filhos[novo->total] != NULL)
      {
        novo->filhos[novo->total]->pai = novo;
      }
      novo->total++;
    }
  novo->filhos[novo->total] = no->filhos[no->total];

  if(novo->filhos[novo->total] != NULL)
  {
    novo->filhos[novo->total]->pai = novo;
  }
  no->total = meio;

  return novo;
}

void adicionaChaveNo(No* no, No* novo, int chave)
{
  int i = no->total - 1;
  while (i >= 0 && chave < no->chaves[i])
  {
    no->chaves[i + 1] = no->chaves[i];
    no->filhos[i + 2] = no->filhos[i + 1];
    i--;
  }
  no->chaves[i + 1] = chave;
  no->filhos[i + 2] = novo;
  if (novo != NULL)
    novo->pai = no;
  no->total++;
}

int transbordo(ArvoreB* arvore, No* no)
{
  return no->total > arvore->ordem * 2;
}

void adicionaChaveRecursivo(ArvoreB* arvore, No* no, No* novo, int chave)
{
  adicionaChaveNo(no, novo, chave);
  if(transbordo(arvore, no))
  {
    int promovido = no->chaves[arvore->ordem];
    No* novo = divideNo(arvore, no);
    if(no->pai == NULL)
    {
      No* pai = criaNo(arvore);
      pai->filhos[0] = no;
      adicionaChaveNo(pai, novo, promovido);

      no->pai = pai;
      novo->pai = pai;
      arvore->raiz = pai;
    } else {
      adicionaChaveRecursivo(arvore, no->pai, novo, promovido);
    }
  }
}

void removeChaveNo(No* no, int chave)
{
  int i = 0;
  while (i < no->total && chave != no->chaves[i])
  {
    i++;
  }
  if (i < no->total)
  {
    for (int j = i; j < no->total - 1; j++)
    {
      no->chaves[j] = no->chaves[j + 1];
      no->filhos[j + 1] = no->filhos[j + 2];
    }
    no->total--;
  }
}

void mergeNos(ArvoreB* arvore, No* no, No* irmao, No* pai, int pos)
{
  // Fusiona o nó 'no' com o nó 'irmao'.
  no->chaves[arvore->ordem - 1] = pai->chaves[pos];
  for (int i = 0; i < irmao->total; i++)
  {
    no->chaves[i + arvore->ordem] = irmao->chaves[i];
    no->filhos[i + arvore->ordem] = irmao->filhos[i];
    if (irmao->filhos[i] != NULL)
      irmao->filhos[i]->pai = no;
  }
  no->filhos[2 * arvore->ordem] = irmao->filhos[irmao->total];
  no->total = 2 * arvore->ordem;

  // Remove a chave e o nó do pai.
  removeChaveNo(pai, pai->chaves[pos]);

  // Libera a memória do nó irmão.
  free(irmao);
}

void removeChaveRecursivo(ArvoreB* arvore, No* no, int chave)
{
  removeChaveNo(no, chave);

  if (no == arvore->raiz && no->total == 0)
  {
    // Se a raiz ficar vazia após a remoção, atualiza a raiz.
    if (no->filhos[0] != NULL)
    {
      arvore->raiz = no->filhos[0];
      arvore->raiz->pai = NULL;
    }
  }
  else if (no->total < arvore->ordem)
  {
    // Verifica se a quantidade de chaves no nó é menor que o mínimo permitido.
    No* pai = no->pai;
    int pos = 0;

    while (pos <= pai->total && pai->filhos[pos] != no)
      pos++;

    if (pos > 0 && pai->filhos[pos - 1]->total > arvore->ordem)
    {
      // Caso 1: O nó tem um irmão esquerdo com chaves extras.
      No* irmao = pai->filhos[pos - 1];

      for (int i = no->total; i > 0; i--)
      {
        no->chaves[i] = no->chaves[i - 1];
        no->filhos[i + 1] = no->filhos[i];
      }

      no->chaves[0] = pai->chaves[pos - 1];
      no->filhos[1] = irmao->filhos[irmao->total];
      if (irmao->filhos[irmao->total] != NULL)
        irmao->filhos[irmao->total]->pai = no;
      pai->chaves[pos - 1] = irmao->chaves[irmao->total - 1];
      no->total++;
      irmao->total--;

      return;
    }
    else if (pos < pai->total && pai->filhos[pos + 1]->total > arvore->ordem)
    {
      // Caso 2: O nó tem um irmão direito com chaves extras.
      No* irmao = pai->filhos[pos + 1];

      no->chaves[no->total] = pai->chaves[pos];
      no->filhos[no->total + 1] = irmao->filhos[0];
      if (irmao->filhos[0] != NULL)
        irmao->filhos[0]->pai = no;
      pai->chaves[pos] = irmao->chaves[0];

      for (int i = 0; i < irmao->total - 1; i++)
      {
        irmao->chaves[i] = irmao->chaves[i + 1];
        irmao->filhos[i] = irmao->filhos[i + 1];
      }

      irmao->filhos[irmao->total - 1] = irmao->filhos[irmao->total];
      irmao->total--;
      no->total++;

      return;
    }
      else
      {
        // Caso 3: Nenhuma redistribuição possível, fusão de nó com irmão.
        if (pos > 0)
        {
          // Fusão com o irmão esquerdo.
          No* irmao = pai->filhos[pos - 1];
          mergeNos(arvore, irmao, no, pai, pos - 1);
          no = irmao;
        }
        else
        {
          // Fusão com o irmão direito.
          No* irmao = pai->filhos[pos + 1];
          mergeNos(arvore, no, irmao, pai, pos);
        }
        removeChaveRecursivo(arvore, pai, pai->chaves[pos]);
      }
  }
}

No* localizaNo(ArvoreB* arvore, int chave)
{
  No* no = arvore->raiz;

  while (no != NULL)
  {
    int i = 0;
    while (i < no->total && chave >= no->chaves[i])
    {
      i++;
    }

    if (i > 0 && chave == no->chaves[i - 1])
    {
      // A chave já existe, e estamos no nó onde ela deve estar.
      return no;
    }
    else if (no->filhos[i] != NULL)
    {
      // A chave não foi encontrada, desça na árvore para encontrar o nó apropriado.
      no = no->filhos[i];
    }
    else
    {
      // Chegou a uma folha, a chave não existe, retornamos o nó onde deveria estar.
      return no;
    }
  }

  return NULL; // Nó não encontrado (caso a árvore esteja vazia).
}


void removeChave(ArvoreB* arvore, int chave)
{
  No* no = localizaNo(arvore, chave);
  removeChaveRecursivo(arvore, no, chave);
}




/*void adicionaChave(ArvoreB* arvore, int chave)
{
  No* no = localizaNo(arvore, chave);
  adicionaChaveRecursivo(arvore, no, NULL, chave);
}*/
void imprimeArvore(No* no)
{
    if (no != NULL)
    {
        int i;
        for (i = 0; i < no->total; i++)
        {
            imprimeArvore(no->filhos[i]);
            printf("%d ", no->chaves[i]);
        }
        imprimeArvore(no->filhos[i]);
    }
}


void adicionaChave(ArvoreB* arvore, int chave)
{
    No* no = localizaNo(arvore, chave);
    adicionaChaveRecursivo(arvore, no, NULL, chave);
    printf("Árvore após adicionar a chave %d: ", chave);
    imprimeArvore(arvore->raiz);
    printf("\n");
}




