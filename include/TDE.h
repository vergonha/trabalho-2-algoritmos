#include <string.h>

#ifndef TDEH
#define TDEH

typedef struct Produto
{
    long id_categoria;
    int id_produto;
    char codigo_categoria[32];
    char marca[12];
    float preco;
    char sessao_usuario[37];
} Produto;

typedef struct Usuario
{
    char tipo_evento[12];
    char data[22];
    int id_usuario;
    char sessao_usuario[37];
    int id_produto;
} Usuario;

typedef struct IndiceProduto
{
    char sessao_usuario[37];
    long posicao;
    int deletado;
} IndiceProduto;

typedef struct IndiceUsuario
{
    char sessao_usuario[37];
    long posicao;
    int deletado;
} IndiceUsuario;

int prodcmp(const void *_a, const void *_b)
{
    IndiceProduto *a = (IndiceProduto *)_a;
    IndiceProduto *b = (IndiceProduto *)_b;

    return strcmp(a->sessao_usuario, b->sessao_usuario);
}

int usrcmp(const void *_a, const void *_b)
{
    IndiceUsuario *a = (IndiceUsuario *)_a;
    IndiceUsuario *b = (IndiceUsuario *)_b;

    return strcmp(a->sessao_usuario, b->sessao_usuario);
}

int idxusrcmp(const void *_a, const void *_b)
{
    IndiceUsuario *a = (IndiceUsuario *)_a;
    IndiceProduto *b = (IndiceProduto *)_b;

    return strcmp(a->sessao_usuario, b->sessao_usuario);
}

int idxprodcmp(const void *_a, const void *_b)
{
    IndiceProduto *a = (IndiceProduto *)_a;
    IndiceProduto *b = (IndiceProduto *)_b;

    return strcmp(a->sessao_usuario, b->sessao_usuario);
}

#endif