// A ideia é pegar o índice exaustivo e separar eles em blocos
// E gerar um índice que a cada X entradas aponta pro início de uma, quase como um índice multicamadas

#define TAMANHO_DOS_BLOCOS 10000

#include "../include/TDE.h"
#include <stdlib.h>
#include <stdio.h>

void gerar_indice_parcial_produtos()
{
    FILE *indice_parcial_produtos = fopen("./indices/indice_produtos_parcial.bin", "wb");
    if (indice_parcial_produtos == NULL)
    {
        perror("Failed to open index file for writing");
        return;
    }

    FILE *indice_exaustivo_produtos = fopen("./indices/indice_produtos_exaustivo.bin", "rb");
    if (indice_exaustivo_produtos == NULL)
    {
        perror("Failed to open index file for reading");
        fclose(indice_parcial_produtos);
        return;
    }

    IndiceProduto *buffer = malloc(sizeof(IndiceProduto));
    int counter = 0;
    while (fread(buffer, sizeof(IndiceProduto), 1, indice_exaustivo_produtos))
    {
        if (counter % TAMANHO_DOS_BLOCOS == 0 || counter == 0)
        {
            buffer->posicao = ftell(indice_exaustivo_produtos) - sizeof(IndiceProduto);
            fwrite(buffer, sizeof(IndiceProduto), 1, indice_parcial_produtos);
        }

        counter++;
    }

    fclose(indice_parcial_produtos);
    fclose(indice_exaustivo_produtos);
}

void gerar_indice_parcial_usuarios()
{
    FILE *indice_parcial_usuarios = fopen("./indices/indice_usuarios_parcial.bin", "wb");
    if (indice_parcial_usuarios == NULL)
    {
        perror("Failed to open index file for writing");
        return;
    }

    FILE *indice_exaustivo_usuarios = fopen("./indices/indice_usuarios_exaustivo.bin", "rb");
    if (indice_exaustivo_usuarios == NULL)
    {
        perror("Failed to open index file for reading");
        fclose(indice_parcial_usuarios);
        return;
    }

    IndiceUsuario *buffer = malloc(sizeof(IndiceUsuario));
    int counter = 0;
    while (fread(buffer, sizeof(IndiceUsuario), 1, indice_exaustivo_usuarios))
    {
        if (counter % TAMANHO_DOS_BLOCOS == 0 || counter == 0)
        {
            buffer->posicao = ftell(indice_exaustivo_usuarios) - sizeof(IndiceUsuario);
            fwrite(buffer, sizeof(IndiceUsuario), 1, indice_parcial_usuarios);
        }

        counter++;
    }

    fclose(indice_parcial_usuarios);
    fclose(indice_exaustivo_usuarios);
}

// int main(int argc, char const *argv[])
// {
//     gerar_indice_parcial_usuarios();
//     gerar_indice_parcial_usuarios();

//     return 0;
// }
