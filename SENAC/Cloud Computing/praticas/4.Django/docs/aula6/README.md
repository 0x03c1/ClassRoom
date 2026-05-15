# Aula 6 — HTTPS com Let's Encrypt e Hardening

> **Tempo estimado:** 2h

---

## Objetivos

Ao final desta aula você terá:

1. Um **domínio gratuito** apontando para sua VM (DuckDNS ou nip.io)
2. Certificado **HTTPS válido** emitido pelo Let's Encrypt via Certbot
3. Renovação **automática** configurada
4. Redirecionamento **HTTP → HTTPS** no Nginx
5. **Headers de segurança** no Django (HSTS, X-Frame-Options, CSRF_TRUSTED_ORIGINS)
6. **SSH endurecido** (sem login por senha)
7. **Fail2ban** instalado e ativo

---

## 1. Conseguindo um domínio gratuito

Let's Encrypt **não emite certificado para IP**, apenas para nome de domínio. Duas opções:

### Opção A: DuckDNS (subdomain estável e gratuito)

1. Acesse https://www.duckdns.org e faça login com GitHub
2. No campo de subdomain, escolha algo como `roni-django` (vai virar `roni-django.duckdns.org`)
3. Clique em **add domain**
4. Em **current ip**, cole `<IP_VM>` e clique em **update ip**

### Opção B: nip.io (sem cadastro)

O serviço resolve qualquer IP automaticamente. Se seu IP é `20.30.40.50`, use:

```
20-30-40-50.nip.io
```

Resolve para `20.30.40.50` sem precisar cadastrar nada.

> Vou usar `<SEU_DOMINIO>` daqui em diante. Substitua pelo seu.

---

## 2. Atualizando Nginx e Django para o domínio

SSH na VM:

```bash
ssh vm-django
sudo nano /etc/nginx/sites-available/django
```

Troque a linha `server_name`:

```nginx
server_name <SEU_DOMINIO>;
```

Atualize o `.env` do Django:

```bash
sudo nano /var/www/django/app/.env
```

```
ALLOWED_HOSTS=<SEU_DOMINIO>,<IP_VM>,localhost,127.0.0.1
```

Recarregue:

```bash
sudo nginx -t && sudo systemctl reload nginx
sudo systemctl restart gunicorn
```

Teste: `http://<SEU_DOMINIO>` deve abrir a aplicação no navegador.

---

## 3. Emitindo o certificado HTTPS com Certbot

O Certbot foi instalado na Aula 3 junto com o plugin `python3-certbot-nginx`, que edita a config automaticamente.

```bash
sudo certbot --nginx -d <SEU_DOMINIO>
```

Responda:

- **E-mail:** seu e-mail real (recebe alertas de renovação)
- **Terms of Service:** `Y`
- **Compartilhar e-mail com a EFF:** `N` (opcional)

> Não é necessário responder sobre redirecionamento, pois a versão atual do plugin do Nginx configura o redirect HTTP→HTTPS por padrão. Confirme no fim da execução.

Se tudo der certo, aparece:

```
Successfully received certificate.
Certificate is saved at: /etc/letsencrypt/live/<SEU_DOMINIO>/fullchain.pem
Key is saved at:         /etc/letsencrypt/live/<SEU_DOMINIO>/privkey.pem
...
Successfully deployed certificate for <SEU_DOMINIO> to /etc/nginx/sites-enabled/django
Congratulations! You have successfully enabled HTTPS on https://<SEU_DOMINIO>
```

Abra `https://<SEU_DOMINIO>` no navegador. Cadeado deve aparecer fechado.

### 3.1. O que o Certbot alterou

Veja:

```bash
sudo cat /etc/nginx/sites-available/django
```

O plugin adicionou:

- Bloco `listen 443 ssl;` com referência aos arquivos `.pem`
- Bloco extra que faz redirecionamento de `http://` para `https://`

### 3.2. Renovação automática

O pacote `certbot` do Ubuntu instala um **systemd timer** que tenta renovar 2x por dia:

```bash
sudo systemctl status certbot.timer
sudo systemctl list-timers | grep certbot
```

Teste a renovação em modo dry-run (não emite de verdade):

```bash
sudo certbot renew --dry-run
```

Deve mostrar `Congratulations, all simulated renewals succeeded`.

> O Certbot só renova certificados que estão a menos de 30 dias do vencimento.

---

## 4. Headers de segurança no Django

Edite `core/settings.py` **no seu projeto local** (não na VM):

```python
SECURE_PROXY_SSL_HEADER = ('HTTP_X_FORWARDED_PROTO', 'https')

if not DEBUG:
    SECURE_SSL_REDIRECT = True
    SESSION_COOKIE_SECURE = True
    CSRF_COOKIE_SECURE = True

    SECURE_HSTS_SECONDS = 31536000     # 1 ano
    SECURE_HSTS_INCLUDE_SUBDOMAINS = True
    SECURE_HSTS_PRELOAD = True

    SECURE_CONTENT_TYPE_NOSNIFF = True
    SECURE_REFERRER_POLICY = 'same-origin'
    X_FRAME_OPTIONS = 'DENY'
```

Adicione também a leitura de `CSRF_TRUSTED_ORIGINS`:

```python
CSRF_TRUSTED_ORIGINS = config(
    'CSRF_TRUSTED_ORIGINS',
    default='',
    cast=Csv()
)
```

> A diretiva `CSRF_TRUSTED_ORIGINS` é obrigatória no Django 4+ para aceitar POSTs vindos de domínios HTTPS. Sem isso, qualquer envio de formulário no admin retorna `CSRF verification failed`.

Atualize o `.env` da VM:

```bash
sudo nano /var/www/django/app/.env
```

Adicione:

```
CSRF_TRUSTED_ORIGINS=https://<SEU_DOMINIO>
```

### 4.1. Commit e redeploy

**Localmente:**

```bash
git add core/settings.py
git commit -m "feat: headers de seguranca HSTS, CSRF, SSL redirect"
git push
```

**Na VM:**

```bash
ssh vm-django
~/redeploy.sh
```

### 4.2. Confirme os headers

```bash
curl -I https://<SEU_DOMINIO>
```

Deve mostrar (entre outras linhas):

```
HTTP/2 200
strict-transport-security: max-age=31536000; includeSubDomains; preload
x-frame-options: DENY
x-content-type-options: nosniff
referrer-policy: same-origin
```

### 4.3. Avaliação externa

- https://securityheaders.com/?q=<SEU_DOMINIO> — vise nota **A**
- https://www.ssllabs.com/ssltest/analyze.html?d=<SEU_DOMINIO> — vise nota **A**

---

## 5. Hardening do SSH

> Antes de mexer em SSH, mantenha sua sessão atual aberta e teste em uma **segunda aba** do terminal. Se algo der errado e você ficar trancado fora, a primeira aba salva.

### 5.1. Desativar login por senha

```bash
sudo nano /etc/ssh/sshd_config
```

Garanta (descomente ou ajuste):

```
PermitRootLogin no
PasswordAuthentication no
ChallengeResponseAuthentication no
KbdInteractiveAuthentication no
PubkeyAuthentication yes
MaxAuthTries 3
LoginGraceTime 30
AllowUsers azureuser
```

> Antes de aplicar, confirme que sua chave está em `~/.ssh/authorized_keys`:
>
> ```bash
> cat ~/.ssh/authorized_keys
> ```
>
> Deve mostrar a chave pública que você cadastrou na criação da VM (Aula 3).

Aplique:

```bash
sudo systemctl reload ssh
```

Em **outra aba** do terminal local:

```bash
ssh vm-django
```

Se entrar, está OK. Só feche a sessão antiga depois de confirmar.

---

## 6. Fail2ban — proteção contra força bruta

```bash
sudo apt install -y fail2ban
```

Crie um override:

```bash
sudo nano /etc/fail2ban/jail.d/django-app.local
```

```ini
[DEFAULT]
bantime  = 1h
findtime = 10m
maxretry = 5

[sshd]
enabled = true
backend = systemd
```

Reinicie:

```bash
sudo systemctl restart fail2ban
sudo systemctl status fail2ban
sudo fail2ban-client status sshd
```

Saída esperada:

```
Status for the jail: sshd
|- Filter
|  |- Currently failed: 0
|  `- Total failed:     0
`- Actions
   |- Currently banned: 0
   `- Total banned:     0
```

---

## 7. Atualizações automáticas de segurança

```bash
sudo apt install -y unattended-upgrades
sudo dpkg-reconfigure -plow unattended-upgrades
```

Quando perguntar **Automatically download and install stable updates?**, escolha `Yes`.

Confirme:

```bash
sudo systemctl status unattended-upgrades
```

Patches de segurança críticos passam a ser aplicados automaticamente.

---

## Checkpoint

- [ ] `https://<SEU_DOMINIO>` abre com cadeado verde no navegador
- [ ] `http://<SEU_DOMINIO>` redireciona automaticamente para HTTPS
- [ ] `curl -I https://<SEU_DOMINIO>` retorna headers `strict-transport-security`, `x-frame-options`, etc.
- [ ] https://securityheaders.com nota **A** ou superior
- [ ] SSH só aceita chave (login por senha falha)
- [ ] `sudo fail2ban-client status sshd` mostra jail ativa
- [ ] `sudo certbot renew --dry-run` passa sem erro

---

## Troubleshooting

### `Domain validation failed` ao rodar certbot
- Confirme que o domínio aponta para o IP correto: `dig +short <SEU_DOMINIO>`
- O Let's Encrypt valida pela porta 80 — ela tem que estar aberta no NSG e no UFW
- Aguarde até 10 minutos após atualizar o DNS (propagação)

### `CSRF verification failed` ao fazer login no /admin
Faltou `CSRF_TRUSTED_ORIGINS=https://<SEU_DOMINIO>` no `.env`. Adicione e reinicie:
```bash
sudo systemctl restart gunicorn
```

### Fiquei trancado fora depois de desativar PasswordAuthentication
- Use **Serial Console** pelo Portal Azure: VM > **Help > Serial Console**. Faça login com a senha do sistema (você pode resetar via VM > **Help > Reset password**)
- Reedite `/etc/ssh/sshd_config` e volte `PasswordAuthentication yes` temporariamente

### SSL Labs nota B em vez de A
Provavelmente o Nginx aceita TLS antigos. Adicione na config do site (dentro do bloco SSL):

```nginx
ssl_protocols TLSv1.2 TLSv1.3;
ssl_prefer_server_ciphers on;
```

Recarregue: `sudo nginx -t && sudo systemctl reload nginx`.

---

## Desafio extra

1. Configure um **rate limit** no Nginx para o endpoint `/admin/` (ex.: 5 requests por minuto por IP), usando o módulo `ngx_http_limit_req_module`.
2. Adicione um header `Content-Security-Policy` no Nginx (requer ajustes finos no template para não quebrar inline styles).
3. No Portal Azure, crie um **Alert rule** que envia e-mail quando o CPU da VM passa de 80% por 5 minutos. (Portal > VM > **Alerts > + Create alert rule**)

---

**Próxima aula:** [Aula 7 — CI/CD com GitHub Actions](../aula7/README.md)
