#include "operacoes/operacoes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // For sleep()
#include <ctype.h>

// Códigos ANSI para cores
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define CYAN "\033[36m"

// Função para limpar o terminal
void clear_screen()
{
    printf("\033[H\033[J");
}

// Função para exibir o menu principal
void mostrar_menu()
{
    clear_screen();
    printf(BLUE "==========📦 Gestão de Produtos e Usuários ==========\n" RESET);
    printf(CYAN "1️⃣  Inserir Produto\n" RESET);
    printf(CYAN "2️⃣  Inserir Usuário\n" RESET);
    printf(CYAN "3️⃣  Remover Produto\n" RESET);
    printf(CYAN "4️⃣  Remover Usuário\n" RESET);
    printf(CYAN "5️⃣  Pesquisar Produto\n" RESET);
    printf(CYAN "6️⃣  Pesquisar Usuário\n" RESET);
    printf(CYAN "7️⃣  Reorganizar Dados\n" RESET);
    printf(CYAN "8️⃣  Sair\n" RESET);
    printf(BLUE "=====================================================\n" RESET);
    printf(YELLOW "Escolha uma opção: " RESET);
}

// Função para inserir produto
void inserir_produto_interface()
{
    Produto *p = (Produto *)malloc(sizeof(Produto)); // Alocação dinâmica
    if (p == NULL)
    {
        printf(RED "❌ Erro ao alocar memória para o produto.\n" RESET);
        return;
    }

    clear_screen();
    printf(GREEN "🆕 Inserindo Produto...\n" RESET);
    printf("ID da Categoria (long): ");
    scanf("%ld", &p->id_categoria);
    printf("ID do Produto (int): ");
    scanf("%d", &p->id_produto);
    printf("Código da Categoria (string de até 32 caracteres): ");
    scanf("%s", p->codigo_categoria);
    printf("Marca (string de até 12 caracteres): ");
    scanf("%s", p->marca);
    printf("Preço (float): ");
    scanf("%f", &p->preco);
    printf("Sessão do Usuário (string de até 37 caracteres): ");
    scanf("%s", p->sessao_usuario);

    if (buscar_produto(p->codigo_categoria) != NULL)
    {
        printf(RED "❌ Produto já existe.\n" RESET);
        sleep(2);
        return;
    }

    // Aqui chamamos a função inserir_produto(p)
    if (inserir_produto(p))
    {
        printf(GREEN "✅ Produto inserido com sucesso!\n" RESET);
    }
    else
    {
        printf(RED "❌ Falha ao inserir o produto.\n" RESET);
    }

    // free(p);  // Liberar a memória alocada
    sleep(2); // Pausa para o usuário ler a mensagem
}

// Função para inserir usuário
void inserir_usuario_interface()
{
    Usuario *u = (Usuario *)malloc(sizeof(Usuario)); // Alocação dinâmica
    if (u == NULL)
    {
        printf(RED "❌ Erro ao alocar memória para o usuário.\n" RESET);
        return;
    }

    clear_screen();
    printf(GREEN "🆕 Inserindo Usuário...\n" RESET);
    printf("Tipo de Evento (string de até 12 caracteres): ");
    scanf("%s", u->tipo_evento);
    printf("Data (string de até 22 caracteres): ");
    scanf("%s", u->data);
    printf("ID do Usuário (int): ");
    scanf("%d", &u->id_usuario);
    printf("Sessão do Usuário (string de até 37 caracteres): ");
    scanf("%s", u->sessao_usuario);
    printf("ID do Produto (int): ");
    scanf("%d", &u->id_produto);

    if (buscar_usuario(u->sessao_usuario))
    {
        printf(RED "❌ Usuário já existe.\n" RESET);
        sleep(2);
        return;
    }

    // Aqui chamamos a função inserir_usuario(u)
    if (inserir_usuario(u))
    {
        printf(GREEN "✅ Usuário inserido com sucesso!\n" RESET);
    }
    else
    {
        printf(RED "❌ Falha ao inserir o usuário.\n" RESET);
    }

    // free(u);  // Liberar a memória alocada
    sleep(2); // Pausa para o usuário ler a mensagem
}

// Função para remover produto
void remover_produto_interface()
{
    char target[37];
    clear_screen();
    printf(RED "🗑️  Remover Produto...\n" RESET);
    printf("Digite o código do produto (string de até 12 caracteres): ");
    scanf("%s", target);

    // Aqui chamamos a função remover_produto(target)
    if (remover_produto(target))
    {
        printf(GREEN "✅ Produto removido com sucesso!\n" RESET);
    }
    else
    {
        printf(RED "❌ Falha ao remover o produto.\n" RESET);
    }
    sleep(2); // Pausa para o usuário ler a mensagem
}

// Função para remover usuário
void remover_usuario_interface()
{
    char target[37];
    clear_screen();
    printf(RED "🗑️  Remover Usuário...\n" RESET);
    printf("Digite a sessão do usuário (string de até 37 caracteres): ");
    scanf("%s", target);

    // Aqui chamamos a função remover_usuario(target)
    if (remover_usuario(target))
    {
        printf(GREEN "✅ Usuário removido com sucesso!\n" RESET);
    }
    else
    {
        printf(RED "❌ Falha ao remover o usuário.\n" RESET);
    }
    sleep(2); // Pausa para o usuário ler a mensagem
}

// Função para pesquisar produto
void pesquisar_produto_interface()
{
    char target[37];
    clear_screen();
    printf(YELLOW "🔍 Pesquisar Produto...\n" RESET);
    printf("Digite o código do produto (string de até 12 caracteres): ");
    scanf("%s", target);

    // Aqui seria chamada uma função que busca o produto pelo código
    Produto *p = buscar_produto(target);
    // Simulando a pesquisa, exemplo:
    if (p)
    {
        printf(GREEN "✅ Produto encontrado!\n" RESET);
        printf("ID da Categoria (long): %ld\n", p->id_categoria);
        printf("ID do Produto (int): %d\n", p->id_produto);
        printf("Código da Categoria (string): %s\n", p->codigo_categoria);
        printf("Marca (string): %s\n", p->marca);
        printf("Preço (float): %.2f\n", p->preco);
        printf("Sessão do Usuário (string): %s\n", p->sessao_usuario);
    }
    else
    {
        printf(RED "❌ Produto não encontrado.\n" RESET);
    }
    free(p);
    printf(YELLOW "Pressione qualquer tecla para voltar ao menu principal...\n" RESET);
    getchar(); // Esperar o usuário pressionar qualquer tecla
    getchar(); // Segunda chamada para capturar o 'Enter' que pode ter sido digitado anteriormente
}

// Função para pesquisar usuário
void pesquisar_usuario_interface()
{
    char target[37];
    clear_screen();
    printf(YELLOW "🔍 Pesquisar Usuário...\n" RESET);
    printf("Digite a sessão do usuário (string de até 37 caracteres): ");
    scanf("%s", target);

    // Aqui seria chamada uma função que busca o usuário pela sessão
    Usuario *u = buscar_usuario(target);
    // Simulando a pesquisa, exemplo:
    if (u)
    {
        printf(GREEN "✅ Usuário encontrado!\n" RESET);
        printf("Tipo de Evento (string): %s\n", u->tipo_evento);
        printf("Data (string): %s\n", u->data);
        printf("ID do Usuário (int): %d\n", u->id_usuario);
        printf("Sessão do Usuário (string): %s\n", u->sessao_usuario);
        printf("ID do Produto (int): %d\n", u->id_produto);
    }
    else
    {
        printf(RED "❌ Usuário não encontrado.\n" RESET);
    }
    free(u);
    printf(YELLOW "Pressione qualquer tecla para voltar ao menu principal...\n" RESET);
    getchar(); // Esperar o usuário pressionar qualquer tecla
    getchar(); // Segunda chamada para capturar o 'Enter' que pode ter sido digitado anteriormente
}

void reorganizar_dados_interface()
{
    clear_screen();
    printf(GREEN "🔄 Reorganizando Dados...\n" RESET);
    printf("> Anexando usuários da área de extensão ao arquivo de dados...\n");
    juntar_usuarios_area_de_extensao();
    sleep(2);
    printf("> Anexando produtos da área de extensão ao arquivo de dados...\n");
    juntar_produtos_area_de_extensao();
    sleep(2);
    printf("> Reorganizando índices de produtos (gerando índice exaustivo)...\n");
    gerar_produtos_indice_exaustivo();
    printf("> Reorganizando índices de produtos (gerando índice parcial)...\n");
    gerar_indice_parcial_produtos();
    printf("> Reorganizando índices de usuários (gerando índice exaustivo)...\n");
    gerar_usuarios_indice_exaustivo();
    printf("> Reorganizando índices de usuários (gerando índice parcial)...\n");
    gerar_indice_parcial_usuarios();
    printf(GREEN "✅ Dados reorganizados com sucesso!\n" RESET);
    sleep(2);
}

int main()
{

    int opcao;

    do
    {
        mostrar_menu();
        scanf("%d", &opcao);
        getchar(); // Limpar o buffer de entrada

        switch (opcao)
        {
        case 1:
            inserir_produto_interface();
            break;
        case 2:
            inserir_usuario_interface();
            break;
        case 3:
            remover_produto_interface();
            break;
        case 4:
            remover_usuario_interface();
            break;
        case 5:
            pesquisar_produto_interface();
            break;
        case 6:
            pesquisar_usuario_interface();
            break;
        case 7:
            reorganizar_dados_interface();
            break;
        case 8:
            printf(GREEN "👋 Saindo do programa...\n" RESET);
            sleep(1);
            break;
        default:
            printf(RED "❌ Opção inválida. Tente novamente.\n" RESET);
            sleep(2);
        }
    } while (opcao != 8);

    return 0;
}
