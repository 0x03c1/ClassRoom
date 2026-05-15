# Aula 4 — Banco PostgreSQL Flexible Server pelo Portal Azure

> **Tempo estimado:** 2h

---

## Objetivos

Ao final desta aula você terá:

1. Um **Azure Database for PostgreSQL Flexible Server** rodando (Burstable B1ms, versão 16)
2. **Banco** e **usuário** dedicados à aplicação Django (princípio do menor privilégio)
3. **Firewall do servidor** configurado: a VM da Aula 3 + seu PC acessam o banco
4. **Conexão validada** da VM via `psql`
5. Projeto Django local apontando para o PostgreSQL Azure

---

## 1. Por que usar banco gerenciado em vez de instalar PostgreSQL na VM?

| Aspecto                   | PostgreSQL na VM                  | Flexible Server (gerenciado)             |
| ------------------------- | --------------------------------- | ----------------------------------------- |
| Backup automático         | Você configura                    | Pronto, retenção configurável             |
| Patches de segurança      | Você cuida                        | Microsoft cuida automaticamente           |
| Alta disponibilidade      | Manual e trabalhosa               | Toggle de configuração                    |
| Custo inicial             | Mais barato                       | Um pouco mais caro                        |
| Realidade de mercado      | Pouco usado em produção           | Padrão da indústria                       |

> Pedagogicamente: separar o banco da VM ensina **arquitetura em camadas** e **single responsibility**.

> Observação: o **Single Server** do Azure Database for PostgreSQL foi descontinuado pela Microsoft, e novos servidores devem ser criados como **Flexible Server**. Documentação oficial: https://learn.microsoft.com/azure/postgresql/flexible-server/quickstart-create-server

---

## 2. Criando o Flexible Server pelo Portal

### 2.1. Abra o wizard de criação

1. Acesse https://portal.azure.com
2. Na barra de busca superior, digite **Azure Database for PostgreSQL flexible servers**
3. Clique no resultado correspondente sob a categoria **Services**
4. Na página de Flexible Servers, clique em **+ Create**
5. Aparece **Select Azure Database for PostgreSQL deployment option**. Escolha **Flexible server** e clique em **Create**

### 2.2. Aba "Basics"

#### Project details

| Campo            | Valor                          |
| ---------------- | ------------------------------ |
| Subscription     | Azure for Students             |
| Resource group   | `rg-django-azure-pratica`      |

#### Server details

| Campo                   | Valor                                       |
| ----------------------- | ------------------------------------------- |
| Server name             | `psql-django-pratica-<sufixo-unico>`        |
| Region                  | `Brazil South`                              |
| PostgreSQL version      | `16`                                        |
| Workload type           | `Development`                               |
| Compute + storage       | (Clique em **Configure server** — veja 2.3) |
| Availability zone       | `No preference`                              |
| Enable high availability| **Desmarcado** (alto custo, desnecessário para aula) |

> O **Server name** precisa ser **globalmente único** dentro de `*.postgres.database.azure.com`. Use seu nome ou matrícula como sufixo, ex.: `psql-django-pratica-roni2026`. Permitido: apenas letras minúsculas, números e hífen. Entre 3 e 63 caracteres.

#### Authentication

| Campo                       | Valor                              |
| --------------------------- | ---------------------------------- |
| Authentication method       | `PostgreSQL authentication only`   |
| Admin username              | `pgadmin`                          |
| Password                    | (gere uma senha forte e guarde)   |
| Confirm password            | (mesma senha)                      |

> **Regras de senha (validadas pela Microsoft):** entre 8 e 128 caracteres, contendo caracteres de pelo menos 3 das 4 categorias: maiúsculas inglesas, minúsculas inglesas, números, caracteres especiais (`!`, `$`, `#`, `%`, etc.). Não pode ser `azure_superuser`, `azure_pg_admin`, `admin`, `administrator`, `root`, `guest`, `public`, nem começar com `pg_`.
>
> Guarde a senha no seu **gerenciador de senhas** (Bitwarden, 1Password, KeePass). Anote como **senha do admin pgadmin** — diferente da senha que vamos criar para o usuário da aplicação.

### 2.3. Configurando Compute + Storage

Clicando em **Configure server** abre uma página lateral:

| Campo                            | Valor                                  |
| -------------------------------- | -------------------------------------- |
| Compute tier                     | `Burstable`                            |
| Compute size                     | `Standard_B1ms - 1 vCore, 2 GiB RAM`   |
| Storage type                     | `Premium SSD V2` ou `Premium SSD`      |
| Storage size                     | `32 GiB` (mínimo)                      |
| Storage autogrow                 | Marcado                                |
| Backup retention period          | `7 days`                               |
| Backup redundancy                | `Locally-redundant backup storage`     |

Clique em **Save** ao final da página lateral.

### 2.4. Aba "Networking"

Clique em **Next: Networking >**.

| Campo                                                | Valor                                |
| ---------------------------------------------------- | ------------------------------------ |
| Connectivity method                                  | `Public access (allowed IP addresses)` |
| Allow public access from any Azure service within Azure | **Marcado**                       |
| Add current client IP address                        | **Clique no botão**                  |

> **Public access** é mais simples para esta aula. Em produção real, prefira **Private access (VNet integration)**, que põe o banco dentro de uma VNet e bloqueia o IP público completamente.

> A opção **Allow public access from any Azure service within Azure** libera acesso de qualquer recurso Azure usando autenticação. Para esta aula é aceitável. Em produção, prefira liberar apenas o IP da sua VM.

Você pode adicionar regras de firewall específicas agora ou depois. Vamos adicionar agora:

1. Confirme que o **seu IP atual** foi adicionado automaticamente (quando você clicou em **Add current client IP**)
2. Adicione manualmente o IP da VM da Aula 3:
   - **Firewall rule name:** `vm-django-app`
   - **Start IP:** `<IP_VM>` (mesmo IP de início e fim)
   - **End IP:** `<IP_VM>`

### 2.5. Demais abas

Avance por **Security**, **Tags** e deixe os padrões.

### 2.6. Review + create

1. Clique em **Review + create**
2. Aguarde a validação aparecer **Validation passed**
3. Clique em **Create**

> O provisionamento demora entre **5 e 10 minutos**.

Quando aparecer **Your deployment is complete**, clique em **Go to resource**.

---

## 3. Anote o hostname do servidor

Na página do servidor, no painel **Essentials**:

- Copie o **Server name** completo, com o sufixo `.postgres.database.azure.com`
- Exemplo: `psql-django-pratica-roni2026.postgres.database.azure.com`

Vamos chamar de `<PG_HOST>` daqui em diante.

---

## 4. Criando o banco da aplicação pelo Portal

> Princípio do menor privilégio: o Django **não** usa o usuário admin. Vamos criar:
> - Um banco chamado `django_pratica`
> - Um usuário `django_app` que só tem acesso a esse banco

### 4.1. Criando o database pelo Portal

1. Na página do servidor Flexible Server, no menu lateral, em **Settings**, clique em **Databases**
2. Clique em **+ Add**
3. Preencha:
   - **Database name:** `django_pratica`
   - **Character set:** `UTF8`
   - **Collation:** `en_US.utf8`
4. Clique em **Save**

> O Portal só permite criar o banco — para criar **usuários** e dar **permissões** é necessário rodar SQL. Vamos fazer isso da VM, que já tem `psql` instalado da Aula 3.

### 4.2. Conectando à VM e criando o usuário da aplicação

Da sua máquina local:

```bash
ssh vm-django
```

Dentro da VM, conecte ao banco como admin (substitua os valores):

```bash
psql "host=<PG_HOST> port=5432 user=pgadmin dbname=postgres sslmode=require"
```

Quando pedir a senha, digite a senha do admin que você criou no passo 2.2.

> Conexões ao Flexible Server usam **porta 5432** por padrão e **exigem SSL** (parâmetro `sslmode=require`). Documentação: https://learn.microsoft.com/azure/postgresql/flexible-server/concepts-networking-public

Dentro do prompt do `psql`, rode:

```sql
-- Cria o usuário da aplicação (escolha uma senha forte diferente da do admin)
CREATE USER django_app WITH PASSWORD 'TrocarPorSenh@Forte5678!';

-- Dá ownership do banco para o novo usuário
ALTER DATABASE django_pratica OWNER TO django_app;
GRANT ALL PRIVILEGES ON DATABASE django_pratica TO django_app;

-- Conecta ao banco e dá permissão no schema public (necessário em PostgreSQL 15+)
\c django_pratica
GRANT ALL ON SCHEMA public TO django_app;

\q
```

> Em PostgreSQL 15 e superior, o `GRANT ON DATABASE` **não** concede mais permissões no schema `public` automaticamente. Por isso o `GRANT ALL ON SCHEMA public` é obrigatório.

### 4.3. Validando como o usuário da aplicação

Ainda dentro da VM:

```bash
psql "host=<PG_HOST> port=5432 user=django_app dbname=django_pratica password=TrocarPorSenh@Forte5678! sslmode=require"
```

Dentro do prompt:

```sql
SELECT current_user, current_database(), version();
\q
```

Se conectar e mostrar `django_app`, `django_pratica` e a versão `PostgreSQL 16.x`, o usuário está configurado corretamente.

---

## 5. Conectando o Django local ao PostgreSQL Azure

> Antes de continuar, confirme no Portal que o **seu IP atual** está na lista de firewall do servidor PostgreSQL (passo 2.4). Se trocou de rede ou seu IP mudou, adicione-o novamente.

### 5.1. Edite o `.env` local

```bash
SECRET_KEY=sua-secret-key
DEBUG=True
ALLOWED_HOSTS=127.0.0.1,localhost

DB_ENGINE=django.db.backends.postgresql
DB_NAME=django_pratica
DB_USER=django_app
DB_PASSWORD=TrocarPorSenh@Forte5678!
DB_HOST=<PG_HOST>
DB_PORT=5432
DB_SSLMODE=require
```

### 5.2. Rode as migrações

No projeto local, com o venv ativo:

```bash
python manage.py migrate
```

Saída esperada:

```
Operations to perform:
  Apply all migrations: admin, auth, contenttypes, sessions, tarefas
Running migrations:
  Applying contenttypes.0001_initial... OK
  Applying auth.0001_initial... OK
  ...
  Applying tarefas.0001_initial... OK
```

As tabelas foram criadas no banco Azure, não no SQLite local.

### 5.3. Confirme no banco

Da sua VM (ou do seu PC com `psql` instalado):

```bash
psql "host=<PG_HOST> port=5432 user=django_app dbname=django_pratica sslmode=require" -c "\dt"
```

Deve listar tabelas: `auth_user`, `tarefas_tarefa`, `django_migrations`, etc.

### 5.4. Crie um superuser e teste

```bash
python manage.py createsuperuser
python manage.py runserver
```

Acesse http://127.0.0.1:8000 — qualquer tarefa criada agora persiste no banco Azure.

---

## 6. Boas práticas de credenciais

**Não faça:**

- Colar senha no chat, e-mail ou código no Git
- Usar a mesma senha para admin e app
- Hardcodar string de conexão no código

**Faça:**

- Senhas em gerenciador (Bitwarden, 1Password, KeePass)
- Senha do admin diferente da senha do usuário da app
- Todas as credenciais via `.env` localmente, **GitHub Secrets** no CI/CD, **Azure Key Vault** em produção corporativa

---

## Checkpoint

Da sua máquina local:

1. Inicie `python manage.py runserver`
2. Acesse http://127.0.0.1:8000
3. Crie 3 tarefas pela interface
4. Pare o servidor (`Ctrl+C`)
5. Inicie novamente
6. As 3 tarefas ainda devem estar lá

Confirmação direta no banco:

```bash
psql "host=<PG_HOST> port=5432 user=django_app dbname=django_pratica sslmode=require" \
  -c "SELECT COUNT(*) FROM tarefas_tarefa;"
```

Deve mostrar `3`. Se sim, pode seguir para a Aula 5.

---

## Troubleshooting

### `FATAL: password authentication failed for user "django_app"`
- Senha errada no `.env`. Confira contra o que foi setado em `CREATE USER`.

### `FATAL: no pg_hba.conf entry for host "X.X.X.X"`
- O IP de onde você está conectando não está nas regras de firewall do servidor PostgreSQL.
- Portal > seu servidor PostgreSQL > **Networking** > **Firewall rules** > adicione o IP.

### `could not connect to server: Connection timed out`
- O servidor pode estar com **Private access** ativo (não é o nosso caso) ou pausado.
- Confirme em **Overview** que o status é **Available**.

### `permission denied for schema public` ao migrar
- Faltou o `GRANT ALL ON SCHEMA public TO django_app;` no banco da aplicação.
- Reconecte como admin, faça `\c django_pratica`, rode o GRANT.

### Lentidão de Recife para o banco
- Confirme que o servidor está em `Brazil South`, não em `East US`.

---

## Desafio extra (conecta com sua disciplina de Banco de Dados)

1. Crie uma tabela `auditoria_tarefa` no PostgreSQL com colunas `id`, `tarefa_id`, `operacao` (`INSERT`, `UPDATE`, `DELETE`) e `timestamp`
2. Crie uma função PL/pgSQL e um **TRIGGER** que insira na auditoria sempre que houver alteração em `tarefas_tarefa`
3. Adicione na view Django uma seção "últimas 5 ações" lendo da tabela de auditoria

---

## Gestão de custos

Para **pausar o servidor PostgreSQL** quando não estiver usando:

1. Portal > seu servidor > **Overview**
2. Clique em **Stop** no topo
3. Confirme

Servidor parado custa apenas o storage. Para retomar, clique em **Start**.

> Atenção: a Microsoft **religa automaticamente** o servidor após 7 dias parado, como medida de segurança e para aplicar patches. Não conte com ele permanentemente parado.

---

**Próxima aula:** [Aula 5 — Deploy manual com Gunicorn + Nginx + systemd](../aula5/README.md)
