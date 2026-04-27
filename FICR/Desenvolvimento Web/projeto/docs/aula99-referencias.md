#  Aula 99 — Referências

> Material complementar, fontes consultadas e bibliografia recomendada para aprofundamento na disciplina de Programação para Web com Django.

---

## 1. Documentação oficial

| Recurso | Link |
|---------|------|
| Django (documentação oficial) | https://docs.djangoproject.com/ |
| Django — Tutorial oficial | https://docs.djangoproject.com/en/stable/intro/tutorial01/ |
| Django REST Framework | https://www.django-rest-framework.org/ |
| Python (documentação oficial) | https://docs.python.org/3/ |
| Bootstrap 5 | https://getbootstrap.com/docs/5.3/ |
| Bootstrap Icons | https://icons.getbootstrap.com/ |
| Pillow (manipulação de imagens) | https://pillow.readthedocs.io/ |
| Git | https://git-scm.com/doc |

---

## 2. Livros recomendados

###  Português

- **MELÉ, Antonio.** *Django 5 by Example*. 5ª ed. Packt Publishing, 2024.
  *(Tem edições brasileiras adaptadas — verifique com sua biblioteca.)*

- **PEREIRA, Caique Rodrigues.** *Aprenda Programação Web com Python e Django*. Casa do Código, 2020.

- **GONÇALVES, Henrique.** *Django para Iniciantes*. Independente, 2023.

###  Inglês

- **VINCENT, William S.** *Django for Beginners* (Build websites with Python and Django). 5th ed. Welcome to Code, 2024.

- **VINCENT, William S.** *Django for Professionals*. Welcome to Code, 2023.

- **GREENFELD, Daniel; ROY, Audrey.** *Two Scoops of Django 3.x: Best Practices for the Django Web Framework*. Two Scoops Press, 2020.

---

## 3. Cursos online (gratuitos e pagos)

| Plataforma | Curso | Tipo |
|------------|-------|------|
| Django Girls | Tutorial oficial em português | Gratuito |
| MDN Web Docs | Tutorial Django | Gratuito |
| YouTube — Pythonista Code | Curso de Django (PT-BR) | Gratuito |
| YouTube — Curso em Vídeo (Gustavo Guanabara) | Python 3 (pré-requisito) | Gratuito |
| Coursera | Django for Everybody Specialization (Univ. Michigan) | Pago / auditoria gratuita |
| Udemy | Diversos cursos | Pago |

**Links:**

- Django Girls (PT-BR): https://tutorial.djangogirls.org/pt/
- MDN Django: https://developer.mozilla.org/pt-BR/docs/Learn/Server-side/Django
- Django for Everybody: https://www.dj4e.com/

---

## 4. Ferramentas e bibliotecas úteis

### Frameworks/Bibliotecas

| Pacote | Para que serve |
|--------|---------------|
| `django-crispy-forms` + `crispy-bootstrap5` | Renderiza forms com Bootstrap automaticamente. |
| `django-debug-toolbar` | Painel de debug (queries SQL, performance). |
| `django-allauth` | Autenticação avançada (social login, e-mail). |
| `django-extensions` | Comandos úteis (`shell_plus`, `runserver_plus`). |
| `Pillow` | Suporte a `ImageField`. |
| `python-decouple` | Variáveis de ambiente. |
| `whitenoise` | Servir arquivos estáticos em produção. |
| `gunicorn` | Servidor WSGI para produção. |
| `dj-database-url` | Configurar DATABASES por URL. |
| `psycopg2-binary` | Driver PostgreSQL. |

### Ferramentas externas

- **DBeaver** — cliente SQL gráfico (https://dbeaver.io)
- **Postman** / **Insomnia** — testar APIs
- **Visual Studio Code** — editor recomendado
- **PyCharm Community** — IDE alternativa, gratuita
- **Djecrety** — gerador de SECRET_KEY (https://djecrety.ir)

---

## 5. Plataformas de deploy

| Plataforma | Plano free | Notas |
|-----------|------------|-------|
| **Render** | Sim | Recomendado neste curso. Suporte a PostgreSQL gratuito. |
| **PythonAnywhere** | Sim | Mais simples, mas com domínio fixo. |
| **Railway** | Limitado (créditos) | Ótima DX, mas paga. |
| **Fly.io** | Sim | Performático, mas exige Docker. |
| **Heroku** |  Sem free tier | Histórico, mas não mais gratuito. |
| **AWS / GCP / Azure** | Limitado (free tier) | Recomendado para projetos profissionais. |

---

## 6. Comunidades e suporte

- **Stack Overflow** — https://stackoverflow.com/questions/tagged/django
- **Reddit r/django** — https://reddit.com/r/django
- **Django Discord** — https://discord.gg/xcRH6mN4fa
- **Django Forum** — https://forum.djangoproject.com/
- **Comunidade Python Brasil** — https://python.org.br/

---

## 7. Ferramentas de aprendizado contínuo

### Boletins e blogs

- **Django News** — https://django-news.com/ (semanal)
- **Real Python** — https://realpython.com/tutorials/django/
- **Mozilla Hacks** — https://hacks.mozilla.org/

### Canais de YouTube

- **Pythonista Code** (PT-BR)
- **Henrique Bastos** (PT-BR)
- **Curso em Vídeo** (Python — pré-requisito)
- **Django Chat Podcast** (EN)
- **CodingEntrepreneurs** (EN)

---

## 8. Padrões e boas práticas

- **Two Scoops of Django** — referência canônica em best practices.
- **Django Best Practices (oficial)** — https://docs.djangoproject.com/en/stable/misc/design-philosophies/
- **PEP 8** — guia de estilo Python — https://peps.python.org/pep-0008/
- **PEP 257** — convenções de docstrings — https://peps.python.org/pep-0257/
- **Conventional Commits** — https://www.conventionalcommits.org/pt-br/v1.0.0/

---

## 9. Próximos passos sugeridos

Após este curso, considere estudar:

1. **Django REST Framework (DRF)** — para construir APIs REST.
2. **Celery + Redis** — para tarefas assíncronas.
3. **Testes automatizados** — `pytest-django`, `Factory Boy`, `Selenium`.
4. **Docker** — containerização da aplicação.
5. **CI/CD** — GitHub Actions para deploy automático.
6. **Frontend moderno** — React/Vue consumindo APIs Django.
7. **Channels** — Django + WebSockets (chat, notificações em tempo real).

---

## 10. Como citar este material

Este material é de uso didático livre. Caso o utilize em outras disciplinas, sugira citar como:

> MACIEL, Ronierison. *Programação para Web — Projeto Livraria com Django*. Material didático. Recife, 2026. Disponível em: `https://github.com/0x03c1/livraria-django`.

---

## 11. Glossário rápido

| Termo | Significado |
|-------|------------|
| **MTV** | Model-Template-View, padrão arquitetural do Django. |
| **ORM** | Object-Relational Mapping. Mapeia classes para tabelas. |
| **Migration** | Versão de mudança no esquema do banco. |
| **CSRF** | Cross-Site Request Forgery, ataque de falsificação de requisições. |
| **CRUD** | Create, Read, Update, Delete. |
| **WSGI** | Web Server Gateway Interface. Padrão para servidores Python. |
| **Slug** | String amigável para URL (sem espaços, acentos). |
| **Fixture** | Dados iniciais em formato JSON/XML/YAML. |
| **Middleware** | Camada que processa requisições/respostas. |
| **QuerySet** | Objeto preguiçoso (lazy) que representa consultas ao banco. |

---

## 12. Cheatsheet de comandos Django

```bash
# Projeto e app
django-admin startproject setup .
python manage.py startapp livros

# Migrações
python manage.py makemigrations
python manage.py migrate
python manage.py showmigrations
python manage.py sqlmigrate livros 0001

# Servidor
python manage.py runserver
python manage.py runserver 0.0.0.0:8080

# Usuários
python manage.py createsuperuser
python manage.py changepassword <usuario>

# Shell e debug
python manage.py shell
python manage.py dbshell
python manage.py check
python manage.py check --deploy

# Estáticos e fixtures
python manage.py collectstatic
python manage.py loaddata dados.json
python manage.py dumpdata livros > dados.json
```

---

- **Aula anterior:** [Aula 10 — Deploy](./aula10-deploy.md)
- **Voltar ao início:** [README](../README.md)
