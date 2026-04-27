#  Aula 8 — CRUD Completo

> **Objetivo:** completar as quatro operações fundamentais (Create, Read, Update, Delete) sobre livros, integrando views, formulários, mensagens e confirmações.

---

## 1. O que é CRUD?

| Letra | Operação | HTTP | Status |
|-------|----------|------|--------|
| **C** | Create | POST |  Aula 7 |
| **R** | Read | GET |  Aula 5 |
| **U** | Update | POST | ⬜ Esta aula |
| **D** | Delete | POST | ⬜ Esta aula |

>  Vamos completar **U** e **D**.

---

## 2. View para editar (Update)

Adicione em `livros/views.py`:

```python
from django.shortcuts import get_object_or_404, redirect, render
from django.contrib import messages
from .forms import LivroForm
from .models import Livro


def editar_livro(request, pk):
    livro = get_object_or_404(Livro, pk=pk)

    if request.method == 'POST':
        form = LivroForm(request.POST, request.FILES, instance=livro)
        if form.is_valid():
            form.save()
            messages.success(request, f'Livro "{livro.titulo}" atualizado com sucesso!')
            return redirect('livros:detalhe', pk=livro.pk)
        else:
            messages.error(request, 'Corrija os erros abaixo.')
    else:
        form = LivroForm(instance=livro)

    return render(request, 'livros/form.html', {
        'form': form,
        'titulo': f'Editar: {livro.titulo}',
        'editando': True,
    })
```

### 2.1. A "mágica" do `instance=`

Ao passar `instance=livro` ao formulário:

- Se `request.method == 'GET'`: o form aparece **preenchido** com os dados atuais.
- Se `request.method == 'POST'`: o `form.save()` faz **UPDATE** no banco em vez de **INSERT**.

**Reuso total** — o mesmo `LivroForm` e o mesmo template `form.html` servem para criar e editar! 

---

## 3. View para deletar (Delete)

```python
def deletar_livro(request, pk):
    livro = get_object_or_404(Livro, pk=pk)

    if request.method == 'POST':
        titulo = livro.titulo
        livro.delete()
        messages.success(request, f'Livro "{titulo}" foi removido.')
        return redirect('livros:lista')

    return render(request, 'livros/confirmar_delete.html', {
        'livro': livro,
    })
```

### 3.1. Por que duas etapas (GET → POST)?

Nunca delete com **GET**. Imagine se um motor de busca rastreasse o link `/livros/5/deletar/` — ele apagaria seu banco inteiro!

 **Boa prática:** GET mostra a tela de **confirmação**, e o POST efetivamente apaga.

---

## 4. Atualizando as URLs

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
    path('livros/<int:pk>/editar/', views.editar_livro, name='editar'),
    path('livros/<int:pk>/deletar/', views.deletar_livro, name='deletar'),
]
```

---

## 5. Template de confirmação de delete

Crie `livros/templates/livros/confirmar_delete.html`:

```html
{% extends 'livros/base.html' %}

{% block title %}Confirmar exclusão — Livraria Django{% endblock %}

{% block content %}
    <div class="row justify-content-center">
        <div class="col-md-8">
            <div class="card border-danger shadow">
                <div class="card-header bg-danger text-white">
                    <h4 class="mb-0">
                        <i class="bi bi-exclamation-triangle"></i> Confirmar exclusão
                    </h4>
                </div>
                <div class="card-body">
                    <p class="lead">
                        Tem certeza que deseja excluir o livro
                        <strong>"{{ livro.titulo }}"</strong>?
                    </p>
                    <p class="text-muted">
                        Esta ação <strong>não pode ser desfeita</strong>.
                    </p>

                    <ul class="list-unstyled">
                        <li><strong>ISBN:</strong> {{ livro.isbn }}</li>
                        <li><strong>Editora:</strong> {{ livro.editora.nome }}</li>
                        <li><strong>Ano:</strong> {{ livro.ano_publicacao }}</li>
                    </ul>

                    <form method="post" class="d-flex gap-2">
                        {% csrf_token %}
                        <button type="submit" class="btn btn-danger">
                            <i class="bi bi-trash"></i> Sim, excluir
                        </button>
                        <a href="{% url 'livros:detalhe' livro.pk %}" class="btn btn-outline-secondary">
                            Cancelar
                        </a>
                    </form>
                </div>
            </div>
        </div>
    </div>
{% endblock %}
```

---

## 6. Adicionando botões de ação

Atualize `livros/templates/livros/detalhe.html` adicionando botões de **Editar** e **Excluir** no final:

```html
<div class="d-flex gap-2 mt-3">
    <a href="{% url 'livros:lista' %}" class="btn btn-outline-secondary">
        ← Voltar à lista
    </a>
    <a href="{% url 'livros:editar' livro.pk %}" class="btn btn-warning">
        <i class="bi bi-pencil"></i> Editar
    </a>
    <a href="{% url 'livros:deletar' livro.pk %}" class="btn btn-danger">
        <i class="bi bi-trash"></i> Excluir
    </a>
</div>
```

---

## 7. Variante: usando Class-Based Views (CBV)

Para informação e prática, vamos ver como o mesmo CRUD ficaria com CBVs. **Crie um arquivo separado** `livros/views_cbv.py` para experimentar (não substituir as FBV ainda):

```python
from django.urls import reverse_lazy
from django.views.generic import CreateView, DeleteView, DetailView, ListView, UpdateView
from .forms import LivroForm
from .models import Livro


class LivroListView(ListView):
    model = Livro
    template_name = 'livros/lista.html'
    context_object_name = 'page_obj'  # nome para casar com o template existente
    paginate_by = 6

    def get_queryset(self):
        qs = super().get_queryset().filter(disponivel=True)
        busca = self.request.GET.get('q')
        if busca:
            qs = qs.filter(titulo__icontains=busca)
        return qs

    def get_context_data(self, **kwargs):
        ctx = super().get_context_data(**kwargs)
        ctx['busca'] = self.request.GET.get('q', '')
        return ctx


class LivroDetailView(DetailView):
    model = Livro
    template_name = 'livros/detalhe.html'
    context_object_name = 'livro'


class LivroCreateView(CreateView):
    model = Livro
    form_class = LivroForm
    template_name = 'livros/form.html'

    def get_context_data(self, **kwargs):
        ctx = super().get_context_data(**kwargs)
        ctx['titulo'] = 'Cadastrar livro'
        return ctx


class LivroUpdateView(UpdateView):
    model = Livro
    form_class = LivroForm
    template_name = 'livros/form.html'

    def get_context_data(self, **kwargs):
        ctx = super().get_context_data(**kwargs)
        ctx['titulo'] = f'Editar: {self.object.titulo}'
        ctx['editando'] = True
        return ctx


class LivroDeleteView(DeleteView):
    model = Livro
    template_name = 'livros/confirmar_delete.html'
    success_url = reverse_lazy('livros:lista')
    context_object_name = 'livro'
```

### 7.1. Comparação rápida

| Aspecto | FBV | CBV |
|---------|-----|-----|
| Curva de aprendizado | Mais simples | Mais íngreme |
| Linhas de código | Mais (verboso) | Menos (compacto) |
| Customização | Direta | Via override de métodos |
| Reuso | Por copy-paste | Por herança |

>  **Recomendação:** comece com FBV, migre para CBV em projetos médios/grandes onde o reuso compense.

---

## 8. View de listagem como tabela administrativa

Vamos criar uma página de gestão (lista com botões de ação) — separada da listagem pública. Adicione em `views.py`:

```python
def gerenciar_livros(request):
    livros = Livro.objects.all().order_by('-criado_em')
    return render(request, 'livros/gerenciar.html', {'livros': livros})
```

URL:

```python
path('livros/gerenciar/', views.gerenciar_livros, name='gerenciar'),
```

>  Atenção à ordem! `livros/gerenciar/` antes de `livros/<int:pk>/`.

Template `livros/templates/livros/gerenciar.html`:

```html
{% extends 'livros/base.html' %}

{% block title %}Gerenciar Livros — Livraria Django{% endblock %}

{% block content %}
    <div class="d-flex justify-content-between align-items-center mb-4">
        <h1>Gerenciar Livros</h1>
        <a href="{% url 'livros:criar' %}" class="btn btn-primary">
            <i class="bi bi-plus-circle"></i> Novo livro
        </a>
    </div>

    <div class="table-responsive">
        <table class="table table-hover align-middle">
            <thead class="table-dark">
                <tr>
                    <th>Título</th>
                    <th>Categoria</th>
                    <th>Editora</th>
                    <th class="text-end">Preço</th>
                    <th class="text-center">Estoque</th>
                    <th class="text-center">Disponível</th>
                    <th class="text-center">Ações</th>
                </tr>
            </thead>
            <tbody>
                {% for livro in livros %}
                    <tr>
                        <td>
                            <a href="{% url 'livros:detalhe' livro.pk %}">{{ livro.titulo }}</a>
                        </td>
                        <td><span class="badge bg-secondary">{{ livro.categoria.nome }}</span></td>
                        <td>{{ livro.editora.nome }}</td>
                        <td class="text-end">R$ {{ livro.preco|floatformat:2 }}</td>
                        <td class="text-center">{{ livro.estoque }}</td>
                        <td class="text-center">
                            {% if livro.disponivel %}
                                <i class="bi bi-check-circle text-success"></i>
                            {% else %}
                                <i class="bi bi-x-circle text-danger"></i>
                            {% endif %}
                        </td>
                        <td class="text-center">
                            <div class="btn-group btn-group-sm">
                                <a href="{% url 'livros:editar' livro.pk %}"
                                   class="btn btn-outline-warning"
                                   title="Editar">
                                    <i class="bi bi-pencil"></i>
                                </a>
                                <a href="{% url 'livros:deletar' livro.pk %}"
                                   class="btn btn-outline-danger"
                                   title="Excluir">
                                    <i class="bi bi-trash"></i>
                                </a>
                            </div>
                        </td>
                    </tr>
                {% empty %}
                    <tr>
                        <td colspan="7" class="text-center py-4 text-muted">
                            Nenhum livro cadastrado.
                        </td>
                    </tr>
                {% endfor %}
            </tbody>
        </table>
    </div>
{% endblock %}
```

---

## 9. Testando o ciclo completo

1. Acesse `/livros/gerenciar/`.
2. Clique em **Novo livro** e cadastre.
3. Edite o livro recém-criado.
4. Tente excluí-lo — deve aparecer a tela de confirmação.
5. Confirme a exclusão.

---

## 10. Commit

```bash
git add .
git commit -m "feat: implementa CRUD completo (criar, editar, deletar) com confirmação"
git push
```

---

##  Exercícios

1. Implemente o CRUD completo de **Categorias** (lista, criar, editar, deletar). Use `CategoriaForm` (criado na Aula 7).

2. Implemente o CRUD completo de **Autores** e **Editoras**.

3. Adicione uma **confirmação JavaScript** no botão de excluir (na tabela de gerenciar) usando `onclick="return confirm('Tem certeza?')"`. Esta é uma camada extra antes da página de confirmação.

4. **Desafio:** Migre todas as views FBV para CBV (com base no exemplo da seção 7) e ajuste o `urls.py`. Documente as diferenças que percebeu em `docs/extra-fbv-vs-cbv.md`.

---

##  Erros comuns

| Erro | Causa | Solução |
|------|-------|---------|
| Form de edição vem em branco | Esqueceu `instance=livro` | Adicione no `LivroForm(instance=livro)`. |
| Tela de confirmação dá erro CSRF | Form sem `{% csrf_token %}` | Adicione no `<form>`. |
| Cannot delete some instances of model 'Editora' because they are referenced through a protected foreign key | Tem livros vinculados | Ou apague os livros antes ou troque para `on_delete=CASCADE` (cuidado). |

---

##  Checklist

- [ ] View `editar_livro` funcionando.
- [ ] View `deletar_livro` com confirmação.
- [ ] Página `gerenciar_livros` listando em tabela.
- [ ] Botões de ação (editar/deletar) na página de detalhe.
- [ ] Template `confirmar_delete.html` criado.
- [ ] Mensagens flash em todas as operações.
- [ ] Commit realizado.

---

- **Próxima aula:** [Aula 9 — Autenticação e Controle de Acesso](./aula9-autenticacao.md)
- **Aula anterior:** [Aula 7 — Forms e Validação](./aula7-forms.md)
