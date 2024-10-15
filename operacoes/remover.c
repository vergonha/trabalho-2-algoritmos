#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include "../buscas/busca-binaria.c"

int remover_produto(char *target)
{
    FILE *area_de_extensao_indice_exaustivo = fopen("./indices/indice_produtos_exaustivo_area_de_extensao.bin", "rb+");

    if (area_de_extensao_indice_exaustivo)
    {
        fseek(area_de_extensao_indice_exaustivo, 0, SEEK_END);
        int tamanho_area_extensao = ftell(area_de_extensao_indice_exaustivo) / sizeof(IndiceProduto);
        rewind(area_de_extensao_indice_exaustivo);

        if (tamanho_area_extensao > 0)
        {
            IndiceProduto *i = busca_binaria_na_area_de_extensao_produto(area_de_extensao_indice_exaustivo, tamanho_area_extensao, 0, target, 1);
            if (i)
            {
                int posicao_fisica = i->posicao;
                fseek(area_de_extensao_indice_exaustivo, i->posicao, SEEK_SET);
                fread(i, sizeof(IndiceProduto), 1, area_de_extensao_indice_exaustivo);
                i->deletado = 1;
                fseek(area_de_extensao_indice_exaustivo, posicao_fisica, SEEK_SET);
                fwrite(i, sizeof(IndiceProduto), 1, area_de_extensao_indice_exaustivo);
                fclose(area_de_extensao_indice_exaustivo);
                free(i);
                return 1;
            }
        }
    }

    FILE *indice_produtos_exaustivo = fopen("./indices/indice_produtos_exaustivo.bin", "rb+");
    if (!indice_produtos_exaustivo)
    {
        perror("Failed to open index file for reading");
        return 0;
    }

    FILE *indice_produtos_parcial = fopen("./indices/indice_produtos_parcial.bin", "rb+");
    fseek(indice_produtos_exaustivo, 0, SEEK_END);
    int size = ftell(indice_produtos_exaustivo) / sizeof(IndiceProduto);
    rewind(indice_produtos_exaustivo);

    IndiceProduto *bloco = busca_binaria_bloco_do_produto(indice_produtos_parcial, target, size / TAMANHO_DOS_BLOCOS);
    fclose(indice_produtos_parcial);
    if (!bloco)
    {
        return 0;
    }

    IndiceProduto *i = busca_binaria_produto_no_bloco(indice_produtos_exaustivo, bloco->posicao / sizeof(IndiceProduto), target, 1);
    if (!i)
    {
        return 0;
    }
    long posicao_fisica = i->posicao;

    fseek(indice_produtos_exaustivo, i->posicao, SEEK_SET);
    fread(i, sizeof(IndiceProduto), 1, indice_produtos_exaustivo);
    i->deletado = 1;
    fseek(indice_produtos_exaustivo, posicao_fisica, SEEK_SET);
    fwrite(i, sizeof(IndiceProduto), 1, indice_produtos_exaustivo);
    fclose(indice_produtos_exaustivo);
    free(i);

    return 1;
}

int remover_usuario(char *target)
{
    FILE *area_de_extensao_indice_exaustivo = fopen("./indices/indice_usuarios_exaustivo_area_de_extensao.bin", "rb+");
    if (area_de_extensao_indice_exaustivo)
    {
        fseek(area_de_extensao_indice_exaustivo, 0, SEEK_END);
        int tamanho_area_extensao = ftell(area_de_extensao_indice_exaustivo) / sizeof(IndiceUsuario);
        rewind(area_de_extensao_indice_exaustivo);

        if (tamanho_area_extensao > 0)
        {
            IndiceUsuario *i = busca_binaria_na_area_de_extensao_usuario(area_de_extensao_indice_exaustivo, tamanho_area_extensao, 0, target, 1);
            if (i)
            {
                int posicao_fisica = i->posicao;
                fseek(area_de_extensao_indice_exaustivo, i->posicao, SEEK_SET);
                fread(i, sizeof(IndiceUsuario), 1, area_de_extensao_indice_exaustivo);
                i->deletado = 1;
                fseek(area_de_extensao_indice_exaustivo, posicao_fisica, SEEK_SET);
                fwrite(i, sizeof(IndiceUsuario), 1, area_de_extensao_indice_exaustivo);
                fclose(area_de_extensao_indice_exaustivo);
                free(i);
                return 1;
            }
        }
    }

    FILE *indice_usuarios_exaustivo = fopen("./indices/indice_usuarios_exaustivo.bin", "rb+");
    if (!indice_usuarios_exaustivo)
    {
        perror("Failed to open index file for reading");
        return 0;
    }

    FILE *indice_usuarios_parcial = fopen("./indices/indice_usuarios_parcial.bin", "rb+");
    fseek(indice_usuarios_exaustivo, 0, SEEK_END);
    int size = ftell(indice_usuarios_exaustivo) / sizeof(IndiceUsuario);
    rewind(indice_usuarios_exaustivo);

    IndiceUsuario *bloco = busca_binaria_bloco_do_usuario(indice_usuarios_parcial, target, size / TAMANHO_DOS_BLOCOS);
    fclose(indice_usuarios_parcial);
    if (!bloco)
    {
        return 0;
    }

    IndiceUsuario *i = busca_binaria_usuario_no_bloco(indice_usuarios_exaustivo, bloco->posicao / sizeof(IndiceUsuario), target, 1);
    if (!i)
    {
        return 0;
    }

    long posicao_fisica = i->posicao;

    fseek(indice_usuarios_exaustivo, i->posicao, SEEK_SET);
    fread(i, sizeof(IndiceUsuario), 1, indice_usuarios_exaustivo);
    i->deletado = 1;
    fseek(indice_usuarios_exaustivo, posicao_fisica, SEEK_SET);
    fwrite(i, sizeof(IndiceUsuario), 1, indice_usuarios_exaustivo);
    fclose(indice_usuarios_exaustivo);
    free(i);

    return 1;
}