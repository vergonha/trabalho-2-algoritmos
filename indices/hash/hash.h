#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct HashTableItem {
  char *chave;
  long int endereco;
  bool deletado;
  struct HashTableItem *proximo;
} HashTableItem;

typedef struct HashTable {
  HashTableItem **itens;
  int tamanho;
  int quantidade;
} HashTable;

void imprimir_status(int registros_inseridos, int colisoes) {
  printf("Registros inseridos: %d | Colisoes: %d\n", registros_inseridos,
         colisoes);
}

void desalocar_item(HashTableItem *i) {
  free(i->chave);
  free(i);
}

void desalocar_tabela(HashTable *t) {
  for (int i = 0; i < t->tamanho; i++) {
    HashTableItem *item = t->itens[i];
    while (item != NULL) {
      HashTableItem *temp = item;
      item = item->proximo;
      desalocar_item(temp);
    }
  }
  free(t->itens);
  free(t);
}

HashTableItem *criar_item(char *chave, long int endereco) {
  HashTableItem *i = (HashTableItem *)malloc(sizeof(HashTableItem));
  i->chave = (char *)malloc(strlen(chave) + 1);
  strcpy(i->chave, chave);
  i->endereco = endereco;
  i->deletado = false;
  i->proximo = NULL;
  return i;
}

HashTable *criar_tabela(int tamanho) {
  HashTable *t = (HashTable *)malloc(sizeof(HashTable));
  t->tamanho = tamanho;
  t->quantidade = 0;
  t->itens = (HashTableItem **)calloc(tamanho, sizeof(HashTableItem *));
  return t;
}

unsigned int hash(const char *key) {
  unsigned int hash = 2166136261u;
  while (*key) {
    hash ^= (unsigned char)(*key);
    hash *= 16777619u;
    key++;
  }
  return hash;
}

HashTableItem *pesquisar(HashTable *t, const char *chave) {
  int indice = hash(chave) % t->tamanho;
  HashTableItem *item = t->itens[indice];
  while (item != NULL) {
    if (strcmp(item->chave, chave) == 0 && !item->deletado) {
      return item;
    }
    item = item->proximo;
  }
  return NULL;
}
HashTableItem *deletar(HashTable *t, const char *chave) {
  int indice = hash(chave) % t->tamanho;
  HashTableItem *item = t->itens[indice];
  while (item != NULL) {
    if (strcmp(item->chave, chave) == 0 && !item->deletado) {
      item->deletado = 1;
      return item;
    }
    item = item->proximo;
  }
  return NULL;
}

void inserir(HashTable *t, char *chave, long int endereco) {
  int indice = hash(chave) % t->tamanho;
  HashTableItem *item = criar_item(chave, endereco);
  if (t->itens[indice] == NULL) {
    t->itens[indice] = item;
  } else {
    HashTableItem *atual = t->itens[indice];
    while (atual->proximo != NULL) {
      atual = atual->proximo;
    }
    atual->proximo = item;
  }
  t->quantidade++;
}

void imprimir_tabela(HashTable *t) {
  printf("Tabela de Hash:\n");
  printf("Tamanho: %d, Quantidade de itens: %d\n", t->tamanho, t->quantidade);

  for (int i = 0; i < t->tamanho; i++) {
    if (t->itens[i] != NULL) {
      printf("Índice %d:\n", i);
      HashTableItem *item = t->itens[i];
      while (item != NULL) {
        printf("\tChave: %s\n", item->chave);
        printf("\tEndereço: %ld\n", item->endereco);
        printf("\tDeletado: %s\n", item->deletado ? "Sim" : "Não");
        item = item->proximo;
      }
    }
  }
}
