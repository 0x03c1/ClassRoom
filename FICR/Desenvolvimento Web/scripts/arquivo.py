# Leitura no documento
'''
with open ("lorem.txt", "r") as arquivo:
    conteudo = arquivo.read()
    print(conteudo)
'''

# Escrita no documento
'''
with open ("escrita.txt", "w") as escrita:
    conteudo = escrita.write('Qualquer coisa\n')
    conteudo = escrita.write('outra coisa')
'''

# Adicionando outro elemento ao arquivo existente
'''
with open ("escrita.txt", "a") as escrita:
    conteudo = escrita.write('\nda mesma coisa')
'''

import csv
'''
with open ('dados.csv', 'r', encoding='utf-8') as arquivo:
    linha = csv.reader(arquivo)
    for i in linha:
        print(i)
'''

import json
'''
with open('arquivo.json', 'r', encoding='utf-8') as arquivo:
    i = json.load(arquivo)
    print(i)
'''

dados = {"nome": "Roni", "idade": 25 , "cidade": "Recife"}

with open('arquivo2.json', 'w', encoding='utf-8') as arquivo:
    i = json.dump(dados, arquivo)
