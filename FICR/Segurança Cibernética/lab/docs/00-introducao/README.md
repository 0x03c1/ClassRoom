# 00 · Introdução à Segurança Ofensiva

> [← Voltar ao índice](../../README.md) · [Próximo: Preparação do Ambiente →](../01-ambiente/README.md)

---

## Objetivos deste módulo

- Entender o que é (e o que **não** é) um pentest
- Conhecer o cenário legal brasileiro e internacional
- Diferenciar pentest, Red Team, Bug Bounty e Vulnerability Assessment
- Apresentar as metodologias de referência

---

## 1. Terminologia Essencial

| Termo | Significado |
|---|---|
| **Pentest (Penetration Testing)** | Avaliação técnica com escopo definido em contrato, prazo curto, objetivo documentar falhas |
| **Red Team** | Simulação adversarial contínua, foco em bypass de detecção e objetivos de negócio |
| **Blue Team** | Equipe defensiva — SOC, DFIR, threat hunting |
| **Purple Team** | Colaboração entre Red e Blue para melhorar detecção |
| **Bug Bounty** | Programa com recompensa por vulnerabilidades em escopo público/privado |
| **Vulnerability Assessment** | Scan automatizado, sem exploração ativa |
| **ROE (Rules of Engagement)** | Documento que define o que pode, o que não pode, janelas de teste, contatos |

---

## 2. Legislação Brasileira (o que todo pentester precisa saber)

- **Lei nº 12.737/2012** (Carolina Dieckmann) — tipifica invasão de dispositivos
- **Lei nº 14.155/2021** — agrava penas para crimes cibernéticos
- **LGPD (Lei nº 13.709/2018)** — tratamento de dados pessoais durante o teste
- **Marco Civil da Internet (Lei nº 12.965/2014)** — responsabilidades e logs

**Regra de ouro:** sem contrato assinado + ROE + autorização explícita = **crime**. Sem exceções.

---

## 3. Metodologias de Referência

### OWASP Testing Guide v4.2
Padrão de facto para pentest web. Organiza testes em 11 categorias:
1. Information Gathering
2. Configuration and Deployment Management
3. Identity Management
4. Authentication
5. Authorization
6. Session Management
7. Input Validation
8. Error Handling
9. Cryptography
10. Business Logic
11. Client-Side

### PTES (Penetration Testing Execution Standard)
Framework de 7 fases:
1. Pre-engagement Interactions
2. Intelligence Gathering
3. Threat Modeling
4. Vulnerability Analysis
5. Exploitation
6. Post Exploitation
7. Reporting

### MITRE ATT&CK
Matriz de **táticas** (o "porquê") e **técnicas** (o "como") de adversários reais. Essencial para mapear achados em contexto defensivo.

### Cyber Kill Chain (Lockheed Martin)
Reconnaissance → Weaponization → Delivery → Exploitation → Installation → Command & Control → Actions on Objectives

---

## 4. Fluxo Prático (visão geral)

```
┌───────────────┐   ┌───────────────┐   ┌───────────────┐   ┌───────────────┐
│ PREPARAÇÃO    │──▶│ RECON         │──▶│ ENUMERAÇÃO    │──▶│ ANÁLISE DE    │
│ DO AMBIENTE   │   │ PASSIVO+ATIVO │   │ E MAPEAMENTO  │   │ VULNS         │
└───────────────┘   └───────────────┘   └───────────────┘   └───────┬───────┘
                                                                    │
┌───────────────┐   ┌───────────────┐   ┌───────────────┐   ┌───────▼───────┐
│ RELATÓRIO     │◀──│ EXFILTRAÇÃO   │◀──│ PÓS-          │◀──│ EXPLORAÇÃO    │
│ E LIMPEZA     │   │ E IMPACTO     │   │ EXPLORAÇÃO    │   │               │
└───────────────┘   └───────────────┘   └───────────────┘   └───────────────┘
```

O fluxo é **cíclico**: uma credencial obtida na exploração pode reabrir a enumeração com novos alvos.

---

## 5. Checklist antes de iniciar qualquer teste

- [ ] Contrato assinado com cláusula de autorização
- [ ] ROE aprovado pelas duas partes
- [ ] Escopo IP/domínio documentado por escrito
- [ ] Janela de teste definida
- [ ] Contato de emergência 24/7 do cliente
- [ ] Backup do ambiente feito pelo cliente
- [ ] Plano de comunicação em caso de incidente crítico
- [ ] VPN/IP fixo para que o Blue Team reconheça o tráfego

---

## Exercícios

1. Pesquise a jurisprudência brasileira sobre a Lei 12.737/2012 e traga dois casos reais.
2. Compare o OWASP Testing Guide v4.2 com o PTES — quais são as principais diferenças de escopo?
3. Elabore um modelo de ROE genérico de uma página para um cliente fictício.

---

[← Voltar ao índice](../../README.md) · [Próximo: Preparação do Ambiente →](../01-ambiente/README.md)
