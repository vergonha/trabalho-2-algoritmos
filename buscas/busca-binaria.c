#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/TDE.h"
#define TAMANHO_DOS_BLOCOS 10000

IndiceProduto *busca_binaria_na_area_de_extensao_produto(FILE *f, int high, int low, char *target, int troca_posicao)
{
    IndiceProduto *buffer = malloc(sizeof(IndiceProduto));
    if (!buffer)
    {
        fprintf(stderr, "Erro ao alocar memória para buffer.\n");
        return NULL;
    }

    while (low <= high)
    {
        int mid = low + (high - low) / 2;

        fseek(f, mid * sizeof(IndiceProduto), SEEK_SET);
        if (fread(buffer, sizeof(IndiceProduto), 1, f) != 1)
        {
            if (troca_posicao == 1)
            {
                buffer->posicao = ftell(f) - sizeof(IndiceProduto);
            }
            return NULL;
        }

        int cmp = strcmp(buffer->sessao_usuario, target);
        if (cmp == 0)
        {
            if (troca_posicao == 1)
            {
                buffer->posicao = ftell(f) - sizeof(IndiceProduto);
            }
            return buffer;
        }

        if (cmp < 0)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }

    free(buffer);
    return NULL;
}

// A busca binária no segundo nível, no índice parcial, consiste em verificar se o hash está dentro do bloco
// Como vou saber? Se o strcmp for menor que 0, eu sei que o hash está no bloco anterior
IndiceProduto *busca_binaria_bloco_do_produto(FILE *f, char *target, int num_blocos)
{
    int low = 0;
    int high = num_blocos - 1;
    IndiceProduto *hash_inicial = malloc(sizeof(IndiceProduto));
    IndiceProduto *hash_final = malloc(sizeof(IndiceProduto));

    while (low <= high)
    {
        int mid = (low + high) / 2;
        long offset = sizeof(IndiceProduto) * mid;

        fseek(f, offset, SEEK_SET);
        if (fread(hash_inicial, sizeof(IndiceProduto), 1, f) != 1)
        {
            free(hash_inicial);
            free(hash_final);
            perror("Erro ao ler hash_inicial");
            return NULL; // Ler falhou
        }
        // Ler o hash final, que deve estar na próxima posição
        if (fread(hash_final, sizeof(IndiceProduto), 1, f) != 1)
        {
            free(hash_inicial);
            free(hash_final);
            perror("Erro ao ler hash_final");
            return NULL; // Ler falhou
        }

        int cmp_inicial = strcmp(target, hash_inicial->sessao_usuario);
        int cmp_final = strcmp(target, hash_final->sessao_usuario);

        // Verifique se o hash_alvo está dentro do intervalo do bloco
        if (cmp_inicial == 0)
        {
            // O target é igual ao hash inicial
            free(hash_final);
            return hash_inicial;
        }
        else if (cmp_final == 0)
        {
            free(hash_inicial);
            return hash_final;
        }
        else if (cmp_inicial > 0 && cmp_final > 0)
        {
            // O target está fora do intervalo
            low = mid + 1;
        }
        else if (cmp_inicial < 0)
        {
            // O target é menor que o hash inicial
            high = mid - 1;
        }
        else
        {
            // O target está entre hash_inicial e hash_final
            free(hash_final);
            return hash_inicial; // Retorna hash_inicial
        }
    }

    // Target not found
    free(hash_inicial);
    free(hash_final);
    return NULL;
}

// o troca_posicao é um parâmetro que indica se a função deve trocar a posição do ponteiro do arquivo ou o endereço físico do índice
IndiceProduto *busca_binaria_produto_no_bloco(FILE *f, long bloco_inicial, char *target, int troca_posicao)
{
    IndiceProduto *buffer = malloc(sizeof(IndiceProduto));
    if (!buffer)
    {
        fprintf(stderr, "Erro ao alocar memória para buffer.\n");
        return NULL; // Verifique se a alocação de memória foi bem-sucedida
    }

    int low = 0;                       // O índice inicial do bloco
    int high = TAMANHO_DOS_BLOCOS - 1; // O índice final do bloco
    long offset = bloco_inicial * sizeof(IndiceProduto);

    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        // Move para a posição correta no arquivo
        fseek(f, offset + mid * sizeof(IndiceProduto), SEEK_SET);
        if (fread(buffer, sizeof(IndiceProduto), 1, f) != 1)
        {
            // fprintf(stderr, "Erro ao ler o buffer na posição %ld.\n", offset + mid * sizeof(IndiceProduto));
            // free(buffer);
            if (troca_posicao == 1)
            {
                buffer->posicao = ftell(f) - sizeof(IndiceProduto);
            }
            return NULL; // Ler falhou
        }

        int cmp = strcmp(buffer->sessao_usuario, target);
        if (cmp == 0)
        {
            if (troca_posicao == 1)
            {
                buffer->posicao = ftell(f) - sizeof(IndiceProduto);
            }
            return buffer; // Produto encontrado
        }

        if (cmp < 0)
        {
            low = mid + 1; // Busca na metade superior
        }
        else
        {
            high = mid - 1; // Busca na metade inferior
        }
    }

    // Target not found
    // free(buffer);
    return NULL;
}

Produto *buscar_produto(char *target)
{
    FILE *indice_produtos_exaustivo = fopen("./indices/indice_produtos_exaustivo.bin", "rb");
    FILE *indice_produtos_parcial = fopen("./indices/indice_produtos_parcial.bin", "rb");

    if (!indice_produtos_exaustivo || !indice_produtos_parcial)
    {
        perror("Erro ao abrir o arquivo de índice");
        return NULL;
    }

    FILE *indice_produtos_exaustivo_area_extensao = fopen("./indices/indice_produtos_exaustivo_area_de_extensao.bin", "rb");
    if (indice_produtos_exaustivo_area_extensao != NULL)
    {
        fseek(indice_produtos_exaustivo_area_extensao, 0, SEEK_END);
        int tamanho_area_extensao = ftell(indice_produtos_exaustivo_area_extensao) / sizeof(IndiceProduto);
        rewind(indice_produtos_exaustivo_area_extensao);

        if (tamanho_area_extensao > 0)
        {
            IndiceProduto *i = busca_binaria_na_area_de_extensao_produto(indice_produtos_exaustivo_area_extensao, tamanho_area_extensao, 0, target, 0);
            fclose(indice_produtos_exaustivo_area_extensao);

            if (i)
            {
                if (i->deletado == 1)
                {
                    printf("Produto deletado\n");
                    return NULL;
                }

                FILE *dados_area_extensao = fopen("./areas_extensao/produtos_area_extensao.bin", "rb");
                fseek(dados_area_extensao, i->posicao, SEEK_SET);
                Produto *p = malloc(sizeof(Produto));
                fread(p, sizeof(Produto), 1, dados_area_extensao);
                fclose(dados_area_extensao);

                return p;
            }
        }
    }

    fseek(indice_produtos_exaustivo, 0, SEEK_END);
    int size = ftell(indice_produtos_exaustivo) / sizeof(IndiceProduto);
    rewind(indice_produtos_exaustivo);

    IndiceProduto *bloco = busca_binaria_bloco_do_produto(indice_produtos_parcial, target, size / TAMANHO_DOS_BLOCOS);
    fclose(indice_produtos_parcial);
    if (!bloco)
    {
        return NULL;
    }

    IndiceProduto *i = busca_binaria_produto_no_bloco(indice_produtos_exaustivo, bloco->posicao / sizeof(IndiceProduto), target, 0);
    if (i)
    {
        if (i->deletado == 1)
        {
            printf("Produto deletado\n");
            return NULL;
        }
        fclose(indice_produtos_exaustivo);
        FILE *dados_produtos = fopen("./dados/dados_produtos.bin", "rb");
        fseek(dados_produtos, i->posicao, SEEK_SET);
        Produto *p = malloc(sizeof(Produto));
        fread(p, sizeof(Produto), 1, dados_produtos);
        fclose(dados_produtos);

        return p;
    }
}

IndiceUsuario *busca_binaria_na_area_de_extensao_usuario(FILE *f, int high, int low, char *target, int troca_posicao)
{
    IndiceUsuario *buffer = malloc(sizeof(IndiceUsuario));
    if (!buffer)
    {
        fprintf(stderr, "Erro ao alocar memória para buffer.\n");
        return NULL;
    }

    while (low <= high)
    {
        int mid = low + (high - low) / 2;

        fseek(f, mid * sizeof(IndiceUsuario), SEEK_SET);
        if (fread(buffer, sizeof(IndiceUsuario), 1, f) != 1)
        {
            if (troca_posicao == 1)
            {
                buffer->posicao = ftell(f) - sizeof(IndiceUsuario);
            }
            return NULL;
        }

        int cmp = strcmp(buffer->sessao_usuario, target);
        if (cmp == 0)
        {
            if (troca_posicao == 1)
            {
                buffer->posicao = ftell(f) - sizeof(IndiceUsuario);
            }
            return buffer;
        }

        if (cmp < 0)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }

    free(buffer);
    return NULL;
}

IndiceUsuario *busca_binaria_bloco_do_usuario(FILE *f, char *target, int num_blocos)
{
    int low = 0;
    int high = num_blocos - 1;
    IndiceUsuario *hash_inicial = malloc(sizeof(IndiceUsuario));
    IndiceUsuario *hash_final = malloc(sizeof(IndiceUsuario));

    while (low <= high)
    {
        int mid = (low + high) / 2;
        long offset = sizeof(IndiceUsuario) * mid;

        fseek(f, offset, SEEK_SET);
        if (fread(hash_inicial, sizeof(IndiceProduto), 1, f) != 1)
            ;
        if (fread(hash_inicial, sizeof(IndiceUsuario), 1, f) != 1)
        {
            free(hash_inicial);
            free(hash_final);
            return NULL; // Ler falhou
        }
        // Ler o hash final, que deve estar na próxima posição
        if (fread(hash_final, sizeof(IndiceUsuario), 1, f) != 1)
        {
            free(hash_inicial);
            free(hash_final);
            return NULL; // Ler falhou
        }

        int cmp_inicial = strcmp(target, hash_inicial->sessao_usuario);
        int cmp_final = strcmp(target, hash_final->sessao_usuario);

        // Verifique se o hash_alvo está dentro do intervalo do bloco
        if (cmp_inicial == 0)
        {
            // O target é igual ao hash inicial
            free(hash_final);
            return hash_inicial;
        }
        else if (cmp_final == 0)
        {
            free(hash_inicial);
            return hash_final;
        }
        else if (cmp_inicial > 0 && cmp_final > 0)
        {
            // O target está fora do intervalo
            low = mid + 1;
        }
        else if (cmp_inicial < 0)
        {
            // O target é menor que o hash inicial
            high = mid - 1;
        }
        else
        {
            // O target está entre hash_inicial e hash_final
            free(hash_final);
            return hash_inicial; // Retorna hash_inicial
        }
    }

    // Target not found
    free(hash_inicial);
    free(hash_final);
    return NULL;
}

IndiceUsuario *busca_binaria_usuario_no_bloco(FILE *f, long bloco_inicial, char *target, int troca_posicao)
{
    IndiceUsuario *buffer = malloc(sizeof(IndiceUsuario));
    if (!buffer)
    {
        fprintf(stderr, "Erro ao alocar memória para buffer.\n");
        return NULL; // Verifique se a alocação de memória foi bem-sucedida
    }

    int low = 0;                       // O índice inicial do bloco
    int high = TAMANHO_DOS_BLOCOS - 1; // O índice final do bloco

    long offset = bloco_inicial * sizeof(IndiceUsuario);
    while (low <= high)
    {
        int mid = low + (high - low) / 2;

        // Move para a posição correta no arquivo
        fseek(f, offset + mid * sizeof(IndiceUsuario), SEEK_SET);
        if (fread(buffer, sizeof(IndiceUsuario), 1, f) != 1)
        {
            if (troca_posicao == 1)
            {
                buffer->posicao = ftell(f) - sizeof(IndiceUsuario);
            }
            return NULL; // Ler falhou
        }

        int cmp = strcmp(buffer->sessao_usuario, target);
        if (cmp == 0)
        {
            if (troca_posicao == 1)
            {
                buffer->posicao = ftell(f) - sizeof(IndiceUsuario);
            }
            return buffer; // Usuário encontrado
        }

        if (cmp < 0)
        {
            low = mid + 1; // Busca na metade superior
        }
        else
        {
            high = mid - 1; // Busca na metade inferior
        }
    }

    // Target not found
    free(buffer);
    return NULL;
}

Usuario *buscar_usuario(char *target)
{
    FILE *indice_usuarios_exaustivo = fopen("./indices/indice_usuarios_exaustivo.bin", "rb");
    FILE *indice_usuarios_parcial = fopen("./indices/indice_usuarios_parcial.bin", "rb");

    FILE *indice_usuarios_exaustivo_area_extensao = fopen("./indices/indice_usuarios_exaustivo_area_de_extensao.bin", "rb");
    if (indice_usuarios_exaustivo_area_extensao != NULL)
    {
        fseek(indice_usuarios_exaustivo_area_extensao, 0, SEEK_END);
        int tamanho_area_extensao = ftell(indice_usuarios_exaustivo_area_extensao) / sizeof(IndiceUsuario);
        rewind(indice_usuarios_exaustivo_area_extensao);
        if (tamanho_area_extensao > 0)
        {
            IndiceUsuario *i = busca_binaria_na_area_de_extensao_usuario(indice_usuarios_exaustivo_area_extensao, tamanho_area_extensao, 0, target, 0);
            fclose(indice_usuarios_exaustivo_area_extensao);

            if (i)
            {
                if (i->deletado == 1)
                {
                    printf("Usuário deletado\n");
                    return NULL;
                }
                FILE *dados_area_extensao = fopen("./areas_extensao/usuarios_area_extensao.bin", "rb");
                fseek(dados_area_extensao, i->posicao, SEEK_SET);
                Usuario *u = malloc(sizeof(Usuario));
                fread(u, sizeof(Usuario), 1, dados_area_extensao);
                fclose(dados_area_extensao);

                return u;
            }
        }
    }

    fseek(indice_usuarios_exaustivo, 0, SEEK_END);
    int size = ftell(indice_usuarios_exaustivo) / sizeof(IndiceUsuario);
    rewind(indice_usuarios_exaustivo);

    IndiceUsuario *bloco = busca_binaria_bloco_do_usuario(indice_usuarios_parcial, target, size / TAMANHO_DOS_BLOCOS);
    fclose(indice_usuarios_parcial);

    if (!bloco)
    {
        return NULL;
    }

    IndiceUsuario *i = busca_binaria_usuario_no_bloco(indice_usuarios_exaustivo, bloco->posicao / sizeof(IndiceUsuario), target, 0);
    if (i)
    {
        if (i->deletado == 1)
        {
            printf("Usuário deletado\n");
            return NULL;
        }
        fclose(indice_usuarios_exaustivo);
        FILE *dados_usuarios = fopen("./dados/dados_usuarios.bin", "rb");
        fseek(dados_usuarios, i->posicao, SEEK_SET);
        Usuario *u = malloc(sizeof(Usuario));
        fread(u, sizeof(Usuario), 1, dados_usuarios);
        fclose(dados_usuarios);
        return u;
    }
}
