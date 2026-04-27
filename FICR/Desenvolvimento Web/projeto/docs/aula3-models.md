#  Aula 3 — Models: Modelando a Livraria

> **Objetivo:** modelar as entidades do domínio da livraria utilizando a ORM do Django, criar e aplicar migrações, e popular o banco com dados iniciais.

---

## 1. O que é um Model?

Um **Model** é uma classe Python que representa uma **tabela** no banco de dados. Cada atributo da classe corresponde a uma **coluna**, e cada instância da classe representa uma **linha**.

A grande vantagem é que você não precisa escrever SQL — o Django (via ORM) traduz suas classes Python em comandos SQL automaticamente.

```python
class Livro(models.Model):     # → Tabela "livros_livro"
    titulo = models.CharField(max_length=200)  # → Coluna "titulo VARCHAR(200)"
```

---

## 2. Modelagem da Livraria

Vamos criar quatro entidades principais:

```
┌─────────────┐         ┌──────────────┐         ┌──────────────┐
│  Categoria  │         │    Autor     │         │   Editora    │
└──────┬──────┘         └──────┬───────┘         └──────┬───────┘
       │                       │                        │
       │ 1:N                   │ N:N                    │ 1:N
       │                       │                        │
       └───────────────┬───────┴────────────────────────┘
                       ▼
                 ┌───────────┐
                 │   Livro   │
                 └───────────┘
```

| Entidade | Atributos | Relacionamentos |
|----------|-----------|----------------|
| **Categoria** | nome, descrição | 1:N com Livro |
| **Autor** | nome, biografia, nacionalidade | N:N com Livro |
| **Editora** | nome, cidade, site | 1:N com Livro |
| **Livro** | título, isbn, páginas, preço, ano, capa, sinopse | FK Categoria, FK Editora, M2M Autor |

---

## 3. Implementando os Models

Abra `livros/models.py` e substitua o conteúdo por:

```python
from django.db import models
from django.urls import reverse


class Categoria(models.Model):
    nome = models.CharField(max_length=100, unique=True)
    descricao = models.TextField(blank=True)

    class Meta:
        verbose_name = 'Categoria'
        verbose_name_plural = 'Categorias'
        ordering = ['nome']

    def __str__(self):
        return self.nome


class Editora(models.Model):
    nome = models.CharField(max_length=150, unique=True)
    cidade = models.CharField(max_length=100, blank=True)
    site = models.URLField(blank=True)

    class Meta:
        verbose_name = 'Editora'
        verbose_name_plural = 'Editoras'
        ordering = ['nome']

    def __str__(self):
        return self.nome


class Autor(models.Model):
    nome = models.CharField(max_length=150)
    biografia = models.TextField(blank=True)
    nacionalidade = models.CharField(max_length=80, blank=True)

    class Meta:
        verbose_name = 'Autor'
        verbose_name_plural = 'Autores'
        ordering = ['nome']

    def __str__(self):
        return self.nome


class Livro(models.Model):
    titulo = models.CharField('Título', max_length=200)
    isbn = models.CharField('ISBN', max_length=20, unique=True)
    paginas = models.PositiveIntegerField('Páginas', default=0)
    preco = models.DecimalField('Preço', max_digits=8, decimal_places=2)
    ano_publicacao = models.PositiveIntegerField('Ano de publicação')
    sinopse = models.TextField('Sinopse', blank=True)
    capa = models.ImageField('Capa', upload_to='capas/', blank=True, null=True)
    estoque = models.PositiveIntegerField('Estoque', default=0)
    disponivel = models.BooleanField('Disponível', default=True)
    criado_em = models.DateTimeField(auto_now_add=True)
    atualizado_em = models.DateTimeField(auto_now=True)

    # Relacionamentos
    categoria = models.ForeignKey(
        Categoria,
        on_delete=models.PROTECT,
        related_name='livros',
        verbose_name='Categoria',
    )
    editora = models.ForeignKey(
        Editora,
        on_delete=models.PROTECT,
        related_name='livros',
        verbose_name='Editora',
    )
    autores = models.ManyToManyField(
        Autor,
        related_name='livros',
        verbose_name='Autores',
    )

    class Meta:
        verbose_name = 'Livro'
        verbose_name_plural = 'Livros'
        ordering = ['titulo']

    def __str__(self):
        return f'{self.titulo} ({self.ano_publicacao})'

    def get_absolute_url(self):
        return reverse('livros:detalhe', kwargs={'pk': self.pk})
```

---

## 4. Entendendo os campos importantes

### 4.1. Tipos de campos mais comuns

| Campo | Uso |
|-------|-----|
| `CharField(max_length=N)` | Texto curto (sempre exige `max_length`). |
| `TextField()` | Texto longo, sem limite definido. |
| `IntegerField()` | Número inteiro. |
| `PositiveIntegerField()` | Inteiro ≥ 0. |
| `DecimalField(max_digits, decimal_places)` | Números monetários (NÃO use `FloatField` para dinheiro!). |
| `DateField` / `DateTimeField` | Datas e datas com hora. |
| `BooleanField()` | Verdadeiro/falso. |
| `EmailField()` | E-mail (com validação). |
| `URLField()` | URL (com validação). |
| `ImageField(upload_to=...)` | Imagem (requer `Pillow`). |

### 4.2. Argumentos comuns

| Argumento | Significado |
|-----------|------------|
| `blank=True` | Pode ser vazio nos formulários. |
| `null=True` | Pode ser `NULL` no banco. |
| `default=X` | Valor padrão. |
| `unique=True` | Valor deve ser único. |
| `verbose_name='...'` | Rótulo exibido na interface. |

### 4.3. Tipos de relacionamento

| Tipo | Quando usar |
|------|-------------|
| `ForeignKey` | 1:N (um livro tem **uma** categoria; uma categoria tem vários livros). |
| `ManyToManyField` | N:N (um livro pode ter vários autores; um autor escreve vários livros). |
| `OneToOneField` | 1:1 (raramente; ex.: extensão de `User`). |

### 4.4. Comportamento `on_delete`

Quando o "pai" é deletado, o que acontece com os "filhos"?

| Opção | Comportamento |
|-------|---------------|
| `CASCADE` | Deleta todos os filhos. |
| `PROTECT` | Impede a deleção se houver filhos. |
| `SET_NULL` | Coloca `NULL` no filho (exige `null=True`). |
| `SET_DEFAULT` | Coloca o valor `default`. |

>  Em catálogos (como nossa livraria), `PROTECT` costuma ser mais seguro: evita perder livros quando alguém apaga uma categoria.

---

## 5. Instalando suporte a imagens

O campo `ImageField` requer a biblioteca **Pillow**:

```bash
pip install Pillow
pip freeze > requirements.txt
```

---

## 6. Configurando arquivos de mídia

Imagens enviadas pelos usuários ficam em uma pasta de "mídia". Configure-a em `setup/settings.py`:

```python
import os
from pathlib import Path

BASE_DIR = Path(__file__).resolve().parent.parent

# ... (outras configurações)

# Arquivos de mídia (uploads)
MEDIA_URL = '/media/'
MEDIA_ROOT = BASE_DIR / 'media'
```

E em `setup/urls.py`, sirva os arquivos em desenvolvimento:

```python
from django.conf import settings
from django.conf.urls.static import static
from django.contrib import admin
from django.urls import include, path

urlpatterns = [
    path('admin/', admin.site.urls),
    path('', include('livros.urls')),
]

if settings.DEBUG:
    urlpatterns += static(settings.MEDIA_URL, document_root=settings.MEDIA_ROOT)
```

>  Em produção, arquivos de mídia são servidos pelo servidor web (Nginx, etc.), não pelo Django.

---

## 7. Criando e aplicando migrações

**Migração** = versão de uma alteração no banco. Toda mudança em models exige uma migração.

### 7.1. Criar o arquivo de migração

```bash
python manage.py makemigrations
```

Saída esperada:

```
Migrations for 'livros':
  livros/migrations/0001_initial.py
    + Create model Autor
    + Create model Categoria
    + Create model Editora
    + Create model Livro
```

### 7.2. Aplicar a migração no banco

```bash
python manage.py migrate
```

Pronto! As tabelas foram criadas no SQLite. 

>  Para ver o SQL gerado por uma migração:
> `python manage.py sqlmigrate livros 0001`

---

## 8. Explorando o ORM no shell

O Django tem um shell interativo riquíssimo. Vamos testar:

```bash
python manage.py shell
```

Dentro do shell:

```python
>>> from livros.models import Categoria, Autor, Editora, Livro

>>> # Criando registros
>>> cat = Categoria.objects.create(nome='Ficção Científica', descricao='Livros de FC')
>>> editora = Editora.objects.create(nome='Aleph', cidade='São Paulo')
>>> autor = Autor.objects.create(nome='Isaac Asimov', nacionalidade='Russo-americano')

>>> livro = Livro.objects.create(
...     titulo='Eu, Robô',
...     isbn='9788576572008',
...     paginas=320,
...     preco=49.90,
...     ano_publicacao=1950,
...     categoria=cat,
...     editora=editora,
... )
>>> livro.autores.add(autor)

>>> # Consultando
>>> Livro.objects.all()
<QuerySet [<Livro: Eu, Robô (1950)>]>

>>> Livro.objects.filter(ano_publicacao__gte=1950)
>>> Livro.objects.filter(titulo__icontains='robô')

>>> # Saindo
>>> exit()
```

### 8.1. Operadores de consulta úteis

| Operador | Significado | Exemplo |
|----------|-------------|---------|
| `__exact` | Igual exato (padrão) | `nome__exact='Ficção'` |
| `__iexact` | Igual ignorando maiúsculas | `nome__iexact='ficção'` |
| `__contains` | Contém substring | `titulo__contains='robô'` |
| `__icontains` | Contém ignorando caso | `titulo__icontains='Robô'` |
| `__gte` / `__lte` | Maior/menor ou igual | `preco__lte=50` |
| `__in` | Está em uma lista | `id__in=[1, 2, 3]` |
| `__startswith` | Começa com | `nome__startswith='A'` |

---

## 9. Estrutura final da aula

```
livraria/
├── livros/
│   ├── migrations/
│   │   ├── __init__.py
│   │   └── 0001_initial.py    ← gerado
│   ├── models.py               ← editado
│   └── ...
├── media/                      ← criado ao subir 1ª imagem
├── setup/
│   ├── settings.py             ← editado (MEDIA_URL/MEDIA_ROOT)
│   └── urls.py                 ← editado (static)
└── db.sqlite3
```

---

## 10. Commit

```bash
git add .
git commit -m "feat: modela entidades Livro, Autor, Categoria e Editora"
git push
```

---

##  Exercícios

1. Adicione um campo `idioma` ao modelo `Livro` (CharField com `choices`: 'PT', 'EN', 'ES'). Crie e aplique a migração.
2. No shell do Django, crie pelo menos **3 categorias**, **3 editoras**, **5 autores** e **10 livros**, relacionando-os corretamente.
3. **Desafio:** Pesquise o que é um **fixture** no Django e crie um arquivo `livros/fixtures/dados_iniciais.json` com os dados acima. Carregue com:
   ```bash
   python manage.py loaddata dados_iniciais.json
   ```

---

##  Erros comuns

| Erro | Causa | Solução |
|------|-------|---------|
| `cannot import name 'ImageField'` | Pillow não instalado | `pip install Pillow` |
| `you are trying to add a non-nullable field` | Adicionou campo obrigatório a tabela existente | Forneça um valor padrão ou aceite `null=True`. |
| `no such table: livros_livro` | Migrações não aplicadas | Rode `python manage.py migrate`. |

---

##  Checklist

- [ ] 4 modelos criados: `Categoria`, `Editora`, `Autor`, `Livro`.
- [ ] Pillow instalado.
- [ ] `MEDIA_URL` e `MEDIA_ROOT` configurados.
- [ ] Migrações criadas e aplicadas.
- [ ] Conseguiu criar registros via shell.
- [ ] Commit realizado.

---

- **Próxima aula:** [Aula 4 — Django Admin](./aula4-admin.md)
- **Aula anterior:** [Aula 2 — Início do Projeto](./aula2-inicio-do-projeto.md)
