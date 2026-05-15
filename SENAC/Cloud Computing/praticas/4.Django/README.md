# Aula Prática — Django + Azure VM + PostgreSQL + CI/CD com GitHub Actions

> **Disciplina:** Cloud Computing

---

## Stack

- Django 5.1 LTS sobre Python 3.12
- PostgreSQL 16 (Azure Database for PostgreSQL Flexible Server)
- Ubuntu Server 24.04 LTS (Azure VM, tamanho Standard_B1s)
- Nginx 1.24 como reverse proxy
- Gunicorn 23.x como WSGI
- GitHub Actions (pipeline `test` + `deploy`)

Todo o provisionamento é feito **pelo Portal Azure** (https://portal.azure.com). Esta aula **não usa Azure CLI** — nenhuma instalação local de ferramentas Azure é necessária além de Git, Python e um cliente SSH.

---

## Objetivo geral

Ao final da sequência o aluno será capaz de provisionar uma aplicação web Django em produção na Microsoft Azure, conectada a um banco de dados gerenciado, versionada no GitHub e com deploy automatizado a cada `push` na branch `main`.

---

## Arquitetura final

```
                  Desenvolvedor (push em main)
                              |
                              v
                    GitHub Actions
            1. Lint + testes (PostgreSQL service)
            2. SSH na VM e executa deploy
                              |
                              v SSH (porta 22)
   +---------------------------------------------------+
   |             Azure — Resource Group                |
   |                                                   |
   |   VM Ubuntu 24.04 (Standard_B1s)                  |
   |      Nginx (80/443) -> Gunicorn (unix socket)     |
   |                            |                      |
   |                            v                      |
   |   Azure Database for PostgreSQL Flexible Server   |
   |      (Burstable B1ms, versão 16)                  |
   +---------------------------------------------------+
                              ^
                              | HTTPS (Let's Encrypt)
                              |
                          Usuário final
```

---

## Estrutura das aulas

| #  | Aula                                                                  | Entrega                                                  | Tempo |
| -- | --------------------------------------------------------------------- | -------------------------------------------------------- | ----- |
| 1  | [Setup e ambiente local](./docs/aula1/README.md)                      | Conta Azure for Students, Git, Python, cliente SSH       | 2h    |
| 2  | [Projeto Django + GitHub](./docs/aula2/README.md)                     | App Django rodando local, repositório no GitHub          | 2h    |
| 3  | [Provisionando a VM pelo Portal Azure](./docs/aula3/README.md)        | VM Ubuntu 24.04 acessível por SSH                        | 2h    |
| 4  | [Banco PostgreSQL Flexible Server](./docs/aula4/README.md)            | Banco gerenciado conectado à VM                          | 2h    |
| 5  | [Deploy manual: Gunicorn + Nginx + systemd](./docs/aula5/README.md)   | Aplicação no ar via HTTP                                 | 2h    |
| 6  | [HTTPS com Let's Encrypt + hardening](./docs/aula6/README.md)         | Certificado válido e configurações de segurança          | 2h    |
| 7  | [CI/CD com GitHub Actions](./docs/aula7/README.md)                    | Push em `main` dispara deploy automático                 | 2h    |
| 8  | [Logs, monitoramento e troubleshooting](./docs/aula8/README.md)       | Backup, observabilidade, playbook de problemas           | 2h    |

Cada aula é cumulativa e termina com um critério objetivo de conclusão (checkpoint). Não pule.

---

## Pré-requisitos do aluno

- Computador com Windows 10/11, macOS ou Linux
- E-mail institucional (`.edu.br` ou equivalente) para ativar Azure for Students sem cartão de crédito
- Conta GitHub
- Navegador moderno (Edge, Chrome ou Firefox)

---

## Custos esperados

Usando Azure for Students (US$100 de crédito por 12 meses, sem cartão):

| Recurso                                | SKU                    | Custo mensal estimado | Coberto pelo crédito? |
| -------------------------------------- | ---------------------- | --------------------- | --------------------- |
| VM Linux Standard_B1s                  | 1 vCPU, 1 GB RAM       | ~US$ 7,60             | Sim                   |
| Disco gerenciado 30 GB Standard SSD    | —                      | ~US$ 3,00             | Sim                   |
| IP público estático Standard           | —                      | ~US$ 3,60             | Sim                   |
| PostgreSQL Flexible Server B1ms        | 1 vCPU, 2 GB RAM, 32GB | ~US$ 12 a 15          | Sim                   |
| **Total**                              |                        | **~US$ 26 a 30/mês**  | **3 a 4 meses**       |

> Os valores acima são estimativas em USD baseadas em **Brazil South**. Confirme no [Pricing Calculator](https://azure.microsoft.com/pricing/calculator/) antes de provisionar, pois preços variam por região e mudam ao longo do tempo.
>
> A Azure for Students também concede **750 horas gratuitas/mês** de VMs B1s pelo período da assinatura. Confirme no [Azure free services](https://azure.microsoft.com/free/free-account-faq) o status atual.
>
> **Ao final da disciplina**, delete o Resource Group inteiro (passo a passo na Aula 8). Isso encerra a cobrança de todos os recursos de uma só vez.

---

## Como usar este repositório

1. Clone o repositório
2. Leia o README de cada aula em ordem
3. Cada aula tem: **Objetivos**, **Passo a passo**, **Checkpoint**, **Troubleshooting**, **Desafio extra**

---

## Avaliação sugerida

| Item                                            | Peso |
| ----------------------------------------------- | ---- |
| Checkpoints das aulas 1 a 6                     | 40%  |
| Pipeline CI/CD funcionando (aula 7)             | 30%  |
| Apresentação final + README do projeto do aluno | 20%  |
| Desafios extras                                 | 10%  |

---

## Autor

Prof. Roni Maciel
