import os

# if not os . path . exists (" novo_diretorio "):
#     os . mkdir (" novo_diretorio ")
#     arquivos = os.remove('C:\Users\professor\Documents\python\kaka.txt')
#     print ( arquivos)

caminho = r'C:\Users\professor\Documents\python\kaka.txt'
if os.path.exists(caminho):
    print(caminho)
    arquivos = os.remove(caminho)