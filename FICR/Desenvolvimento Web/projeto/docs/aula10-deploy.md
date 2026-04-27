#  Aula 10 — Deploy da Aplicação

> **Objetivo:** preparar a aplicação Django para produção e publicá-la em uma plataforma gratuita (PythonAnywhere ou Render), tornando-a acessível na internet.

---

## 1. Diferenças entre desenvolvimento e produção

| Aspecto | Desenvolvimento | Produção |
|---------|-----------------|----------|
| `DEBUG` | `True` | **`False`** |
| Banco de dados | SQLite | PostgreSQL/MySQL |
| Servidor | `runserver` (Django) | Gunicorn / uWSGI |
| Estáticos | servidos pelo Django | servidos pelo Nginx / WhiteNoise |
| `SECRET_KEY` | hardcoded | variável de ambiente |
| HTTPS | http://localhost | **https://** obrigatório |

>  **Nunca** suba código em produção com `DEBUG=True`. Isso expõe variáveis sensíveis e o conteúdo do banco em qualquer página de erro.

---

## 2. Preparando o projeto para produção

### 2.1. Variáveis de ambiente com `python-decouple`

Instale:

```bash
pip install python-decouple
```

Crie um arquivo `.env` na raiz (não comite!):

```env
SECRET_KEY=django-insecure-altere-essa-chave-em-producao
DEBUG=True
ALLOWED_HOSTS=localhost,127.0.0.1
```

>  **Lembre-se:** `.env` já está no `.gitignore` desde a Aula 1.

Em `setup/settings.py`, no topo:

```python
from decouple import config, Csv
```

E substitua:

```python
SECRET_KEY = config('SECRET_KEY')
DEBUG = config('DEBUG', default=False, cast=bool)
ALLOWED_HOSTS = config('ALLOWED_HOSTS', default='', cast=Csv())
```

Crie também um `.env.example` (este sim deve ser comitado), só com a estrutura:

```env
SECRET_KEY=
DEBUG=False
ALLOWED_HOSTS=
```

---

### 2.2. Servindo arquivos estáticos com WhiteNoise

WhiteNoise é uma biblioteca que permite servir arquivos estáticos diretamente pela aplicação Django, sem precisar de Nginx.

Instale:

```bash
pip install whitenoise
```

Adicione ao `MIDDLEWARE` de `settings.py` — **logo após** o `SecurityMiddleware`:

```python
MIDDLEWARE = [
    'django.middleware.security.SecurityMiddleware',
    'whitenoise.middleware.WhiteNoiseMiddleware',  # ← adicionar aqui
    # ... outros middlewares ...
]
```

E configure o storage:

```python
STATIC_URL = '/static/'
STATIC_ROOT = BASE_DIR / 'staticfiles'
STATICFILES_DIRS = [BASE_DIR / 'static']

STORAGES = {
    'default': {
        'BACKEND': 'django.core.files.storage.FileSystemStorage',
    },
    'staticfiles': {
        'BACKEND': 'whitenoise.storage.CompressedManifestStaticFilesStorage',
    },
}
```

---

### 2.3. Gunicorn (servidor WSGI)

```bash
pip install gunicorn
```

Para testar localmente:

```bash
gunicorn setup.wsgi
```

A aplicação subirá em `http://127.0.0.1:8000/`.

---

### 2.4. Atualize o `requirements.txt`

```bash
pip freeze > requirements.txt
```

Verifique se ele contém:

```
Django==5.x
Pillow==10.x
gunicorn==21.x
whitenoise==6.x
python-decouple==3.x
psycopg2-binary==2.x   # somente se for usar PostgreSQL
```

---

### 2.5. Criando o `Procfile` (para Render/Heroku-like)

Crie na raiz um arquivo chamado `Procfile` (sem extensão):

```
web: gunicorn setup.wsgi
release: python manage.py migrate
```

---

## 3. Opção A: Deploy no Render (recomendado)

### 3.1. Por que o Render?

-  Plano gratuito.
-  Suporta Django nativamente.
-  Deploy automático via GitHub.
-  HTTPS gratuito.
-  PostgreSQL gerenciado (free tier).

### 3.2. Passo a passo

1. **Crie uma conta** em https://render.com (login com GitHub é mais rápido).
2. No dashboard, clique em **New + → Web Service**.
3. Conecte seu repositório do GitHub (`livraria-django`).
4. Configure:
   - **Name:** `livraria-django`
   - **Region:** mais próxima de você
   - **Branch:** `main`
   - **Runtime:** Python 3
   - **Build Command:**
     ```
     pip install -r requirements.txt && python manage.py collectstatic --no-input && python manage.py migrate
     ```
   - **Start Command:**
     ```
     gunicorn setup.wsgi
     ```
   - **Plan:** Free
5. Em **Environment Variables**, adicione:
   - `SECRET_KEY` → algo gerado aleatório (use https://djecrety.ir).
   - `DEBUG` → `False`
   - `ALLOWED_HOSTS` → `livraria-django.onrender.com` (ou o domínio que o Render gerar).
6. Clique em **Create Web Service**.

O Render começa o build automaticamente. Em ~5 minutos sua app está no ar! 

>  Para banco PostgreSQL gerenciado: **New + → PostgreSQL** (free tier), copie a `Internal Database URL` para uma env var `DATABASE_URL` e ajuste o settings com `dj-database-url`.

---

## 4. Opção B: Deploy no PythonAnywhere

### 4.1. Vantagens

-  Plano gratuito **permanente** (sem cartão de crédito).
-  Interface web simples.
-  Bash console no navegador.
-  A aplicação fica em `seuusuario.pythonanywhere.com` (não permite domínio próprio no plano free).

### 4.2. Passo a passo

1. Crie conta em https://www.pythonanywhere.com.
2. Vá em **Consoles → Bash**.
3. Clone o repositório:
   ```bash
   git clone https://github.com/SEU-USUARIO/livraria-django.git
   cd livraria-django
   ```
4. Crie venv e instale:
   ```bash
   python3.11 -m venv venv
   source venv/bin/activate
   pip install -r requirements.txt
   ```
5. Crie o `.env` com as variáveis (use `nano .env`).
6. Aplique migrações e collectstatic:
   ```bash
   python manage.py migrate
   python manage.py collectstatic --no-input
   python manage.py createsuperuser
   ```
7. Vá em **Web → Add a new web app**:
   - Framework: **Manual configuration** (não escolha Django pronto).
   - Versão Python: 3.11.
8. Configure:
   - **Source code:** `/home/SEU-USUARIO/livraria-django`
   - **Working directory:** mesmo path
   - **WSGI configuration file:** edite o arquivo apontando o `setup.wsgi`:
     ```python
     import os
     import sys

     path = '/home/SEU-USUARIO/livraria-django'
     if path not in sys.path:
         sys.path.append(path)

     os.environ['DJANGO_SETTINGS_MODULE'] = 'setup.settings'

     from django.core.wsgi import get_wsgi_application
     application = get_wsgi_application()
     ```
   - **Virtualenv:** `/home/SEU-USUARIO/livraria-django/venv`
   - **Static files:**
     - URL: `/static/` → Path: `/home/SEU-USUARIO/livraria-django/staticfiles`
     - URL: `/media/` → Path: `/home/SEU-USUARIO/livraria-django/media`
9. Clique em **Reload** no topo da página Web.

Sua app está em `https://SEU-USUARIO.pythonanywhere.com`! 

---

## 5. Checklist de segurança em produção

Antes de declarar "deploy concluído", confira:

- [ ] `DEBUG = False`
- [ ] `SECRET_KEY` em variável de ambiente (não no código).
- [ ] `ALLOWED_HOSTS` com seu domínio real.
- [ ] `CSRF_TRUSTED_ORIGINS` configurado (Django 4+):
  ```python
  CSRF_TRUSTED_ORIGINS = ['https://seu-dominio.onrender.com']
  ```
- [ ] HTTPS funcionando (certificado válido).
- [ ] `SECURE_SSL_REDIRECT = True` (força HTTPS).
- [ ] `SESSION_COOKIE_SECURE = True`.
- [ ] `CSRF_COOKIE_SECURE = True`.
- [ ] Senhas fortes para superusuários.
- [ ] Pacotes atualizados: `pip list --outdated`.
- [ ] Comando `python manage.py check --deploy` sem warnings críticos.

---

## 6. Configurações de produção recomendadas

Adicione ao final de `settings.py`:

```python
if not DEBUG:
    # Segurança
    SECURE_SSL_REDIRECT = True
    SESSION_COOKIE_SECURE = True
    CSRF_COOKIE_SECURE = True
    SECURE_HSTS_SECONDS = 31536000
    SECURE_HSTS_INCLUDE_SUBDOMAINS = True
    SECURE_HSTS_PRELOAD = True
    SECURE_REFERRER_POLICY = 'same-origin'
    X_FRAME_OPTIONS = 'DENY'
    SECURE_PROXY_SSL_HEADER = ('HTTP_X_FORWARDED_PROTO', 'https')
```

---

## 7. Commit final

```bash
git add .
git commit -m "feat: configura projeto para produção (whitenoise, decouple, gunicorn)"
git push
```

---

##  Exercícios

1. Faça o deploy real da sua livraria (Render ou PythonAnywhere) e envie o link funcional ao professor.

2. Acesse `/admin/` no domínio público e cadastre 3 livros pelo painel.

3. Execute `python manage.py check --deploy` localmente (com `DEBUG=False`) e corrija pelo menos 3 alertas.

4. **Desafio:** Configure o PostgreSQL no Render (em vez do SQLite) usando o pacote `dj-database-url`. Documente em `docs/extra-postgres.md`.

---

##  Erros comuns

| Erro | Causa | Solução |
|------|-------|---------|
| `DisallowedHost at /` | Domínio não está em `ALLOWED_HOSTS` | Adicione na env var. |
| Página 500 sem detalhe | `DEBUG=False` (correto!) | Veja os logs do servidor; nunca volte para True. |
| CSS não carrega | Esqueceu `collectstatic` | Rode no build/release. |
| `OperationalError: no such table` | Esqueceu `migrate` | Adicione `python manage.py migrate` no build. |
| `SECRET_KEY not set` | Variável de ambiente faltando | Configure no painel da plataforma. |

---

##  Checklist

- [ ] `python-decouple` instalado e configurado.
- [ ] `whitenoise` configurado.
- [ ] `gunicorn` instalado.
- [ ] `Procfile` criado.
- [ ] `requirements.txt` atualizado.
- [ ] Variáveis de ambiente configuradas na plataforma.
- [ ] Aplicação publicada e acessível por HTTPS.
- [ ] `python manage.py check --deploy` sem alertas críticos.
- [ ] Commit realizado.

---

 **Parabéns! Você concluiu o projeto.**

- **Material complementar:** [Aula 99 — Referências](./aula99-referencias.md)
- **Aula anterior:** [Aula 9 — Autenticação](./aula9-autenticacao.md)
