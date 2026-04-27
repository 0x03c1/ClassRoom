#  Aula 4 — Django Admin

> **Objetivo:** ativar e customizar o painel administrativo do Django para cadastrar e gerenciar livros, autores, categorias e editoras de forma visual.

---

## 1. O Django Admin

O **Admin** é uma das funcionalidades mais poderosas do Django: um painel administrativo **automático** gerado a partir dos seus models, com CRUD completo, filtros, busca e autenticação — tudo "fora da caixa".

>  **Para que serve?**
> Para que administradores do sistema (não usuários finais) cadastrem dados internos. Geralmente fica em `/admin/`.

---

## 2. Criando um superusuário

Para acessar o admin, é necessário um usuário com permissões de administrador.

```bash
python manage.py createsuperuser
```

O Django pedirá:

```
Nome de usuário: roni
Endereço de email: roni@livraria.com
Password: ********
Password (again): ********
Superuser created successfully.
```

>  Em desenvolvimento, é comum usar senhas simples como `123` (o Django avisa, mas permite).

---

## 3. Acessando o admin

Rode o servidor:

```bash
python manage.py runserver
```

Acesse: **http://127.0.0.1:8000/admin/**

Faça login com o superusuário criado.

Você verá apenas dois grupos: **Authentication and Authorization** (Users e Groups). **Nossos modelos ainda não aparecem.**

---

## 4. Registrando os models no Admin

Abra `livros/admin.py` e adicione:

```python
from django.contrib import admin
from .models import Autor, Categoria, Editora, Livro


admin.site.register(Categoria)
admin.site.register(Editora)
admin.site.register(Autor)
admin.site.register(Livro)
```

Recarregue a página do admin. Agora os 4 modelos aparecem! 

---

## 5. Customizando a exibição (ModelAdmin)

O registro simples funciona, mas é "feio" e pouco produtivo. Vamos customizar.

Substitua o conteúdo de `livros/admin.py` por:

```python
from django.contrib import admin
from .models import Autor, Categoria, Editora, Livro


@admin.register(Categoria)
class CategoriaAdmin(admin.ModelAdmin):
    list_display = ('nome', 'descricao')
    search_fields = ('nome',)


@admin.register(Editora)
class EditoraAdmin(admin.ModelAdmin):
    list_display = ('nome', 'cidade', 'site')
    search_fields = ('nome', 'cidade')
    list_filter = ('cidade',)


@admin.register(Autor)
class AutorAdmin(admin.ModelAdmin):
    list_display = ('nome', 'nacionalidade')
    search_fields = ('nome',)
    list_filter = ('nacionalidade',)


@admin.register(Livro)
class LivroAdmin(admin.ModelAdmin):
    list_display = (
        'titulo',
        'categoria',
        'editora',
        'preco',
        'ano_publicacao',
        'estoque',
        'disponivel',
    )
    list_filter = ('categoria', 'editora', 'disponivel', 'ano_publicacao')
    search_fields = ('titulo', 'isbn', 'autores__nome')
    list_editable = ('preco', 'estoque', 'disponivel')
    autocomplete_fields = ('categoria', 'editora', 'autores')
    readonly_fields = ('criado_em', 'atualizado_em')
    fieldsets = (
        ('Informações principais', {
            'fields': ('titulo', 'isbn', 'sinopse', 'capa')
        }),
        ('Detalhes', {
            'fields': ('paginas', 'preco', 'ano_publicacao', 'estoque', 'disponivel')
        }),
        ('Relacionamentos', {
            'fields': ('categoria', 'editora', 'autores')
        }),
        ('Metadados', {
            'fields': ('criado_em', 'atualizado_em'),
            'classes': ('collapse',),
        }),
    )
    list_per_page = 20
```

---

## 6. Entendendo as customizações

| Atributo | O que faz |
|----------|-----------|
| `list_display` | Colunas exibidas na listagem. |
| `list_filter` | Filtros laterais. |
| `search_fields` | Campos pesquisáveis na barra de busca. |
| `list_editable` | Campos editáveis direto na listagem. |
| `autocomplete_fields` | Substitui o `<select>` por busca dinâmica (ótimo para FK e M2M com muitos itens). |
| `readonly_fields` | Campos somente leitura. |
| `fieldsets` | Organiza os campos em seções no formulário. |
| `list_per_page` | Quantidade de itens por página. |
| `prepopulated_fields` | Preenche automaticamente um campo a partir de outro (ex.: slug a partir do título). |

>  Para usar `autocomplete_fields`, o admin do modelo apontado **precisa** ter `search_fields` configurado.

---

## 7. Inline: editar livros dentro de uma categoria

Imagine ver todos os livros de uma categoria diretamente na tela da categoria. Adicione no `admin.py`:

```python
class LivroInline(admin.TabularInline):
    model = Livro
    extra = 0
    fields = ('titulo', 'preco', 'estoque', 'disponivel')
    show_change_link = True


@admin.register(Categoria)
class CategoriaAdmin(admin.ModelAdmin):
    list_display = ('nome', 'descricao')
    search_fields = ('nome',)
    inlines = [LivroInline]
```

>  Existem dois tipos de inline: `TabularInline` (formato tabela) e `StackedInline` (formato empilhado).

---

## 8. Personalizando o cabeçalho do admin

Em `setup/urls.py` (ou em qualquer arquivo carregado), adicione:

```python
from django.contrib import admin

admin.site.site_header = ' Livraria — Administração'
admin.site.site_title = 'Livraria Admin'
admin.site.index_title = 'Painel de controle'
```

---

## 9. Cadastrando dados pelo admin

Agora vá no admin e cadastre:

1. **3 categorias**: Ficção Científica, Romance, Tecnologia.
2. **3 editoras**: Aleph, Companhia das Letras, Novatec.
3. **5 autores**: Isaac Asimov, Machado de Assis, Robert C. Martin, etc.
4. **8 a 10 livros**, relacionando-os adequadamente.

>  Faça upload de capas (imagens). Elas serão salvas em `media/capas/`.

---

## 10. Commit

```bash
git add .
git commit -m "feat: registra models no admin com customizações (filters, search, inlines)"
git push
```

---

##  Exercícios

1. Adicione `prepopulated_fields = {'slug': ('nome',)}` no admin de `Categoria`. Para isso, antes você precisará adicionar um campo `slug = models.SlugField(unique=True)` ao model `Categoria` e criar a migração.

2. Crie uma **action customizada** no `LivroAdmin` que marca livros como indisponíveis em massa. Dica:
   ```python
   @admin.action(description='Marcar selecionados como indisponíveis')
   def marcar_indisponivel(modeladmin, request, queryset):
       queryset.update(disponivel=False)

   class LivroAdmin(admin.ModelAdmin):
       actions = [marcar_indisponivel]
   ```

3. **Desafio:** Pesquise sobre o pacote **`django-jazzmin`** (tema visual para o admin). Instale-o no projeto e configure-o. Documente o passo a passo em um arquivo `docs/extra-jazzmin.md`.

---

##  Checklist

- [ ] Superusuário criado.
- [ ] Todos os models registrados no admin.
- [ ] `list_display`, `list_filter` e `search_fields` configurados.
- [ ] `autocomplete_fields` funcionando para `Livro`.
- [ ] Inline de livros na tela de categoria.
- [ ] Cadastro de pelo menos 8 livros com capas.
- [ ] Commit realizado.

---

- **Próxima aula:** [Aula 5 — Views e URLs](./aula5-views-urls.md)
- **Aula anterior:** [Aula 3 — Models](./aula3-models.md)
