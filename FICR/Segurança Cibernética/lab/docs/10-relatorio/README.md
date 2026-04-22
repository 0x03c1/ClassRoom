# 10 · Relatório e Limpeza

> [← Anterior: Exfiltração](../09-exfiltracao/README.md) · [Voltar ao índice](../../README.md) · [Próximo: Labs Práticos →](../11-labs-praticos/README.md)

---

## Princípio

O relatório é o **único entregável** do pentest. Um pentest brilhante com relatório ruim = pentest ruim aos olhos do cliente.

---

## 1. Limpeza do Ambiente (Clean-up)

**Antes** de fechar o engagement, reverta tudo que foi alterado.

### Checklist obrigatório

- [ ] Remover todas as webshells implantadas
- [ ] Remover contas de usuário criadas
- [ ] Remover backdoors, scheduled tasks, services
- [ ] Remover chaves SSH adicionadas
- [ ] Reverter alterações em arquivos de configuração
- [ ] Remover arquivos temporários e tools transferidas
- [ ] Documentar **cada artefato** implantado para o Blue Team verificar
- [ ] Remover/expirar credenciais usadas (se o cliente ainda não trocou)

### Entrega explícita ao cliente

No relatório, inclua uma seção **"Artifacts Implanted"** listando:

| Artefato | Host | Path | Removido? |
|---|---|---|---|
| Webshell `x.php` | 10.0.0.10 | /var/www/html/up/ | ✅ |
| User `pentest_tmp` | DC01 | AD | ✅ |
| Scheduled task "Updater" | WS-045 | | ✅ |

---

## 2. Estrutura Profissional do Relatório

### Template recomendado (7 seções)

```
1. Sumário Executivo
   └── 1 página. Para o CEO/CISO. Zero jargão. Impacto em $.
   
2. Escopo e Metodologia
   ├── Datas e horas
   ├── IPs/domínios em escopo
   ├── Tipos de teste realizados
   ├── Metodologia (OWASP/PTES/Kill Chain)
   └── Limitações e exclusões
   
3. Classificação de Risco
   ├── Distribuição por severidade (Critical/High/Medium/Low/Info)
   └── Matriz de risco (impacto x probabilidade)
   
4. Cadeia de Ataque (Kill Chain Narrativa)
   └── Storytelling: "Começamos por X, descobrimos Y, exploramos Z..."
   
5. Achados Detalhados (uma seção por vulnerabilidade)
   ├── ID único (VULN-001)
   ├── Título
   ├── Severidade + CVSS vetor
   ├── Descrição técnica
   ├── Request/Response de evidência
   ├── Screenshots
   ├── Passos para reprodução
   ├── Impacto para o negócio
   ├── Probabilidade de exploração
   ├── Recomendação de correção
   └── Referências (CWE, OWASP, CVE)
   
6. Recomendações Priorizadas
   ├── Curto prazo (< 30 dias)
   ├── Médio prazo (30-90 dias)
   └── Longo prazo (> 90 dias)
   
7. Anexos Técnicos
   ├── Saídas de ferramentas
   ├── Artefatos implantados (lista de limpeza)
   ├── Glossário
   └── Assinaturas e responsabilidades
```

---

## 3. Ferramentas de Relatório

### Ghostwriter

Plataforma web colaborativa para Red Teams. Templates, rastreamento de findings, geração em DOCX/PDF.

```bash
# Instalação via Docker
git clone https://github.com/GhostManager/Ghostwriter.git
cd Ghostwriter
./ghostwriter-cli install
```

### SysReptor

Open-source moderno, baseado em Markdown + LaTeX/HTML templates. Excelente para pentesters independentes.

```bash
curl -sL https://docs.sysreptor.com/install.sh | bash
```

### Pwndoc

Node.js. Templates em DOCX com placeholders.

### Dradis

Framework clássico de colaboração e geração de relatórios.

### Fluxo manual (para quem prefere)

1. Markdown → Pandoc → PDF com template LaTeX profissional
2. Word (DOCX) com style guide padronizado
3. LaTeX direto (curva de aprendizado maior, mas resultado superior)

---

## 4. CVSS v3.1 — Scoring Obrigatório

Todo achado precisa de vetor CVSS. Use a [calculadora oficial FIRST](https://www.first.org/cvss/calculator/3-1).

### Exemplo de entrada de relatório

```
ID:        VULN-003
Título:    SQL Injection em /api/v1/search
Severidade: Critical (9.8)
CVSS:      AV:N/AC:L/PR:N/UI:N/S:U/C:H/I:H/A:H
CWE:       CWE-89
OWASP:     A03:2021 - Injection

Descrição:
O parâmetro `q` do endpoint GET /api/v1/search não sanitiza entrada
do usuário antes de concatenar na query SQL. Atacante não autenticado
consegue extrair o banco de dados completo.

Impacto:
- Leitura completa da base de 2.4M clientes (PII, endereços, CPF)
- Escrita arbitrária no banco
- Execução de comandos no host (via --os-shell do SQLMap)

Prova de Conceito:
GET /api/v1/search?q=test'+OR+'1'='1 HTTP/1.1
Host: alvo.com.br
[screenshot da resposta extraindo INFORMATION_SCHEMA]

Recomendação:
1. Usar prepared statements (PDO em PHP, parametrização nativa)
2. Validação de entrada com whitelist
3. Privilégio mínimo do usuário de banco da aplicação
4. WAF com regras específicas para SQLi como camada extra
```

---

## 5. Sumário Executivo — a parte mais importante

Gestão **só** lê o sumário executivo. Regras:

- **Uma página, no máximo duas.**
- **Zero jargão técnico.** Nada de "RCE", "CVE", "CVSS".
- **Impacto em linguagem de negócio**: "acesso a 2.4M clientes", "R$ X expostos", "downtime de Y horas possível".
- **Gráfico visual**: distribuição de severidade em pizza/barra.
- **Próximos passos claros**: 3-5 ações prioritárias.

### Template de abertura

```
A empresa [X] contratou [Y] para realizar um Pentest Web da aplicação
[nome], entre DD/MM e DD/MM/2026. Durante [N] dias úteis, nossa equipe
identificou [N] vulnerabilidades, sendo [N] críticas, [N] altas, [N]
médias e [N] baixas.

As falhas mais críticas permitiriam a um atacante externo, sem
credenciais, [ação de impacto concreto]. Estimamos que a exploração
completa levaria a [impacto financeiro/reputacional/regulatório].

Recomendamos ações imediatas em três frentes: [1, 2, 3].
```

---

## 6. Debriefing com o Cliente

Após entrega do relatório, ofereça:

1. **Apresentação executiva** (30 min) para liderança
2. **Sessão técnica** (2h) com Blue Team e devs
3. **Retest gratuito** após correções (comum em contratos)

---

## Exercícios

1. Pegue um achado real de CTF (HTB retired machine) e escreva uma seção de relatório seguindo o template. Entregue em PDF.
2. Elabore o Sumário Executivo (1 página) de um engagement fictício com 3 critical + 5 high + 8 medium.
3. Gere um vetor CVSS para 5 vulnerabilidades diferentes (SQLi, XSS, CSRF, Missing MFA, Session Fixation) e justifique.

---

[← Anterior: Exfiltração](../09-exfiltracao/README.md) · [Voltar ao índice](../../README.md) · [Próximo: Labs Práticos →](../11-labs-praticos/README.md)
