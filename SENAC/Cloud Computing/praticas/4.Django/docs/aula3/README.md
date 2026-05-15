# Aula 3 — Provisionando a VM pelo Portal Azure

> **Tempo estimado:** 2h

---

## Objetivos

Ao final desta aula você terá:

1. Um **Resource Group** dedicado ao projeto
2. Uma **VM Ubuntu Server 24.04 LTS** (tamanho Standard_B1s)
3. **Acesso SSH** funcionando da sua máquina para a VM
4. **Regras de NSG** corretas para SSH, HTTP e HTTPS
5. **IP público estático** associado à VM
6. Ubuntu atualizado e dependências base instaladas

Todo o provisionamento é feito **pelo Portal Azure**, em https://portal.azure.com.

---

## Conceitos rápidos

| Conceito              | O que é                                                                            |
| --------------------- | ---------------------------------------------------------------------------------- |
| **Resource Group**    | Container lógico que agrupa todos os recursos de um projeto. Apagá-lo apaga tudo.  |
| **Virtual Machine**   | Máquina virtual Linux/Windows acessada por SSH/RDP                                 |
| **Virtual Network**   | Rede privada onde a VM reside                                                      |
| **Subnet**            | Subdivisão da Virtual Network                                                      |
| **Network Security Group (NSG)** | Firewall por regras: quais portas estão abertas                         |
| **Public IP**         | IP acessível pela internet. Pode ser **Dynamic** (muda no reboot) ou **Static**    |

---

## 1. Criando o Resource Group

1. Acesse https://portal.azure.com e faça login
2. Na barra de busca superior, digite **Resource groups** e clique no resultado
3. Clique em **+ Create** (no topo da página)
4. Preencha:

| Campo            | Valor                          |
| ---------------- | ------------------------------ |
| Subscription     | Azure for Students             |
| Resource group   | `rg-django-azure-pratica`      |
| Region           | `Brazil South`                 |

5. Clique em **Review + create**
6. Aguarde a validação aparecer com a mensagem **Validation passed**
7. Clique em **Create**

> A região **Brazil South** fica em São Paulo e tem a menor latência para alunos de PE. Use essa para todos os recursos.

---

## 2. Criando a Virtual Machine

### 2.1. Abra o wizard de criação

1. Na barra de busca superior, digite **Virtual machines**
2. Clique em **Virtual machines** sob a categoria **Services**
3. Na página de Virtual machines, clique em **+ Create** > **Azure virtual machine**

A página **Create a virtual machine** abre com 8 abas (Basics, Disks, Networking, Management, Monitoring, Advanced, Tags, Review + create).

### 2.2. Aba "Basics"

#### Project details

| Campo             | Valor                          |
| ----------------- | ------------------------------ |
| Subscription      | Azure for Students             |
| Resource group    | `rg-django-azure-pratica`      |

#### Instance details

| Campo                    | Valor                                      |
| ------------------------ | ------------------------------------------ |
| Virtual machine name     | `vm-django-app`                            |
| Region                   | `(South America) Brazil South`             |
| Availability options     | `No infrastructure redundancy required`    |
| Security type            | `Standard`                                 |
| Image                    | `Ubuntu Server 24.04 LTS - x64 Gen2`       |
| VM architecture          | `x64`                                      |
| Run with Azure Spot discount | Deixe **desmarcado**                   |
| Size                     | `Standard_B1s - 1 vcpu, 1 GiB memory`      |

> **Como achar o tamanho B1s:** clique em **See all sizes**. No filtro lateral esquerdo, em **Family**, escolha **General purpose** e em **vCPUs** marque **1**. Procure por **B1s**. Selecione e clique em **Select**.

#### Administrator account

| Campo                         | Valor                                                    |
| ----------------------------- | -------------------------------------------------------- |
| Authentication type           | `SSH public key`                                         |
| Username                      | `azureuser`                                              |
| SSH public key source         | `Use existing public key`                                |
| SSH public key                | Cole o conteúdo do seu arquivo `~/.ssh/id_ed25519.pub`   |

> Se você ainda não gerou a chave SSH ou perdeu, escolha **Generate new key pair** em vez de **Use existing public key**. O Azure vai oferecer download do arquivo `.pem` ao final — guarde-o em local seguro. Mas o recomendado é usar a chave que você gerou na Aula 1.

#### Inbound port rules

| Campo                    | Valor                          |
| ------------------------ | ------------------------------ |
| Public inbound ports     | `Allow selected ports`         |
| Select inbound ports     | `SSH (22)`                     |

> Por enquanto só liberamos SSH. As portas HTTP (80) e HTTPS (443) serão liberadas na aba **Networking**.

### 2.3. Aba "Disks"

Clique em **Next : Disks >**.

| Campo                | Valor                                     |
| -------------------- | ----------------------------------------- |
| OS disk size         | `Default (30 GiB)` ou explicitamente **30 GiB** |
| OS disk type         | `Standard SSD (locally-redundant storage)` |
| Delete with VM       | **Marcado** (importante para limpeza)     |
| Encryption type      | Deixe o padrão                            |

### 2.4. Aba "Networking"

Clique em **Next : Networking >**.

#### Network interface

| Campo                       | Valor                                                                |
| --------------------------- | -------------------------------------------------------------------- |
| Virtual network             | (Create new) `vnet-django-app`                                       |
| Subnet                      | `default (10.0.0.0/24)` (gerado automaticamente)                     |
| Public IP                   | (Create new) `pip-vm-django-app`                                     |
| NIC network security group  | `Basic`                                                              |
| Public inbound ports        | `Allow selected ports`                                               |
| Select inbound ports        | `HTTP (80)`, `HTTPS (443)`, `SSH (22)`                               |
| Delete public IP and NIC when VM is deleted | **Marcado**                                          |

> Se você não vir o link para criar a Virtual Network: clique no dropdown e selecione **Create new** abaixo da lista.

### 2.5. Demais abas

Avance para **Management**, **Monitoring**, **Advanced**, **Tags** e deixe os valores padrão. Recomendado em **Management**:

- **Auto-shutdown**: ative se quiser desligar a VM automaticamente todo dia para economizar crédito (ex.: às 22h, fuso `(UTC-03:00) Brasilia`). Conforme documentação Microsoft, isso é opcional e pode ser configurado depois.

### 2.6. Aba "Review + create"

1. Confira o resumo
2. Aguarde a validação aparecer **Validation passed**
3. Confira o custo estimado por hora no topo da página
4. Clique em **Create**

A implantação demora cerca de 2 a 5 minutos. Quando aparecer **Your deployment is complete**, clique em **Go to resource**.

---

## 3. Anote o IP público e torne-o estático

### 3.1. Anote o IP atual

Na página da VM, no painel **Essentials**:

- Copie o **Public IP address** — vamos chamar de `<IP_VM>` daqui em diante

### 3.2. Mude o IP público para estático

Por padrão o IP é **Dynamic**: ele pode mudar quando a VM for deallocada e reiniciada, o que quebraria DNS, certificado e CI/CD.

1. Na página da VM, role para baixo no painel **Properties** ou abra o menu lateral **Networking > Network settings**
2. Clique no nome do **Public IP** (ex.: `pip-vm-django-app`)
3. No menu lateral do recurso de IP, clique em **Configuration** (sob **Settings**)
4. Em **Assignment**, mude de `Dynamic` para `Static`
5. Clique em **Save** no topo da página

> O IP **não vai mudar** durante esta operação — apenas fica reservado.

---

## 4. Acessando a VM por SSH

Da sua máquina local, abra um terminal (PowerShell, Git Bash ou Terminal):

```bash
ssh azureuser@<IP_VM>
```

Substitua `<IP_VM>` pelo IP que você anotou no passo 3.1.

Na primeira conexão, vai aparecer:

```
The authenticity of host '<IP_VM>' can't be established.
ED25519 key fingerprint is SHA256:...
Are you sure you want to continue connecting (yes/no/[fingerprint])?
```

Digite `yes` e Enter.

Se tudo funcionar, você verá:

```
Welcome to Ubuntu 24.04 LTS (GNU/Linux 6.x.x-azure x86_64)
...
azureuser@vm-django-app:~$
```

Você está dentro da VM.

---

## 5. Atualizando a VM e instalando dependências base

**Já dentro da VM** (via SSH), rode:

```bash
sudo apt update
sudo apt upgrade -y

# Pacotes essenciais para Django + PostgreSQL + Nginx + HTTPS
sudo apt install -y \
    python3 python3-pip python3-venv python3-dev \
    build-essential libssl-dev libffi-dev \
    libpq-dev \
    postgresql-client \
    nginx \
    git \
    curl \
    ufw \
    certbot python3-certbot-nginx
```

Confirme as versões:

```bash
python3 --version   # Esperado: Python 3.12.x
nginx -v            # Esperado: nginx version: nginx/1.24.x ou superior
git --version
psql --version
```

---

## 6. Configurando o firewall interno (UFW) da VM

Apesar do NSG da Azure já filtrar, é boa prática também ativar o firewall do sistema operacional (defesa em profundidade).

```bash
sudo ufw allow OpenSSH
sudo ufw allow 'Nginx Full'   # libera portas 80 e 443
sudo ufw --force enable
sudo ufw status
```

Saída esperada:

```
Status: active

To                         Action      From
--                         ------      ----
OpenSSH                    ALLOW       Anywhere
Nginx Full                 ALLOW       Anywhere
OpenSSH (v6)               ALLOW       Anywhere (v6)
Nginx Full (v6)            ALLOW       Anywhere (v6)
```

---

## 7. Bônus: configurando um alias SSH localmente

Pra não digitar `ssh azureuser@<IP_VM>` toda vez, edite `~/.ssh/config` na **sua máquina local**:

**Linux / macOS / Git Bash:**

```bash
nano ~/.ssh/config
```

**Windows PowerShell:**

```powershell
notepad $env:USERPROFILE\.ssh\config
```

Adicione (substitua `<IP_VM>`):

```
Host vm-django
    HostName <IP_VM>
    User azureuser
    IdentityFile ~/.ssh/id_ed25519
```

Salve. Agora basta:

```bash
ssh vm-django
```

---

## Checkpoint

Da sua máquina local, este comando deve funcionar e retornar as linhas indicadas:

```bash
ssh vm-django 'whoami && hostname && nginx -v && python3 --version'
```

Saída esperada (a versão exata do nginx pode variar um pouco):

```
azureuser
vm-django-app
nginx version: nginx/1.24.x
Python 3.12.x
```

Abrindo `http://<IP_VM>` no navegador, você deve ver a página padrão do Nginx ("Welcome to nginx!"). Se ver, está pronto para a Aula 4.

---

## Troubleshooting

### `ssh: connect to host <IP> port 22: Connection timed out`
- Verifique no Portal Azure se o NSG da VM tem a regra **SSH** (porta 22) liberada. Para isso: VM > **Networking > Network settings** > confirme em **Inbound port rules** que há uma regra com Destination port 22, Protocol TCP, Action Allow.
- Verifique se está usando o IP correto.
- Em algumas redes corporativas/universitárias o ISP bloqueia saída na porta 22. Tente de outra rede (4G do celular como teste) ou use o **Bastion** do Azure (recurso pago).

### `Permission denied (publickey)` ao conectar
- A chave pública cadastrada na VM não corresponde à privada que você está usando.
- **Solução pelo Portal:** VM > **Help > Reset password**. Em vez de resetar senha, escolha **Reset SSH public key**, informe o usuário (`azureuser`) e cole o conteúdo de `~/.ssh/id_ed25519.pub` no campo SSH public key. Clique em **Update**.

### `sudo: unable to resolve host vm-django-app`
Edite `/etc/hosts` na VM e adicione a linha:
```
127.0.1.1   vm-django-app
```

### Página do Nginx não abre no navegador
- Confirme que o serviço está rodando: `sudo systemctl status nginx`. Deve mostrar `active (running)`.
- Confirme no NSG da Azure que a porta 80 está aberta (Inbound rule HTTP).
- Confirme o firewall interno: `sudo ufw status`.

---

## Desafio extra

Configure **Auto-shutdown** na VM para 22h00 no fuso `(UTC-03:00) Brasilia`. Documentação oficial: https://learn.microsoft.com/azure/virtual-machines/auto-shutdown-vm.

Isso economiza crédito quando você não está usando a VM. Para religar manualmente, basta usar **Start** na página da VM no Portal.

---

## Gestão de custos

Para **pausar a VM (sem perder o disco)** quando não estiver usando:

1. Vá para a página da VM no Portal
2. Clique em **Stop** no topo da página
3. Aguarde o status mudar para **Stopped (deallocated)**

VM **Stopped (deallocated)** não cobra computação, só disco (~US$3/mês).

Para retomar, clique em **Start** na mesma página.

---

**Próxima aula:** [Aula 4 — Banco PostgreSQL Flexible Server](../aula4/README.md)
