# 99 · Referências e Recursos

> [← Anterior: Labs Práticos](../11-labs-praticos/README.md) · [Voltar ao índice](../../README.md)

---

## Livros Essenciais

### Fundamentos e Pentest
- **The Web Application Hacker's Handbook** — Dafydd Stuttard, Marcus Pinto
- **The Hacker Playbook 3** — Peter Kim
- **Penetration Testing: A Hands-On Introduction** — Georgia Weidman
- **Red Team Field Manual (RTFM)** — Ben Clark
- **Blue Team Field Manual (BTFM)** — Alan White

### Especializações
- **Real-World Bug Hunting** — Peter Yaworski (bug bounty)
- **Web Security for Developers** — Malcolm McDonald (defensivo)
- **The Tangled Web** — Michal Zalewski (web security profundo)
- **Attacking Network Protocols** — James Forshaw (protocolos)
- **Practical Binary Analysis** — Dennis Andriesse (reversing)
- **Advanced Penetration Testing** — Wil Allsopp (Red Team)

### Active Directory
- **Pentesting Active Directory** — cursos e livros especializados
- **A Field Guide to Offensive Security** — série de artigos

### Brasileiros
- **Introdução ao Pentest** — Daniel Moreno (Novatec)
- **Pentest em Redes e Hosts** — Daniel Moreno

---

## Wikis e Referências Online

### Obrigatórias
- [**HackTricks**](https://book.hacktricks.xyz) — Wiki gigantesca, atualizada, referência #1
- [**PayloadsAllTheThings**](https://github.com/swisskyrepo/PayloadsAllTheThings) — Payloads para todas as vulnerabilidades
- [**OWASP Testing Guide v4.2**](https://owasp.org/www-project-web-security-testing-guide/)
- [**PortSwigger Web Security Academy**](https://portswigger.net/web-security) — Teoria + labs gratuitos

### Linux Privesc
- [**GTFOBins**](https://gtfobins.github.io) — Binários Linux abusáveis
- [**PayloadsAllTheThings — Linux Privilege Escalation**](https://github.com/swisskyrepo/PayloadsAllTheThings/blob/master/Methodology%20and%20Resources/Linux%20-%20Privilege%20Escalation.md)

### Windows Privesc & AD
- [**LOLBAS**](https://lolbas-project.github.io) — Binários Windows abusáveis
- [**WADComs**](https://wadcoms.github.io) — Cheatsheet interativo AD
- [**The Hacker Recipes**](https://www.thehacker.recipes) — Guia técnico profundo AD
- [**ired.team**](https://www.ired.team) — Notas de Red Team

### Web
- [**OWASP Top 10**](https://owasp.org/www-project-top-ten/) — Atual: 2021
- [**OWASP API Security Top 10**](https://owasp.org/API-Security/) — Atual: 2023
- [**OWASP Cheat Sheet Series**](https://cheatsheetseries.owasp.org)

---

## Wordlists e Payloads

| Recurso | Conteúdo |
|---|---|
| [**SecLists**](https://github.com/danielmiessler/SecLists) | Coleção definitiva (pré-instalado no Kali em `/usr/share/seclists`) |
| [**FuzzDB**](https://github.com/fuzzdb-project/fuzzdb) | Database de padrões de ataque |
| [**Assetnote Wordlists**](https://wordlists.assetnote.io) | Curadas para bug bounty |
| [**OneListForAll**](https://github.com/six2dez/OneListForAll) | Megalist consolidada |
| [**SmallSecList**](https://github.com/sdcampbell/SmallSecList) | Versão reduzida do SecLists |

### Senhas
- `rockyou.txt` — 14M senhas (Kali: `/usr/share/wordlists/rockyou.txt.gz`)
- **HashMob Founds** — quebrados em CTFs
- **CrackStation** dictionary
- **weakpass.com** — múltiplos dumps históricos

---

## Canais e Comunidades

### YouTube (técnico)
- **IppSec** — writeups HTB, essencial
- **John Hammond** — CTFs, malware
- **The Cyber Mentor (TCM)** — didático
- **LiveOverflow** — binary exploitation
- **STÖK** — bug bounty
- **InsiderPhD** — bug bounty iniciante
- **NahamSec** — bug bounty

### YouTube (Brasil)
- **DefCon Group Brasil**
- **Bsides São Paulo**
- **Hackers do Bem** (Ministério + Softex)

### Twitter/X (seguir)
- Pesquisadores de segurança publicam 0-days e técnicas em tempo real
- Hashtags: #BugBounty #RedTeam #InfoSec

### Discord/Slack
- **HackTheBox Discord** — community
- **TryHackMe Discord**
- **The Cyber Mentor Discord**

---

## Eventos no Brasil

| Evento | Local | Perfil |
|---|---|---|
| **H2HC** (Hackers to Hackers Conference) | São Paulo | Técnico profundo |
| **Bsides** (SP, Recife, Curitiba, etc.) | Várias | Comunitário |
| **Roadsec** | Itinerante | Amplo público |
| **YSTS** (You Shot the Sheriff) | São Paulo | Seleto |
| **SI6** | Brasília | Corporativo |

---

## Bases de Dados de Vulnerabilidades

| Fonte | Uso |
|---|---|
| [**NVD**](https://nvd.nist.gov) | CVE oficial, CVSS, referências |
| [**MITRE CVE**](https://cve.mitre.org) | Registro de CVEs |
| [**Exploit-DB**](https://exploit-db.com) | PoCs públicos |
| [**PacketStorm**](https://packetstormsecurity.com) | Exploits e advisories |
| [**CVE Trends**](https://cvetrends.com) | CVEs em alta no momento |
| [**CISA KEV**](https://www.cisa.gov/known-exploited-vulnerabilities-catalog) | Exploited in the wild |

---

## Newsletters

- [**tl;dr sec**](https://tldrsec.com) — semanal, AppSec
- [**The Daily Swig**](https://portswigger.net/daily-swig) — PortSwigger (descontinuado em 2023, arquivo ainda útil)
- [**Risky Business**](https://risky.biz) — podcast + newsletter
- [**OSINT Newsletter**](https://osintnewsletter.com)
- [**Pentester Land**](https://pentester.land/newsletter/) — writeups curados

---

## Frameworks de Metodologia (detalhados)

- [**OWASP Testing Guide v4.2**](https://owasp.org/www-project-web-security-testing-guide/v42/)
- [**PTES (Penetration Testing Execution Standard)**](http://www.pentest-standard.org)
- [**MITRE ATT&CK**](https://attack.mitre.org)
- [**OSSTMM**](https://www.isecom.org/OSSTMM.3.pdf) — Open Source Security Testing Methodology Manual
- [**NIST SP 800-115**](https://csrc.nist.gov/publications/detail/sp/800-115/final) — Technical Guide to Information Security Testing

---

## Legislação Brasileira

- [**Lei nº 12.737/2012 (Carolina Dieckmann)**](http://www.planalto.gov.br/ccivil_03/_ato2011-2014/2012/lei/l12737.htm)
- [**Lei nº 14.155/2021**](http://www.planalto.gov.br/ccivil_03/_ato2019-2022/2021/lei/L14155.htm)
- [**LGPD (Lei nº 13.709/2018)**](http://www.planalto.gov.br/ccivil_03/_ato2015-2018/2018/lei/l13709.htm)
- [**Marco Civil da Internet (Lei nº 12.965/2014)**](http://www.planalto.gov.br/ccivil_03/_ato2011-2014/2014/lei/l12965.htm)

---

## Trilhas de Certificação

```
Iniciante:
  CompTIA Security+ → eJPT → CEH

Intermediário:
  CPTS (HTB) → PNPT (TCM) → OSCP

Avançado Red Team:
  OSEP → CRTO → CRTL → OSCE3

Especialização Web:
  OSWE (OffSec) → Burp Suite Certified Practitioner

Especialização Cloud:
  AWS Security Specialty → CCSP

Especialização DFIR:
  GCIH → GCFA → GREM
```

---

[← Anterior: Labs Práticos](../11-labs-praticos/README.md) · [Voltar ao índice](../../README.md)
