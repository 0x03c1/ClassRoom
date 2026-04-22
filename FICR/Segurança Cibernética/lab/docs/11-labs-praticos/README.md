# 11 · Labs Práticos

> [← Anterior: Relatório](../10-relatorio/README.md) · [Voltar ao índice](../../README.md) · [Próximo: Referências →](../99-referencias/README.md)

---

## Objetivo

Consolidar a teoria com roteiros práticos em ambientes **autorizados e legais**. Cada lab aqui mapeia para um ou mais módulos anteriores.

---

## 1. Plataformas de Laboratório Recomendadas

### Gratuitas

| Plataforma | Foco | Nível |
|---|---|---|
| [TryHackMe](https://tryhackme.com) | Trilhas guiadas | Iniciante → intermediário |
| [PortSwigger Web Academy](https://portswigger.net/web-security) | Web vulns | Iniciante → avançado |
| [PentesterLab](https://pentesterlab.com) | Exercícios focados | Intermediário |
| [VulnHub](https://vulnhub.com) | VMs offline | Todos |
| [OverTheWire](https://overthewire.org) | Wargames | Iniciante |
| [HackTheBox Academy](https://academy.hackthebox.com) | Módulos estruturados | Todos |

### Pagas (valem investimento)

| Plataforma | Foco | Nível |
|---|---|---|
| [HackTheBox](https://hackthebox.com) | Labs tipo CTF | Intermediário → avançado |
| [HackTheBox Pro Labs](https://hackthebox.com/pro-labs) | AD, Red Team | Avançado |
| [OffSec Proving Grounds](https://portal.offsec.com/practice) | Estilo OSCP | Intermediário |
| [TCM Academy](https://academy.tcm-sec.com) | Cursos completos | Todos |

### Self-hosted (sem internet necessária)

- **DVWA** — Damn Vulnerable Web Application
- **OWASP Juice Shop** — aplicação moderna vulnerável
- **bWAPP** — 100+ vulnerabilidades em PHP
- **WebGoat** — OWASP, em Java
- **Metasploitable 2/3** — VM Linux vulnerável
- **VulnHub: Kioptrix series** — clássicos para privesc

---

## 2. Ambiente AD Self-hosted — GOAD

[Game Of Active Directory](https://github.com/Orange-Cyberdefense/GOAD) — laboratório AD completo com múltiplos forests e vulnerabilidades clássicas.

```bash
git clone https://github.com/Orange-Cyberdefense/GOAD
cd GOAD
# Siga o setup com Vagrant + Ansible (~40GB de RAM recomendados)
```

Conteúdo pedagógico:
- Kerberoasting
- AS-REP Roasting
- DCSync
- Unconstrained / Constrained Delegation
- ACL abuses
- Trust abuses
- PrintSpooler / PetitPotam / Coerce

---

## 3. Roteiros de Aula (sugestões)

### Lab 01 — Recon completo em alvo autorizado

**Módulos:** 02, 03

**Alvo:** seu próprio domínio ou `scanme.nmap.org`

**Entregáveis:**
1. `subdomains.txt` via Amass + Subfinder + crt.sh
2. `live-hosts.txt` via httpx
3. `nmap-full.xml` de portas completas
4. Screenshot grid com gowitness

### Lab 02 — Fuzzing web no DVWA

**Módulos:** 04

**Alvo:** DVWA local

**Entregáveis:**
1. Mapa completo de diretórios via ffuf
2. Site Map do Burp exportado
3. Lista de parâmetros descobertos via Arjun

### Lab 03 — SQLi e RCE no Juice Shop

**Módulos:** 05, 06

**Alvo:** OWASP Juice Shop local

**Entregáveis:**
1. Identificar login bypass via SQLi
2. Escrever relatório resumido (1 página) no template do módulo 10
3. Vetor CVSS justificado

### Lab 04 — Privesc no Linux

**Módulos:** 07

**Alvo:** VulnHub "Kioptrix Level 1" ou HTB "Lame"

**Entregáveis:**
1. Caminho de exploração: porta → serviço → CVE → shell
2. Saída do LinPEAS
3. Caminho de privesc escolhido (kernel exploit, SUID, etc.)

### Lab 05 — Active Directory no GOAD

**Módulos:** 07, 08

**Alvo:** GOAD local ou HTB "Forest"

**Entregáveis:**
1. Coleta BloodHound
2. Shortest path para Domain Admin
3. Exploração completa de pelo menos 2 técnicas (ex.: Kerberoasting + DCSync)

### Lab 06 — Relatório completo

**Módulo:** 10

**Alvo:** qualquer HTB retired machine

**Entregáveis:**
1. Relatório em PDF com todas as 7 seções
2. Sumário executivo
3. Pelo menos 3 achados com CVSS

---

## 4. CTFs Recomendados para Prática Contínua

### Sempre ativos
- **picoCTF** — educacional, gratuito
- **HackTheBox CTFs** — eventuais
- **CTFtime.org** — calendário global

### Nacionais (Brasil)
- **H2HC** CTF — Hackers to Hackers Conference (São Paulo)
- **Roadsec** CTF
- **You Shot the Sheriff** (YSTS)

---

## 5. Metodologia de Estudo Sugerida

**Ciclo de 2 semanas:**

- **Semana 1:** Teoria (um módulo deste repositório) + 2 salas THM
- **Semana 2:** 1 máquina HTB fácil + escrever writeup

**Writeups** são a melhor forma de consolidar conhecimento. Mantenha um blog/GitHub com os seus.

### Template de writeup

```markdown
# Máquina: Nome

## Recon
- Nmap scan e achados
- Serviços interessantes

## Enumeração
- Diretórios, parâmetros, tecnologias

## Exploração
- Vulnerabilidade identificada
- PoC passo a passo

## Privilege Escalation
- Vetor usado
- Comando final

## Lições Aprendidas
- O que aprendi
- O que eu faria diferente
```

---

## 6. Certificações e Próximos Passos

### Ordem sugerida de progressão

1. **[eJPT](https://security.ine.com/certifications/ejpt-certification/)** (INE/eLearnSecurity) — fundamentos, bom para começar
2. **[CPTS](https://academy.hackthebox.com/preview/certifications/htb-certified-penetration-testing-specialist)** (HTB) — hands-on, cobre muito
3. **[OSCP](https://www.offsec.com/courses/pen-200/)** (Offensive Security) — padrão da indústria
4. **[CRTO](https://training.zeropointsecurity.co.uk/courses/red-team-ops)** (Zero-Point Security) — Red Team moderno
5. **[OSEP](https://www.offsec.com/courses/pen-300/)** / **[OSWE](https://www.offsec.com/courses/web-300/)** — especializações

### Certificações reconhecidas no Brasil
- **CEH** (EC-Council) — forte em CVs corporativos, fraco tecnicamente
- **LPT Master** — complementa CEH
- **CompTIA PenTest+** — baseline

---

## Sugestão de Cronograma Semestral (60h)

| Semana | Tópico | Horas |
|---|---|---|
| 1 | Módulo 00 + ética/legislação | 4h |
| 2 | Módulo 01 — Ambiente | 4h |
| 3-4 | Módulo 02 — Recon Passivo + Lab 01 | 8h |
| 5 | Módulo 03 — Recon Ativo | 4h |
| 6-7 | Módulo 04 — Enumeração + Lab 02 | 8h |
| 8-9 | Módulo 05 — Vulns + Lab 03 | 8h |
| 10-11 | Módulo 06 — Exploração | 8h |
| 12 | Módulo 07 — Pós-Ex + Lab 04 | 4h |
| 13 | Módulo 08 — Lateral + Lab 05 | 4h |
| 14 | Módulo 09 — Exfil | 2h |
| 15 | Módulo 10 — Relatório + Lab 06 | 4h |
| 16 | Apresentações finais | 2h |

---

[← Anterior: Relatório](../10-relatorio/README.md) · [Voltar ao índice](../../README.md) · [Próximo: Referências →](../99-referencias/README.md)
