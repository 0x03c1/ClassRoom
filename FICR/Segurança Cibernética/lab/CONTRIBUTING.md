# Contribuindo com o Repositório

Este é um material didático vivo — correções, melhorias e novos conteúdos são bem-vindos, especialmente de alunos e colegas docentes.

## Como contribuir

1. **Fork** este repositório
2. Crie uma branch com nome descritivo: `correcao-modulo-04` ou `novo-lab-docker`
3. Faça suas mudanças em Markdown
4. Abra um **Pull Request** descrevendo:
   - O que mudou
   - Por quê
   - Em qual(is) módulo(s)

## Tipos de contribuição aceitas

| Rótulo | Descrição |
|---|---|
| `[correção]` | Erros técnicos, typos, comandos errados |
| `[atualização]` | Ferramenta descontinuada, novo release, mudança de flag |
| `[novo-conteúdo]` | Novo módulo, novo lab, novo exercício |
| `[melhoria]` | Reescrita clareza, melhor exemplo |
| `[tradução]` | Versão em inglês/espanhol |

## Padrões de escrita

- **Português brasileiro** como idioma padrão
- **Markdown GFM** (GitHub Flavored Markdown)
- Blocos de código com linguagem especificada (` ```bash `, ` ```python `)
- Frontmatter não é necessário
- Navegação entre módulos nas bordas superior e inferior
- Todo comando deve ser **testado** antes do commit

## Estrutura de um módulo

```markdown
# NN · Título do Módulo

> [← Anterior](../...) · [Voltar ao índice](../../README.md) · [Próximo →](../...)

## Objetivos
...

## 1. Tópico
...

## Exercícios
...

[← Anterior](...) · [Voltar ao índice](...) · [Próximo →](...)
```

## Checklist antes do PR

- [ ] Testei todos os comandos citados
- [ ] Conferi links externos (que funcionam)
- [ ] Mantive o tom didático (não é blog post técnico)
- [ ] Adicionei exercícios ao final se criei novo módulo
- [ ] Navegação anterior/próximo funciona
- [ ] Sem informações sensíveis (tokens, IPs reais, credenciais)

## Código de Conduta

Este é ambiente educacional. Seja respeitoso, construtivo e ético. Material que incentive uso malicioso será rejeitado.
