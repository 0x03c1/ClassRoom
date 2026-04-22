# Laboratório de Segurança Ofensiva

> Material didático para disciplinas de **Cibersegurança**, **Pentest Web** e **Red Team** — Prof. Roni (`0x03C1`)
>
> Unidades de Ensino

---

## Aviso Legal

**Todo o material deste repositório tem finalidade estritamente educacional.** As ferramentas e técnicas descritas devem ser utilizadas **apenas** em ambientes controlados, autorizados e com escopo contratual definido (Rules of Engagement). O uso destas técnicas fora de um ambiente autorizado **é crime** sob a Lei nº 12.737/2012 (Lei Carolina Dieckmann), Lei nº 14.155/2021 e a Lei Geral de Proteção de Dados (LGPD).

---

## Objetivos de Aprendizagem

Ao final desta disciplina, o aluno será capaz de:

1. Compreender o ciclo completo de um pentest (PTES, OWASP Testing Guide, MITRE ATT&CK)
2. Operar ferramentas profissionais do ecossistema Kali Linux
3. Executar reconhecimento passivo e ativo de forma ética
4. Identificar, explorar e documentar vulnerabilidades web
5. Produzir relatórios técnicos e executivos de qualidade profissional

---

## Como Navegar Este Repositório

O material está organizado seguindo o **fluxo natural de um pentest**. Cada pasta `docs/NN-tema/` contém um `README.md` específico do tópico, com teoria, ferramentas, exemplos práticos e exercícios.

### Trilha Sugerida (sequencial)

| # | Módulo | Descrição |
|---|---|---|
| 00 | [Introdução](docs/00-introducao/README.md) | Conceitos, ética, legislação, metodologias |
| 01 | [Preparação do Ambiente](docs/01-ambiente/README.md) | Kali, VMs, anonimato, gerenciamento de notas |
| 02 | [Reconhecimento Passivo (OSINT)](docs/02-recon-passivo/README.md) | Shodan, Censys, theHarvester, Amass, Maltego |
| 03 | [Reconhecimento Ativo](docs/03-recon-ativo/README.md) | Nmap, Masscan, WhatWeb, WafW00f |
| 04 | [Enumeração e Mapeamento](docs/04-enumeracao/README.md) | ffuf, Gobuster, Burp Suite, Katana, LinkFinder |
| 05 | [Análise de Vulnerabilidades](docs/05-analise-vulns/README.md) | Nuclei, Nikto, WPScan, scanners comerciais |
| 06 | [Exploração](docs/06-exploracao/README.md) | Metasploit, SQLMap, XSStrike, Hydra, JWT |
| 07 | [Pós-Exploração](docs/07-pos-exploracao/README.md) | LinPEAS, WinPEAS, Mimikatz, Hashcat |
| 08 | [Lateral Movement & Persistência](docs/08-lateral-persistencia/README.md) | BloodHound, Impacket, CrackMapExec/NetExec |
| 09 | [Exfiltração e Prova de Impacto](docs/09-exfiltracao/README.md) | DNScat2, túneis, evidências éticas |
| 10 | [Relatório e Limpeza](docs/10-relatorio/README.md) | Ghostwriter, SysReptor, CVSS, estrutura |
| 11 | [Labs Práticos](docs/11-labs-praticos/README.md) | Roteiros de laboratório por fase |
| 99 | [Referências](docs/99-referencias/README.md) | Livros, CVEs, comunidades, wordlists |

### Trilhas por Perfil

- **Iniciante:** 00 → 01 → 02 → 03 → 04 (parar aqui e praticar em TryHackMe)
- **Bug Hunter:** 02 → 04 → 05 → 06 (foco em web)
- **Red Team:** 01 → 06 → 07 → 08 → 09 (foco em pós-exploração e AD)
- **Relatório e Gestão:** 00 → 10 → 99

---

## Stack de Ferramentas (Kali Linux)

Este laboratório assume uso do **Kali Linux 2025.x**. Todas as ferramentas citadas estão disponíveis nos repositórios oficiais via `apt`, `pipx`, ou diretamente no GitHub.

```bash
# Atualização inicial recomendada
sudo apt update && sudo apt full-upgrade -y
sudo apt install -y kali-linux-large
```

Ferramentas não-nativas do Kali serão instaladas módulo a módulo conforme necessário. Veja [docs/01-ambiente/README.md](docs/01-ambiente/README.md).

---

## Pré-requisitos

- Conhecimento básico de redes (TCP/IP, HTTP, DNS)
- Linha de comando Linux (bash, pipes, grep)
- Fundamentos de aplicações web (HTML, JS, HTTP methods)
- Noção básica de programação (Python ou C)

---

## Ambiente de Laboratório

Os exercícios práticos usam **apenas alvos autorizados**:

- [TryHackMe](https://tryhackme.com) — trilhas guiadas
- [HackTheBox](https://hackthebox.com) — labs avançados
- [PortSwigger Web Security Academy](https://portswigger.net/web-security) — gratuito
- [VulnHub](https://vulnhub.com) — VMs vulneráveis para rodar offline
- [DVWA](https://github.com/digininja/DVWA) — Damn Vulnerable Web App (local)
- [OWASP Juice Shop](https://owasp.org/www-project-juice-shop/) — app moderna vulnerável

---

## Contribuição

Este é um repositório didático vivo. Alunos e colegas docentes podem contribuir via Pull Request. Para sugestões, abra uma *issue* com o rótulo `[aula]`, `[correção]` ou `[novo-conteúdo]`.

---

## Licença

Material distribuído sob licença [CC BY-NC-SA 4.0](https://creativecommons.org/licenses/by-nc-sa/4.0/) — uso educacional, com atribuição e compartilhamento nos mesmos termos.

---

**Prof. Roni** · GitHub: [`0x03C1`](https://github.com/0x03C1)
