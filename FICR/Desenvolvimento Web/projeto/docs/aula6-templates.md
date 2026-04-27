#  Aula 6 — Templates e Bootstrap

> **Objetivo:** dominar o sistema de templates do Django, aplicar herança de templates e estilizar a livraria com Bootstrap 5.

---

## 1. Sistema de templates do Django

O Django Template Language (DTL) é uma linguagem **simples e segura** voltada para designers — não para programadores. Ela é intencionalmente limitada.

### 1.1. Os 4 elementos do DTL

| Elemento | Sintaxe | Função |
|----------|---------|--------|
| **Variáveis** | `{{ variavel }}` | Imprime o valor. |
| **Tags** | `{% tag %}` | Lógica (loops, ifs, urls, etc.). |
| **Filtros** | `{{ var\|filtro }}` | Transforma o valor. |
| **Comentários** | `{# comentário #}` | Não é renderizado. |

---

## 2. Filtros mais úteis

```django
{{ nome|upper }}                {# JOSÉ #}
{{ nome|lower }}                {# josé #}
{{ nome|title }}                {# José Da Silva #}
{{ nome|length }}               {# 4 #}
{{ texto|truncatechars:50 }}    {# corta após 50 caracteres #}
{{ preco|floatformat:2 }}       {# 49.90 #}
{{ data|date:"d/m/Y" }}         {# 27/04/2026 #}
{{ data|date:"d/m/Y H:i" }}     {# 27/04/2026 14:30 #}
{{ valor|default:"Sem valor" }} {# fallback se vazio/None #}
{{ texto|linebreaks }}          {# converte \n em <p> e <br> #}
{{ html|safe }}                 {# desativa o escape de HTML (cuidado!) #}
{{ lista|join:", " }}           {# une elementos por vírgula #}
```

---

## 3. Tags mais úteis

### 3.1. Condicionais

```django
{% if livro.estoque > 0 %}
    Em estoque
{% elif livro.disponivel %}
    Aguardando reposição
{% else %}
    Indisponível
{% endif %}
```

### 3.2. Loops

```django
{% for livro in livros %}
    {{ forloop.counter }}. {{ livro.titulo }}
{% empty %}
    Nenhum livro.
{% endfor %}
```

>  Dentro de um for, você tem acesso a:
> - `forloop.counter` (contador a partir de 1)
> - `forloop.counter0` (a partir de 0)
> - `forloop.first` / `forloop.last` (booleano)
> - `forloop.revcounter`

### 3.3. URL

```django
<a href="{% url 'livros:detalhe' livro.pk %}">Ver detalhes</a>
```

### 3.4. Static (arquivos estáticos)

```django
{% load static %}
<link rel="stylesheet" href="{% static 'css/styles.css' %}">
<img src="{% static 'img/logo.png' %}">
```

---

## 4. Herança de templates: o template base

Em vez de repetir o `<head>`, navbar e footer em cada página, criamos **um template base** e os outros **estendem** dele.

### 4.1. Configurando arquivos estáticos

Em `setup/settings.py`, adicione/ajuste:

```python
STATIC_URL = '/static/'
STATICFILES_DIRS = [BASE_DIR / 'static']     # arquivos do projeto
STATIC_ROOT = BASE_DIR / 'staticfiles'        # usado em produção
```

Crie a pasta `static/` na raiz do projeto:

```
livraria/
├── static/
│   ├── css/
│   │   └── styles.css
│   └── img/
│       └── logo.png
```

### 4.2. Criando o template base

Crie `livros/templates/livros/base.html`:

```html
{% load static %}
<!DOCTYPE html>
<html lang="pt-br">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>{% block title %}Livraria Django{% endblock %}</title>

    <!-- Bootstrap 5 via CDN -->
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/css/bootstrap.min.css" rel="stylesheet">
    <link href="https://cdn.jsdelivr.net/npm/bootstrap-icons@1.11.3/font/bootstrap-icons.min.css" rel="stylesheet">

    <!-- CSS customizado -->
    <link rel="stylesheet" href="{% static 'css/styles.css' %}">

    {% block extra_css %}{% endblock %}
</head>
<body class="d-flex flex-column min-vh-100">

    <!-- Navbar -->
    <nav class="navbar navbar-expand-lg navbar-dark bg-primary">
        <div class="container">
            <a class="navbar-brand fw-bold" href="{% url 'livros:home' %}">
                 Livraria Django
            </a>
            <button class="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#nav">
                <span class="navbar-toggler-icon"></span>
            </button>
            <div class="collapse navbar-collapse" id="nav">
                <ul class="navbar-nav me-auto">
                    <li class="nav-item">
                        <a class="nav-link" href="{% url 'livros:home' %}">Início</a>
                    </li>
                    <li class="nav-item">
                        <a class="nav-link" href="{% url 'livros:lista' %}">Catálogo</a>
                    </li>
                </ul>
                <ul class="navbar-nav">
                    {% if user.is_authenticated %}
                        <li class="nav-item">
                            <span class="nav-link">Olá, {{ user.username }}</span>
                        </li>
                        <li class="nav-item">
                            <a class="nav-link" href="/admin/">Admin</a>
                        </li>
                    {% endif %}
                </ul>
            </div>
        </div>
    </nav>

    <!-- Mensagens flash -->
    {% if messages %}
        <div class="container mt-3">
            {% for msg in messages %}
                <div class="alert alert-{{ msg.tags|default:'info' }} alert-dismissible fade show">
                    {{ msg }}
                    <button type="button" class="btn-close" data-bs-dismiss="alert"></button>
                </div>
            {% endfor %}
        </div>
    {% endif %}

    <!-- Conteúdo principal -->
    <main class="container my-4 flex-grow-1">
        {% block content %}{% endblock %}
    </main>

    <!-- Rodapé -->
    <footer class="bg-dark text-white py-4 mt-auto">
        <div class="container text-center">
            <p class="mb-0">
                © {% now "Y" %} Livraria Django · Programação para Web
            </p>
        </div>
    </footer>

    <!-- Bootstrap JS -->
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/js/bootstrap.bundle.min.js"></script>
    {% block extra_js %}{% endblock %}
</body>
</html>
```

### 4.3. Entendendo os blocos

| Bloco | Função |
|-------|--------|
| `{% block title %}` | Título da aba do navegador. |
| `{% block content %}` | Conteúdo principal de cada página. |
| `{% block extra_css %}` | CSS adicional por página. |
| `{% block extra_js %}` | JS adicional por página. |

Cada filho pode **sobrescrever** os blocos do pai.

---

## 5. Refatorando os templates

### 5.1. `home.html`

```html
{% extends 'livros/base.html' %}

{% block title %}Início — Livraria Django{% endblock %}

{% block content %}
    <div class="bg-light p-5 rounded mb-4">
        <h1 class="display-5 fw-bold"> Bem-vindo à Livraria Django</h1>
        <p class="lead">Os melhores livros, das mais variadas categorias.</p>
        <a href="{% url 'livros:lista' %}" class="btn btn-primary btn-lg">
            Ver catálogo completo
        </a>
    </div>

    <h2 class="mb-4">Lançamentos</h2>

    <div class="row g-4">
        {% for livro in livros %}
            <div class="col-12 col-sm-6 col-md-4 col-lg-3">
                <div class="card h-100 shadow-sm">
                    {% if livro.capa %}
                        <img src="{{ livro.capa.url }}"
                             class="card-img-top"
                             alt="{{ livro.titulo }}"
                             style="height: 250px; object-fit: cover;">
                    {% else %}
                        <div class="bg-secondary text-white d-flex align-items-center justify-content-center"
                             style="height: 250px;">
                            <i class="bi bi-book" style="font-size: 4rem;"></i>
                        </div>
                    {% endif %}
                    <div class="card-body d-flex flex-column">
                        <h5 class="card-title">{{ livro.titulo|truncatechars:40 }}</h5>
                        <p class="card-text text-muted small">
                            {{ livro.editora.nome }} · {{ livro.ano_publicacao }}
                        </p>
                        <p class="card-text fw-bold text-primary fs-5">
                            R$ {{ livro.preco|floatformat:2 }}
                        </p>
                        <a href="{% url 'livros:detalhe' livro.pk %}"
                           class="btn btn-outline-primary mt-auto">
                            Ver detalhes
                        </a>
                    </div>
                </div>
            </div>
        {% empty %}
            <div class="col-12">
                <div class="alert alert-info">Nenhum livro cadastrado ainda.</div>
            </div>
        {% endfor %}
    </div>
{% endblock %}
```

### 5.2. `lista.html`

```html
{% extends 'livros/base.html' %}

{% block title %}Catálogo — Livraria Django{% endblock %}

{% block content %}
    <h1 class="mb-4">Catálogo</h1>

    <form method="get" class="mb-4">
        <div class="input-group">
            <input type="text"
                   name="q"
                   class="form-control"
                   placeholder="Buscar por título..."
                   value="{{ busca }}">
            <button type="submit" class="btn btn-primary">
                <i class="bi bi-search"></i> Buscar
            </button>
        </div>
    </form>

    {% if page_obj %}
        <div class="row g-4">
            {% for livro in page_obj %}
                <div class="col-12 col-sm-6 col-md-4">
                    <div class="card h-100 shadow-sm">
                        <div class="card-body">
                            <h5 class="card-title">{{ livro.titulo }}</h5>
                            <p class="text-muted small mb-2">
                                <span class="badge bg-secondary">{{ livro.categoria.nome }}</span>
                            </p>
                            <p class="card-text">
                                {{ livro.sinopse|truncatewords:20|default:"Sem sinopse." }}
                            </p>
                            <p class="fw-bold text-primary">
                                R$ {{ livro.preco|floatformat:2 }}
                            </p>
                            <a href="{% url 'livros:detalhe' livro.pk %}" class="btn btn-sm btn-primary">
                                Ver detalhes
                            </a>
                        </div>
                    </div>
                </div>
            {% endfor %}
        </div>

        <!-- Paginação -->
        {% if page_obj.has_other_pages %}
            <nav class="mt-4">
                <ul class="pagination justify-content-center">
                    {% if page_obj.has_previous %}
                        <li class="page-item">
                            <a class="page-link"
                               href="?page={{ page_obj.previous_page_number }}{% if busca %}&q={{ busca }}{% endif %}">
                                ← Anterior
                            </a>
                        </li>
                    {% endif %}

                    <li class="page-item active">
                        <span class="page-link">
                            Página {{ page_obj.number }} de {{ page_obj.paginator.num_pages }}
                        </span>
                    </li>

                    {% if page_obj.has_next %}
                        <li class="page-item">
                            <a class="page-link"
                               href="?page={{ page_obj.next_page_number }}{% if busca %}&q={{ busca }}{% endif %}">
                                Próxima →
                            </a>
                        </li>
                    {% endif %}
                </ul>
            </nav>
        {% endif %}
    {% else %}
        <div class="alert alert-warning">
            Nenhum livro encontrado{% if busca %} para "{{ busca }}"{% endif %}.
        </div>
    {% endif %}
{% endblock %}
```

### 5.3. `detalhe.html`

```html
{% extends 'livros/base.html' %}

{% block title %}{{ livro.titulo }} — Livraria Django{% endblock %}

{% block content %}
    <nav aria-label="breadcrumb">
        <ol class="breadcrumb">
            <li class="breadcrumb-item"><a href="{% url 'livros:home' %}">Início</a></li>
            <li class="breadcrumb-item"><a href="{% url 'livros:lista' %}">Catálogo</a></li>
            <li class="breadcrumb-item active">{{ livro.titulo|truncatechars:30 }}</li>
        </ol>
    </nav>

    <div class="row">
        <div class="col-md-4 mb-4">
            {% if livro.capa %}
                <img src="{{ livro.capa.url }}" class="img-fluid rounded shadow" alt="{{ livro.titulo }}">
            {% else %}
                <div class="bg-secondary text-white d-flex align-items-center justify-content-center rounded"
                     style="height: 400px;">
                    <i class="bi bi-book" style="font-size: 6rem;"></i>
                </div>
            {% endif %}
        </div>

        <div class="col-md-8">
            <h1>{{ livro.titulo }}</h1>
            <p class="text-muted">
                <span class="badge bg-secondary">{{ livro.categoria.nome }}</span>
                <span class="badge bg-info">{{ livro.editora.nome }}</span>
            </p>

            <h3 class="text-primary fw-bold mb-3">R$ {{ livro.preco|floatformat:2 }}</h3>

            <table class="table table-sm">
                <tr><th>ISBN</th><td>{{ livro.isbn }}</td></tr>
                <tr><th>Páginas</th><td>{{ livro.paginas }}</td></tr>
                <tr><th>Ano</th><td>{{ livro.ano_publicacao }}</td></tr>
                <tr><th>Estoque</th><td>{{ livro.estoque }} unidade(s)</td></tr>
                <tr>
                    <th>Autores</th>
                    <td>
                        {% for autor in livro.autores.all %}
                            {{ autor.nome }}{% if not forloop.last %}, {% endif %}
                        {% empty %}
                            <span class="text-muted">Sem autores cadastrados.</span>
                        {% endfor %}
                    </td>
                </tr>
            </table>

            <h4>Sinopse</h4>
            <p>{{ livro.sinopse|linebreaks|default:"Sem sinopse cadastrada." }}</p>

            <a href="{% url 'livros:lista' %}" class="btn btn-outline-secondary">
                ← Voltar à lista
            </a>
        </div>
    </div>
{% endblock %}
```

---

## 6. CSS customizado

Crie `static/css/styles.css`:

```css
/* Estilos personalizados da Livraria */

body {
    font-family: 'Segoe UI', Tahoma, sans-serif;
    background-color: #f8f9fa;
}

.card {
    transition: transform 0.2s, box-shadow 0.2s;
}

.card:hover {
    transform: translateY(-4px);
    box-shadow: 0 4px 12px rgba(0, 0, 0, 0.15) !important;
}

.navbar-brand {
    font-size: 1.5rem;
}

footer {
    border-top: 4px solid #0d6efd;
}
```

---

## 7. Commit

```bash
git add .
git commit -m "feat: aplica Bootstrap 5 e implementa herança de templates"
git push
```

---

##  Exercícios

1. Crie um **filtro customizado** chamado `formata_brl` que formata um número como moeda brasileira: `R$ 1.234,56`. Crie em `livros/templatetags/livros_extras.py`.

2. Adicione no template `base.html` um campo de busca **sempre visível** na navbar (em vez de só na página da lista).

3. **Desafio:** Crie uma sidebar (à esquerda) na página `lista.html` mostrando as categorias com a contagem de livros em cada uma. Use um **template tag inclusion** para reaproveitamento.

---

##  Erros comuns

| Erro | Causa | Solução |
|------|-------|---------|
| Estilos não aplicam | Esqueceu de `{% load static %}` | Adicione no topo do template. |
| `Invalid filter: 'floatformat'` | Esqueceu de carregar uma tag | Geralmente filtros nativos não precisam. Se for custom, use `{% load livros_extras %}`. |
| Imagem não aparece | `MEDIA_URL` não servido | Confira o `urls.py` (visto na Aula 3). |

---

##  Checklist

- [ ] Template `base.html` com herança configurada.
- [ ] Bootstrap 5 carregado.
- [ ] `home.html`, `lista.html` e `detalhe.html` estendem o `base.html`.
- [ ] Arquivo `static/css/styles.css` aplicando estilos custom.
- [ ] Site responsivo (testar no celular ou redimensionar a janela).
- [ ] Commit realizado.

---

- **Próxima aula:** [Aula 7 — Forms e Validação](./aula7-forms.md)
- **Aula anterior:** [Aula 5 — Views e URLs](./aula5-views-urls.md)
