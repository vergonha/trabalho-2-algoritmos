#include "btree.h"
#include "../../include/TDE.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct BTreeNode BTree;

void gerar_arvore_produtos(BTree **raiz) {
  FILE *indice = fopen("./indices/indice_produtos_exaustivo.bin", "rb");
  if (!indice) {
    perror("Houve um erro ao abrir o índice exaustivo de produtos para gerar a "
           "árvore.");
    exit(EXIT_FAILURE);
  }

  IndiceProduto *buffer = malloc(sizeof(IndiceProduto));
  while (fread(buffer, sizeof(IndiceProduto), 1, indice) == 1) {
    if (buffer->deletado == 1)
      continue;
    BTreeData data = {"", buffer->posicao, buffer->deletado};
    strcpy(data.sessao_usuario, buffer->sessao_usuario);
    inserir(raiz, data);
  }
}

void gerar_arvore_usuarios(BTree **raiz) {
  FILE *indice = fopen("./indices/indice_produtos_exaustivo.bin", "rb");
  if (!indice) {
    perror("Houve um erro ao abrir o índice exaustivo de usuários para gerar a "
           "árvore.");
    exit(EXIT_FAILURE);
  }

  IndiceUsuario *buffer = malloc(sizeof(IndiceUsuario));
  while (fread(buffer, sizeof(IndiceUsuario), 1, indice) == 1) {
    if (buffer->deletado == 1)
      continue;
    BTreeData data = {"", buffer->posicao, buffer->deletado};
    strcpy(data.sessao_usuario, buffer->sessao_usuario);
    inserir(raiz, data);
  }
}
