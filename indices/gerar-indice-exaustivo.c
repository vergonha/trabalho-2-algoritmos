#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/TDE.h"

void gerar_usuarios_indice_exaustivo_area_extensao()
{
    Usuario *usuario = malloc(sizeof(Usuario));

    FILE *indice = fopen("./indices/indice_usuarios_exaustivo_area_de_extensao.bin", "wb");
    if (indice == NULL)
    {
        perror("Failed to open index file for writing");
        free(usuario);
        return;
    }

    FILE *area_de_extensao = fopen("./areas_extensao/usuarios_area_extensao.bin", "rb");
    fseek(area_de_extensao, 0, SEEK_END);
    int quantidade_de_usuarios_area_de_extensao = ftell(area_de_extensao) / sizeof(Usuario);
    rewind(area_de_extensao);

    if (quantidade_de_usuarios_area_de_extensao == 0)
    {
        printf("Nenhum usuário na área de extensão\n");
        fclose(indice);
        fclose(area_de_extensao);
        free(usuario);
        return;
    }

    if (quantidade_de_usuarios_area_de_extensao == 1)
    {
        fread(usuario, sizeof(Usuario), 1, area_de_extensao);
        IndiceUsuario *i = malloc(sizeof(IndiceUsuario));
        strcpy(i->sessao_usuario, usuario->sessao_usuario);
        i->posicao = 0;
        fwrite(i, sizeof(IndiceUsuario), 1, indice);
        fclose(indice);
        fclose(area_de_extensao);
        free(usuario);
        return;
    }

    IndiceUsuario *indices = malloc((quantidade_de_usuarios_area_de_extensao) * sizeof(IndiceUsuario));
    if (indices == NULL)
    {
        perror("Failed to allocate memory for indices");
        free(usuario);
        fclose(indice);
        return;
    }

    int counter = 0;
    while (fread(usuario, sizeof(Usuario), 1, area_de_extensao))
    {
        strcpy(indices[counter].sessao_usuario, usuario->sessao_usuario);
        indices[counter].posicao = ftell(area_de_extensao) - sizeof(Usuario);
        counter++;
    }

    qsort(indices, counter, sizeof(IndiceUsuario), usrcmp);

    fwrite(indices, sizeof(IndiceUsuario), counter, indice);
    free(usuario);
    free(indices);
    fclose(indice);
    fclose(area_de_extensao);
}

void gerar_produtos_indice_exaustivo_area_extensao()
{
    Produto *produto = malloc(sizeof(Produto));

    FILE *indice = fopen("./indices/indice_produtos_exaustivo_area_de_extensao.bin", "wb");
    if (indice == NULL)
    {
        perror("Failed to open index file for writing");
        free(produto);
        return;
    }

    FILE *area_de_extensao = fopen("./areas_extensao/produtos_area_extensao.bin", "rb");
    fseek(area_de_extensao, 0, SEEK_END);
    int quantidade_de_produtos_area_de_extensao = ftell(area_de_extensao) / sizeof(Produto);
    rewind(area_de_extensao);

    if (quantidade_de_produtos_area_de_extensao == 0)
    {
        printf("Nenhum produto na área de extensão\n");
        fclose(indice);
        fclose(area_de_extensao);
        free(produto);
        return;
    }

    if (quantidade_de_produtos_area_de_extensao == 1)
    {
        fread(produto, sizeof(Produto), 1, area_de_extensao);
        IndiceProduto *i = malloc(sizeof(IndiceProduto));
        strcpy(i->sessao_usuario, produto->sessao_usuario);
        i->posicao = 0;
        fwrite(i, sizeof(IndiceProduto), 1, indice);
        fclose(indice);
        fclose(area_de_extensao);
        free(produto);
        return;
    }

    IndiceProduto *indices = malloc((quantidade_de_produtos_area_de_extensao) * sizeof(IndiceProduto));
    if (indices == NULL)
    {
        perror("Failed to allocate memory for indices");
        free(produto);
        fclose(indice);
        return;
    }

    int counter = 0;
    while (fread(produto, sizeof(Produto), 1, area_de_extensao))
    {
        strcpy(indices[counter].sessao_usuario, produto->sessao_usuario);
        indices[counter].posicao = ftell(area_de_extensao) - sizeof(Produto);
        // printf("Produto extensao: %s\n", produto->marca);
        counter++;
    }

    qsort(indices, counter, sizeof(IndiceProduto), idxprodcmp);

    fwrite(indices, sizeof(IndiceProduto), counter, indice);

    fclose(indice);
    fclose(area_de_extensao);
    free(produto);
    free(indices);
}

IndiceUsuario *buscar_usuarios_excluidos(int *quantidade_de_usuarios_excluidos)
{
    FILE *indice_exaustivo = fopen("./indices/indice_usuarios_exaustivo.bin", "rb");
    if (indice_exaustivo == NULL)
    {
        perror("Failed to open index file for reading");
        return NULL;
    }

    IndiceUsuario *usuarios_excluidos = NULL;
    IndiceUsuario buffer;
    int counter = 0;

    // Ler o arquivo e realocar a memória dinamicamente conforme necessário
    while (fread(&buffer, sizeof(IndiceUsuario), 1, indice_exaustivo))
    {
        if (buffer.deletado == 1)
        {
            // Realocar o array para armazenar o próximo usuário excluído
            IndiceUsuario *temp = realloc(usuarios_excluidos, (counter + 1) * sizeof(IndiceUsuario));
            if (temp == NULL)
            {
                perror("Failed to reallocate memory for excluded users");
                free(usuarios_excluidos);
                fclose(indice_exaustivo);
                return NULL;
            }

            usuarios_excluidos = temp;
            usuarios_excluidos[counter] = buffer; // Copiar os dados diretamente
            printf("Usuário que deve ser excluído: %s\n", buffer.sessao_usuario);
            counter++;
        }
    }

    fclose(indice_exaustivo);

    if (counter == 0)
    {
        printf("Nenhum usuário excluído no índice exaustivo de usuários.\n");
        free(usuarios_excluidos);
        return NULL;
    }

    *quantidade_de_usuarios_excluidos = counter;
    return usuarios_excluidos;
}

IndiceProduto *buscar_produtos_excluidos(int *quantidade_de_produtos_excluidos)
{
    FILE *indice_exaustivo = fopen("./indices/indice_produtos_exaustivo.bin", "rb");
    if (indice_exaustivo == NULL)
    {
        perror("Failed to open index file for reading");
        return NULL;
    }

    IndiceProduto *produtos_excluidos = NULL;
    IndiceProduto buffer;
    int counter = 0;

    // Ler o arquivo de uma só vez e realocar conforme necessário
    while (fread(&buffer, sizeof(IndiceProduto), 1, indice_exaustivo))
    {
        if (buffer.deletado == 1)
        {
            // Realoca o array para armazenar o próximo produto excluído
            IndiceProduto *temp = realloc(produtos_excluidos, (counter + 1) * sizeof(IndiceProduto));
            if (temp == NULL)
            {
                perror("Failed to reallocate memory for excluded products");
                free(produtos_excluidos);
                fclose(indice_exaustivo);
                return NULL;
            }

            produtos_excluidos = temp;
            produtos_excluidos[counter] = buffer; // Copia os dados diretamente
            printf("Produto que deve ser excluído: %s\n", buffer.sessao_usuario);
            counter++;
        }
    }

    fclose(indice_exaustivo);

    if (counter == 0)
    {
        printf("Nenhum produto excluído no índice exaustivo de produtos.\n");
        free(produtos_excluidos);
        return NULL;
    }

    *quantidade_de_produtos_excluidos = counter;
    return produtos_excluidos;
}

void gerar_produtos_indice_exaustivo()
{
    FILE *dados_produtos = fopen("./dados/dados_produtos.bin", "rb");
    Produto *produto = malloc(sizeof(Produto));
    if (produto == NULL)
    {
        perror("Failed to allocate memory for Produto");
        return;
    }

    int quantidade_de_produtos_excluidos = 0;
    IndiceProduto *excluidos = buscar_produtos_excluidos(&quantidade_de_produtos_excluidos);

    FILE *indice = fopen("./indices/indice_produtos_exaustivo.bin", "wb");
    if (indice == NULL)
    {
        perror("Failed to open index file for writing");
        free(produto);
        return;
    }

    fseek(dados_produtos, 0, SEEK_END);
    int size = ftell(dados_produtos) / sizeof(Produto);
    rewind(dados_produtos);

    IndiceProduto *indices = malloc(size * sizeof(IndiceProduto));
    if (indices == NULL)
    {
        perror("Failed to allocate memory for indices");
        free(produto);
        fclose(indice);
        return;
    }

    int counter = 0;
    int flag = 0;
    while (fread(produto, sizeof(Produto), 1, dados_produtos))
    {
        if (quantidade_de_produtos_excluidos > 0)
        {
            for (int i = 0; i < quantidade_de_produtos_excluidos; i++)
            {
                if (strcmp(produto->sessao_usuario, excluidos[i].sessao_usuario) == 0)
                {
                    // printf("Produto de fato excluído: %s\n", produto->sessao_usuario);
                    flag = 1;
                    continue;
                }
            }
        }

        if (flag == 1)
        {
            flag = 0;
            continue;
        }

        strcpy(indices[counter].sessao_usuario, produto->sessao_usuario);
        indices[counter].posicao = ftell(dados_produtos) - sizeof(Produto);
        counter++;
    }

    // Sort the indices
    qsort(indices, counter, sizeof(IndiceProduto), prodcmp);

    // Write the indices to the index file
    fwrite(indices, sizeof(IndiceProduto), counter, indice);

    // Clean up
    fclose(indice);
    free(produto);
    free(indices);
}

void gerar_usuarios_indice_exaustivo()
{
    FILE *dados_usuarios = fopen("./dados/dados_usuarios.bin", "rb");
    Usuario *usuario = malloc(sizeof(Usuario));
    if (usuario == NULL)
    {
        perror("Failed to allocate memory for Usuario");
        return;
    }
    int quantidade_de_usuarios_excluidos = 0;
    IndiceUsuario *excluidos = buscar_usuarios_excluidos(&quantidade_de_usuarios_excluidos);

    FILE *indice = fopen("./indices/indice_usuarios_exaustivo.bin", "wb");
    if (indice == NULL)
    {
        perror("Failed to open index file for writing");
        free(usuario);
        return;
    }

    fseek(dados_usuarios, 0, SEEK_END);
    int size = ftell(dados_usuarios) / sizeof(Usuario);
    rewind(dados_usuarios);

    IndiceUsuario *indices = malloc(size * sizeof(IndiceUsuario));
    if (indices == NULL)
    {
        perror("Failed to allocate memory for indices");
        free(usuario);
        fclose(indice);
        return;
    }

    int counter = 0;
    int flag = 0;
    while (fread(usuario, sizeof(Usuario), 1, dados_usuarios))
    {
        if (quantidade_de_usuarios_excluidos > 0)
        {
            for (int i = 0; i < quantidade_de_usuarios_excluidos; i++)
            {
                if (strcmp(usuario->sessao_usuario, excluidos[i].sessao_usuario) == 0)
                {
                    // printf("Usuário de fato excluído: %s\n", usuario->sessao_usuario);
                    flag = 1;
                    continue;
                }
            }
        }

        if (flag == 1)
        {
            flag = 0;
            continue;
        }

        strcpy(indices[counter].sessao_usuario, usuario->sessao_usuario);
        indices[counter].posicao = ftell(dados_usuarios) - sizeof(Usuario);
        counter++;
    }

    // Sort the indices
    qsort(indices, counter, sizeof(IndiceUsuario), usrcmp);

    // Write the indices to the index file
    fwrite(indices, sizeof(IndiceUsuario), counter, indice);

    // Clean up
    fclose(indice);
    free(usuario);
    free(indices);
}

IndiceUsuario *buscar_usuarios_excluidos_area_de_extensao(int *quantidade_de_excluidos)
{
    FILE *indice_exaustivo = fopen("./indices/indice_usuarios_exaustivo_area_de_extensao.bin", "rb");
    if (indice_exaustivo == NULL)
    {
        return NULL;
    }

    IndiceUsuario *usuarios_excluidos = NULL;
    IndiceUsuario buffer;
    int counter = 0;

    // Ler o arquivo e realocar a memória dinamicamente conforme necessário
    while (fread(&buffer, sizeof(IndiceUsuario), 1, indice_exaustivo))
    {
        if (buffer.deletado == 1)
        {
            // Realocar o array para armazenar o próximo usuário excluído
            IndiceUsuario *temp = realloc(usuarios_excluidos, (counter + 1) * sizeof(IndiceUsuario));
            if (temp == NULL)
            {
                perror("Failed to reallocate memory for excluded users");
                free(usuarios_excluidos);
                fclose(indice_exaustivo);
                return NULL;
            }

            usuarios_excluidos = temp;
            usuarios_excluidos[counter] = buffer; // Copiar os dados diretamente
            printf("Usuário que deve ser excluído: %s\n", buffer.sessao_usuario);
            counter++;
        }
    }

    fclose(indice_exaustivo);

    if (counter == 0)
    {
        printf("Nenhum usuário excluído no índice exaustivo de usuários da área de extensão.\n");
        free(usuarios_excluidos);
        return NULL;
    }

    *quantidade_de_excluidos = counter;
    return usuarios_excluidos;
}

void juntar_usuarios_area_de_extensao()
{
    FILE *usuarios_area_de_extensao = fopen("./areas_extensao/usuarios_area_extensao.bin", "rb");
    FILE *usuarios = fopen("./dados/dados_usuarios.bin", "a");

    int quantidade_de_excluidos = 0;
    IndiceUsuario *excluidos = buscar_usuarios_excluidos_area_de_extensao(&quantidade_de_excluidos);

    if (usuarios_area_de_extensao == NULL || usuarios == NULL)
    {
        printf("Não há usuários na área de extensão!\n");
        return;
    }

    fseek(usuarios_area_de_extensao, 0, SEEK_END);
    int quantidade_de_usuarios_area_de_extensao = ftell(usuarios_area_de_extensao) / sizeof(Usuario);
    rewind(usuarios_area_de_extensao);

    if (quantidade_de_usuarios_area_de_extensao == 0)
    {
        remove("./indices/indice_usuarios_exaustivo_area_de_extensao.bin");
        remove("./areas_extensao/usuarios_area_extensao.bin");
        return;
    }

    Usuario *buffer = malloc(sizeof(Usuario));
    while (fread(buffer, sizeof(Usuario), 1, usuarios_area_de_extensao))
    {
        int flag = 0;
        if (quantidade_de_excluidos > 0)
        {
            for (int i = 0; i < quantidade_de_excluidos; i++)
            {
                if (strcmp(buffer->sessao_usuario, excluidos[i].sessao_usuario) == 0)
                {
                    // printf("Usuário de fato excluído: %s\n", buffer->sessao_usuario);
                    flag = 1;
                    continue;
                }
            }
        }

        if (flag == 1)
        {
            flag = 0;
            continue;
        }

        printf("Usuário adicionado da área de extensão: %s\n", buffer->sessao_usuario);
        int ret = fwrite(buffer, sizeof(Usuario), 1, usuarios);
        if (ret != 1)
        {
            perror("Failed to write to file");
        }
    }

    free(buffer);
    remove("./indices/indice_usuarios_exaustivo_area_de_extensao.bin");
    remove("./areas_extensao/usuarios_area_extensao.bin");
    fclose(usuarios_area_de_extensao);
    fclose(usuarios);
}

IndiceProduto *buscar_produtos_excluidos_area_de_extensao(int *quantidade_de_produtos_excluidos)
{
    FILE *indice_exaustivo = fopen("./indices/indice_produtos_exaustivo_area_de_extensao.bin", "rb");
    if (indice_exaustivo == NULL)
    {
        return NULL;
    }

    IndiceProduto *produtos_excluidos = NULL;
    IndiceProduto buffer;
    int counter = 0;

    // Ler o arquivo de uma só vez e realocar conforme necessário
    while (fread(&buffer, sizeof(IndiceProduto), 1, indice_exaustivo))
    {
        if (buffer.deletado == 1)
        {
            // Realoca o array para armazenar o próximo produto excluído
            IndiceProduto *temp = realloc(produtos_excluidos, (counter + 1) * sizeof(IndiceProduto));
            if (temp == NULL)
            {
                free(produtos_excluidos);
                fclose(indice_exaustivo);
                return NULL;
            }

            produtos_excluidos = temp;
            produtos_excluidos[counter] = buffer; // Copia os dados diretamente
            printf("Produto que deve ser excluído: %s\n", buffer.sessao_usuario);
            counter++;
        }
    }

    fclose(indice_exaustivo);

    if (counter == 0)
    {
        printf("Nenhum produto excluído no índice exaustivo de produtos.\n");
        free(produtos_excluidos);
        return NULL;
    }

    *quantidade_de_produtos_excluidos = counter;
    return produtos_excluidos;
}

void juntar_produtos_area_de_extensao()
{
    FILE *produtos_area_de_extensao = fopen("./areas_extensao/produtos_area_extensao.bin", "rb");
    FILE *produtos = fopen("./dados/dados_produtos.bin", "a");

    int quantidade_de_excluidos = 0;
    IndiceProduto *excluidos = buscar_produtos_excluidos_area_de_extensao(&quantidade_de_excluidos);

    if (produtos_area_de_extensao == NULL || produtos == NULL)
    {
        printf("Não há produtos na área de extensão!\n");
        return;
    }

    fseek(produtos_area_de_extensao, 0, SEEK_END);
    int quantidade_de_produtos_area_de_extensao = ftell(produtos_area_de_extensao) / sizeof(Produto);
    rewind(produtos_area_de_extensao);

    if (quantidade_de_produtos_area_de_extensao == 0)
    {
        remove("./indices/indice_usuarios_exaustivo_area_de_extensao.bin");
        remove("./areas_extensao/usuarios_area_extensao.bin");
        return;
    }

    Produto *buffer = malloc(sizeof(Produto));
    while (fread(buffer, sizeof(Produto), 1, produtos_area_de_extensao))
    {
        int flag = 0;
        if (quantidade_de_excluidos > 0)
        {
            for (int i = 0; i < quantidade_de_excluidos; i++)
            {
                if (strcmp(buffer->sessao_usuario, excluidos[i].sessao_usuario) == 0)
                {
                    // printf("Produto de fato excluído: %s\n", buffer->sessao_usuario);
                    flag = 1;
                    continue;
                }
            }
        }

        if (flag == 1)
        {
            flag = 0;
            continue;
        }

        printf("Produto adicionado da área de extensão: %s\n", buffer->sessao_usuario);
        int ret = fwrite(buffer, sizeof(Produto), 1, produtos);
        if (ret != 1)
        {
            perror("Failed to write to file");
        }
    }

    free(buffer);
    remove("./indices/indice_produtos_exaustivo_area_de_extensao.bin");
    remove("./areas_extensao/produtos_area_extensao.bin");
    fclose(produtos_area_de_extensao);
    fclose(produtos);
}
