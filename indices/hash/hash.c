
#include "hash.h"
#include "../../include/TDE.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_TAMANHO 100000

void gerar_tabela_usuarios(HashTable **tabela) {
  FILE *indice = fopen("./indices/indice_usuarios_exaustivo.bin", "rb");
  if (!indice) {
    perror("Houve um erro ao abrir o índice exaustivo de usuários para gerar a "
           "tabela de hash.");
    exit(EXIT_FAILURE);
  }

  IndiceUsuario *buffer = malloc(sizeof(IndiceUsuario));
  IndiceUsuario *temp = malloc(sizeof(IndiceUsuario));

  while (fread(buffer, sizeof(IndiceUsuario), 1, indice) == 1) {
    if (strcmp(buffer->sessao_usuario, temp->sessao_usuario) == 0)
      continue;

    if (buffer->deletado == 1)
      continue;

    unsigned int indice_hash =
        hash(buffer->sessao_usuario) % (*tabela)->tamanho;

    HashTableItem *item = (HashTableItem *)malloc(sizeof(HashTableItem));
    item->chave =
        strdup(buffer->sessao_usuario); // Sessão do usuário como chave
    item->endereco = buffer->posicao;   // Posição como o endereço (valor)
    item->deletado = false;
    item->proximo = NULL;

    if ((*tabela)->itens[indice_hash] == NULL) {
      (*tabela)->itens[indice_hash] = item;
    } else {
      HashTableItem *atual = (*tabela)->itens[indice_hash];
      while (atual->proximo != NULL) {
        atual = atual->proximo;
      }
      atual->proximo = item;
    }

    (*tabela)->quantidade++;

    // printf("%s\n", buffer->sessao_usuario);
    strcpy(temp->sessao_usuario, buffer->sessao_usuario);
  }

  free(buffer);
  fclose(indice);
}

void gerar_tabela_produtos(HashTable **tabela) {
  FILE *indice = fopen("./indices/indice_produtos_exaustivo.bin", "rb");
  if (!indice) {
    perror("Houve um erro ao abrir o índice exaustivo de produtos para gerar a "
           "tabela de hash.");
    exit(EXIT_FAILURE);
  }

  IndiceProduto *buffer = malloc(sizeof(IndiceProduto));
  IndiceProduto *temp = malloc(sizeof(IndiceProduto));

  while (fread(buffer, sizeof(IndiceProduto), 1, indice) == 1) {
    if (strcmp(buffer->sessao_usuario, temp->sessao_usuario) == 0)
      continue;

    if (buffer->deletado == 1)
      continue;

    unsigned int indice_hash =
        hash(buffer->sessao_usuario) % (*tabela)->tamanho;

    HashTableItem *item = (HashTableItem *)malloc(sizeof(HashTableItem));
    item->chave =
        strdup(buffer->sessao_usuario); // Sessão do usuário como chave
    item->endereco = buffer->posicao;   // Posição como o endereço (valor)
    item->deletado = false;
    item->proximo = NULL;

    if ((*tabela)->itens[indice_hash] == NULL) {
      (*tabela)->itens[indice_hash] = item;
    } else {
      HashTableItem *atual = (*tabela)->itens[indice_hash];
      while (atual->proximo != NULL) {
        atual = atual->proximo;
      }
      atual->proximo = item;
    }

    (*tabela)->quantidade++;

    // printf("%s\n", buffer->sessao_usuario);
    strcpy(temp->sessao_usuario, buffer->sessao_usuario);
  }
  free(temp);
  free(buffer);
  fclose(indice);
}
