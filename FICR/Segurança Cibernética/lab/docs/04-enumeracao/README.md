# 04 · Enumeração e Mapeamento Web

> [← Anterior: Recon Ativo](../03-recon-ativo/README.md) · [Voltar ao índice](../../README.md) · [Próximo: Análise de Vulns →](../05-analise-vulns/README.md)

---

## Princípio

Mapear **tudo** que a aplicação expõe: diretórios, arquivos, endpoints, parâmetros, subdomínios web, arquivos JS e seus segredos.

---

## 1. Fuzzing de Diretórios e Arquivos

### ffuf — o fuzzer mais flexível do Kali

Rust-like, escrito em Go, ultrarrápido. Aprenda **este** com profundidade.

#### Uso básico — descoberta de diretórios

```bash
ffuf -u https://alvo.com.br/FUZZ \
     -w /usr/share/seclists/Discovery/Web-Content/raft-large-directories.txt \
     -mc 200,301,302,403 \
     -o ffuf-dirs.json -of json
```

#### Descoberta de arquivos com extensões

```bash
ffuf -u https://alvo.com.br/FUZZ \
     -w /usr/share/seclists/Discovery/Web-Content/raft-large-files.txt \
     -e .php,.asp,.aspx,.jsp,.html,.bak,.old,.zip \
     -mc 200
```

#### Fuzzing de parâmetros GET

```bash
ffuf -u "https://alvo.com.br/search?FUZZ=test" \
     -w /usr/share/seclists/Discovery/Web-Content/burp-parameter-names.txt \
     -fs 4242   # filtra respostas com 4242 bytes (baseline)
```

#### Fuzzing de valores (detecção de SQLi manual, por exemplo)

```bash
ffuf -u "https://alvo.com.br/product?id=FUZZ" \
     -w payloads/sqli.txt \
     -mc all -fc 404
```

#### Fuzzing de subdomínios (vhost)

```bash
ffuf -u https://alvo.com.br \
     -H "Host: FUZZ.alvo.com.br" \
     -w subs-wordlist.txt \
     -fs 0
```

#### Flags de filtro (as mais importantes)

| Flag | Uso |
|---|---|
| `-mc` | Match status codes (mostra) |
| `-fc` | Filter status codes (oculta) |
| `-ms` / `-fs` | Match/filter por tamanho |
| `-mw` / `-fw` | Match/filter por número de palavras |
| `-ml` / `-fl` | Match/filter por número de linhas |
| `-mr` / `-fr` | Match/filter por regex |
| `-t` | Threads (default: 40) |
| `-p` | Delay aleatório entre requests (ex: `0.1-2`) |
| `-x` | Proxy (ex: `http://127.0.0.1:8080` para Burp) |
| `-recursion` | Recursão em diretórios encontrados |
| `-recursion-depth` | Profundidade da recursão |

### Gobuster

Alternativa mais antiga, ainda muito usada. Simples e confiável.

```bash
# Diretórios
gobuster dir -u https://alvo.com.br \
  -w /usr/share/seclists/Discovery/Web-Content/raft-large-directories.txt \
  -t 50 \
  -x php,html,txt,bak \
  -o gobuster-dirs.txt

# DNS (subdomínios, brute force real)
gobuster dns -d alvo.com.br -w subdomains-top1million-5000.txt -t 50

# Vhost
gobuster vhost -u https://alvo.com.br -w vhosts.txt
```

### Feroxbuster

Recursivo por padrão, ótima saída visual.

```bash
feroxbuster -u https://alvo.com.br \
  -w /usr/share/seclists/Discovery/Web-Content/raft-medium-directories.txt \
  --depth 3 \
  -x php,html,txt
```

### Dirsearch

Python, simples, bom para iniciantes.

```bash
dirsearch -u https://alvo.com.br -e php,asp,aspx,jsp,html,js -x 404
```

### Comparativo rápido

| Ferramenta | Linguagem | Velocidade | Recursão | Nota |
|---|---|---|---|---|
| **ffuf** | Go | ⚡⚡⚡⚡⚡ | opcional | Mais flexível |
| **Feroxbuster** | Rust | ⚡⚡⚡⚡⚡ | padrão | Saída visual bonita |
| **Gobuster** | Go | ⚡⚡⚡⚡ | não | Clássico, confiável |
| **Dirsearch** | Python | ⚡⚡⚡ | opcional | Bom para iniciantes |

**Recomendação da disciplina:** domine `ffuf` primeiro. Os outros viram opção.

---

## 2. Enumeração de APIs

### Kiterunner

Brute-force de rotas de API REST e GraphQL com wordlists específicas.

```bash
kr scan https://api.alvo.com.br -w routes-large.kite -o kr-out.txt
kr brute https://api.alvo.com.br -w wordlist.txt
```

### Arjun

Descobre parâmetros HTTP ocultos.

```bash
arjun -u https://alvo.com.br/page -m GET,POST -oT params.txt
```

### ParamSpider

Coleta parâmetros do histórico do Wayback Machine.

```bash
paramspider -d alvo.com.br -o params.txt
```

### gau + uro

Pipeline poderoso para coletar URLs históricas com parâmetros.

```bash
gau alvo.com.br | uro | grep "=" > historical-params.txt
```

---

## 3. Proxy Interceptador — Burp Suite (O HUB CENTRAL)

**Burp Suite é a ferramenta mais importante de pentest web.** Todo o tráfego da aplicação passa por ele.

### Configuração inicial

1. **Proxy → Options:** confirme que está em `127.0.0.1:8080`
2. **Instalar certificado CA:** no navegador, visite `http://burp`, baixe `cacert.der`, importe nas autoridades confiáveis
3. **Usar FoxyProxy** no Firefox para alternar proxy ON/OFF rapidamente

### Abas principais

| Aba | Uso |
|---|---|
| **Proxy** | Intercepta e modifica requests |
| **Target → Site map** | Mapa completo da aplicação |
| **Target → Scope** | Define o que está dentro/fora do escopo |
| **Intruder** | Fuzzing customizável (payloads em pontos específicos) |
| **Repeater** | Repete um request quantas vezes quiser (teste manual de vulns) |
| **Decoder** | Encode/decode (base64, URL, hex, HTML) |
| **Comparer** | Diff entre duas respostas |
| **Sequencer** | Análise de entropia de tokens |
| **Extensions** | BApp Store — extensões da comunidade |

### Extensions essenciais (BApp Store)

- **Logger++** — log avançado de requests
- **Autorize** — testes automáticos de autorização (IDOR, BAC)
- **Param Miner** — descobre parâmetros e headers ocultos
- **Backslash Powered Scanner** — scanner avançado
- **JSON Web Tokens** — manipulação de JWT
- **Turbo Intruder** — Intruder com performance de milhões de req/min
- **HTTP Request Smuggler** — detecção de smuggling
- **Active Scan++** — estende o scanner ativo

### Community Edition vs Pro

| Recurso | Community | Pro |
|---|---|---|
| Proxy, Repeater, Decoder | ✅ | ✅ |
| Intruder | ⚠️ (throttled) | ✅ |
| Scanner ativo/passivo | ❌ | ✅ |
| Collaborator | ❌ | ✅ |
| Extensions pagas | ❌ | ✅ |

Para aulas, **Community Edition é suficiente** na maior parte do tempo.

---

## 4. Alternativas ao Burp

### OWASP ZAP

Open-source, gratuito e completo.

```bash
zaproxy   # abre GUI
zap.sh -daemon -port 8090 -config api.key=SUA_KEY   # modo headless
```

**Quando usar ZAP em vez de Burp:**
- Automação CI/CD (tem excelente API)
- Budget zero sem poder pagar Burp Pro
- Scans agendados automatizados

### Caido

Proxy moderno em Rust. Interface limpa, rápido, bom para APIs. Roadmap promissor.

---

## 5. Crawling / Spidering

### Katana (ProjectDiscovery)

```bash
# Crawl básico
katana -u https://alvo.com.br -d 5 -o urls.txt

# Crawl com parsing de JavaScript (extrai endpoints de JS)
katana -u https://alvo.com.br -d 5 -jc -o urls-full.txt

# Crawl headless (executa JS no Chrome)
katana -u https://alvo.com.br -headless -d 3 -o urls-headless.txt
```

### Hakrawler

```bash
echo "https://alvo.com.br" | hakrawler -d 3 -u
```

---

## 6. JavaScript Analysis

Arquivos JS contêm **tesouro**: endpoints de API, chaves hardcoded, lógica de negócio.

### LinkFinder — extrai endpoints

```bash
# Arquivo único
linkfinder -i https://alvo.com.br/main.js -o cli

# Domínio inteiro (percorre todos os JS)
linkfinder -i https://alvo.com.br -d -o results.html
```

### SecretFinder — encontra chaves e tokens

```bash
secretfinder -i https://alvo.com.br/app.js -o cli
```

Detecta: API keys (AWS, Google, Stripe), tokens JWT, senhas em claro, bearer tokens.

### TruffleHog

```bash
trufflehog git https://github.com/alvo/repo --json
trufflehog filesystem /caminho/clonado
```

### RetireJS — detecta bibliotecas JS com CVEs

```bash
retire --js --path /caminho/do/site/
```

### Pipeline combinado

```bash
# 1. Crawl todo o site coletando URLs e JS
katana -u https://alvo.com.br -d 5 -jc -o urls.txt
grep "\.js$" urls.txt > jsfiles.txt

# 2. Para cada JS, rode LinkFinder e SecretFinder
while read js; do
  linkfinder -i "$js" -o cli >> endpoints.txt
  secretfinder -i "$js" -o cli >> secrets.txt
done < jsfiles.txt

# 3. Filtre endpoints únicos
sort -u endpoints.txt > endpoints-unique.txt
```

---

## 7. Roteiro de Enumeração Completo

```bash
ALVO="https://alvo.com.br"

# 1. Fuzzing de diretórios (com proxy no Burp para análise posterior)
ffuf -u $ALVO/FUZZ \
     -w /usr/share/seclists/Discovery/Web-Content/raft-large-directories.txt \
     -mc 200,301,302,403 \
     -x http://127.0.0.1:8080 \
     -o dirs.json -of json

# 2. Crawl
katana -u $ALVO -d 5 -jc -proxy http://127.0.0.1:8080 -o urls.txt

# 3. Descoberta de parâmetros
arjun -u $ALVO/api/user -m GET,POST -oT params.txt

# 4. Análise de JS
grep "\.js" urls.txt | while read js; do
  secretfinder -i "$js" -o cli
done > secrets.txt

# 5. No Burp: Target → Site map populado automaticamente pelas ações acima
```

---

## Exercícios

1. No [OWASP Juice Shop](https://juice-shop.herokuapp.com) (ou local), descubra todos os diretórios `/api/*` usando `ffuf`.
2. Configure o Burp e instale 3 extensões da BApp Store. Navegue pela aplicação e tire screenshot do Site Map.
3. Em um JS público (ex.: `https://juice-shop.herokuapp.com/main.js`), rode SecretFinder. Que tipo de strings ele sinaliza?

---

[← Anterior: Recon Ativo](../03-recon-ativo/README.md) · [Voltar ao índice](../../README.md) · [Próximo: Análise de Vulns →](../05-analise-vulns/README.md)
