#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define M 10000

typedef struct BTreeData {
    char sessao_usuario[37];
    long endereco;
    int deletado;
} BTreeData;

typedef struct BTreeNode {
    int num_chaves;
    BTreeData chaves[M - 1];
    struct BTreeNode *filhos[M];
    bool eh_folha;
} BTreeNode;

// Função para criar um novo nó
BTreeNode *criar_node(bool eh_folha) {
    BTreeNode *novo_node = (BTreeNode *)malloc(sizeof(BTreeNode));
    novo_node->num_chaves = 0;
    novo_node->eh_folha = eh_folha;
    for (int i = 0; i < M; i++) {
        novo_node->filhos[i] = NULL;
    }
    return novo_node;
}

// Função para dividir um nó (split)
void splitfilho(BTreeNode *pai, int index) {
    BTreeNode *filho = pai->filhos[index];
    BTreeNode *novo_node = criar_node(filho->eh_folha);

    novo_node->num_chaves = M / 2 - 1;

    // Mover as chaves para o novo nó
    for (int i = 0; i < M / 2 - 1; i++) {
        novo_node->chaves[i] = filho->chaves[i + M / 2];
    }

    if (!filho->eh_folha) {
        for (int i = 0; i < M / 2; i++) {
            novo_node->filhos[i] = filho->filhos[i + M / 2];
        }
    }

    filho->num_chaves = M / 2 - 1;

    // Mover os filhos do nó pai para dar espaço para o novo filho
    for (int i = pai->num_chaves; i > index; i--) {
        pai->filhos[i + 1] = pai->filhos[i];
    }

    pai->filhos[index + 1] = novo_node;

    // Mover as chaves do pai para dar espaço para a nova chave
    for (int i = pai->num_chaves - 1; i >= index; i--) {
        pai->chaves[i + 1] = pai->chaves[i];
    }

    pai->chaves[index] = filho->chaves[M / 2 - 1];
    pai->num_chaves++;
}

// Função para inserir uma chave em um nó não cheio
void inserir_nao_cheio(BTreeNode *node, BTreeData chave) {
    int i = node->num_chaves - 1;

    if (node->eh_folha) {
        while (i >= 0 && strcmp(node->chaves[i].sessao_usuario, chave.sessao_usuario) > 0) {
            node->chaves[i + 1] = node->chaves[i];
            i--;
        }
        node->chaves[i + 1] = chave;
        node->num_chaves++;
    } else {
        while (i >= 0 && strcmp(node->chaves[i].sessao_usuario, chave.sessao_usuario) > 0) {
            i--;
        }
        i++;

        if (node->filhos[i]->num_chaves == M - 1) {
            splitfilho(node, i);

            if (strcmp(node->chaves[i].sessao_usuario, chave.sessao_usuario) < 0) {
                i++;
            }
        }
        inserir_nao_cheio(node->filhos[i], chave);
    }
}

// Função para inserir uma chave na árvore
void inserir(BTreeNode **raiz, BTreeData chave) {
    BTreeNode *node = *raiz;

    if (node == NULL) {
        *raiz = criar_node(true);
        (*raiz)->chaves[0] = chave;
        (*raiz)->num_chaves = 1;
    } else {
        if (node->num_chaves == M - 1) {
            BTreeNode *nova_raiz = criar_node(false);
            nova_raiz->filhos[0] = node;
            splitfilho(nova_raiz, 0);
            *raiz = nova_raiz;
        }
        inserir_nao_cheio(*raiz, chave);
    }
}

// Função para percorrer a árvore e imprimir as chaves
void traverse(BTreeNode *raiz) {
    if (raiz != NULL) {
        int i;
        for (i = 0; i < raiz->num_chaves; i++) {
            traverse(raiz->filhos[i]);
            printf("%s\n", raiz->chaves[i].sessao_usuario);
        }
        traverse(raiz->filhos[i]);
    }
}

// Função para buscar uma chave na árvore (busca pelo sessao_usuario)
BTreeData *search(BTreeNode *node, const char *sessao_usuario) {
    int i = 0;

    if (node == NULL) {
        return NULL;
    }

    // Procura a chave correta dentro do nó, comparando com sessao_usuario
    while (i < node->num_chaves && strcmp(sessao_usuario, node->chaves[i].sessao_usuario) > 0) {
        i++;
    }

    // Se a chave correspondente for encontrada
    if (i < node->num_chaves && strcmp(sessao_usuario, node->chaves[i].sessao_usuario) == 0) {
        return &node->chaves[i];
    }

    // Se o nó for folha, significa que não encontramos a chave
    if (node->eh_folha) {
        return NULL;
    }

    // Busca recursivamente na subárvore correspondente
    return search(node->filhos[i], sessao_usuario);
}

   

