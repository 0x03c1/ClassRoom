# Aula 1 — Setup e Ambiente Local

> **Tempo estimado:** 2h

---

## Objetivos

Ao final desta aula você terá:

1. Uma conta **Azure for Students** ativa com US$100 de crédito
2. **Git**, **Python 3.12** e **VS Code** instalados
3. Cliente **SSH** disponível no seu sistema (já vem por padrão no Windows 10/11, macOS e Linux)
4. Conta no **GitHub** com chave SSH cadastrada para autenticação

> **Importante:** esta aula prática **não usa o Azure CLI**. Todo o provisionamento será feito pelo navegador, no Portal Azure. Você não precisa instalar nada da Microsoft localmente.

---

## 1. Ativando a Azure for Students

A oferta dá US$100 de crédito válidos por 12 meses, sem necessidade de cartão de crédito, mediante verificação por e-mail institucional.

1. Acesse https://azure.microsoft.com/free/students
2. Clique em **Activate now** (ou **Comece grátis**)
3. Faça login com seu e-mail institucional (`.edu.br` ou domínio reconhecido pela instituição)
4. Complete a verificação (telefone via SMS)
5. Aceite os termos

> Se você já tiver uma assinatura Azure pessoal/profissional ativa, o sistema vai oferecer **adicionar uma subscription de estudante** à sua conta existente.

**Verificação:**
- Acesse https://portal.azure.com
- Clique no seu avatar (canto superior direito) e confirme que a subscription **Azure for Students** aparece
- Em **Subscriptions** confirme que a subscription está com status **Active**

> Se o e-mail institucional não for reconhecido pelo verificador automático (SheerID), peça ao coordenador para validar manualmente, ou tente o domínio alternativo da instituição.

---

## 2. Instalando ferramentas locais

### 2.1. Git

**Windows:** baixe e instale https://git-scm.com/download/win — aceite todas as opções padrão. O instalador inclui o **Git Bash**, que é onde você vai rodar comandos Unix-like.

**macOS:**
```bash
xcode-select --install
```

**Linux (Ubuntu/Debian):**
```bash
sudo apt update && sudo apt install -y git
```

**Verificação:**
```bash
git --version
# Espera-se: git version 2.4x.x ou superior
```

Configure seu nome, e-mail e a branch padrão (rode uma vez na vida):
```bash
git config --global user.name "Seu Nome"
git config --global user.email "seu.email@exemplo.com"
git config --global init.defaultBranch main
```

### 2.2. Python 3.12 ou superior

**Windows:** baixe em https://www.python.org/downloads/. Na primeira tela do instalador, **marque a caixa "Add Python to PATH"** antes de clicar em Install.

**macOS:**
```bash
brew install python@3.12
```

**Linux:**
```bash
sudo apt install -y python3.12 python3.12-venv python3-pip
```

**Verificação:**
```bash
python --version    # Windows
python3 --version   # macOS / Linux
# Espera-se: Python 3.12.x
```

### 2.3. VS Code

Baixe em https://code.visualstudio.com/. Extensões recomendadas (Ctrl+Shift+X dentro do VS Code):

- `ms-python.python`
- `ms-vscode-remote.remote-ssh`
- `github.vscode-github-actions`

---

## 3. Cliente SSH

O cliente SSH (`ssh`, `ssh-keygen`) **já vem pré-instalado** em:

- Windows 10 versão 1809+ e Windows 11 (OpenSSH Client embutido)
- macOS (qualquer versão recente)
- Distribuições Linux modernas

**Verificação:**
```bash
ssh -V
# Exemplo de saída: OpenSSH_9.x ou similar
```

Se o comando não for reconhecido no Windows: vá em **Configurações > Apps > Recursos opcionais > Adicionar um recurso opcional** e instale **Cliente OpenSSH**.

---

## 4. Gerando uma chave SSH

> **Decisão pedagógica:** vamos gerar a chave SSH **localmente** agora. Na Aula 3, ao criar a VM no Portal Azure, você terá duas opções:
> - Usar esta chave que você gerou agora (recomendado para portfólio profissional)
> - Pedir para o Azure gerar um novo par de chaves (a Microsoft permite e simplifica)
>
> Vamos seguir com a sua chave própria, gerada agora, porque é a mesma que você cadastrará no GitHub.

Rode:
```bash
ssh-keygen -t ed25519 -C "seu.email@exemplo.com"
```

Pressione Enter três vezes (aceitar localização padrão `~/.ssh/id_ed25519`, sem passphrase para começar).

Dois arquivos são gerados:

| Arquivo                  | O que é          | Pode compartilhar? |
| ------------------------ | ---------------- | ------------------ |
| `~/.ssh/id_ed25519`      | Chave privada    | **Nunca**          |
| `~/.ssh/id_ed25519.pub`  | Chave pública    | Sim                |

> A chave privada equivale à senha-mestre de todos os servidores onde a pública estiver cadastrada. Trate com o mesmo cuidado que sua senha bancária.

**Veja sua chave pública** (vamos precisar dela):

```bash
# Linux / macOS / Git Bash no Windows
cat ~/.ssh/id_ed25519.pub

# PowerShell (Windows)
Get-Content $env:USERPROFILE\.ssh\id_ed25519.pub
```

A saída se parece com:
```
ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAA... seu.email@exemplo.com
```

Copie esse texto **inteiro**. Você vai colá-lo no GitHub agora e na Azure na Aula 3.

---

## 5. Conectando ao GitHub

### 5.1. Conta GitHub

Crie em https://github.com/signup se ainda não tem. Recomendado ativar 2FA em **Settings > Password and authentication**.

### 5.2. Cadastrando a chave SSH no GitHub

1. Acesse https://github.com/settings/keys
2. Clique em **New SSH key**
3. **Title:** algo identificável (ex.: `Notebook Senac`)
4. **Key type:** Authentication Key
5. **Key:** cole o conteúdo do `id_ed25519.pub`
6. Clique em **Add SSH key**

### 5.3. Teste a conexão

```bash
ssh -T git@github.com
```

Na primeira vez, responda `yes` à pergunta sobre o fingerprint. Resposta esperada:

```
Hi <seu-usuario>! You've successfully authenticated, but GitHub does not provide shell access.
```

---

## 6. Acesso ao Portal Azure

1. Acesse https://portal.azure.com
2. Em **Subscriptions**, confirme que aparece **Azure for Students**
3. Anote o **Subscription ID** — vai aparecer várias vezes no curso
4. Familiarize-se com os elementos da interface:
   - Barra de busca superior (procura recursos e serviços)
   - Menu lateral à esquerda (atalhos para recursos)
   - **Resource groups** — onde organizamos tudo
   - **Cost Management + Billing** — onde acompanhamos o crédito

---

## 7. Verificação final

Rode os comandos abaixo. Todos devem retornar a versão sem erro:

```bash
git --version
python --version       # ou python3 --version
ssh -V
code --version
```

Confirme também:

- [ ] Portal Azure abre em https://portal.azure.com e mostra a subscription **Azure for Students**
- [ ] `cat ~/.ssh/id_ed25519.pub` mostra sua chave pública (Linux/macOS/Git Bash) ou `Get-Content` mostra (PowerShell)
- [ ] `ssh -T git@github.com` autentica com sucesso

---

## Checkpoint

Execute, na ordem:

```bash
git clone git@github.com:torvalds/linux.git --depth=1 /tmp/teste-clone
rm -rf /tmp/teste-clone
```

> No Windows substitua `/tmp/teste-clone` por `C:\Temp\teste-clone` ou rode no Git Bash.

Se o `git clone` iniciar (pode demorar, é repositório grande), está pronto para a Aula 2.

---

## Troubleshooting

### "ssh-keygen não é reconhecido como comando" no Windows
Vá em **Configurações > Apps > Recursos opcionais > Adicionar um recurso opcional** e instale **Cliente OpenSSH**.

### Push para o GitHub pede usuário e senha
Você não configurou SSH corretamente. Verifique:
```bash
git remote -v
# Tem que mostrar: git@github.com:... (NÃO https://)
```
Se mostrar `https://`, troque:
```bash
git remote set-url origin git@github.com:<usuario>/<repo>.git
```

### "Permission denied (publickey)" ao testar GitHub
- Confirme que cadastrou a chave **pública** (`.pub`), não a privada
- Teste novamente: `ssh -T git@github.com`

### Não recebo o crédito de estudante
- Verifique que o e-mail institucional aparece reconhecido pelo SheerID (sistema da Microsoft)
- Alternativa: peça ao coordenador para validar manualmente
- A verificação pode levar até 24h em alguns casos

---

## Desafio extra

Crie um repositório vazio chamado `django-azure-pratica` na sua conta GitHub, clone-o localmente via SSH e crie um `README.md` com seu nome. Faça commit e push.

```bash
git clone git@github.com:<seu-usuario>/django-azure-pratica.git
cd django-azure-pratica
echo "# Aula prática Django + Azure" > README.md
git add README.md
git commit -m "first commit"
git push origin main
```

Esse será o repositório usado até o fim do curso.

---

**Próxima aula:** [Aula 2 — Projeto Django + GitHub](../aula2/README.md)
