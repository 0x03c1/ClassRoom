# Projetos para consolidar o aprendizado

Escolha projetos de dificuldade crescente. Em todos, mantenha um README com requisitos, como executar e exemplos de uso.

## Projeto 1 — Calculadora de terminal

**Nível:** iniciante.

Requisitos:
- operações aritméticas básicas;
- entrada de dados;
- validação de entrada;
- mensagens de erro claras.

Conceitos: `IO`, funções, `Maybe`/`Either`, tipos numéricos.

## Projeto 2 — Analisador de texto

**Nível:** iniciante a intermediário.

Requisitos:
- ler um arquivo;
- contar linhas, palavras e caracteres;
- listar palavras mais frequentes;
- ignorar maiúsculas/minúsculas opcionalmente.

Conceitos: listas, `Map`, funções de ordem superior, IO.

## Projeto 3 — Gerenciador de tarefas

**Nível:** intermediário.

Requisitos:
- criar, listar, concluir e remover tarefas;
- representar status com tipo algébrico;
- salvar e carregar dados;
- validar entradas.

Conceitos: modelagem de domínio, módulos, serialização, tratamento de erros.

## Projeto 4 — Jogo de adivinhação

**Nível:** intermediário.

Requisitos:
- escolher um número;
- receber tentativas;
- informar se o palpite é maior ou menor;
- limitar tentativas;
- validar entrada inválida.

Conceitos: recursão, `IO`, estado explícito e funções puras.

## Projeto 5 — Interpretador de expressões aritméticas

**Nível:** intermediário a avançado.

Requisitos:
- reconhecer números, parênteses e operadores;
- construir uma árvore sintática;
- respeitar precedência;
- produzir mensagens de erro;
- avaliar a expressão.

Conceitos: tipos algébricos, parsers, recursão, `Either`.

## Projeto 6 — API HTTP

**Nível:** avançado.

Requisitos:
- expor endpoints;
- validar entrada;
- modelar domínio;
- organizar módulos;
- adicionar testes;
- documentar a API.

Escolha uma biblioteca HTTP compatível com sua versão do GHC e siga a documentação oficial do pacote.

Conceitos: Cabal, JSON, IO, testes, arquitetura.

## Projeto 7 — Pipeline de processamento de dados

**Nível:** avançado.

Requisitos:
- ler dados CSV ou JSON;
- validar e normalizar registros;
- calcular estatísticas;
- separar erros dos resultados válidos;
- medir uso de memória e tempo.

Conceitos: transformações puras, tipos de erro, estruturas de dados e profiling.

## Projeto final — Um pacote publicável

Crie um pacote com:
- biblioteca reutilizável;
- executável de demonstração;
- testes unitários e de propriedades;
- documentação;
- integração contínua;
- instruções de instalação e uso;
- decisões de arquitetura registradas.

## Rubrica de avaliação

Avalie cada projeto de 0 a 2 em cada critério:
1. Correção funcional.
2. Modelagem com tipos.
3. Clareza e organização.
4. Tratamento de erros.
5. Testes.
6. Documentação.

Pontuação máxima: 12. A meta não é apenas “funcionar”, mas tornar o programa compreensível, verificável e fácil de evoluir.
