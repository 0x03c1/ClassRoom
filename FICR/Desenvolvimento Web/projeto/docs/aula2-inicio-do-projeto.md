#  Aula 2 — Início do Projeto

> **Objetivo:** criar a estrutura de um projeto Django, entender sua organização interna, criar o primeiro app e exibir uma página de "Olá, mundo".

---

## 1. Recapitulando

Antes de iniciar esta aula, certifique-se de que:

-  O ambiente virtual `venv` está **ativado** (você deve ver `(venv)` no terminal).
-  O Django está instalado (`python -m django --version`).
-  Você está dentro da pasta `livraria/`.

---

## 2. Arquitetura MTV do Django

Diferente do tradicional **MVC** (Model–View–Controller), o Django utiliza o padrão **MTV**:

| Sigla | Componente | Responsabilidade |
|-------|------------|-----------------|
| **M** | Model | Representa os dados (tabelas do banco). |
| **T** | Template | Camada de apresentação (HTML). |
| **V** | View | Lógica de negócio (recebe requisição → retorna resposta). |

>  O "Controller" do MVC, no Django, é o próprio framework: ele que faz o roteamento entre URLs e Views.

```
Requisição (URL) ──► urls.py ──► views.py ──► models.py (banco)
                                     │
                                     ▼
                                 templates/ ──► HTML renderizado ──► Resposta
```

---

## 3. Criando o projeto Django

Com o venv ativo, execute na pasta `livraria/`:

```bash
django-admin startproject setup .
```

>  O **ponto final** (`.`) no comando é importante! Ele evita criar uma pasta extra desnecessária.

A estrutura criada será:

```
livraria/
├── venv/
├── setup/
│   ├── __init__.py
│   ├── asgi.py
│   ├── settings.py
│   ├── urls.py
│   └── wsgi.py
├── manage.py
├── .gitignore
└── requirements.txt
```

### 3.1. Entendendo cada arquivo

| Arquivo | Função |
|---------|--------|
| `manage.py` | Utilitário de linha de comando do Django (rodar servidor, migrações, etc.). |
| `setup/settings.py` | **Configurações do projeto** (banco, apps instalados, idioma, etc.). |
| `setup/urls.py` | Arquivo principal de rotas. |
| `setup/wsgi.py` | Interface para servidores web em produção (síncrono). |
| `setup/asgi.py` | Interface para servidores web em produção (assíncrono). |

>  O nome `setup` é uma convenção pessoal. Muitos projetos usam `core` ou o próprio nome do projeto. **Evite** usar nomes genéricos como `myproject`.

---

## 4. Rodando o servidor de desenvolvimento

```bash
python manage.py runserver
```

A saída deve ser similar a:

```
Watching for file changes with StatReloader
Performing system checks...

System check identified no issues (0 silenced).

You have 18 unapplied migration(s)...
Django version 5.1.4, using settings 'setup.settings'
Starting development server at http://127.0.0.1:8000/
Quit the server with CTRL-BREAK.
```

Abra no navegador: **http://127.0.0.1:8000/**

Você verá a página padrão do Django:  **"The install worked successfully!"**

>  Para **parar** o servidor, pressione `Ctrl + C` no terminal.

---

## 5. Aplicando as migrações iniciais

O Django já vem com apps internos (autenticação, sessões, etc.) que precisam de tabelas no banco. Aplique as migrações iniciais:

```bash
python manage.py migrate
```

Será criado o arquivo `db.sqlite3` na raiz do projeto. Esse é o banco de dados local (SQLite) usado em desenvolvimento.

---

## 6. Criando o primeiro app: `livros`

Em Django, um **projeto** é composto por vários **apps** (módulos). Cada app deve ser responsável por uma funcionalidade específica.

```bash
python manage.py startapp livros
```

A estrutura agora é:

```
livraria/
├── livros/                ← novo app
│   ├── migrations/
│   ├── __init__.py
│   ├── admin.py
│   ├── apps.py
│   ├── models.py
│   ├── tests.py
│   └── views.py
├── setup/
├── manage.py
└── ...
```

### 6.1. Registrando o app em `settings.py`

Abra `setup/settings.py` e localize a lista `INSTALLED_APPS`. Adicione `'livros'` no final:

```python
INSTALLED_APPS = [
    'django.contrib.admin',
    'django.contrib.auth',
    'django.contrib.contenttypes',
    'django.contrib.sessions',
    'django.contrib.messages',
    'django.contrib.staticfiles',

    # Apps locais
    'livros',
]
```

>  **Esquecer este passo é o erro mais comum em iniciantes.** Se o app não estiver registrado, o Django o ignora.

---

## 7. Configurando idioma e fuso horário

Ainda em `settings.py`, ajuste:

```python
LANGUAGE_CODE = 'pt-br'

TIME_ZONE = 'America/Recife'

USE_I18N = True

USE_TZ = True
```

>  Use `'America/Sao_Paulo'` se preferir o fuso horário de Brasília. O Pernambuco segue o mesmo fuso (UTC-3).

---

## 8. Criando a primeira view

Abra `livros/views.py` e digite:

```python
from django.http import HttpResponse


def home(request):
    return HttpResponse("<h1>Olá, mundo! </h1><p>Bem-vindo à Livraria Django.</p>")
```

### 8.1. Criando o arquivo de URLs do app

Crie um novo arquivo em `livros/urls.py` (ele **não existe** por padrão):

```python
from django.urls import path
from . import views

app_name = 'livros'

urlpatterns = [
    path('', views.home, name='home'),
]
```

>  O `app_name` permite usar **namespaces** nas URLs (ex.: `{% url 'livros:home' %}` nos templates).

### 8.2. Incluindo as URLs do app no projeto

Abra `setup/urls.py` e altere para:

```python
from django.contrib import admin
from django.urls import path, include

urlpatterns = [
    path('admin/', admin.site.urls),
    path('', include('livros.urls')),
]
```

---

## 9. Testando

Rode o servidor novamente:

```bash
python manage.py runserver
```

Acesse **http://127.0.0.1:8000/** e veja sua página personalizada! 

---

## 10. Estrutura final desta aula

```
livraria/
├── venv/
├── livros/
│   ├── migrations/
│   ├── __init__.py
│   ├── admin.py
│   ├── apps.py
│   ├── models.py
│   ├── tests.py
│   ├── urls.py            ← criado por nós
│   └── views.py           ← editado
├── setup/
│   ├── __init__.py
│   ├── asgi.py
│   ├── settings.py        ← editado
│   ├── urls.py            ← editado
│   └── wsgi.py
├── db.sqlite3
├── manage.py
├── .gitignore
└── requirements.txt
```

---

## 11. Commit

```bash
git add .
git commit -m "feat: cria projeto Django e app 'livros' com view inicial"
git push
```

---

##  Exercícios

1. Crie uma segunda view chamada `sobre(request)` que retorne um `HttpResponse` com uma breve descrição da livraria. Mapeie-a para a URL `/sobre/`.
2. Crie uma view chamada `contato(request)` acessível em `/contato/` exibindo um e-mail e telefone fictícios.
3. **Desafio:** Pesquise o que é o parâmetro `name=` no `path(...)` e descreva, em até 3 linhas, sua utilidade.

---

##  Erros comuns

| Erro | Causa | Solução |
|------|-------|---------|
| `ModuleNotFoundError: No module named 'django'` | venv não ativado | Ative o venv antes de rodar comandos. |
| `Page not found (404)` | URL não cadastrada | Verifique `urls.py` do app e do projeto. |
| `App 'livros' isn't installed` | Esqueceu de adicionar em `INSTALLED_APPS` | Adicione em `settings.py`. |

---

##  Checklist

- [ ] Projeto `setup` criado.
- [ ] Servidor de desenvolvimento rodando em `127.0.0.1:8000`.
- [ ] App `livros` criado e registrado em `INSTALLED_APPS`.
- [ ] Idioma e fuso horário configurados.
- [ ] View `home` exibindo página personalizada.
- [ ] Commit realizado.

---

- **Próxima aula:** [Aula 3 — Models](./aula3-models.md)
- **Aula anterior:** [Aula 1 — Configuração do Ambiente](./aula1-configuracao-ambiente.md)
