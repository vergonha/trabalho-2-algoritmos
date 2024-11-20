#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <uuid/uuid.h>

#include "hash.c"

void gerar_uuid_v4(char *uuid_str) {
  uuid_t uuid;
  uuid_generate_random(uuid);
  uuid_unparse_lower(uuid, uuid_str);
}

double medir_tempo_criacao_tabela(void (*funcao_gerar_tabela)(HashTable **),
                                  HashTable **tabela) {
  clock_t inicio = clock();

  funcao_gerar_tabela(tabela);

  clock_t fim = clock();

  double tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
  return tempo_execucao;
}

void medir_inclusao(HashTable *tabela, char *chave, long int endereco) {
  clock_t inicio = clock();

  inserir(tabela, chave, endereco);

  clock_t fim = clock();

  double tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
  printf("Tempo de inserção para chave '%s': %.6f segundos\n", chave,
         tempo_execucao);
}

void medir_remocao(HashTable *tabela, const char *chave) {
  clock_t inicio = clock();

  HashTableItem *removido = deletar(tabela, chave);

  clock_t fim = clock();

  double tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
  printf("Tempo de remoção para chave '%s': %.6f segundos\n", chave,
         tempo_execucao);

  if (removido != NULL) {
    printf("Chave '%s' removida com sucesso.\n", chave);
  } else {
    printf("Chave '%s' não encontrada para remoção.\n", chave);
  }
}

void medir_reorganizacao(HashTable **tabela) {
  clock_t inicio = clock();

  reorganizar_tabela(tabela);

  clock_t fim = clock();

  double tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
  printf("Tempo de reorganização da tabela: %.6f segundos\n", tempo_execucao);
}

void medir_pesquisa(HashTable *tabela, const char *chave) {
  clock_t inicio = clock();

  HashTableItem *item = pesquisar(tabela, chave);

  clock_t fim = clock();

  double tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
  printf("Tempo de pesquisa para chave '%s': %.6f segundos\n", chave,
         tempo_execucao);

  if (item != NULL) {
    printf("Chave '%s' encontrada! Endereço: %ld\n", chave, item->endereco);
  } else {
    printf("Chave '%s' não encontrada.\n", chave);
  }
}

int main() {

  HashTable *tabela_produtos = criar_tabela(HASH_TAMANHO);

  double tempo_criacao_produtos =
      medir_tempo_criacao_tabela(gerar_tabela_produtos, &tabela_produtos);
  printf("Tempo para criar a tabela de produtos: %.6f segundos\n",
         tempo_criacao_produtos);

  for (int i = 0; i < 5; i++) {
    char uuid_insercao[37];
    gerar_uuid_v4(uuid_insercao);
    printf("\nRealizando inserção...");
    medir_inclusao(tabela_produtos, uuid_insercao, 123456 + i);
  }

  printf("\n\n");
  medir_pesquisa(tabela_produtos, "b5119ea8-9b48-4f8b-91d9-a602f6086a0d");
  medir_pesquisa(tabela_produtos, "739fa793-aa27-44ca-b6fb-c813c24b83ce");
  medir_pesquisa(tabela_produtos, "7f4fa237-d0fe-4f36-b2d2-6676fe985055");
  medir_pesquisa(tabela_produtos, "ff1e7d25-91e9-4dd0-805e-4d30f6f4f167");
  medir_pesquisa(tabela_produtos, "a1ca9e58-a88d-43e5-ae85-54108e18c994");

  printf("\nRealizando remoção...\n");
  medir_remocao(tabela_produtos, "7894ba54-51f9-40f3-bff7-11c862279d0f");

  printf("\nPesquisa do item removido...\n");
  medir_pesquisa(tabela_produtos, "7894ba54-51f9-40f3-bff7-11c862279d0f");

  printf("\nMedindo o tempo de reorganização...\n");
  medir_reorganizacao(&tabela_produtos);

  printf("\nPesquisando na tabela nova...\n");
  medir_pesquisa(tabela_produtos, "a1ca9e58-a88d-43e5-ae85-54108e18c994");

  desalocar_tabela(tabela_produtos);

  return 0;
}
