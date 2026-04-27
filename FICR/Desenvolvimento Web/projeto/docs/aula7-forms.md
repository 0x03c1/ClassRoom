#  Aula 7 — Forms e Validação

> **Objetivo:** criar formulários para cadastrar e editar livros utilizando `ModelForm`, aplicar validações customizadas e exibir erros amigáveis ao usuário.

---

## 1. Por que usar Forms do Django?

Você poderia escrever HTML manualmente e processar `request.POST` na view... mas perderia tudo isso:

 Geração automática de HTML.
 Validação automática de tipos.
 Mensagens de erro consistentes.
 Proteção contra **CSRF** (Cross-Site Request Forgery).
 Conversão automática para Python (`int`, `Decimal`, `date`, etc.).
 Suporte a uploads de arquivos.
 Reuso entre criar/editar.

---

## 2. `Form` vs `ModelForm`

| `Form` | `ModelForm` |
|--------|-------------|
| Campos definidos manualmente. | Campos derivados de um model. |
| Você processa `cleaned_data` "na mão". | Tem `.save()` que insere/atualiza no banco. |
| Bom para formulários sem persistência (busca, filtros, login). | Bom para CRUD. |

Como temos models, vamos usar **ModelForm**.

---

## 3. Criando o forms.py

Crie o arquivo `livros/forms.py`:

```python
from django import forms
from django.core.exceptions import ValidationError
from datetime import datetime
from .models import Autor, Categoria, Editora, Livro


class LivroForm(forms.ModelForm):
    class Meta:
        model = Livro
        fields = [
            'titulo',
            'isbn',
            'paginas',
            'preco',
            'ano_publicacao',
            'sinopse',
            'capa',
            'estoque',
            'disponivel',
            'categoria',
            'editora',
            'autores',
        ]
        widgets = {
            'titulo': forms.TextInput(attrs={'class': 'form-control', 'placeholder': 'Ex.: Dom Casmurro'}),
            'isbn': forms.TextInput(attrs={'class': 'form-control', 'placeholder': '978-XX-XXXXX-XX-X'}),
            'paginas': forms.NumberInput(attrs={'class': 'form-control', 'min': 1}),
            'preco': forms.NumberInput(attrs={'class': 'form-control', 'step': '0.01', 'min': 0}),
            'ano_publicacao': forms.NumberInput(attrs={'class': 'form-control'}),
            'sinopse': forms.Textarea(attrs={'class': 'form-control', 'rows': 4}),
            'capa': forms.FileInput(attrs={'class': 'form-control'}),
            'estoque': forms.NumberInput(attrs={'class': 'form-control', 'min': 0}),
            'disponivel': forms.CheckboxInput(attrs={'class': 'form-check-input'}),
            'categoria': forms.Select(attrs={'class': 'form-select'}),
            'editora': forms.Select(attrs={'class': 'form-select'}),
            'autores': forms.SelectMultiple(attrs={'class': 'form-select', 'size': 5}),
        }
        labels = {
            'titulo': 'Título do livro',
            'isbn': 'ISBN',
            'preco': 'Preço (R$)',
            'ano_publicacao': 'Ano de publicação',
            'capa': 'Capa do livro',
        }
        help_texts = {
            'isbn': 'Informe o ISBN com ou sem hífens.',
            'autores': 'Segure Ctrl (ou Cmd) para selecionar múltiplos autores.',
        }

    # ----- Validações customizadas -----

    def clean_isbn(self):
        """Remove hífens e valida tamanho do ISBN."""
        isbn = self.cleaned_data['isbn'].replace('-', '').replace(' ', '')
        if len(isbn) not in (10, 13):
            raise ValidationError('ISBN deve ter 10 ou 13 dígitos.')
        if not isbn.isdigit():
            raise ValidationError('ISBN deve conter apenas números (e hífens).')
        return isbn

    def clean_ano_publicacao(self):
        ano = self.cleaned_data['ano_publicacao']
        ano_atual = datetime.now().year
        if ano < 1450:
            raise ValidationError('Ano inválido (antes da invenção da imprensa).')
        if ano > ano_atual + 1:
            raise ValidationError(f'Ano não pode ser maior que {ano_atual + 1}.')
        return ano

    def clean_preco(self):
        preco = self.cleaned_data['preco']
        if preco <= 0:
            raise ValidationError('O preço deve ser maior que zero.')
        return preco

    def clean(self):
        """Validação envolvendo múltiplos campos."""
        cleaned_data = super().clean()
        estoque = cleaned_data.get('estoque', 0)
        disponivel = cleaned_data.get('disponivel', False)

        if disponivel and estoque == 0:
            self.add_error(
                'estoque',
                'Para marcar como disponível, o estoque deve ser maior que zero.'
            )
        return cleaned_data
```

---

## 4. Entendendo a estrutura do ModelForm

### 4.1. `class Meta`

| Atributo | Função |
|----------|--------|
| `model` | A qual model este form está conectado. |
| `fields` | Lista dos campos a incluir (ou `'__all__'`). |
| `exclude` | Lista de campos a ignorar (alternativa ao `fields`). |
| `widgets` | Customiza o HTML de cada campo. |
| `labels` | Sobrescreve o `verbose_name`. |
| `help_texts` | Texto de ajuda (abaixo do campo). |

### 4.2. Métodos `clean_<campo>()`

São executados em **cada campo individualmente**. Devem retornar o valor (eventualmente transformado) ou levantar `ValidationError`.

### 4.3. Método `clean()`

É executado **depois** de todos os `clean_<campo>()`. Útil quando a validação envolve múltiplos campos.

---

## 5. View para cadastrar livros

Edite `livros/views.py` e adicione:

```python
from django.contrib import messages
from django.shortcuts import redirect, render
from .forms import LivroForm


def criar_livro(request):
    if request.method == 'POST':
        form = LivroForm(request.POST, request.FILES)
        if form.is_valid():
            livro = form.save()
            messages.success(request, f'Livro "{livro.titulo}" cadastrado com sucesso!')
            return redirect('livros:detalhe', pk=livro.pk)
        else:
            messages.error(request, 'Corrija os erros abaixo.')
    else:
        form = LivroForm()

    return render(request, 'livros/form.html', {
        'form': form,
        'titulo': 'Cadastrar Livro',
    })
```

### 5.1. Por que `request.FILES`?

Sempre que o formulário envia **arquivos** (como a capa do livro), você precisa passar `request.FILES` ao instanciar o form. Caso contrário, o upload é silenciosamente ignorado.

### 5.2. O fluxo completo (padrão GET/POST)

```
GET  /livros/novo/   → form = LivroForm()                  → mostra form vazio
POST /livros/novo/   → form = LivroForm(POST, FILES)       → valida
                       ├─ válido    → save() + redirect
                       └─ inválido  → renderiza com erros
```

---

## 6. URL para o formulário

Edite `livros/urls.py`:

```python
from django.urls import path
from . import views

app_name = 'livros'

urlpatterns = [
    path('', views.home, name='home'),
    path('livros/', views.lista_livros, name='lista'),
    path('livros/novo/', views.criar_livro, name='criar'),
    path('livros/<int:pk>/', views.detalhe_livro, name='detalhe'),
]
```

>  **Atenção à ordem!** A URL `livros/novo/` deve vir **antes** de `livros/<int:pk>/`, senão o Django interpreta "novo" como um pk e dá erro.

---

## 7. Template do formulário

Crie `livros/templates/livros/form.html`:

```html
{% extends 'livros/base.html' %}

{% block title %}{{ titulo }} — Livraria Django{% endblock %}

{% block content %}
    <h1 class="mb-4">{{ titulo }}</h1>

    <form method="post" enctype="multipart/form-data" novalidate>
        {% csrf_token %}

        {% if form.non_field_errors %}
            <div class="alert alert-danger">
                {{ form.non_field_errors }}
            </div>
        {% endif %}

        <div class="row">
            {% for field in form %}
                <div class="{% if field.name == 'sinopse' or field.name == 'autores' %}col-12{% else %}col-md-6{% endif %} mb-3">
                    <label for="{{ field.id_for_label }}" class="form-label">
                        {{ field.label }}
                        {% if field.field.required %}<span class="text-danger">*</span>{% endif %}
                    </label>

                    {{ field }}

                    {% if field.help_text %}
                        <div class="form-text">{{ field.help_text }}</div>
                    {% endif %}

                    {% if field.errors %}
                        <div class="invalid-feedback d-block">
                            {% for erro in field.errors %}{{ erro }}<br>{% endfor %}
                        </div>
                    {% endif %}
                </div>
            {% endfor %}
        </div>

        <div class="d-flex gap-2">
            <button type="submit" class="btn btn-primary">
                <i class="bi bi-save"></i> Salvar
            </button>
            <a href="{% url 'livros:lista' %}" class="btn btn-outline-secondary">
                Cancelar
            </a>
        </div>
    </form>
{% endblock %}
```

### 7.1. Pontos críticos

 **`{% csrf_token %}`** é **obrigatório** em todo formulário POST. O Django bloqueia requisições sem ele.

 **`enctype="multipart/form-data"`** é **obrigatório** quando há upload de arquivos.

 **`novalidate`** desativa a validação nativa do navegador (deixamos o Django validar).

---

## 8. Adicionando link no menu

Atualize a navbar em `base.html`:

```html
<li class="nav-item">
    <a class="nav-link" href="{% url 'livros:criar' %}">
        <i class="bi bi-plus-circle"></i> Novo livro
    </a>
</li>
```

---

## 9. Testando

1. Acesse **http://127.0.0.1:8000/livros/novo/**.
2. Tente enviar o formulário **vazio** → veja as mensagens de erro automáticas.
3. Tente enviar com **ano = 1300** → veja a validação custom funcionar.
4. Tente cadastrar com **disponível marcado mas estoque = 0** → veja o `clean()` em ação.
5. Cadastre um livro válido → será redirecionado para a página de detalhe.

---

## 10. Forms para outras entidades

Crie também forms para Autor, Categoria e Editora — você usará na próxima aula:

```python
class CategoriaForm(forms.ModelForm):
    class Meta:
        model = Categoria
        fields = ['nome', 'descricao']
        widgets = {
            'nome': forms.TextInput(attrs={'class': 'form-control'}),
            'descricao': forms.Textarea(attrs={'class': 'form-control', 'rows': 3}),
        }


class EditoraForm(forms.ModelForm):
    class Meta:
        model = Editora
        fields = ['nome', 'cidade', 'site']
        widgets = {
            'nome': forms.TextInput(attrs={'class': 'form-control'}),
            'cidade': forms.TextInput(attrs={'class': 'form-control'}),
            'site': forms.URLInput(attrs={'class': 'form-control'}),
        }


class AutorForm(forms.ModelForm):
    class Meta:
        model = Autor
        fields = ['nome', 'biografia', 'nacionalidade']
        widgets = {
            'nome': forms.TextInput(attrs={'class': 'form-control'}),
            'biografia': forms.Textarea(attrs={'class': 'form-control', 'rows': 4}),
            'nacionalidade': forms.TextInput(attrs={'class': 'form-control'}),
        }
```

---

## 11. Mensagens flash configuradas

No `setup/settings.py`, garanta que `messages` está em `INSTALLED_APPS` (já vem por padrão) e em `MIDDLEWARE`:

```python
MIDDLEWARE = [
    # ...
    'django.contrib.messages.middleware.MessageMiddleware',
    # ...
]
```

E configure as classes CSS para casarem com Bootstrap:

```python
from django.contrib.messages import constants as messages

MESSAGE_TAGS = {
    messages.DEBUG: 'secondary',
    messages.INFO: 'info',
    messages.SUCCESS: 'success',
    messages.WARNING: 'warning',
    messages.ERROR: 'danger',
}
```

---

## 12. Commit

```bash
git add .
git commit -m "feat: cria LivroForm com validações e view de cadastro"
git push
```

---

##  Exercícios

1. Adicione uma validação para que o **ISBN não se repita ao editar** (já é unique no model, mas mostre uma mensagem amigável: "Já existe um livro com este ISBN.").

2. No `LivroForm`, faça com que o campo **sinopse** seja obrigatório, com no mínimo **30 caracteres**.

3. **Desafio:** Implemente o pacote `django-crispy-forms` com `crispy-bootstrap5` para renderizar formulários com `{% crispy form %}` em uma só linha. Documente o passo a passo em `docs/extra-crispy-forms.md`.

---

##  Erros comuns

| Erro | Causa | Solução |
|------|-------|---------|
| `Forbidden (CSRF token missing)` | Esqueceu `{% csrf_token %}` | Adicione dentro do `<form>`. |
| Upload silenciosamente ignorado | Esqueceu `enctype` ou `request.FILES` | Adicione ambos. |
| `'NoneType' has no attribute '...'` em `form.is_valid()` | Não está em request POST | Verifique o método antes de validar. |
| Erros não aparecem em Bootstrap | Classe `is-invalid` não aplicada | Use `{{ field.errors }}` ou crispy-forms. |

---

##  Checklist

- [ ] `forms.py` criado com `LivroForm`.
- [ ] Validações customizadas (`clean_isbn`, `clean_ano_publicacao`, `clean()`).
- [ ] View `criar_livro` tratando GET e POST.
- [ ] URL `livros/novo/` mapeada.
- [ ] Template `form.html` exibindo erros corretamente.
- [ ] Mensagens flash funcionando.
- [ ] Commit realizado.

---

- **Próxima aula:** [Aula 8 — CRUD Completo](./aula8-crud.md)
- **Aula anterior:** [Aula 6 — Templates e Bootstrap](./aula6-templates.md)
