# FastAPI no Azure App Service

**Guia Prático para Estudantes — Deploy de API REST em nuvem**

---

## Sumário

1. [Visão Geral](#1-visão-geral)
2. [Pré-requisitos](#2-pré-requisitos)
3. [Estrutura dos Arquivos](#3-estrutura-dos-arquivos)
4. [Criando os Recursos no Azure](#4-criando-os-recursos-no-azure)
5. [Escrevendo a Aplicação via SSH](#5-escrevendo-a-aplicação-via-ssh)
6. [Configurando o Comando de Inicialização](#6-configurando-o-comando-de-inicialização)
7. [Verificando e Testando a API](#7-verificando-e-testando-a-api)
8. [Referência da API](#8-referência-da-api)
9. [Resolução de Problemas](#9-resolução-de-problemas)
10. [Limpando os Recursos](#10-limpando-os-recursos)
11. [Próximos Passos](#11-próximos-passos)

---

## 1. Visão Geral

Neste guia você vai implantar uma API REST completa na nuvem **sem instalar nada no seu computador**. Tudo é feito diretamente pelo navegador, usando o portal do Azure e o terminal SSH embutido.

A API que vamos construir gerencia uma lista de tarefas (ToDo). Ela serve de base para qualquer projeto que precise expor dados via HTTP — e é exatamente o tipo de aplicação que aparece em entrevistas e projetos reais.

### O que você vai aprender

- Criar recursos no Azure pelo portal (grupo de recursos, plano de serviço, Web App)
- Escrever código Python diretamente no servidor via SSH
- Configurar o servidor Gunicorn + Uvicorn para rodar FastAPI
- Testar endpoints REST com Swagger UI e `curl`

### Tecnologias utilizadas

| Componente | Tecnologia | Motivo da escolha |
|---|---|---|
| Linguagem | Python 3.11 | Amplamente adotado para APIs e análise de dados |
| Framework | FastAPI | Moderno, rápido e gera documentação automaticamente |
| Servidor ASGI | Uvicorn via Gunicorn | Padrão de produção para apps Python assíncronos |
| Hospedagem | Azure App Service F1 (gratuito) | Suficiente para estudo e prototipagem |
| Sistema operacional | Linux | Ambiente padrão de servidores em nuvem |

> ⚠️ **Limitação do plano gratuito (F1):** O plano F1 tem limite de 60 minutos de CPU por dia e não suporta domínios personalizados. Para projetos em produção, use o plano B1 ou superior.

---

## 2. Pré-requisitos

Você precisa de apenas um item antes de começar:

- **Conta Azure for Students** ativa — acesse [portal.azure.com](https://portal.azure.com) e faça login.

Nenhuma ferramenta local é necessária. Não há instalação de Python, VS Code ou CLI.

---

## 3. Estrutura dos Arquivos

Todos os arquivos da aplicação ficam em `/home/minha-api/` dentro do servidor. Esse diretório é **persistente** — ele sobrevive a reinicializações do container, ao contrário de `/home/site/wwwroot/`, que pode ser sobrescrito por deploys.

```
/home/minha-api/
    main.py            # Aplicação FastAPI (rotas e modelos de dados)
    requirements.txt   # Dependências Python
    startup.sh         # Script executado na inicialização do serviço
```

Entenda o papel de cada arquivo antes de criá-los — isso evita confusão durante o deploy:

| Arquivo | Responsabilidade |
|---|---|
| `main.py` | Contém toda a lógica da API: modelos de dados, rotas HTTP e regras de negócio. |
| `requirements.txt` | Lista as bibliotecas que precisam ser instaladas. O `startup.sh` lê esse arquivo na inicialização. |
| `startup.sh` | Script de entrada do serviço. Instala as dependências e sobe o servidor Gunicorn. |

---

## 4. Criando os Recursos no Azure

Esta etapa é feita inteiramente pelo portal do Azure, sem digitar nenhum comando. Siga as subseções em ordem.

### 4.1 Grupo de Recursos

Um grupo de recursos é como uma **pasta no Azure**: agrupa tudo que pertence ao seu projeto, facilitando gerenciar e excluir ao final.

1. No portal, pesquise por **Grupos de recursos** na barra de busca e abra o serviço.
2. Clique em **Criar**.
3. Preencha os campos:
   - **Nome do grupo de recursos:** `rg-fastapi-demo`
   - **Região:** Central US
4. Clique em **Examinar + criar** e depois em **Criar**.

### 4.2 Plano do Serviço de Aplicativo

O plano define o servidor (CPU, memória, sistema operacional) que vai executar sua aplicação. Vamos usar o plano gratuito F1.

1. Pesquise por **Planos do Serviço de Aplicativo** e abra.
2. Clique em **Criar**.
3. Preencha os campos:
   - **Grupo de recursos:** `rg-fastapi-demo`
   - **Nome:** `plan-fastapi-free`
   - **Sistema Operacional:** Linux
   - **Região:** Central US
   - **Tipo de preço:** F1 (Gratuito)
4. Clique em **Examinar + criar** e depois em **Criar**.

### 4.3 Web App (Aplicativo Web)

O Web App é o serviço que efetivamente executa o seu código. Ele usa o plano criado no passo anterior.

1. Pesquise por **Serviços de Aplicativos** e abra.
2. Clique em **Criar** > **Aplicativo Web**.
3. Preencha os campos:

   | Campo | Valor |
   |---|---|
   | Grupo de recursos | `rg-fastapi-demo` |
   | Nome | `fastapi-demo-SEUNOME` (deve ser único no Azure) |
   | Publicar | Código |
   | Pilha de runtime | Python 3.11 |
   | Sistema Operacional | Linux |
   | Região | Central US |
   | Plano do Serviço de Aplicativo | `plan-fastapi-free (F1)` |

4. Clique em **Examinar + criar** e depois em **Criar**.
5. Quando o deploy finalizar, clique em **Ir para o recurso**.

> 💡 **Dica — Nome único:** O nome do Web App vira parte da URL pública (ex.: `fastapi-demo-roni.azurewebsites.net`). Se o nome já estiver em uso, o portal avisa com um X vermelho — experimente adicionar suas iniciais ou um número.

### 4.4 Abrindo o Terminal SSH

O Azure oferece um terminal integrado ao navegador, que abre uma sessão diretamente dentro do container que roda sua aplicação.

1. Na página do Web App, expanda **Ferramentas de Desenvolvimento** no menu lateral.
2. Clique em **SSH** e depois em **Ir**.
3. Aguarde alguns segundos — uma janela de terminal será aberta no próprio navegador.

> 💡 **O que é esse terminal?** Você está dentro do Linux que roda sua aplicação na nuvem. Tudo que você criar em `/home` persiste. É como ter acesso SSH ao servidor, mas sem precisar de chave ou cliente externo.

---

## 5. Escrevendo a Aplicação via SSH

Com o terminal aberto, execute os comandos abaixo em ordem. Cada subseção explica o que o comando faz antes de você digitá-lo.

### 5.1 Criando o Diretório de Trabalho

Criamos a pasta onde todos os arquivos vão ficar. O parâmetro `-p` garante que a pasta seja criada mesmo que já exista.

```bash
mkdir -p /home/minha-api
cd /home/minha-api
```

### 5.2 Criando o `requirements.txt`

Este arquivo lista as bibliotecas que a aplicação precisa. O comando `cat` com heredoc (`EOF`) é uma forma de criar arquivos de texto diretamente no terminal, sem editor.

```bash
cat > /home/minha-api/requirements.txt << 'EOF'
fastapi
uvicorn[standard]
gunicorn
EOF
```

Entendendo cada dependência:

- **`fastapi`** — o framework que permite definir rotas HTTP com Python puro.
- **`uvicorn[standard]`** — servidor ASGI que executa a aplicação de forma assíncrona.
- **`gunicorn`** — gerenciador de processos que cria múltiplos workers do Uvicorn, aumentando a capacidade de atender requisições simultâneas.

### 5.3 Criando o `main.py`

Este é o coração da aplicação. O código está dividido em três blocos lógicos explicados após o comando.

```bash
cat > /home/minha-api/main.py << 'EOF'
from fastapi import FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel
from typing import Optional, List

app = FastAPI(
    title="ToDo API",
    description="API REST de gerenciamento de tarefas no Azure.",
    version="1.0.0"
)

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_methods=["*"],
    allow_headers=["*"],
)


# ---------------------------------------------------------------------------
# Modelos de dados
# ---------------------------------------------------------------------------

class TaskIn(BaseModel):
    title: str
    description: Optional[str] = None
    completed: bool = False


class TaskOut(BaseModel):
    id: int
    title: str
    description: Optional[str] = None
    completed: bool


# ---------------------------------------------------------------------------
# Armazenamento em memória (substitua por um banco de dados em produção)
# ---------------------------------------------------------------------------

store: List[TaskOut] = [
    TaskOut(id=1, title="Estudar FastAPI", description="Aprender design de APIs REST", completed=False),
    TaskOut(id=2, title="Fazer deploy no Azure", description="Usar App Service via SSH", completed=True),
    TaskOut(id=3, title="Escrever documentação", description="README e referência da API", completed=False),
]
next_id = 4


# ---------------------------------------------------------------------------
# Rotas
# ---------------------------------------------------------------------------

@app.get("/", tags=["Saúde"])
def root():
    return {"status": "ok", "docs": "/docs"}


@app.get("/tasks", response_model=List[TaskOut], tags=["Tarefas"])
def list_tasks():
    """Retorna todas as tarefas."""
    return store


@app.get("/tasks/{task_id}", response_model=TaskOut, tags=["Tarefas"])
def get_task(task_id: int):
    """Retorna uma tarefa pelo ID."""
    for task in store:
        if task.id == task_id:
            return task
    raise HTTPException(status_code=404, detail=f"Tarefa {task_id} não encontrada.")


@app.post("/tasks", response_model=TaskOut, status_code=201, tags=["Tarefas"])
def create_task(task: TaskIn):
    """Cria uma nova tarefa."""
    global next_id
    new_task = TaskOut(id=next_id, **task.model_dump())
    store.append(new_task)
    next_id += 1
    return new_task
EOF
```

Entendendo a estrutura do código:

| Bloco | O que faz |
|---|---|
| `app = FastAPI(...)` | Cria a instância da aplicação com metadados que aparecem na documentação automática. |
| `CORSMiddleware` | Permite que navegadores de qualquer origem acessem a API. Em produção, substitua `"*"` pelos domínios autorizados. |
| `TaskIn` / `TaskOut` (Pydantic) | Definem e validam automaticamente o formato dos dados que entram e saem da API. |
| `store` (lista em memória) | Simula um banco de dados. Os dados são perdidos a cada reinicialização do container. |
| Rotas `GET` / `POST` | Cada função decorada com `@app.get` ou `@app.post` define um endpoint HTTP. |

### 5.4 Criando o `startup.sh`

O `startup.sh` é executado toda vez que o Azure inicia seu container. Ele instala as dependências (porque o ambiente é recriado a cada reinicialização) e sobe o servidor.

```bash
cat > /home/minha-api/startup.sh << 'EOF'
#!/bin/bash
echo "[startup] Instalando dependências..."
pip install -r /home/minha-api/requirements.txt --quiet

echo "[startup] Iniciando Gunicorn..."
cd /home/minha-api
exec gunicorn -w 4 -k uvicorn.workers.UvicornWorker main:app --bind 0.0.0.0:8000
EOF

chmod +x /home/minha-api/startup.sh
```

> 💡 **Por que instalar as dependências no `startup.sh`?** O Azure recria o container do zero a cada reinicialização. Pacotes instalados manualmente com `pip` (fora de `/home`) são perdidos. Como `/home` persiste, colocamos o `requirements.txt` lá e sempre reinstalamos na inicialização.

> ⚠️ **O `chmod +x` é obrigatório.** Sem essa permissão de execução, o Azure não consegue rodar o script e a aplicação não sobe.

### 5.5 Verificando os Arquivos

Confirme que os três arquivos foram criados corretamente:

```bash
ls -lh /home/minha-api/
```

Saída esperada:

```
-rwxr-xr-x  startup.sh
-rw-r--r--  main.py
-rw-r--r--  requirements.txt
```

---

## 6. Configurando o Comando de Inicialização

Agora precisamos dizer ao Azure qual script executar quando o container iniciar. Isso é feito pelo portal.

1. Na página do Web App, vá em **Configurações** > **Configuração**.
2. Clique na aba **Configurações gerais**.
3. No campo **Comando de inicialização**, digite exatamente:

   ```
   bash /home/minha-api/startup.sh
   ```

4. Clique em **Salvar** e depois em **Continuar** para confirmar.
5. O aplicativo reinicia automaticamente. Aguarde 30 a 60 segundos.

> ⚠️ **Atenção ao caminho.** O caminho `/home/minha-api/startup.sh` deve ser exato, incluindo a barra inicial. Um erro tipográfico aqui faz o Azure exibir a página padrão em vez da sua API.

---

## 7. Verificando e Testando a API

### 7.1 Encontrando a URL Pública

Na página de **Visão geral** do Web App, copie o valor do campo **Domínio padrão**:

```
https://fastapi-demo-SEUNOME.azurewebsites.net
```

### 7.2 Verificação Básica

Abra a URL no navegador. Se tudo estiver funcionando, você verá:

```json
{"status": "ok", "docs": "/docs"}
```

### 7.3 Documentação Interativa (Swagger UI)

Acesse `/docs` para abrir a interface Swagger gerada automaticamente pelo FastAPI. Ela permite testar todos os endpoints diretamente no navegador — sem instalar nenhum cliente HTTP.

> 💡 **Swagger UI na prática:** Clique em qualquer endpoint, depois em **Try it out** e **Execute**. O FastAPI valida os dados, faz a requisição e exibe a resposta — ótimo para demonstrações em sala.

### 7.4 Testando com `curl`

Você pode testar pelo terminal SSH ou por qualquer terminal com acesso à internet.

**Listar todas as tarefas:**

```bash
GET https://fastapi-demo-SEUNOME.azurewebsites.net/tasks
```

**Buscar uma tarefa pelo ID:**

```bash
GET https://fastapi-demo-SEUNOME.azurewebsites.net/tasks/1
```

**Criar uma nova tarefa:**

```bash
{
    "title": "Minha nova tarefa",
    "description": "Descrição opcional",
    "completed": false
}
```

### 7.5 Monitorando os Logs

No portal, acesse **Fluxo de log** para acompanhar a saída do Gunicorn em tempo real. Uma inicialização bem-sucedida mostra:

```
[startup] Instalando dependências...
[startup] Iniciando Gunicorn...
[INFO] Booting worker with pid: ...
[INFO] Application startup complete.
```

---

## 8. Referência da API

| Método | Endpoint | Descrição |
|---|---|---|
| `GET` | `/` | Health check — confirma que o serviço está no ar. |
| `GET` | `/tasks` | Retorna a lista completa de tarefas. |
| `GET` | `/tasks/{id}` | Retorna uma tarefa específica. Responde 404 se não encontrada. |
| `POST` | `/tasks` | Cria uma nova tarefa. Retorna o objeto criado com status 201. |

### GET /tasks — resposta 200 OK

```json
[
  {
    "id": 1,
    "title": "Estudar FastAPI",
    "description": "Aprender design de APIs REST",
    "completed": false
  }
]
```

### GET /tasks/99 — resposta 404 Not Found

```json
{"detail": "Tarefa 99 não encontrada."}
```

### POST /tasks — corpo da requisição

```json
{
  "title": "Minha nova tarefa",
  "description": "Descrição opcional",
  "completed": false
}
```

### POST /tasks — resposta 201 Created

```json
{
  "id": 4,
  "title": "Minha nova tarefa",
  "description": "Descrição opcional",
  "completed": false
}
```

---

## 9. Resolução de Problemas

### `ModuleNotFoundError: No module named 'uvicorn'`

**Causa:** o container reiniciou e os pacotes instalados manualmente (fora de `/home`) foram perdidos.

**Solução:** verifique se o `startup.sh` realmente executa o `pip install` antes do Gunicorn (seção 5.4) e se o comando de inicialização está configurado corretamente (seção 6).

---

### O Azure exibe a página padrão, não a API

**Causa:** o Startup Command não está configurado ou aponta para um caminho errado.

**Solução:** acesse **Configurações** > **Configuração** > **Configurações gerais** e confirme que o campo **Comando de inicialização** contém exatamente:

```
bash /home/minha-api/startup.sh
```

---

### Alterações feitas via SSH somem após reinicialização

**Causa:** arquivos gravados fora de `/home` não sobrevivem à recriação do container.

**Solução:** sempre salve todos os arquivos da aplicação dentro de `/home/minha-api/`. Esse diretório está em armazenamento persistente.

---

## 10. Limpando os Recursos

Ao terminar o exercício, exclua o grupo de recursos para evitar qualquer cobrança futura:

1. No portal, acesse **Grupos de recursos**.
2. Clique em `rg-fastapi-demo`.
3. Clique em **Excluir grupo de recursos**.
4. Digite o nome do grupo para confirmar.
5. Clique em **Excluir**.

Essa operação remove o Web App, o Plano de Serviço de Aplicativo e todos os recursos associados de uma vez.

---

## 11. Próximos Passos

Este guia usou armazenamento em memória para simplificar. Em uma aplicação real, considere:

- **Persistência de dados:** substitua o `store` por Azure Database for PostgreSQL ou Azure Cosmos DB.
- **Segurança CORS:** restrinja `allow_origins=["*"]` para os domínios específicos do seu front-end.
- **Autenticação:** adicione Azure Active Directory ou OAuth2.
- **Variáveis de ambiente:** configure credenciais pelo portal em vez de valores fixos no código.
- **CI/CD:** use GitHub Actions ou Azure DevOps para automatizar o deploy a cada novo commit.

---

> By Roni Maciel
