# 02 · Reconhecimento Passivo (OSINT)

> [← Anterior: Ambiente](../01-ambiente/README.md) · [Voltar ao índice](../../README.md) · [Próximo: Recon Ativo →](../03-recon-ativo/README.md)

---

## Princípio Fundamental

**Extrair o máximo de informação sem enviar um único pacote ao alvo.** Toda query acontece contra bases públicas ou serviços de terceiros.

---

## 1. Infraestrutura e Superfície de Ataque

### Shodan

Motor de busca de dispositivos conectados à internet.

```bash
# Instalação
pipx install shodan
shodan init SEU_API_KEY

# Buscas
shodan search hostname:alvo.com.br
shodan search "org:Cliente X" port:22,80,443
shodan search 'vuln:CVE-2024-6387' net:192.0.2.0/24

# Host específico
shodan host 192.0.2.1

# Estatísticas (facets)
shodan stats --facets port,org,os net:192.0.2.0/24
```

### Censys

Similar ao Shodan, foco especial em certificados TLS.

```bash
# CLI: pipx install censys
censys search "services.service_name: HTTP and services.port: 443 and alvo.com.br"
```

Excelente para descobrir **subdomínios via Certificate Transparency**: certificados wildcard revelam infraestrutura.

### Google Dorks

Operadores avançados do Google.

```
site:alvo.com.br filetype:pdf
site:alvo.com.br inurl:admin
site:alvo.com.br intitle:"index of"
site:alvo.com.br ext:sql OR ext:log OR ext:env
"@alvo.com.br" -site:alvo.com.br            # emails fora do site
site:github.com "alvo.com.br" password      # leaks em repositórios
```

Referência: [Google Hacking Database (GHDB)](https://www.exploit-db.com/google-hacking-database).

### FOCA

Extração de metadados de documentos públicos (PDF, DOC, PPT, XLS). Revela:
- Usernames (autor do documento)
- Versões de software
- Paths internos de servidores
- Impressoras de rede

### theHarvester

Coleta emails, subdomínios, IPs e hostnames de múltiplas fontes.

```bash
theHarvester -d alvo.com.br -b all -l 500
theHarvester -d alvo.com.br -b bing,crtsh,duckduckgo -f resultado.html
```

### Maltego

Plataforma visual de OSINT com **transforms** automáticos. Mapeia relações entre:
- Domínios ↔ IPs ↔ ASNs
- Emails ↔ Pessoas ↔ Redes Sociais
- Empresas ↔ Funcionários ↔ Tecnologias

Community Edition é gratuita e suficiente para aulas.

---

## 2. Pessoas e Credenciais Vazadas

| Ferramenta | Uso |
|---|---|
| **Hunter.io** | Descobre padrão de email corporativo (`nome.sobrenome@`) |
| **LinkedIn + PhantomBuster** | Scraping de funcionários para gerar wordlists |
| **Have I Been Pwned** | Verifica se email/domínio apareceu em leaks públicos |
| **DeHashed** | Base paga de credenciais vazadas (com senhas em claro) |
| **IntelX** | Motor de busca em leaks, pastebins e darknet |
| **Wayback Machine** | Snapshots históricos — descobre endpoints removidos |
| **SecurityTrails** | Histórico de DNS (A, MX, NS) — infra passada ainda ativa |

---

## 3. DNS e Subdomínios (passivo)

### Amass

```bash
# Apenas fontes passivas — não toca no alvo
amass enum -passive -d alvo.com.br -o subs.txt

# Resolução de grafo (também passivo)
amass intel -org "Cliente X"
```

### Subfinder

Muito rápido, múltiplas fontes via API keys em `~/.config/subfinder/provider-config.yaml`.

```bash
subfinder -d alvo.com.br -all -o subs.txt
subfinder -dL dominios.txt -all -silent | httpx -silent
```

### crt.sh (Certificate Transparency)

```bash
# Query direta à API pública
curl -s "https://crt.sh/?q=%25.alvo.com.br&output=json" \
  | jq -r '.[].name_value' \
  | sort -u \
  | tee crtsh-subs.txt
```

### Pipeline combinado (recomendado)

```bash
# Combina fontes, remove duplicados, resolve e identifica ativos
(subfinder -d alvo.com.br -all -silent; \
 amass enum -passive -d alvo.com.br -silent; \
 curl -s "https://crt.sh/?q=%25.alvo.com.br&output=json" | jq -r '.[].name_value') \
 | sort -u \
 | httpx -silent -title -tech-detect -status-code \
 > live-hosts.txt
```

---

## 4. Ferramentas Agregadoras

| Ferramenta | Função |
|---|---|
| **Spiderfoot** | Automatiza 200+ fontes OSINT, interface web |
| **Recon-ng** | Framework modular estilo Metasploit para OSINT |
| **Photon** | Crawler focado em endpoints e parâmetros do Wayback |

---

## 5. Exemplo de Roteiro Passivo Completo

```bash
ALVO="alvo.com.br"
mkdir -p recon/$ALVO && cd recon/$ALVO

# 1. Subdomínios
subfinder -d $ALVO -all -silent -o subs.txt
amass enum -passive -d $ALVO -silent -o amass.txt
cat subs.txt amass.txt | sort -u > all-subs.txt

# 2. Validação de quem está ativo (ainda não é "ativo" pois httpx faz só HTTP public check)
httpx -l all-subs.txt -title -tech-detect -status-code -o live.txt

# 3. Emails e metadados
theHarvester -d $ALVO -b all -f harvester.html

# 4. Verificação de leaks
# (manual via Have I Been Pwned / IntelX)

# 5. Google Dorks salvos
echo "site:$ALVO filetype:pdf" > dorks.txt
echo "site:$ALVO inurl:admin" >> dorks.txt
```

---

## Exercícios

1. Execute o roteiro acima contra um **domínio próprio ou autorizado** (ex.: seu site pessoal). Entregue `live.txt` e `harvester.html`.
2. Pesquise no Shodan (sem conta paga): `http.title:"Welcome" country:BR port:8080`. Quantos resultados? Que padrões de banner aparecem?
3. No crt.sh, busque o domínio `cesar.school` e liste 5 subdomínios.

---

[← Anterior: Ambiente](../01-ambiente/README.md) · [Voltar ao índice](../../README.md) · [Próximo: Recon Ativo →](../03-recon-ativo/README.md)
