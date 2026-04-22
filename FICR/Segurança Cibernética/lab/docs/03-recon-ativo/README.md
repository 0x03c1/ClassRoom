# 03 · Reconhecimento Ativo

> [← Anterior: Recon Passivo](../02-recon-passivo/README.md) · [Voltar ao índice](../../README.md) · [Próximo: Enumeração →](../04-enumeracao/README.md)

---

## Princípio

Agora interagimos **diretamente** com o alvo: pacotes TCP/UDP, requests HTTP, banners de serviço. **Só prosseguir com autorização escrita.**

---

## 1. Varredura de Portas e Serviços

### Nmap — a ferramenta mais importante de pentest

```bash
# Scan rápido inicial (top 1000 portas TCP)
nmap -sC -sV -oA quick alvo.com.br

# Full port scan (todas as 65535 portas, TCP)
sudo nmap -p- -T4 --min-rate=1000 alvo.com.br -oA full

# Após descobrir portas abertas, scan aprofundado só nelas
sudo nmap -sC -sV -sU -O -p22,80,443,3306 alvo.com.br -oA deep

# Scan com scripts de vulnerabilidades
sudo nmap -sV --script=vuln alvo.com.br -oA vuln

# Scan stealth (SYN) — só com root
sudo nmap -sS -Pn alvo.com.br

# Evasão básica (contra IDS simples)
sudo nmap -f -D RND:10 -T2 alvo.com.br
```

**Flags essenciais:**
- `-sC` scripts padrão · `-sV` versão · `-O` OS · `-A` tudo
- `-Pn` não fazer ping (alvos com ICMP bloqueado)
- `-oA` saída em 3 formatos (normal, XML, grepable)
- `-T0..T5` timing (T4 = padrão para internet, T2 = stealth)
- `--min-rate` / `--max-rate` controle de pacotes por segundo

### Masscan — velocidade bruta

Quando o escopo é grande (`/16`, `/8`), Nmap demora horas. Masscan varre a internet inteira em minutos.

```bash
sudo masscan -p1-65535 --rate=10000 192.0.2.0/24 -oG masscan.gnmap

# Workflow híbrido: Masscan descobre, Nmap aprofunda
sudo masscan -p1-65535 --rate=10000 alvo.com.br -oG ports.gnmap
awk '/Ports:/ {print $2, $5}' ports.gnmap | sed 's|/.*||' \
  | while read ip port; do echo "$ip:$port"; done
```

### RustScan

Wrapper em Rust que faz o scan inicial rápido e passa para o Nmap.

```bash
rustscan -a alvo.com.br --ulimit 5000 -- -sC -sV -oA rustscan
```

### Unicornscan

Varredura assíncrona, alternativa stealth.

---

## 2. Fingerprinting Web

### WhatWeb

Identifica tecnologias em um site (CMS, frameworks, linguagens, servidores).

```bash
whatweb -a 3 https://alvo.com.br
whatweb -a 3 -i live-hosts.txt --log-json=whatweb.json
```

### Wappalyzer

Extensão de browser. Instale no Firefox/Chrome — identifica tech stack ao vivo.

Alternativa CLI: `pipx install wappalyzer-cli`.

### WafW00f — detecção de WAF

**Essencial antes de qualquer fuzzing.** Se existe um WAF e você dispara ffuf com 10k req/s, sua VPN vai para blacklist em 30 segundos.

```bash
wafw00f https://alvo.com.br
wafw00f -i live-hosts.txt -o wafs.txt
```

WAFs comuns detectados: Cloudflare, AWS WAF, Akamai, F5 BIG-IP, Imperva, ModSecurity.

### Netcraft

Web-based. Fornece histórico de IPs, OS, servidor HTTP.

---

## 3. Descoberta de Hosts Vivos em Massa

### httpx (ProjectDiscovery)

O canivete suíço para validar quem está ativo na lista de subdomínios.

```bash
httpx -l subs.txt \
      -title \
      -tech-detect \
      -status-code \
      -tls-probe \
      -follow-redirects \
      -o live.txt
```

### naabu — scanner de portas em Go

```bash
naabu -l subs.txt -p - -rate 1000 -o ports.txt
```

---

## 4. Enumeração de Serviços Específicos

### SMB (Windows shares)

```bash
nmap -p 139,445 --script=smb-enum-shares,smb-enum-users,smb-vuln-* alvo
smbclient -L //alvo -N
enum4linux-ng -A alvo
```

### SNMP

```bash
nmap -sU -p 161 --script=snmp-* alvo
snmpwalk -v2c -c public alvo
onesixtyone -c community-strings.txt alvo
```

### FTP

```bash
nmap -p 21 --script=ftp-anon,ftp-syst,ftp-vuln-* alvo
```

### SSH

```bash
nmap -p 22 --script=ssh2-enum-algos,ssh-hostkey,ssh-auth-methods alvo
ssh-audit alvo
```

### SMTP

```bash
nmap -p 25 --script=smtp-enum-users,smtp-commands alvo
smtp-user-enum -M VRFY -U users.txt -t alvo
```

---

## 5. Screenshot em Massa

Antes de perder tempo lendo cada subdomínio, tire screenshots.

### Aquatone / gowitness / EyeWitness

```bash
# gowitness — moderno e rápido
gowitness scan file -f live.txt
gowitness report serve   # abre UI web com grid de screenshots

# EyeWitness — clássico
eyewitness --web -f live.txt --no-prompt -d screens/
```

Um grid visual com 200 sites permite identificar em 2 minutos:
- Páginas de login padrão
- Erros expostos
- Admin panels
- Dashboards de desenvolvimento

---

## 6. Roteiro Ativo Típico

```bash
ALVO="alvo.com.br"

# 1. Descoberta de portas
sudo masscan -p1-65535 --rate=10000 $ALVO -oG mass.gnmap
PORTS=$(awk '/Ports:/ {print $5}' mass.gnmap | cut -d/ -f1 | sort -un | paste -sd,)

# 2. Aprofundamento
sudo nmap -sC -sV -p$PORTS $ALVO -oA nmap-deep

# 3. Descoberta HTTP em todos os subdomínios
httpx -l subs.txt -status-code -title -tech-detect -o live.txt

# 4. Detecção de WAF
wafw00f -i <(awk -F'[' '{print $1}' live.txt) -o wafs.txt

# 5. Screenshots
gowitness scan file -f <(awk -F'[' '{print $1}' live.txt)
```

---

## Exercícios

1. Em um alvo autorizado (ex.: `scanme.nmap.org`), execute um full scan e identifique a versão do SSH. Entregue a saída XML.
2. Compare o tempo de Nmap vs RustScan vs Masscan em um `/24` local (sua rede doméstica, com seu próprio router como alvo).
3. Em `scanme.nmap.org`, rode `nmap --script=vuln`. Quais CVEs são reportados? Eles são confirmados ou apenas potenciais?

---

[← Anterior: Recon Passivo](../02-recon-passivo/README.md) · [Voltar ao índice](../../README.md) · [Próximo: Enumeração →](../04-enumeracao/README.md)
