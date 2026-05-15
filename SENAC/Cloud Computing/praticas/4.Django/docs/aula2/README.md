# Aula 2 — Projeto Django + GitHub

> **Tempo estimado:** 2h

---

## Objetivos

Ao final desta aula você terá:

1. Um projeto Django funcional rodando em `http://localhost:8000`
2. Banco SQLite local para o desenvolvimento + uma view simples (lista de tarefas)
3. Configuração de **variáveis de ambiente** com `python-decouple`
4. Projeto versionado no GitHub
5. Estrutura preparada para produção (já com `requirements.txt`, `.gitignore`, `.env.example`)

---

## 1. Criando o projeto Django

### 1.1. Estrutura do diretório

```bash
cd ~/projetos              # ou onde você guarda seus projetos
mkdir django-azure-pratica && cd django-azure-pratica
```

### 1.2. Ambiente virtual (venv)

Por que: para isolar as dependências do projeto. Cada projeto Python deve ter seu próprio venv.

```bash
# Linux / macOS / Git Bash
python3 -m venv venv
source venv/bin/activate

# Windows PowerShell
python -m venv venv
.\venv\Scripts\Activate.ps1
```

Se aparecer `(venv)` no início do prompt, está ativado.

### 1.3. Instalando dependências

```bash
pip install --upgrade pip
pip install "django>=5.1,<5.2" "psycopg[binary]>=3.2" python-decouple gunicorn whitenoise
pip freeze > requirements.txt
```

| Pacote              | Função                                                       |
| ------------------- | ------------------------------------------------------------ |
| `django`            | O framework web (5.1 LTS)                                    |
| `psycopg[binary]`   | Driver PostgreSQL para Django/Python (será usado na Aula 4)  |
| `python-decouple`   | Lê variáveis de ambiente a partir de um arquivo `.env`       |
| `gunicorn`          | Servidor WSGI de produção (será usado na Aula 5)             |
| `whitenoise`        | Serve arquivos estáticos em produção sem precisar de S3/Blob |

### 1.4. Criando o projeto e a app

```bash
django-admin startproject core .
python manage.py startapp tarefas
```

> O ponto final em `startproject core .` é importante — cria o projeto na pasta atual ao invés de criar um subdiretório a mais.

Estrutura esperada:

```
django-azure-pratica/
├── core/                 # configuração do projeto
│   ├── __init__.py
│   ├── settings.py
│   ├── urls.py
│   └── wsgi.py
├── tarefas/              # app
│   ├── migrations/
│   ├── admin.py
│   ├── apps.py
│   ├── models.py
│   ├── tests.py
│   └── views.py
├── manage.py
├── venv/
└── requirements.txt
```

---

## 2. Configurando `settings.py`

Edite `core/settings.py`. Apenas as mudanças importantes estão destacadas.

### 2.1. Importações no topo do arquivo

```python
from pathlib import Path
from decouple import config, Csv
```

### 2.2. Substitua as variáveis sensíveis

Encontre no arquivo:

```python
SECRET_KEY = 'django-insecure-...'
DEBUG = True
ALLOWED_HOSTS = []
```

Substitua por:

```python
SECRET_KEY = config('SECRET_KEY')
DEBUG = config('DEBUG', default=False, cast=bool)
ALLOWED_HOSTS = config('ALLOWED_HOSTS', default='127.0.0.1,localhost', cast=Csv())
```

### 2.3. Adicione a app `tarefas` e o WhiteNoise

```python
INSTALLED_APPS = [
    'django.contrib.admin',
    'django.contrib.auth',
    'django.contrib.contenttypes',
    'django.contrib.sessions',
    'django.contrib.messages',
    'django.contrib.staticfiles',
    'tarefas',
]

MIDDLEWARE = [
    'django.middleware.security.SecurityMiddleware',
    'whitenoise.middleware.WhiteNoiseMiddleware',
    'django.contrib.sessions.middleware.SessionMiddleware',
    'django.middleware.common.CommonMiddleware',
    'django.middleware.csrf.CsrfViewMiddleware',
    'django.contrib.auth.middleware.AuthenticationMiddleware',
    'django.contrib.messages.middleware.MessageMiddleware',
    'django.middleware.clickjacking.XFrameOptionsMiddleware',
]
```

### 2.4. Configure o banco

Esta configuração permite **SQLite localmente** e **PostgreSQL em produção** apenas trocando o `.env`:

```python
DATABASES = {
    'default': {
        'ENGINE': config('DB_ENGINE', default='django.db.backends.sqlite3'),
        'NAME': config('DB_NAME', default=str(BASE_DIR / 'db.sqlite3')),
        'USER': config('DB_USER', default=''),
        'PASSWORD': config('DB_PASSWORD', default=''),
        'HOST': config('DB_HOST', default=''),
        'PORT': config('DB_PORT', default=''),
    }
}

if config('DB_ENGINE', default='').endswith('postgresql'):
    DATABASES['default']['OPTIONS'] = {
        'sslmode': config('DB_SSLMODE', default='require'),
    }
```

### 2.5. Arquivos estáticos

No final do arquivo:

```python
STATIC_URL = 'static/'
STATIC_ROOT = BASE_DIR / 'staticfiles'
STATICFILES_STORAGE = 'whitenoise.storage.CompressedManifestStaticFilesStorage'
```

---

## 3. Arquivos `.env` e `.env.example`

Na raiz do projeto, crie `.env` (que **NUNCA** vai para o Git):

```bash
SECRET_KEY=troque-isso-por-uma-secret-key-longa-e-aleatoria
DEBUG=True
ALLOWED_HOSTS=127.0.0.1,localhost
```

Gere uma SECRET_KEY real:

```bash
python -c "from django.core.management.utils import get_random_secret_key; print(get_random_secret_key())"
```

Copie a saída e cole no `.env`.

Agora crie um `.env.example` (que **VAI** para o Git, sem valores reais — serve de template para outros desenvolvedores):

```bash
SECRET_KEY=
DEBUG=False
ALLOWED_HOSTS=

DB_ENGINE=django.db.backends.postgresql
DB_NAME=
DB_USER=
DB_PASSWORD=
DB_HOST=
DB_PORT=5432
DB_SSLMODE=require
```

---

## 4. Criando model, view, URL e template

### 4.1. Model — `tarefas/models.py`

```python
from django.db import models


class Tarefa(models.Model):
    titulo = models.CharField(max_length=200)
    concluida = models.BooleanField(default=False)
    criada_em = models.DateTimeField(auto_now_add=True)

    class Meta:
        ordering = ['-criada_em']

    def __str__(self):
        return self.titulo
```

### 4.2. View — `tarefas/views.py`

```python
from django.http import JsonResponse
from django.shortcuts import render, redirect
from .models import Tarefa


def lista_tarefas(request):
    if request.method == 'POST':
        titulo = request.POST.get('titulo', '').strip()
        if titulo:
            Tarefa.objects.create(titulo=titulo)
        return redirect('lista_tarefas')

    tarefas = Tarefa.objects.all()
    return render(request, 'tarefas/lista.html', {'tarefas': tarefas})


def health(request):
    """Endpoint para verificação de saúde (usado no CI/CD)."""
    return JsonResponse({'status': 'ok'})
```

### 4.3. Template — `tarefas/templates/tarefas/lista.html`

Crie as pastas e o arquivo:

```bash
mkdir -p tarefas/templates/tarefas
```

Conteúdo de `tarefas/templates/tarefas/lista.html`:

```html
<!DOCTYPE html>
<html lang="pt-br">
<head>
    <meta charset="UTF-8">
    <title>Tarefas</title>
    <style>
        body { font-family: system-ui, sans-serif; max-width: 600px; margin: 2rem auto; padding: 1rem; }
        ul { list-style: none; padding: 0; }
        li { padding: .5rem; border-bottom: 1px solid #eee; }
        .ok { text-decoration: line-through; color: #888; }
        form { display: flex; gap: .5rem; margin-bottom: 1rem; }
        input[type=text] { flex: 1; padding: .5rem; }
    </style>
</head>
<body>
    <h1>Minhas tarefas</h1>
    <form method="post">
        {% csrf_token %}
        <input type="text" name="titulo" placeholder="Nova tarefa..." required>
        <button>Adicionar</button>
    </form>
    <ul>
        {% for t in tarefas %}
            <li class="{% if t.concluida %}ok{% endif %}">{{ t.titulo }}</li>
        {% empty %}
            <li>Nenhuma tarefa ainda.</li>
        {% endfor %}
    </ul>
</body>
</html>
```

### 4.4. URLs

Crie `tarefas/urls.py`:

```python
from django.urls import path
from . import views

urlpatterns = [
    path('', views.lista_tarefas, name='lista_tarefas'),
    path('health/', views.health, name='health'),
]
```

Edite `core/urls.py`:

```python
from django.contrib import admin
from django.urls import path, include

urlpatterns = [
    path('admin/', admin.site.urls),
    path('', include('tarefas.urls')),
]
```

---

## 5. Rodando localmente

```bash
python manage.py makemigrations
python manage.py migrate
python manage.py createsuperuser
python manage.py runserver
```

Acesse:

- http://127.0.0.1:8000 — lista de tarefas
- http://127.0.0.1:8000/admin — painel admin
- http://127.0.0.1:8000/health/ — retorna `{"status": "ok"}`

---

## 6. Configurando `.gitignore`

Na raiz, crie `.gitignore`:

```gitignore
# Python
__pycache__/
*.py[cod]
*.egg-info/

# venv
venv/
env/

# Django
*.sqlite3
*.log
local_settings.py
staticfiles/
media/

# Variáveis de ambiente
.env
.env.*
!.env.example

# IDE
.vscode/
.idea/
*.swp
.DS_Store
```

---

## 7. Adicionando um teste básico

Conteúdo de `tarefas/tests.py`:

```python
from django.test import TestCase
from .models import Tarefa


class TarefaTests(TestCase):
    def test_health_endpoint(self):
        response = self.client.get('/health/')
        self.assertEqual(response.status_code, 200)
        self.assertJSONEqual(response.content, {'status': 'ok'})

    def test_criar_tarefa(self):
        Tarefa.objects.create(titulo='Estudar Django')
        self.assertEqual(Tarefa.objects.count(), 1)
        self.assertEqual(str(Tarefa.objects.first()), 'Estudar Django')

    def test_lista_acessivel(self):
        response = self.client.get('/')
        self.assertEqual(response.status_code, 200)
```

Rode:
```bash
python manage.py test
# Esperado: Ran 3 tests in 0.0xxs — OK
```

---

## 8. Enviando para o GitHub

### 8.1. Crie o repositório no GitHub

1. Acesse https://github.com/new
2. **Repository name:** `django-azure-pratica`
3. **Public**
4. **NÃO** marque "Add a README" (vamos enviar o nosso)
5. **Create repository**

### 8.2. Inicialize, comite e envie

```bash
git init
git add .
git commit -m "feat: projeto Django inicial com app tarefas"
git branch -M main
git remote add origin git@github.com:<seu-usuario>/django-azure-pratica.git
git push -u origin main
```

> Confirme no GitHub que o `.env` **não foi enviado** (deve estar bloqueado pelo `.gitignore`).
> Confirme que `.env.example` **foi enviado**.

---

## Checkpoint

Execute na ordem (testa reprodutibilidade do projeto a partir do zero):

```bash
deactivate
rm -rf venv
python -m venv venv         # python3 no macOS/Linux
source venv/bin/activate    # .\venv\Scripts\Activate.ps1 no Windows
pip install -r requirements.txt
python manage.py migrate
python manage.py test
python manage.py runserver
```

Se a aplicação rodar e os 3 testes passarem com o venv recém-criado, o projeto está reprodutível.

---

## Troubleshooting

### `decouple.UndefinedValueError: SECRET_KEY not found`
Você não criou o arquivo `.env` na raiz do projeto. Crie e adicione `SECRET_KEY=...`.

### `ModuleNotFoundError: No module named 'tarefas'`
A app `tarefas` não foi adicionada ao `INSTALLED_APPS` em `settings.py`. Volte ao passo 2.3.

### Push pro GitHub pede usuário e senha
Você não está usando SSH. Confirme com `git remote -v`. Se aparecer `https://`, corrija com:
```bash
git remote set-url origin git@github.com:<usuario>/<repo>.git
```

### `Page not found (404)` na raiz
Você não incluiu `tarefas.urls` no `core/urls.py`. Volte ao passo 4.4.

---

## Desafio extra

1. Adicione um campo `prioridade` (`'baixa'`, `'média'`, `'alta'`) ao model `Tarefa`
2. Crie e aplique a migração
3. Mostre a prioridade na listagem com cores diferentes
4. Adicione um teste para o novo campo
5. Faça commit com mensagem `feat: campo de prioridade nas tarefas`

---

**Próxima aula:** [Aula 3 — Provisionando a VM pelo Portal Azure](../aula3/README.md)
