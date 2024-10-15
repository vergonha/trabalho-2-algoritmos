# Organizações de Arquivos e Índices

Esse repositório temporário tem como principal objetivo descrever o trabalho feito para a matéria de Algoritmos e Estruturas de Dados ll.

O trabalho utilizou uma extração de dados sobre produtos/compras de um e-commerce disponibilizado no Kaggle. Por motivos óbvios, o dataset de 9GB não está incluído nesse repositório e deve ser baixado através do [seguinte endereço](https://www.kaggle.com/datasets/mkechinov/ecommerce-behavior-data-from-multi-category-store/data).

## Organização dos Dados

De todas as colunas presentes no arquivo original, elas foram separadas em dois arquivos: `dados_produtos.csv` e `dados_usuarios.csv`, contendo as seguintes colunas:

Colunas presentes na estrutura dos `Produtos`:

| id_categoria | id_produto | codigo_categoria | marca | preco | sessao_usuario 🔑 |
| ------------ | ---------- | ---------------- | ----- | ----- | ----------------- |

Colunas presentes na estrutura dos `Usuários`:

| tipo_evento | data | id_usuario | id_produto | id_usuario | sessao_usuario 🔑 |
| ----------- | ---- | ---------- | ---------- | ---------- | ----------------- |

Cada coluna corresponde a um campo nas seguintes estruturas de dados:

```c
typedef struct Produto
{
    long id_categoria;
    int id_produto;
    char codigo_categoria[32];
    char marca[12];
    float preco;
    char sessao_usuario[37];
} Produto;
```

```c
typedef struct Usuario
{
    char tipo_evento[12];
    char data[22];
    int id_usuario;
    char sessao_usuario[37];
    int id_produto;
} Usuario;
```

Posteriormente, cada linha de seu respectivo arquivo CSV foi convertida para um arquivo binário, `dados_produtos.bin` e `dados_usuarios.bin`. O parse desses dados foi feito no arquivo `pre-processamento/transformar_dados.py` e `pre-processamento/converter_para_binario.c`.

## Geração de Índices Parciais e Exaustivos

Os primeiros índices são criados para os produtos e usuários existentes no csv. Cada entrada no arquivo binário convertido deve ter seu endereço físico apontado por uma entrada no índice exaustivo.

A estrutura do dado de índice exaustivo:

```c
typedef struct IndiceProduto
{
    char sessao_usuario[37];
    long posicao;
    int deletado;
} IndiceProduto;
```

```c
typedef struct IndiceUsuario
{
    char sessao_usuario[37];
    long posicao;
    int deletado;
} IndiceUsuario;
```

Um arquivo de índice exaustivo é gerado, ordenado pelo campo `sessao_usuario`, apontando cada registro para uma entrada no arquivo binário. Existem registros parciais e exaustivos tanto para produtos quanto para usuários.

Após isso, é gerado um arquivo de índice parcial. O número 0 e a cada 10.000 registros, um início de bloco é gravado no arquivo de índice parcial. Ou seja, existe um índice com a posição do registro número 1, depois 10.000, depois 20.000, separando-os em blocos.

Assim, posso fazer uma pesquisa binária para encontrar o bloco, limitando a próxima pesquisa binária no arquivo binário em um intervalo menor de registros, aumentando drasticamente a performance da busca.

## Inserção e Remoção

Os registros inseridos são adicionados em outro arquivo chamado de `area_de_extensao`, existe um para produtos e outro para usuários.

A cada inserção, é gerado um índice exaustivo para a área de extensão. Isso não é problema, a geração é do índice ordenado é rápida, visto que não são muitos arquivos que são adicionados.

A remoção, tanto na área de extensão quanto nos outros índices ocorre da mesma forma. O produto ou usuário é localizado e seu registro é regravado mudando o campo `deletado` para `1`. Assim, a busca é capaz de ignorar os registros excluídos.

## Reorganização

Os arquivos da área de extensão tem seus registros concatenados no arquivo binário original, os índices antigos são apagados e gerados novamente, com exceção dos registros excluídos, que são ignorados.
