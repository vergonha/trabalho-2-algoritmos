#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/TDE.h"

int main(int argc, char const *argv[])
{

    // Pra evitar que eu compile sem querer, execute e perca todos os dados binários que já estão OK
    // exit(1);
    FILE *arquivo_origem = fopen("./dados/dados_produtos.csv", "r");
    FILE *arquivo_destino = fopen("./dados/dados_produtos.bin", "wb");

    if (arquivo_origem == NULL)
    {
        perror("Failed to open input file for produtos");
        return EXIT_FAILURE;
    }
    if (arquivo_destino == NULL)
    {
        perror("Failed to open output file for produtos");
        fclose(arquivo_origem);
        return EXIT_FAILURE;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), arquivo_origem))
    {
        Produto *produto = malloc(sizeof(Produto));
        if (produto == NULL)
        {
            perror("Failed to allocate memory for Produto");
            continue;
        }

        char *token;

        // Parse id_categoria
        token = strtok(buffer, ",");
        produto->id_categoria = (token != NULL) ? strtol(token, NULL, 10) : -1;

        // Parse id_produto
        token = strtok(NULL, ",");
        produto->id_produto = (token != NULL) ? strtol(token, NULL, 10) : -1;

        // Parse codigo_categoria
        token = strtok(NULL, ",");
        strncpy(produto->codigo_categoria, token ? token : "N/A", sizeof(produto->codigo_categoria) - 1);
        produto->codigo_categoria[sizeof(produto->codigo_categoria) - 1] = '\0'; // Ensure null-termination

        // Parse marca
        token = strtok(NULL, ",");
        strncpy(produto->marca, token ? token : "N/A", sizeof(produto->marca) - 1);
        produto->marca[sizeof(produto->marca) - 1] = '\0'; // Ensure null-termination

        // Parse preco
        token = strtok(NULL, ",");
        produto->preco = (token != NULL) ? strtof(token, NULL) : 0.0f;

        // Parse sessao_usuario
        token = strtok(NULL, "\n");
        strncpy(produto->sessao_usuario, token ? token : "N/A", sizeof(produto->sessao_usuario) - 1);
        produto->sessao_usuario[sizeof(produto->sessao_usuario) - 1] = '\0'; // Ensure null-termination

        // Write the structure to the output file
        fwrite(produto, sizeof(Produto), 1, arquivo_destino);
        free(produto);
    }

    fclose(arquivo_origem);
    fclose(arquivo_destino);

    arquivo_origem = fopen("./dados/dados_usuarios.csv", "r");
    arquivo_destino = fopen("./dados/dados_usuarios.bin", "wb");

    if (arquivo_origem == NULL)
    {
        perror("Failed to open input file for usuarios");
        return EXIT_FAILURE;
    }
    if (arquivo_destino == NULL)
    {
        perror("Failed to open output file for usuarios");
        fclose(arquivo_origem);
        return EXIT_FAILURE;
    }

    while (fgets(buffer, sizeof(buffer), arquivo_origem))
    {
        Usuario *usuario = malloc(sizeof(Usuario));
        if (usuario == NULL)
        {
            perror("Failed to allocate memory for Usuario");
            continue;
        }

        char *token;

        // Parse tipo_evento
        token = strtok(buffer, ",");
        strncpy(usuario->tipo_evento, token ? token : "N/A", sizeof(usuario->tipo_evento) - 1);
        usuario->tipo_evento[sizeof(usuario->tipo_evento) - 1] = '\0'; // Ensure null-termination

        // Parse data
        token = strtok(NULL, ",");
        strncpy(usuario->data, token ? token : "N/A", sizeof(usuario->data) - 1);
        usuario->data[sizeof(usuario->data) - 1] = '\0'; // Ensure null-termination

        // Parse id_usuario
        token = strtok(NULL, ",");
        usuario->id_usuario = (token != NULL) ? strtol(token, NULL, 10) : -1;

        // Parse sessao_usuario
        token = strtok(NULL, ",");
        strncpy(usuario->sessao_usuario, token ? token : "N/A", sizeof(usuario->sessao_usuario) - 1);
        usuario->sessao_usuario[sizeof(usuario->sessao_usuario) - 1] = '\0'; // Ensure null-termination

        // Parse id_produto
        token = strtok(NULL, "\n");
        usuario->id_produto = (token != NULL) ? strtol(token, NULL, 10) : -1;

        // Write the structure to the output file
        fwrite(usuario, sizeof(Usuario), 1, arquivo_destino);
        free(usuario);
    }

    fclose(arquivo_origem);
    fclose(arquivo_destino);

    return 0;
}
