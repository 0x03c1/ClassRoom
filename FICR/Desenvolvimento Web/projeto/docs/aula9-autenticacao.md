#  Aula 9 — Autenticação e Controle de Acesso

> **Objetivo:** implementar login, logout e cadastro de usuários, e restringir o acesso a operações sensíveis (cadastrar/editar/excluir livros) somente a usuários autenticados.

---

## 1. O sistema de autenticação do Django

O Django já vem com um sistema de autenticação completo:

-  Modelo `User` (em `django.contrib.auth.models`).
-  Views prontas para login/logout/troca de senha.
-  Decorators e mixins para controle de acesso.
-  Permissões e grupos.

Tudo integrado ao admin.

---

## 2. Configurando URLs de autenticação

Edite `setup/urls.py`:

```python
from django.conf import settings
from django.conf.urls.static import static
from django.contrib import admin
from django.urls import include, path

urlpatterns = [
    path('admin/', admin.site.urls),
    path('contas/', include('django.contrib.auth.urls')),  # ← novo
    path('', include('livros.urls')),
]

if settings.DEBUG:
    urlpatterns += static(settings.MEDIA_URL, document_root=settings.MEDIA_ROOT)
```

Isso ativa as seguintes rotas automaticamente:

| URL | Nome | O que faz |
|-----|------|-----------|
| `/contas/login/` | `login` | Tela de login. |
| `/contas/logout/` | `logout` | Faz logout (requer POST). |
| `/contas/password_change/` | `password_change` | Trocar senha. |
| `/contas/password_change/done/` | `password_change_done` | Confirmação. |
| `/contas/password_reset/` | `password_reset` | Esqueci a senha (envia e-mail). |

---

## 3. Configurações no settings.py

Adicione ao final de `setup/settings.py`:

```python
# URL de login (para onde redirecionar quando @login_required falha)
LOGIN_URL = 'login'

# Para onde redirecionar após login bem-sucedido
LOGIN_REDIRECT_URL = 'livros:home'

# Para onde redirecionar após logout
LOGOUT_REDIRECT_URL = 'livros:home'
```

---

## 4. Templates de autenticação

O Django procura templates de auth em `registration/`. Crie a estrutura:

```
livros/
└── templates/
    └── registration/
        └── login.html
```

### 4.1. `login.html`

```html
{% extends 'livros/base.html' %}

{% block title %}Login — Livraria Django{% endblock %}

{% block content %}
    <div class="row justify-content-center">
        <div class="col-md-5">
            <div class="card shadow">
                <div class="card-body p-4">
                    <h2 class="card-title text-center mb-4">
                        <i class="bi bi-box-arrow-in-right"></i> Entrar
                    </h2>

                    {% if form.errors %}
                        <div class="alert alert-danger">
                            Usuário ou senha inválidos.
                        </div>
                    {% endif %}

                    <form method="post" novalidate>
                        {% csrf_token %}

                        <div class="mb-3">
                            <label for="id_username" class="form-label">Usuário</label>
                            <input type="text"
                                   name="username"
                                   id="id_username"
                                   class="form-control"
                                   autofocus
                                   required>
                        </div>

                        <div class="mb-3">
                            <label for="id_password" class="form-label">Senha</label>
                            <input type="password"
                                   name="password"
                                   id="id_password"
                                   class="form-control"
                                   required>
                        </div>

                        <input type="hidden" name="next" value="{{ next }}">

                        <button type="submit" class="btn btn-primary w-100">
                            Entrar
                        </button>
                    </form>

                    <hr>
                    <p class="text-center mb-0">
                        Não tem conta?
                        <a href="{% url 'cadastro' %}">Cadastre-se</a>
                    </p>
                </div>
            </div>
        </div>
    </div>
{% endblock %}
```

---

## 5. View customizada de cadastro

O Django **não inclui** uma view de cadastro de novos usuários por padrão (apenas login/logout/senha). Vamos criar a nossa.

Crie um novo app para usuários (boa prática):

```bash
python manage.py startapp usuarios
```

Registre em `INSTALLED_APPS`:

```python
INSTALLED_APPS = [
    # ...
    'livros',
    'usuarios',
]
```

### 5.1. View de cadastro

Em `usuarios/views.py`:

```python
from django.contrib import messages
from django.contrib.auth import login
from django.contrib.auth.forms import UserCreationForm
from django.shortcuts import redirect, render


def cadastro(request):
    if request.method == 'POST':
        form = UserCreationForm(request.POST)
        if form.is_valid():
            user = form.save()
            login(request, user)  # já loga após cadastrar
            messages.success(request, f'Bem-vindo(a), {user.username}!')
            return redirect('livros:home')
    else:
        form = UserCreationForm()

    return render(request, 'registration/cadastro.html', {'form': form})
```

### 5.2. URL do cadastro

Crie `usuarios/urls.py`:

```python
from django.urls import path
from . import views

urlpatterns = [
    path('cadastro/', views.cadastro, name='cadastro'),
]
```

E inclua em `setup/urls.py`:

```python
urlpatterns = [
    path('admin/', admin.site.urls),
    path('contas/', include('django.contrib.auth.urls')),
    path('contas/', include('usuarios.urls')),  # ← novo
    path('', include('livros.urls')),
]
```

### 5.3. Template do cadastro

Crie `usuarios/templates/registration/cadastro.html`:

```html
{% extends 'livros/base.html' %}

{% block title %}Cadastro — Livraria Django{% endblock %}

{% block content %}
    <div class="row justify-content-center">
        <div class="col-md-6">
            <div class="card shadow">
                <div class="card-body p-4">
                    <h2 class="card-title text-center mb-4">
                        <i class="bi bi-person-plus"></i> Criar conta
                    </h2>

                    <form method="post" novalidate>
                        {% csrf_token %}

                        {% for field in form %}
                            <div class="mb-3">
                                <label for="{{ field.id_for_label }}" class="form-label">
                                    {{ field.label }}
                                </label>
                                {{ field }}
                                {% if field.help_text %}
                                    <div class="form-text small">{{ field.help_text|safe }}</div>
                                {% endif %}
                                {% if field.errors %}
                                    <div class="text-danger small">
                                        {% for erro in field.errors %}{{ erro }}<br>{% endfor %}
                                    </div>
                                {% endif %}
                            </div>
                        {% endfor %}

                        <button type="submit" class="btn btn-success w-100">
                            Cadastrar
                        </button>
                    </form>

                    <hr>
                    <p class="text-center mb-0">
                        Já tem conta?
                        <a href="{% url 'login' %}">Entrar</a>
                    </p>
                </div>
            </div>
        </div>
    </div>
{% endblock %}
```

>  **Dica:** os campos do `UserCreationForm` vêm sem classes CSS. Você pode aplicar Bootstrap criando um form customizado que herda dele e sobrescreve os widgets, ou usando `django-crispy-forms`.

---

## 6. Atualizando a navbar com login/logout

Em `livros/templates/livros/base.html`, atualize o lado direito da navbar:

```html
<ul class="navbar-nav">
    {% if user.is_authenticated %}
        <li class="nav-item">
            <span class="nav-link">
                <i class="bi bi-person-circle"></i> {{ user.username }}
            </span>
        </li>
        {% if user.is_staff %}
            <li class="nav-item">
                <a class="nav-link" href="/admin/">Admin</a>
            </li>
        {% endif %}
        <li class="nav-item">
            <form method="post" action="{% url 'logout' %}" class="d-inline">
                {% csrf_token %}
                <button type="submit" class="btn btn-link nav-link">
                    <i class="bi bi-box-arrow-right"></i> Sair
                </button>
            </form>
        </li>
    {% else %}
        <li class="nav-item">
            <a class="nav-link" href="{% url 'login' %}">
                <i class="bi bi-box-arrow-in-right"></i> Entrar
            </a>
        </li>
        <li class="nav-item">
            <a class="nav-link" href="{% url 'cadastro' %}">
                <i class="bi bi-person-plus"></i> Cadastrar
            </a>
        </li>
    {% endif %}
</ul>
```

>  A partir do Django 5.0, **logout exige POST**. Por isso usamos um `<form>` em vez de um `<a href>`.

---

## 7. Restringindo acesso com `@login_required`

Vamos exigir login para criar, editar e deletar livros. Em `livros/views.py`:

```python
from django.contrib.auth.decorators import login_required


@login_required
def criar_livro(request):
    # ... código já existente ...


@login_required
def editar_livro(request, pk):
    # ... código já existente ...


@login_required
def deletar_livro(request, pk):
    # ... código já existente ...


@login_required
def gerenciar_livros(request):
    # ... código já existente ...
```

Agora, ao tentar acessar `/livros/novo/` sem login, o usuário é **automaticamente redirecionado** para `/contas/login/?next=/livros/novo/`. Após logar, retorna ao destino.

---

## 8. Restringindo somente a admins (staff)

Quer permitir cadastro de livros **apenas para staff**? Use `@user_passes_test`:

```python
from django.contrib.auth.decorators import user_passes_test


def is_staff(user):
    return user.is_authenticated and user.is_staff


@user_passes_test(is_staff)
def criar_livro(request):
    # ...
```

Ou use `@permission_required` se estiver usando o sistema de permissões granular do Django.

---

## 9. Em CBVs: `LoginRequiredMixin`

Se estiver usando Class-Based Views (Aula 8, seção 7):

```python
from django.contrib.auth.mixins import LoginRequiredMixin, UserPassesTestMixin


class LivroCreateView(LoginRequiredMixin, CreateView):
    # ...
    login_url = 'login'  # opcional


class LivroDeleteView(LoginRequiredMixin, UserPassesTestMixin, DeleteView):
    def test_func(self):
        return self.request.user.is_staff
```

---

## 10. Mostrando/escondendo botões nos templates

Já temos as views protegidas, mas é bom também **esconder os botões** que o usuário não pode usar:

```html
{% if user.is_authenticated %}
    <a href="{% url 'livros:editar' livro.pk %}" class="btn btn-warning">Editar</a>
{% endif %}

{% if user.is_staff %}
    <a href="{% url 'livros:deletar' livro.pk %}" class="btn btn-danger">Excluir</a>
{% endif %}
```

>  **Atenção:** esconder botões é só **conveniência visual**. A segurança real é a do **decorator** na view. Nunca confie só no template para autorização.

---

## 11. Testando

1. Faça **logout** (se estiver logado).
2. Tente acessar `/livros/novo/` → deve ser redirecionado para o login.
3. Faça **cadastro** com um usuário novo.
4. Após cadastro, deve voltar para a home logado.
5. Acesse `/livros/novo/` → agora funciona.
6. **Logout** → tente acessar `/livros/gerenciar/` → bloqueado.

---

## 12. Commit

```bash
git add .
git commit -m "feat: implementa autenticação (login, logout, cadastro) e protege views"
git push
```

---

##  Exercícios

1. Crie uma view `meu_perfil(request)` que mostre os dados do usuário logado: username, e-mail, data de cadastro, último login. Acessível em `/contas/perfil/` e protegida por `@login_required`.

2. Personalize a página `password_change.html` com Bootstrap (template em `registration/password_change_form.html`).

3. Adicione no model `Livro` um campo `cadastrado_por = models.ForeignKey(User, on_delete=PROTECT, null=True)`. Na view `criar_livro`, antes do `form.save()`, faça:
   ```python
   livro = form.save(commit=False)
   livro.cadastrado_por = request.user
   livro.save()
   form.save_m2m()  # necessário quando usa commit=False com M2M
   ```

4. **Desafio:** Crie dois grupos no admin: **Vendedores** (podem criar/editar) e **Gerentes** (podem deletar). Use o sistema de permissões nativo do Django (`Permission`) e ajuste as views para usar `@permission_required`.

---

##  Erros comuns

| Erro | Causa | Solução |
|------|-------|---------|
| `TemplateDoesNotExist: registration/login.html` | Template não está no caminho correto | Crie em `<app>/templates/registration/login.html`. |
| Logout dá erro 405 (Method Not Allowed) | Tentou GET em vez de POST | Use `<form method="post">`. |
| Após login, vai para `/accounts/profile/` | `LOGIN_REDIRECT_URL` não definido | Configure no settings. |
| `CSRF verification failed` | Form sem `{% csrf_token %}` | Adicione. |

---

##  Checklist

- [ ] URLs `contas/` configuradas.
- [ ] Login funcionando em `/contas/login/`.
- [ ] Cadastro funcionando em `/contas/cadastro/`.
- [ ] Logout funcionando (via POST).
- [ ] `LOGIN_URL`, `LOGIN_REDIRECT_URL` e `LOGOUT_REDIRECT_URL` configurados.
- [ ] Views protegidas com `@login_required`.
- [ ] Navbar adapta-se ao estado de login.
- [ ] Commit realizado.

---

- **Próxima aula:** [Aula 10 — Deploy](./aula10-deploy.md)
- **Aula anterior:** [Aula 8 — CRUD Completo](./aula8-crud.md)
