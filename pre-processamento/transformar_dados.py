import pandas as pd

def carregar_dataset(chunksize=100000):
    for chunk in pd.read_csv('./dados/dados_completos.csv', chunksize=chunksize):
        separar_dados_usuarios(chunk)
        separar_dados_produto(chunk)

def separar_dados_usuarios(dataset):
    # Select relevant columns
    dados_usuarios = dataset[['event_type', 'event_time', 'user_id', 'user_session', 'product_id']].copy()
    
    # Fill empty strings with "N/A" and numeric columns with -1 using .loc
    dados_usuarios.loc[:, 'event_type'] = dados_usuarios['event_type'].fillna('N/A')
    dados_usuarios.loc[:, 'event_time'] = dados_usuarios['event_time'].fillna('N/A')
    dados_usuarios.loc[:, 'user_id'] = dados_usuarios['user_id'].fillna(-1)
    dados_usuarios.loc[:, 'user_session'] = dados_usuarios['user_session'].fillna('N/A')
    dados_usuarios.loc[:, 'product_id'] = dados_usuarios['product_id'].fillna(-1)

    salvar_dados(dados_usuarios, 'dados_usuarios')

def separar_dados_produto(dataset):
    # Select relevant columns
    dados_produtos = dataset[['category_id', 'product_id', 'category_code', 'brand', 'price', 'user_session']].copy()
    
    # Fill empty strings with "N/A" and numeric columns with -1 using .loc
    dados_produtos.loc[:, 'category_id'] = dados_produtos['category_id'].fillna(-1)
    dados_produtos.loc[:, 'product_id'] = dados_produtos['product_id'].fillna(-1)
    dados_produtos.loc[:, 'category_code'] = dados_produtos['category_code'].fillna('N/A')
    dados_produtos.loc[:, 'brand'] = dados_produtos['brand'].fillna('N/A')
    dados_produtos.loc[:, 'price'] = dados_produtos['price'].fillna(-1)
    dados_produtos.loc[:, 'user_session'] = dados_produtos['user_session'].fillna('N/A')

    salvar_dados(dados_produtos, 'dados_produtos')

def salvar_dados(dados, nome_arquivo):
    # Save DataFrame to CSV, appending if the file already exists
    dados.to_csv(f'./dados/{nome_arquivo}.csv', mode='a', header=False, index=False)

if __name__ == '__main__':
    carregar_dataset()
