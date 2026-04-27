#  Aula 1 — Configuração do Ambiente

> **Objetivo:** preparar a máquina do(a) aluno(a) para desenvolvimento Python/Django, deixando todas as ferramentas instaladas e testadas.

---

## 1. Pré-requisitos

Antes de começar, você precisa ter instalado:

| Ferramenta | Versão recomendada | Observação |
|------------|-------------------|------------|
| Python | 3.11 ou superior | https://www.python.org/downloads |
| Git | 2.40+ | https://git-scm.com/downloads |
| VS Code | última | https://code.visualstudio.com |
| Navegador | Chrome / Firefox / Edge | qualquer atualizado |

---

## 2. Verificando a instalação do Python

Abra o terminal (PowerShell no Windows, Terminal no macOS/Linux) e execute:

```bash
python --version
```

>  No Linux/macOS, pode ser necessário usar `python3` no lugar de `python`.

A saída deve ser algo como:

```
Python 3.11.7
```

Se aparecer "comando não encontrado", reinstale o Python marcando a opção **"Add Python to PATH"** durante a instalação.

---

## 3. Criando o ambiente virtual (venv)

Um **ambiente virtual** isola as dependências de cada projeto Python. É uma boa prática **sempre** trabalhar dentro de um venv.

### 3.1. Crie a pasta do projeto

```bash
mkdir livraria
cd livraria
```

### 3.2. Crie o ambiente virtual

```bash
python -m venv venv
```

Isso cria uma pasta `venv/` com uma cópia isolada do interpretador Python.

### 3.3. Ative o ambiente virtual

**Windows (PowerShell):**

```powershell
.\venv\Scripts\Activate.ps1
```

>  Se aparecer erro de execução de scripts no PowerShell, rode antes:
> `Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser`

**Windows (CMD):**

```cmd
venv\Scripts\activate.bat
```

**Linux / macOS:**

```bash
source venv/bin/activate
```

Após ativar, o prompt deve exibir `(venv)` no início:

```
(venv) usuario@maquina:~/livraria$
```

### 3.4. Para desativar (quando quiser sair):

```bash
deactivate
```

---

## 4. Instalando o Django

Com o **venv ativado**, instale o Django:

```bash
pip install --upgrade pip
pip install django
```

Verifique a versão instalada:

```bash
python -m django --version
```

A saída deve ser algo como `5.1.4` (ou superior).

---

## 5. Congelando as dependências

É uma boa prática registrar as dependências em um arquivo `requirements.txt`:

```bash
pip freeze > requirements.txt
```

O conteúdo será similar a:

```
asgiref==3.8.1
Django==5.1.4
sqlparse==0.5.3
```

>  Esse arquivo permite que outra pessoa replique seu ambiente com:
> `pip install -r requirements.txt`

---

## 6. Configurando o VS Code

### 6.1. Extensões recomendadas

Instale as seguintes extensões no VS Code:

- **Python** (Microsoft)
- **Pylance** (Microsoft)
- **Django** (Baptiste Darthenay)
- **GitLens** (opcional, mas muito útil)
- **Material Icon Theme** (opcional, melhora a visualização)

### 6.2. Selecionando o interpretador Python

1. Abra a pasta do projeto no VS Code: `code .`
2. Pressione `Ctrl + Shift + P` (ou `Cmd + Shift + P` no macOS).
3. Digite: **Python: Select Interpreter**.
4. Escolha o interpretador que aponta para `./venv/Scripts/python.exe` (Windows) ou `./venv/bin/python` (Linux/macOS).

---

## 7. Configurando o Git

### 7.1. Configuração inicial (apenas uma vez na vida)

```bash
git config --global user.name "Seu Nome"
git config --global user.email "seu.email@exemplo.com"
```

### 7.2. Inicializando o repositório

Dentro da pasta `livraria/`:

```bash
git init
```

### 7.3. Criando o `.gitignore`

Crie um arquivo chamado `.gitignore` na raiz do projeto com o seguinte conteúdo:

```gitignore
# Ambiente virtual
venv/
env/

# Cache do Python
__pycache__/
*.py[cod]
*$py.class

# Banco de dados local
*.sqlite3
*.sqlite3-journal

# Arquivos do Django
*.log
local_settings.py
db.sqlite3
media/

# Arquivos de configuração de IDE
.vscode/
.idea/
*.swp

# Variáveis de ambiente
.env
.env.local

# Sistema operacional
.DS_Store
Thumbs.db
```

>  **Por que isso importa?** O `.gitignore` evita que arquivos desnecessários ou sensíveis (como o banco de dados local e variáveis de ambiente) sejam enviados para o GitHub.

---

## 8. Estrutura inicial esperada

Após esta aula, sua pasta deve estar assim:

```
livraria/
├── venv/
├── .gitignore
└── requirements.txt
```

---

## 9. Primeiro commit

```bash
git add .gitignore requirements.txt
git commit -m "chore: configura ambiente inicial do projeto"
```

---

##  Exercícios

1. Execute `python --version` e `pip list` (com o venv ativo) e tire um print da saída.
2. Crie um repositório no GitHub chamado `livraria-django` e faça o push do projeto:
   ```bash
   git branch -M main
   git remote add origin https://github.com/SEU-USUARIO/livraria-django.git
   git push -u origin main
   ```
3. Verifique se o repositório no GitHub **não contém** a pasta `venv/`. Se contiver, o `.gitignore` está incorreto.

---

##  Checklist

- [ ] Python 3.11+ instalado e no PATH.
- [ ] Ambiente virtual criado e ativado.
- [ ] Django instalado dentro do venv.
- [ ] `requirements.txt` gerado.
- [ ] VS Code com interpretador apontando para o venv.
- [ ] Git configurado e primeiro commit realizado.
- [ ] Repositório criado no GitHub.

---

- **Próxima aula:** [Aula 2 — Início do Projeto](./aula2-inicio-do-projeto.md)
