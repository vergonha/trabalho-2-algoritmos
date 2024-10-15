#include "../include/TDE.h"
#include "../indices/indices.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int inserir_usuario(Usuario *u)
{
    FILE *usuarios_area_extensao = fopen("./areas_extensao/usuarios_area_extensao.bin", "ab");
    size_t sz = fwrite(u, sizeof(Usuario), 1, usuarios_area_extensao);
    if (sz != 1)
    {
        perror("Failed to write Usuario to usuarios_area_extensao");
        fclose(usuarios_area_extensao);
        return 0;
    }
    free(u);
    fclose(usuarios_area_extensao);

    gerar_usuarios_indice_exaustivo_area_extensao();
    return 1;
}

int inserir_produto(Produto *p)
{

    FILE *produtos_area_extensao = fopen("./areas_extensao/produtos_area_extensao.bin", "ab");
    size_t sz = fwrite(p, sizeof(Produto), 1, produtos_area_extensao);
    if (sz != 1)
    {
        perror("Failed to write Produto to produtos_area_extensao");
        fclose(produtos_area_extensao);
        return 0;
    }
    free(p);
    fclose(produtos_area_extensao);

    gerar_produtos_indice_exaustivo_area_extensao();
    return 1;
}
