# Aula 5 — Deploy Manual: Gunicorn + Nginx + systemd

> **Tempo estimado:** 2h

---

## Objetivos

Ao final desta aula você terá:

1. Código do projeto **clonado na VM**
2. **Gunicorn** executando o Django como serviço gerenciado por systemd
3. **Nginx** como reverse proxy + servindo arquivos estáticos
4. Aplicação acessível publicamente em `http://<IP_VM>`
5. Serviço resiliente: reinicia sozinho se cair, sobe automaticamente no boot

---

## 1. Arquitetura

```
  Usuário
     |
     |  HTTP :80
     v
  +----------------+
  |    NGINX       |  estáticos -> /var/www/django/staticfiles
  |  reverse-proxy |
  +-------+--------+
          |  Unix socket /run/gunicorn.sock
          v
  +----------------+
  |   GUNICORN     |  3 workers, gerenciado pelo systemd
  +-------+--------+
          |
          v
       Django
          |
          v
       PostgreSQL Azure (Aula 4)
```

| Componente   | Papel                                                                       |
| ------------ | --------------------------------------------------------------------------- |
| Nginx        | Recebe HTTP da internet, serve estáticos, faz proxy para o Gunicorn         |
| Gunicorn     | Servidor WSGI de produção — executa o Django em processos worker            |
| systemd      | Mantém o Gunicorn vivo (restart, auto-start no boot, logs centralizados)    |
| Unix socket  | Comunicação Nginx<->Gunicorn local, sem custo de TCP                        |

---

## 2. Clonando o projeto na VM

SSH na VM:

```bash
ssh vm-django
```

### 2.1. Crie o diretório do projeto

```bash
sudo mkdir -p /var/www/django
sudo chown -R azureuser:www-data /var/www/django
sudo chmod -R 775 /var/www/django
```

### 2.2. Clone o repositório

Como o repositório é **público** (criado na Aula 2), use HTTPS (não precisa de chave SSH para clonar):

```bash
cd /var/www/django
git clone https://github.com/<seu-usuario>/django-azure-pratica.git app
cd app
```

> Se você tornou o repositório privado, será necessário configurar uma **deploy key** (chave SSH específica) na VM. Para esta aula, mantenha público.

### 2.3. Crie o venv e instale dependências

```bash
python3 -m venv venv
source venv/bin/activate
pip install --upgrade pip
pip install -r requirements.txt
```

### 2.4. Crie o `.env` na VM

O `.env` **não está no Git**, então tem que criar manualmente na VM:

```bash
nano /var/www/django/app/.env
```

Cole, ajustando os valores reais:

```
SECRET_KEY=cole-aqui-uma-secret-key-aleatoria-de-50-caracteres
DEBUG=False
ALLOWED_HOSTS=<IP_VM>,localhost,127.0.0.1

DB_ENGINE=django.db.backends.postgresql
DB_NAME=django_pratica
DB_USER=django_app
DB_PASSWORD=TrocarPorSenh@Forte5678!
DB_HOST=<PG_HOST>.postgres.database.azure.com
DB_PORT=5432
DB_SSLMODE=require
```

Salve com `Ctrl+O`, Enter, `Ctrl+X`.

> Confirme `DEBUG=False`. Com DEBUG=True em produção, qualquer erro mostra stack traces com variáveis sensíveis para o mundo.

Gere uma SECRET_KEY nova diretamente na VM:

```bash
cd /var/www/django/app
source venv/bin/activate
python -c "from django.core.management.utils import get_random_secret_key; print(get_random_secret_key())"
```

Cole no `.env`.

### 2.5. Rode migrações e colete estáticos

```bash
cd /var/www/django/app
source venv/bin/activate

python manage.py migrate
python manage.py collectstatic --noinput
```

### 2.6. Teste o Gunicorn manualmente (sanity check)

```bash
gunicorn --bind 0.0.0.0:8000 core.wsgi:application
```

Em outra aba SSH (ou no navegador): `http://<IP_VM>:8000` — deve carregar.

> Se aparecer `DisallowedHost`, confirme que `<IP_VM>` está em `ALLOWED_HOSTS` no `.env`. Se aparecer "site can't be reached", verifique no Portal Azure que a porta **8000 NÃO precisa estar liberada** (estamos só testando localmente, mas o NSG só libera 22/80/443 — está correto). Para esse teste, abra temporariamente:
>
> Portal > VM > **Networking > Network settings** > **Add inbound port rule** > Destination port: 8000, Protocol: TCP, Action: Allow. Apague essa regra depois.

Pressione `Ctrl+C` para parar o Gunicorn de teste.

---

## 3. Configurando Gunicorn como serviço systemd

Vamos criar **dois arquivos**: o socket (canal de comunicação) e o service (o processo em si).

### 3.1. Socket

```bash
sudo nano /etc/systemd/system/gunicorn.socket
```

Conteúdo:

```ini
[Unit]
Description=gunicorn socket for Django

[Socket]
ListenStream=/run/gunicorn.sock
SocketUser=www-data
SocketGroup=www-data
SocketMode=0660

[Install]
WantedBy=sockets.target
```

### 3.2. Service

```bash
sudo nano /etc/systemd/system/gunicorn.service
```

Conteúdo:

```ini
[Unit]
Description=gunicorn daemon for Django app
Requires=gunicorn.socket
After=network.target

[Service]
User=www-data
Group=www-data
WorkingDirectory=/var/www/django/app
EnvironmentFile=/var/www/django/app/.env
ExecStart=/var/www/django/app/venv/bin/gunicorn \
          --access-logfile - \
          --workers 3 \
          --bind unix:/run/gunicorn.sock \
          core.wsgi:application
Restart=always
RestartSec=5

[Install]
WantedBy=multi-user.target
```

> A regra comum para `--workers` é `(2 × núcleos) + 1`. Em uma B1s de 1 vCPU, 3 workers cabem sem stress excessivo.

### 3.3. Ajuste permissões

```bash
sudo chown -R www-data:www-data /var/www/django/app
sudo chmod 640 /var/www/django/app/.env
```

### 3.4. Habilite e inicie

```bash
sudo systemctl daemon-reload
sudo systemctl enable --now gunicorn.socket
sudo systemctl status gunicorn.socket
```

Saída deve conter:

```
Active: active (listening) since ...
Listen: /run/gunicorn.sock (Stream)
```

Teste o socket (isso aciona o service automaticamente):

```bash
curl --unix-socket /run/gunicorn.sock http://localhost/
```

Deve retornar o HTML da página.

Confirme que o service está rodando:

```bash
sudo systemctl status gunicorn
```

---

## 4. Configurando o Nginx

### 4.1. Remova a config padrão

```bash
sudo rm /etc/nginx/sites-enabled/default
```

### 4.2. Crie a config do site

```bash
sudo nano /etc/nginx/sites-available/django
```

Conteúdo (substitua `<IP_VM>` pelo IP real):

```nginx
server {
    listen 80;
    listen [::]:80;
    server_name <IP_VM>;

    client_max_body_size 10M;

    access_log /var/log/nginx/django-access.log;
    error_log  /var/log/nginx/django-error.log;

    location = /favicon.ico {
        access_log off;
        log_not_found off;
    }

    location /static/ {
        alias /var/www/django/app/staticfiles/;
        expires 30d;
        add_header Cache-Control "public, immutable";
    }

    location /media/ {
        alias /var/www/django/app/media/;
    }

    location / {
        include proxy_params;
        proxy_pass http://unix:/run/gunicorn.sock;
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
        proxy_set_header X-Forwarded-Proto $scheme;
    }
}
```

### 4.3. Ative o site e teste

```bash
sudo ln -s /etc/nginx/sites-available/django /etc/nginx/sites-enabled/
sudo nginx -t
```

Saída esperada:

```
nginx: the configuration file /etc/nginx/nginx.conf syntax is ok
nginx: configuration file /etc/nginx/nginx.conf test is successful
```

Recarregue:

```bash
sudo systemctl reload nginx
```

### 4.4. Permissões para o Nginx ler os estáticos

```bash
sudo chmod o+x /var/www /var/www/django /var/www/django/app
sudo chmod -R o+rX /var/www/django/app/staticfiles
```

---

## 5. Teste final

No navegador, abra:

`http://<IP_VM>`

Deve carregar a lista de tarefas com estilo aplicado (se o CSS aparece, é porque Nginx + estáticos está OK).

Endpoints para validar:

- `/` — lista
- `/health/` — `{"status": "ok"}`
- `/admin/` — tela de login do admin

---

## 6. Resiliência: simule um crash

```bash
sudo systemctl kill gunicorn
sleep 6
sudo systemctl status gunicorn
```

Com `Restart=always` + `RestartSec=5`, o systemd reinicia o serviço sozinho.

Reinicie a VM toda para garantir que sobe no boot:

```bash
sudo reboot
```

Aguarde ~1 minuto, reconecte:

```bash
ssh vm-django
sudo systemctl is-active nginx gunicorn
```

Ambos devem retornar `active`.

---

## 7. Script de redeploy manual

Vai ser usado pela CI/CD na Aula 7. Crie:

```bash
nano ~/redeploy.sh
```

Conteúdo:

```bash
#!/bin/bash
set -euo pipefail

PROJECT_DIR=/var/www/django/app

cd "$PROJECT_DIR"

# Permite que o azureuser puxe do git
sudo chown -R azureuser:www-data .

git fetch origin main
git reset --hard origin/main

source venv/bin/activate
pip install --quiet -r requirements.txt
python manage.py migrate --noinput
python manage.py collectstatic --noinput

# Devolve ownership ao www-data
sudo chown -R www-data:www-data .
sudo chmod 640 .env

sudo systemctl restart gunicorn
echo "Deploy concluido em $(date -u +%FT%TZ)"
```

```bash
chmod +x ~/redeploy.sh
```

> Esse script ainda usa `sudo`, então precisa de senha. A Aula 7 vai configurar `sudoers` para permitir esses comandos sem senha (necessário no CI/CD).

---

## Checkpoint

Verifique todos os itens:

- [ ] `http://<IP_VM>` carrega a lista de tarefas com CSS aplicado
- [ ] Criar uma tarefa pela interface funciona e persiste após F5
- [ ] `/admin/` mostra a tela de login com estilo correto
- [ ] `sudo systemctl is-active gunicorn` retorna `active`
- [ ] `sudo systemctl is-active nginx` retorna `active`
- [ ] Após `sudo reboot`, os serviços voltam sozinhos
- [ ] `~/redeploy.sh` executa sem erro

Se tudo confirmado, pode seguir para a Aula 6.

---

## Troubleshooting

### `502 Bad Gateway` no Nginx
Causa quase sempre é Gunicorn caído ou socket sem permissão. Diagnóstico:

```bash
sudo journalctl -u gunicorn -n 50 --no-pager
```

Possíveis causas:
- **Permissão**: `www-data` não consegue ler algum arquivo. Revise `chown` em 3.3.
- **`.env` ausente** ou variável faltando.
- **Senha do PostgreSQL errada** no `.env`.

### Estáticos não carregam (página sem CSS)
- Esqueceu `python manage.py collectstatic`
- Permissão: `sudo chmod -R o+rX /var/www/django/app/staticfiles`
- Verifique o `alias` no Nginx — tem que terminar com `/`

### `connect() to unix:/run/gunicorn.sock failed (13: Permission denied)`
- O socket está sem permissão para o www-data
- Confira `/etc/systemd/system/gunicorn.socket` — `SocketUser=www-data` e `SocketGroup=www-data`
- `sudo systemctl restart gunicorn.socket gunicorn`

### `DisallowedHost at /`
- O IP/domínio não está em `ALLOWED_HOSTS` do `.env`
- Edite e reinicie: `sudo systemctl restart gunicorn`

### Onde ver os logs?

```bash
sudo journalctl -u gunicorn -f                 # Gunicorn em tempo real
sudo tail -f /var/log/nginx/django-access.log  # acessos ao Nginx
sudo tail -f /var/log/nginx/django-error.log   # erros do Nginx
```

---

## Desafio extra

1. Ajuste `gunicorn.service` para usar `--worker-class gthread --workers 2 --threads 4`. Pesquise a diferença entre **sync workers** e **threaded workers** e justifique a escolha em comentário no service file.
2. Adicione um endpoint `/version/` que retorna o hash do commit Git atual. Isso ajuda a confirmar qual versão está no ar:
   ```python
   import subprocess
   def version(request):
       sha = subprocess.check_output(['git', '-C', '/var/www/django/app', 'rev-parse', 'HEAD']).decode().strip()
       return JsonResponse({'commit': sha})
   ```
3. Configure **logrotate** para `django-access.log` rotacionar a cada 100 MB.

---

**Próxima aula:** [Aula 6 — HTTPS com Let's Encrypt e hardening](../aula6/README.md)
