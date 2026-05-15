# Aula 7 — CI/CD com GitHub Actions

> **Tempo estimado:** 2h

---

## Objetivos

Ao final desta aula você terá:

1. Um workflow do **GitHub Actions** rodando a cada `push` na `main`
2. **Fase de CI** rodando `flake8` (lint) e testes Django (`manage.py test`)
3. **Fase de CD** que conecta na VM via SSH e executa o `redeploy.sh`
4. **Secrets** armazenados de forma segura no GitHub
5. Badge de status do build no README do projeto

---

## 1. Visão geral do pipeline

```
   git push origin main
         |
         v
   GitHub Actions
         |
   +-----+---------------------+
   | Job: test                 |
   |   - flake8                |
   |   - manage.py test        |
   |   (PostgreSQL service)    |
   +---------+-----------------+
             |
             v se test passou
   +---------+-----------------+
   | Job: deploy               |
   |   - SSH na VM             |
   |   - executa redeploy.sh   |
   |   - health check          |
   +---------+-----------------+
             |
             v
       VM atualizada
```

---

## 2. Preparando a VM para receber deploy automático

O script `redeploy.sh` já existe desde a Aula 5. Vamos só permitir que ele rode via SSH **não-interativo** (sem prompt de senha do `sudo`).

### 2.1. SSH na VM e ajuste do sudoers

```bash
ssh vm-django
sudo visudo -f /etc/sudoers.d/deploy-azureuser
```

Cole exatamente o conteúdo abaixo (caminhos absolutos importam):

```
azureuser ALL=(ALL) NOPASSWD: /usr/bin/chown -R azureuser\:www-data /var/www/django/app
azureuser ALL=(ALL) NOPASSWD: /usr/bin/chown -R www-data\:www-data /var/www/django/app
azureuser ALL=(ALL) NOPASSWD: /bin/chmod 640 /var/www/django/app/.env
azureuser ALL=(ALL) NOPASSWD: /bin/systemctl restart gunicorn
```

Salve (`Ctrl+O`, Enter, `Ctrl+X`). O `visudo` valida a sintaxe antes de salvar — se houver erro, ele avisa.

> Algumas instalações Ubuntu têm `chmod` em `/usr/bin/chmod` em vez de `/bin/chmod`. Confirme com:
>
> ```bash
> which chown chmod systemctl
> ```
>
> Ajuste o `sudoers.d/deploy-azureuser` para o caminho correto encontrado.

Teste sem senha:

```bash
sudo -n systemctl restart gunicorn
```

Se rodar sem pedir senha, está OK.

---

## 3. Gerando uma chave SSH dedicada para o CI/CD

**Nunca use a sua chave pessoal no GitHub Actions.** Crie uma chave **dedicada para deploy** — se vazar, o impacto é limitado a essa VM e fácil de revogar.

### 3.1. Na sua máquina local

```bash
ssh-keygen -t ed25519 -f ~/.ssh/azure_deploy_key -C "github-actions-deploy" -N ""
```

> A flag `-N ""` cria a chave **sem passphrase** — necessário para automação.

Dois arquivos são criados:

- `~/.ssh/azure_deploy_key` (privada — vai pro **GitHub Secrets**)
- `~/.ssh/azure_deploy_key.pub` (pública — vai pra **VM**)

### 3.2. Adicione a chave pública na VM

```bash
ssh vm-django "echo '$(cat ~/.ssh/azure_deploy_key.pub)' >> ~/.ssh/authorized_keys"
```

> Se o comando acima der erro de quoting (Windows), faça em duas etapas: rode `cat ~/.ssh/azure_deploy_key.pub` localmente, copie a saída, depois `ssh vm-django`, depois `nano ~/.ssh/authorized_keys` e cole no fim do arquivo.

Teste:

```bash
ssh -i ~/.ssh/azure_deploy_key azureuser@<IP_VM> 'whoami'
# Saída esperada: azureuser
```

---

## 4. Configurando GitHub Secrets

No GitHub:

1. Acesse seu repo `django-azure-pratica`
2. **Settings > Secrets and variables > Actions > New repository secret**

Crie estes 4 secrets:

| Nome                 | Valor                                                                                  |
| -------------------- | -------------------------------------------------------------------------------------- |
| `AZURE_VM_HOST`      | `<IP_VM>` (ou `<SEU_DOMINIO>` da Aula 6)                                               |
| `AZURE_VM_USER`      | `azureuser`                                                                            |
| `AZURE_VM_PORT`      | `22`                                                                                   |
| `AZURE_VM_SSH_KEY`   | Conteúdo completo de `~/.ssh/azure_deploy_key` (privada, **com** o `-----BEGIN` e `-----END`) |

> Para copiar a chave privada inteira:
>
> - **Linux:** `cat ~/.ssh/azure_deploy_key | xclip -selection clipboard`
> - **macOS:** `cat ~/.ssh/azure_deploy_key | pbcopy`
> - **Windows PowerShell:** `Get-Content $env:USERPROFILE\.ssh\azure_deploy_key | Set-Clipboard`
>
> Cole no campo **Value** do secret e clique em **Add secret**.

---

## 5. Criando o workflow

No projeto local, crie a estrutura:

```bash
mkdir -p .github/workflows
```

### 5.1. Arquivo `.github/workflows/ci-cd.yml`

```yaml
name: CI-CD - Test and Deploy

on:
  push:
    branches: [main]
  pull_request:
    branches: [main]

jobs:
  test:
    name: Test
    runs-on: ubuntu-latest

    services:
      postgres:
        image: postgres:16
        env:
          POSTGRES_USER: testuser
          POSTGRES_PASSWORD: testpass
          POSTGRES_DB: testdb
        ports:
          - 5432:5432
        options: >-
          --health-cmd pg_isready
          --health-interval 10s
          --health-timeout 5s
          --health-retries 5

    steps:
      - name: Checkout
        uses: actions/checkout@v4

      - name: Set up Python
        uses: actions/setup-python@v5
        with:
          python-version: '3.12'
          cache: 'pip'

      - name: Install dependencies
        run: |
          python -m pip install --upgrade pip
          pip install -r requirements.txt
          pip install flake8

      - name: Lint with flake8
        run: |
          flake8 . --count --select=E9,F63,F7,F82 --show-source --statistics
          flake8 . --count --max-complexity=10 --max-line-length=120 --statistics --exit-zero

      - name: Run Django tests
        env:
          SECRET_KEY: test-secret-key-not-for-production
          DEBUG: 'False'
          ALLOWED_HOSTS: 127.0.0.1,localhost
          DB_ENGINE: django.db.backends.postgresql
          DB_NAME: testdb
          DB_USER: testuser
          DB_PASSWORD: testpass
          DB_HOST: 127.0.0.1
          DB_PORT: 5432
          DB_SSLMODE: disable
        run: |
          python manage.py migrate
          python manage.py test

  deploy:
    name: Deploy
    needs: test
    if: github.event_name == 'push' && github.ref == 'refs/heads/main'
    runs-on: ubuntu-latest

    steps:
      - name: Deploy via SSH
        uses: appleboy/ssh-action@v1
        with:
          host: ${{ secrets.AZURE_VM_HOST }}
          username: ${{ secrets.AZURE_VM_USER }}
          port: ${{ secrets.AZURE_VM_PORT }}
          key: ${{ secrets.AZURE_VM_SSH_KEY }}
          script_stop: true
          script: |
            bash ~/redeploy.sh

      - name: Health check
        run: |
          sleep 5
          curl -fsS https://${{ secrets.AZURE_VM_HOST }}/health/ \
            || curl -fsS http://${{ secrets.AZURE_VM_HOST }}/health/
```

> A action `appleboy/ssh-action@v1` é a versão estável (1.x) e a mais usada para SSH em pipelines. Documentação: https://github.com/appleboy/ssh-action

### 5.2. Ajuste do `DB_SSLMODE` no settings

O job de teste usa um Postgres local sem SSL, por isso passamos `DB_SSLMODE=disable`. Confirme que o `core/settings.py` (Aula 2) honra esse valor:

```python
if config('DB_ENGINE', default='').endswith('postgresql'):
    DATABASES['default']['OPTIONS'] = {
        'sslmode': config('DB_SSLMODE', default='require'),
    }
```

### 5.3. Commit e push

```bash
git add .github/workflows/ci-cd.yml
git commit -m "ci: pipeline GitHub Actions com test e deploy"
git push origin main
```

---

## 6. Acompanhando a primeira execução

1. Vá no GitHub > aba **Actions**
2. Clique no workflow em execução
3. Acompanhe os logs em tempo real

Cenário ideal:

```
Test          OK   1m 30s
Deploy        OK   45s
```

Testando o ciclo completo: faça uma mudança no template (ex. mude o `<h1>` para `Minhas Tarefas (v2)`) e push:

```bash
git commit -am "chore: atualiza titulo"
git push
```

Aguarde ~2 minutos e confirme:

```bash
curl https://<SEU_DOMINIO>/ | grep "v2"
```

Deploy automatizado funcionando.

---

## 7. Badge no README

No README principal do projeto, adicione abaixo do título:

```markdown
![CI-CD](https://github.com/<seu-usuario>/django-azure-pratica/actions/workflows/ci-cd.yml/badge.svg)
```

Vai mostrar verde quando passar e vermelho quando falhar.

---

## 8. Branch protection (avançado, recomendado)

Para garantir que ninguém faça push direto na `main` sem passar pelos testes:

1. GitHub > repo > **Settings > Branches > Add classic branch protection rule** (ou **Add ruleset** em interfaces mais recentes)
2. **Branch name pattern:** `main`
3. Marque:
   - **Require a pull request before merging**
   - **Require status checks to pass before merging**
   - Em **Status checks**, busque e adicione o check **Test**
4. **Create** / **Save changes**

Agora você trabalha em branches separadas, abre PR, espera o CI passar, e só depois faz merge.

```bash
git checkout -b feature/nova-funcionalidade
# edita arquivos
git push -u origin feature/nova-funcionalidade
# abre Pull Request no GitHub
```

---

## Checkpoint

- [ ] A aba **Actions** mostra o workflow rodando após cada push em `main`
- [ ] O job `test` passa (verde)
- [ ] O job `deploy` passa (verde)
- [ ] Modificar o template e dar push reflete em produção em ~2 minutos
- [ ] Quebrar um teste (mude uma asserção propositalmente) faz o job `deploy` **não rodar** — esse é o ponto mais importante
- [ ] Badge no README aparece verde

---

## Troubleshooting

### `Permission denied (publickey)` no passo de deploy
- Confira se a chave **privada completa** está no secret `AZURE_VM_SSH_KEY` (com `-----BEGIN OPENSSH PRIVATE KEY-----` e `-----END OPENSSH PRIVATE KEY-----`)
- Confirme que a chave **pública** correspondente está em `~/.ssh/authorized_keys` da VM
- Teste local: `ssh -i ~/.ssh/azure_deploy_key azureuser@<IP_VM>`

### `sudo: a terminal is required to read the password`
- Faltou ajustar `/etc/sudoers.d/deploy-azureuser` (passo 2.1)
- O caminho do binário no sudoers tem que bater **exatamente** com o que o script chama. Use `which` para conferir.

### Testes falham com "connection refused" no PostgreSQL do CI
- O service container do Postgres demora pra subir. Confirme `health-retries: 5` no YAML.
- Em regiões com latência maior, aumente para `health-retries: 10`.

### Deploy rodou mas a página não mudou
- Cache do navegador. Faça hard reload (`Ctrl+Shift+R`)
- Confirme que o git pull rodou: olhe os logs do step **Deploy via SSH**
- SSH na VM e confira: `cd /var/www/django/app && git log -1`

### `actions/setup-python@v5: Error: Version 3.12 was not found`
- Versão muito específica não encontrada. Mude para `python-version: '3.12'` (já está assim no exemplo) ou `'3.x'`.

---

## Desafios extras

1. **Health check com retry:** modifique o step "Health check" para tentar até 5x com 10s de intervalo, evitando falsos negativos.
2. **Environments com aprovação:** crie um Environment "production" no GitHub (Settings > Environments) que exige aprovação manual de um revisor antes do deploy.
3. **Rollback automático:** se o `/health/` retornar erro 3 vezes seguidas após deploy, faça o pipeline executar `git reset --hard HEAD~1` na VM.
4. **Notificação no Slack/Discord:** ao final do job de deploy, envie uma mensagem via webhook indicando sucesso/falha.

---

**Próxima aula:** [Aula 8 — Logs, monitoramento e troubleshooting](../aula8/README.md)
