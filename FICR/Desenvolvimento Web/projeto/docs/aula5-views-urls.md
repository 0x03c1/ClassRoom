#  Aula 5 — Views e URLs

> **Objetivo:** criar views que listam e detalham livros, mapeá-las em URLs limpas e entender como o Django roteia as requisições.

---

## 1. O que é uma View?

Uma **view** é uma função (ou classe) Python que:

1. Recebe um objeto `HttpRequest` como argumento.
2. Faz alguma lógica (consulta banco, processa formulário, etc.).
3. Retorna um objeto `HttpResponse` (geralmente HTML, mas pode ser JSON, PDF, etc.).

```python
def minha_view(request):
    # lógica
    return HttpResponse('algo')
```

Existem dois estilos:

- **Function-Based Views (FBV):** funções tradicionais. Mais explícitas, ótimas para iniciantes.
- **Class-Based Views (CBV):** classes que herdam de `View` e oferecem reuso. Veremos as duas.

>  Nesta aula, focaremos em **FBV**. CBVs entrarão na aula de CRUD.

---

## 2. View de listagem de livros

Edite `livros/views.py`:

```python
from django.shortcuts import render, get_object_or_404
from .models import Livro


def home(request):
    """Página inicial — exibe os 6 livros mais recentes."""
    livros = Livro.objects.filter(disponivel=True).order_by('-criado_em')[:6]
    contexto = {'livros': livros}
    return render(request, 'livros/home.html', contexto)


def lista_livros(request):
    """Lista todos os livros disponíveis com busca."""
    busca = request.GET.get('q', '')
    livros = Livro.objects.filter(disponivel=True)

    if busca:
        livros = livros.filter(titulo__icontains=busca)

    contexto = {
        'livros': livros,
        'busca': busca,
    }
    return render(request, 'livros/lista.html', contexto)


def detalhe_livro(request, pk):
    """Exibe os detalhes de um livro específico."""
    livro = get_object_or_404(Livro, pk=pk, disponivel=True)
    contexto = {'livro': livro}
    return render(request, 'livros/detalhe.html', contexto)
```

### 2.1. Funções importantes

| Função | Para que serve |
|--------|----------------|
| `render(request, template, contexto)` | Renderiza um template HTML com dados. |
| `get_object_or_404(Model, **filtros)` | Busca um objeto; se não encontrar, retorna 404 automaticamente. |
| `redirect('nome_url')` | Redireciona para outra URL. |

### 2.2. O dicionário `contexto`

É o "ponto de troca" entre a view e o template: tudo que estiver nele estará disponível no HTML.

---

## 3. Mapeando URLs

Edite `livros/urls.py`:

```python
from django.urls import path
from . import views

app_name = 'livros'

urlpatterns = [
    path('', views.home, name='home'),
    path('livros/', views.lista_livros, name='lista'),
    path('livros/<int:pk>/', views.detalhe_livro, name='detalhe'),
]
```

### 3.1. Conversores de path

| Conversor | Aceita | Exemplo |
|-----------|--------|---------|
| `<int:pk>` | inteiros | `/livros/42/` |
| `<str:nome>` | strings (sem `/`) | `/categoria/ficcao/` |
| `<slug:slug>` | slugs (a-z, 0-9, `-`, `_`) | `/livro/dom-casmurro/` |
| `<uuid:id>` | UUIDs | `/usuario/abc-123-.../` |
| `<path:caminho>` | strings (com `/`) | `/arquivos/docs/manual.pdf` |

---

## 4. Templates: estrutura mínima

Vamos criar a pasta de templates. Por convenção do Django:

```
livros/
└── templates/
    └── livros/         ← subpasta com o nome do app (boa prática)
        ├── home.html
        ├── lista.html
        └── detalhe.html
```

>  **Por que essa subpasta repetida?** Porque o Django junta todos os `templates/` de todos os apps em um único namespace. Se dois apps tivessem `home.html`, haveria conflito.

### 4.1. Template provisório `home.html`

Crie `livros/templates/livros/home.html`:

```html
<!DOCTYPE html>
<html lang="pt-br">
<head>
    <meta charset="UTF-8">
    <title>Livraria Django</title>
</head>
<body>
    <h1> Livraria Django</h1>
    <h2>Lançamentos</h2>

    {% if livros %}
        <ul>
            {% for livro in livros %}
                <li>
                    <a href="{% url 'livros:detalhe' livro.pk %}">
                        {{ livro.titulo }}
                    </a>
                    — R$ {{ livro.preco }}
                </li>
            {% endfor %}
        </ul>
    {% else %}
        <p>Nenhum livro cadastrado ainda.</p>
    {% endif %}

    <p><a href="{% url 'livros:lista' %}">Ver todos os livros →</a></p>
</body>
</html>
```

### 4.2. Template provisório `lista.html`

```html
<!DOCTYPE html>
<html lang="pt-br">
<head>
    <meta charset="UTF-8">
    <title>Livros — Livraria Django</title>
</head>
<body>
    <h1> Catálogo</h1>

    <form method="get">
        <input type="text" name="q" placeholder="Buscar..." value="{{ busca }}">
        <button type="submit">Buscar</button>
    </form>

    {% if livros %}
        <p>Total: {{ livros|length }} livro(s)</p>
        <ul>
            {% for livro in livros %}
                <li>
                    <a href="{% url 'livros:detalhe' livro.pk %}">{{ livro.titulo }}</a>
                    — {{ livro.categoria.nome }}
                    — R$ {{ livro.preco }}
                </li>
            {% endfor %}
        </ul>
    {% else %}
        <p>Nenhum livro encontrado.</p>
    {% endif %}

    <p><a href="{% url 'livros:home' %}">← Voltar</a></p>
</body>
</html>
```

### 4.3. Template provisório `detalhe.html`

```html
<!DOCTYPE html>
<html lang="pt-br">
<head>
    <meta charset="UTF-8">
    <title>{{ livro.titulo }}</title>
</head>
<body>
    <h1>{{ livro.titulo }}</h1>

    {% if livro.capa %}
        <img src="{{ livro.capa.url }}" alt="{{ livro.titulo }}" width="200">
    {% endif %}

    <p><strong>ISBN:</strong> {{ livro.isbn }}</p>
    <p><strong>Editora:</strong> {{ livro.editora.nome }}</p>
    <p><strong>Categoria:</strong> {{ livro.categoria.nome }}</p>
    <p><strong>Ano:</strong> {{ livro.ano_publicacao }}</p>
    <p><strong>Páginas:</strong> {{ livro.paginas }}</p>
    <p><strong>Preço:</strong> R$ {{ livro.preco }}</p>

    <h3>Autores</h3>
    <ul>
        {% for autor in livro.autores.all %}
            <li>{{ autor.nome }}</li>
        {% empty %}
            <li>Sem autores cadastrados.</li>
        {% endfor %}
    </ul>

    <h3>Sinopse</h3>
    <p>{{ livro.sinopse|default:"(Sem sinopse cadastrada.)" }}</p>

    <p><a href="{% url 'livros:lista' %}">← Voltar à lista</a></p>
</body>
</html>
```

---

## 5. Testando

```bash
python manage.py runserver
```

- **http://127.0.0.1:8000/** → home com 6 livros recentes.
- **http://127.0.0.1:8000/livros/** → lista completa.
- **http://127.0.0.1:8000/livros/?q=python** → lista filtrada.
- **http://127.0.0.1:8000/livros/1/** → detalhe do livro 1.

---

## 6. Tag `{% url %}`: nunca escreva URLs hardcoded!

 **Errado:**
```html
<a href="/livros/{{ livro.pk }}/">{{ livro.titulo }}</a>
```

 **Certo:**
```html
<a href="{% url 'livros:detalhe' livro.pk %}">{{ livro.titulo }}</a>
```

>  **Por quê?** Se você decidir mudar a URL de `/livros/<id>/` para `/catalogo/<id>/`, basta alterar uma linha em `urls.py`. Todas as referências nos templates continuam funcionando.

---

## 7. Paginação

Quando o catálogo cresce, é bom paginar. Atualize a view `lista_livros`:

```python
from django.core.paginator import Paginator

def lista_livros(request):
    busca = request.GET.get('q', '')
    livros = Livro.objects.filter(disponivel=True)

    if busca:
        livros = livros.filter(titulo__icontains=busca)

    paginator = Paginator(livros, 6)  # 6 livros por página
    page_number = request.GET.get('page')
    page_obj = paginator.get_page(page_number)

    contexto = {
        'page_obj': page_obj,
        'busca': busca,
    }
    return render(request, 'livros/lista.html', contexto)
```

E no template `lista.html`, troque o loop e adicione navegação:

```html
{% for livro in page_obj %}
    <li>
        <a href="{% url 'livros:detalhe' livro.pk %}">{{ livro.titulo }}</a>
    </li>
{% endfor %}

{% if page_obj.has_other_pages %}
    <nav>
        {% if page_obj.has_previous %}
            <a href="?page={{ page_obj.previous_page_number }}{% if busca %}&q={{ busca }}{% endif %}">← Anterior</a>
        {% endif %}

        Página {{ page_obj.number }} de {{ page_obj.paginator.num_pages }}

        {% if page_obj.has_next %}
            <a href="?page={{ page_obj.next_page_number }}{% if busca %}&q={{ busca }}{% endif %}">Próxima →</a>
        {% endif %}
    </nav>
{% endif %}
```

---

## 8. Commit

```bash
git add .
git commit -m "feat: cria views home, lista e detalhe com paginação e busca"
git push
```

---

##  Exercícios

1. Crie uma view `lista_por_categoria(request, pk)` em `/categoria/<int:pk>/` que liste os livros de uma categoria específica.

2. Crie uma view `detalhe_autor(request, pk)` em `/autor/<int:pk>/` exibindo nome, biografia e os livros do autor.

3. Adicione um filtro por **faixa de preço** na view `lista_livros`. O usuário poderá enviar `?preco_min=10&preco_max=50` na URL.

4. **Desafio:** Adicione na view `home` um bloco com os "5 livros mais caros" e os "5 livros mais baratos". Exiba em duas colunas no template.

---

##  Erros comuns

| Erro | Causa | Solução |
|------|-------|---------|
| `TemplateDoesNotExist` | Template não encontrado | Verifique o caminho. Deve ser `livros/templates/livros/arquivo.html`. |
| `NoReverseMatch` | URL com `name` errado | Confira se o `name=` no `urls.py` bate com o `{% url %}` no template. |
| `'livros' is not a registered namespace` | Esqueceu o `app_name` | Adicione `app_name = 'livros'` no `urls.py` do app. |

---

##  Checklist

- [ ] 3 views criadas: `home`, `lista_livros`, `detalhe_livro`.
- [ ] URLs nomeadas com namespace.
- [ ] Templates HTML em `livros/templates/livros/`.
- [ ] Tag `{% url %}` usada em todos os links.
- [ ] Busca funcionando.
- [ ] Paginação funcionando.
- [ ] Commit realizado.

---

- **Próxima aula:** [Aula 6 — Templates e Bootstrap](./aula6-templates.md)
- **Aula anterior:** [Aula 4 — Django Admin](./aula4-admin.md)
