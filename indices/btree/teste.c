#include "btree.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <uuid/uuid.h>

void gerar_uuid_v4(char *uuid_str) {
  uuid_t uuid;
  uuid_generate_random(uuid);
  uuid_unparse_lower(uuid, uuid_str);
}

double medir_tempo_criacao_arvore(void (*funcao_gerar_arvore)(BTree **),
                                  BTree **raiz) {
  clock_t inicio = clock();

  funcao_gerar_arvore(raiz);

  clock_t fim = clock();

  double tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
  return tempo_execucao;
}

void medir_inclusao(BTree **raiz, BTreeData chave) {
  clock_t inicio = clock();

  inserir(raiz, chave);

  clock_t fim = clock();

  double tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
  printf("Tempo de inserção para chave '%s': %.6f segundos\n",
         chave.sessao_usuario, tempo_execucao);
}

void medir_pesquisa(BTree *raiz, const char *sessao_usuario) {
  clock_t inicio = clock();

  BTreeData *item = pesquisa(raiz, sessao_usuario);

  clock_t fim = clock();

  double tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
  printf("Tempo de pesquisa para chave '%s': %.6f segundos\n", sessao_usuario,
         tempo_execucao);

  if (item != NULL) {
    printf("Chave '%s' encontrada! Endereço: %ld\n", sessao_usuario,
           item->endereco);
    FILE *f = fopen("./dados/dados_produtos.bin", "rb");
    if (!f) {
      perror("Deu errado: ");
    }

    fseek(f, item->endereco, SEEK_SET);
    Produto *buffer = malloc(sizeof(Produto));
    int i = fread(buffer, sizeof(Produto), 1, f);
    if (1 != i || !buffer) {
      perror("Nao leu: ");
    } else {
      printf(">Marca: %s | Categoria: %s | Sess: %s\n\n", buffer->marca,
             buffer->codigo_categoria, buffer->sessao_usuario);
    }

    fclose(f);
    free(buffer);
  } else {
    printf("Chave '%s' não encontrada.\n", sessao_usuario);
  }
}

void medir_remocao(BTree *raiz, const char *sessao_usuario) {
  clock_t inicio = clock();

  BTreeData *item = pesquisa(raiz, sessao_usuario);
  if (item != NULL) {
    item->deletado = 1;
  }

  clock_t fim = clock();

  double tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
  printf("Tempo de remoção para chave '%s': %.6f segundos\n", sessao_usuario,
         tempo_execucao);
}

int main() {
  BTree *raiz_produtos = NULL;

  double tempo_criacao_produtos =
      medir_tempo_criacao_arvore(gerar_arvore_produtos, &raiz_produtos);
  printf("Tempo para criar a árvore de produtos: %.6f segundos\n",
         tempo_criacao_produtos);

  for (int i = 0; i < 5; i++) {
    BTreeData chave = {"", 123456 + i, 0};
    gerar_uuid_v4(chave.sessao_usuario);
    printf("\nRealizando inserção...\n");
    medir_inclusao(&raiz_produtos, chave);
  }

  printf("\nPesquisando na árvore...\n");
  medir_pesquisa(raiz_produtos, "b5119ea8-9b48-4f8b-91d9-a602f6086a0d");
  medir_pesquisa(raiz_produtos, "739fa793-aa27-44ca-b6fb-c813c24b83ce");

  printf("\nRealizando remoção...\n");
  medir_remocao(raiz_produtos, "b5119ea8-9b48-4f8b-91d9-a602f6086a0d");

  printf("\nPesquisa do item removido...\n");
  medir_pesquisa(raiz_produtos, "b5119ea8-9b48-4f8b-91d9-a602f6086a0d");

  desalocar_arvore(raiz_produtos);

  return 0;
}
