# Aula 8 — Logs, Monitoramento e Troubleshooting

> **Tempo estimado:** 2h

---

## Objetivos

Ao final desta aula você terá:

1. Logs centralizados via **journald** e rotação configurada
2. **Métricas pelo Portal Azure** (Azure Monitor) e alertas por e-mail
3. **Backup automatizado** do banco PostgreSQL via cron na VM
4. Um **playbook de troubleshooting** consolidado para usar em qualquer problema futuro
5. Procedimento de **limpeza completa** ao fim da disciplina (deletar o Resource Group)

> Esta é a aula de **fechamento**. Aqui você consolida tudo e ganha autonomia para investigar problemas sozinho.

---

## 1. Logs com journald

O `systemctl` + `journald` já guarda toda a saída dos serviços que criamos (`gunicorn`, `nginx`). Comandos essenciais que você vai usar muito:

```bash
# Tempo real (parecido com tail -f)
sudo journalctl -u gunicorn -f
sudo journalctl -u nginx -f

# Última hora
sudo journalctl -u gunicorn --since "1 hour ago"

# Filtrar erros
sudo journalctl -u gunicorn -p err --since today

# Logs de boot anterior (se a VM reiniciou)
sudo journalctl -u gunicorn -b -1
```

### 1.1. Configurando retenção do journald

```bash
sudo nano /etc/systemd/journald.conf
```

Descomente e ajuste:

```ini
[Journal]
SystemMaxUse=500M
MaxRetentionSec=2week
ForwardToSyslog=no
```

Aplique:

```bash
sudo systemctl restart systemd-journald
```

### 1.2. Logrotate para Nginx

Já configurado por padrão no Ubuntu. Confirme:

```bash
cat /etc/logrotate.d/nginx
```

Teste em modo dry-run:

```bash
sudo logrotate -d /etc/logrotate.d/nginx
```

---

## 2. Monitoramento pelo Portal Azure

### 2.1. Métricas pelo Portal

1. Acesse https://portal.azure.com
2. Vá para sua VM
3. No menu lateral, em **Monitoring**, clique em **Metrics**
4. **Scope** já vem preenchido com a VM
5. Em **Metric Namespace**, deixe **Virtual Machine Host**
6. Em **Metric**, adicione:
   - **Percentage CPU**
7. Clique em **+ Add metric** e adicione:
   - **Available Memory Bytes**
8. Repita para:
   - **Network In Total**
   - **Network Out Total**

Você pode salvar o gráfico como dashboard: clique em **Save to dashboard > Pin to dashboard**.

### 2.2. Criando alerta por e-mail quando o CPU passa de 80%

1. Na página da VM, em **Monitoring > Alerts**, clique em **+ Create > Alert rule**
2. **Scope** já vem com a VM
3. **Condition**: clique em **+ Add condition**
   - **Signal name**: `Percentage CPU`
   - **Threshold**: `Static`
   - **Operator**: `Greater than`
   - **Aggregation type**: `Average`
   - **Threshold value**: `80`
   - **Aggregation granularity (Period)**: `5 minutes`
   - **Frequency of evaluation**: `1 minute`
   - Clique em **Done**
4. **Actions**: clique em **+ Create action group**
   - **Subscription** e **Resource group** já preenchidos
   - **Action group name**: `ag-alertas-django`
   - **Display name**: `alerta-django`
   - Em **Notifications**, adicione:
     - **Notification type**: `Email/SMS message/Push/Voice`
     - **Name**: `email-prof`
     - **Email**: marque e informe seu e-mail
   - **Review + create > Create**
5. **Details** (de volta ao Create alert rule):
   - **Severity**: `2 - Warning`
   - **Alert rule name**: `vm-cpu-alta`
6. **Review + create > Create**

Pronto. Você recebe e-mail quando a VM ficar com CPU acima de 80% por 5 minutos.

---

## 3. Backup do PostgreSQL

### 3.1. Backup gerenciado pela Azure (já está ativo)

A retenção configurada na Aula 4 (7 dias) já roda automaticamente. Para **restaurar** um banco em ponto no tempo:

1. Portal > seu servidor PostgreSQL
2. No menu superior, clique em **Restore**
3. Escolha **Point-in-time restore**
4. Defina a data/hora desejada
5. Forneça nome para o **novo servidor** (a restauração cria um servidor novo)
6. **Review + create > Create**

> Documentação oficial: https://learn.microsoft.com/azure/postgresql/flexible-server/concepts-backup-restore

### 3.2. Backup manual extra com pg_dump

Útil para snapshots antes de mudanças grandes ou para portabilidade.

Crie o script `/usr/local/bin/backup-postgres.sh` na VM:

```bash
sudo nano /usr/local/bin/backup-postgres.sh
```

```bash
#!/bin/bash
set -euo pipefail

DATE=$(date -u +%Y%m%d-%H%M)
BACKUP_DIR=/var/backups/postgres
mkdir -p "$BACKUP_DIR"

# Lê credenciais do .env do Django
set -a
source /var/www/django/app/.env
set +a

export PGPASSWORD="$DB_PASSWORD"

pg_dump \
  -h "$DB_HOST" \
  -U "$DB_USER" \
  -d "$DB_NAME" \
  --no-owner --no-acl \
  --format=custom \
  --file="$BACKUP_DIR/backup-$DATE.dump"

# Mantém só os 7 backups mais recentes
find "$BACKUP_DIR" -name "backup-*.dump" -mtime +7 -delete

echo "Backup $DATE concluido"
```

```bash
sudo chmod +x /usr/local/bin/backup-postgres.sh
sudo /usr/local/bin/backup-postgres.sh
ls -lh /var/backups/postgres/
```

### 3.3. Agendar com cron

```bash
sudo crontab -e
```

Se for a primeira vez, escolha `1` para `nano`. Adicione (todo dia às 3h UTC, equivalente a 0h em Recife):

```cron
0 3 * * * /usr/local/bin/backup-postgres.sh >> /var/log/pg-backup.log 2>&1
```

Salve.

---

## 4. Ferramentas de diagnóstico na VM

```bash
sudo apt install -y htop glances ncdu
```

| Comando      | Para quê                                    |
| ------------ | ------------------------------------------- |
| `htop`       | Processos, CPU, memória em tempo real       |
| `glances`    | Visão geral (CPU, RAM, disco, rede, alertas)|
| `ncdu /`     | Quem está ocupando disco                    |
| `df -h`      | Uso de disco resumido                       |
| `free -h`    | Uso de memória                              |
| `ss -tulpn`  | Quais processos escutam quais portas        |

---

## 5. Playbook de troubleshooting

### Quando "o site caiu"

**Triagem em 30 segundos:**

```bash
# 1. A VM está viva?
ssh vm-django 'uptime'

# 2. Os serviços estão rodando?
ssh vm-django 'sudo systemctl is-active nginx gunicorn'
# Resposta esperada: active / active

# 3. Tem resposta localmente na VM?
ssh vm-django 'curl -I http://127.0.0.1'

# 4. DNS resolve corretamente?
dig +short <SEU_DOMINIO>
# Tem que retornar o IP da VM

# 5. Certificado ainda válido?
echo | openssl s_client -connect <SEU_DOMINIO>:443 2>/dev/null \
  | openssl x509 -noout -dates
```

### Decision tree

```
SSH funciona?
+-- NAO -> Portal Azure -> VM -> "Connect" -> "Bastion" ou "Serial Console"
|          (porta 22 bloqueada? VM deallocated? IP mudou?)
|
+-- SIM -> nginx esta rodando?
           +-- NAO -> sudo systemctl restart nginx
           |          sudo journalctl -u nginx -n 50
           |
           +-- SIM -> gunicorn esta rodando?
                      +-- NAO -> sudo systemctl restart gunicorn
                      |          sudo journalctl -u gunicorn -n 100
                      |          (.env faltando? Erro de migracao? Senha do banco?)
                      |
                      +-- SIM -> banco responde?
                                 +-- NAO -> psql ... 'SELECT 1;'
                                 |          (Flexible Server pausado? Firewall?
                                 |           Seu IP mudou e nao esta liberado?)
                                 |
                                 +-- SIM -> Problema esta na aplicacao.
                                            Ative DEBUG=True por 5 minutos
                                            e capture o stack trace.
```

### Erros mais comuns vistos em turma

| Sintoma                                       | Causa típica                                    | Como diagnosticar                                |
| --------------------------------------------- | ----------------------------------------------- | ------------------------------------------------ |
| `502 Bad Gateway`                             | Gunicorn caído ou socket sem permissão          | `sudo journalctl -u gunicorn -n 50`              |
| `400 Bad Request` do Django                   | `ALLOWED_HOSTS` não inclui o domínio acessado   | Edite `.env`, `restart gunicorn`                 |
| `CSRF verification failed` no admin           | Falta `CSRF_TRUSTED_ORIGINS` para HTTPS         | Edite settings, redeploy                         |
| Disco cheio                                   | Logs do Nginx, dumps antigos, `__pycache__`     | `sudo ncdu /`                                    |
| VM travada / muito lenta                      | Memória esgotada (B1s tem só 1 GB)              | `htop`, considere migrar para B2s                |
| Deploy não atualiza o site                    | Cache do browser ou commit não foi puxado       | `cd app && git log -1`, hard reload no browser   |
| `connection refused` ao banco                 | Seu IP mudou e saiu do firewall                 | Portal > PostgreSQL > Networking > add IP atual  |
| `no pg_hba.conf entry for host`               | Mesmo problema acima                            | Mesma solução                                    |

### Comandos "primeiros socorros"

```bash
# Status geral
sudo systemctl --failed
sudo systemctl status nginx gunicorn

# Reiniciar tudo
sudo systemctl restart gunicorn nginx

# Limpar disco rapidamente
sudo journalctl --vacuum-time=3d
sudo apt clean
sudo find /var/www/django/app -name "__pycache__" -type d -exec rm -rf {} +

# Quem está usando porta 80/443/8000
sudo ss -tulpn | grep -E ':80|:443|:8000'

# Últimas falhas de SSH
sudo journalctl -u ssh --since "1 hour ago" | grep -i fail
```

---

## 6. Acessando a VM quando o SSH falha (Serial Console)

Se você ficou trancado fora por algum motivo (ex.: configurou `PasswordAuthentication no` mas perdeu a chave), use o **Serial Console** do Portal Azure:

1. Portal > sua VM
2. Menu lateral em **Help** > **Serial console**
3. Aguarde o console abrir (até 30 segundos)
4. Faça login com usuário e **senha** do sistema

> Para usar Serial Console, a VM precisa ter senha configurada. Se nunca configurou:
> 1. Portal > sua VM > **Help > Reset password**
> 2. Mode: **Reset password**
> 3. Defina usuário e senha
> 4. **Update**

---

## Checkpoint final do curso

- [ ] `sudo journalctl -u gunicorn -f` mostra requests em tempo real
- [ ] `htop` na VM mostra processos do gunicorn (3 workers)
- [ ] `/usr/local/bin/backup-postgres.sh` roda manualmente e gera um arquivo `.dump`
- [ ] `sudo crontab -l` mostra a entrada do backup diário
- [ ] Alerta de CPU está configurado e visível no Portal (VM > **Alerts**)
- [ ] Você consegue percorrer o decision tree acima sem consultar este README

Se tudo confirmado: **curso concluído**.

---

## Limpeza ao final da disciplina

Quando o semestre acabar, para parar de consumir crédito:

### Opção A: Deletar o Resource Group inteiro pelo Portal

1. Portal > **Resource groups**
2. Clique em `rg-django-azure-pratica`
3. No topo da página, clique em **Delete resource group**
4. Digite o nome do resource group para confirmar
5. Clique em **Delete**

Demora ~10 minutos e roda em background. Apaga **tudo**: VM, disco, IP público, VNet, NSG, PostgreSQL Flexible Server, snapshots, regras de firewall.

### Opção B: Apenas pausar tudo (caso queira retomar no próximo semestre)

- **VM**: Portal > VM > **Stop**. VM fica em **Stopped (deallocated)**, paga apenas storage (~US$3/mês).
- **PostgreSQL**: Portal > Flexible Server > **Stop**. Paga apenas storage. A Microsoft religa automaticamente após 7 dias para aplicar patches.

---

## Desafios finais (para avaliação)

Escolha **dois** para a entrega final da disciplina:

1. **Container**: containerize a aplicação com `Dockerfile` + `docker-compose.yml` (Django + Postgres). Documente como rodar localmente.
2. **Azure App Service**: faça um deploy paralelo da mesma aplicação em **App Service** (PaaS). Escreva um relatório de 1 página comparando com sua VM (IaaS): pros, contras, custo, latência percebida.
3. **Infrastructure as Code**: refaça a infra das Aulas 3 e 4 usando um **Bicep template**. Coloque no repositório em `/infra` e documente. O Portal Azure permite implantar Bicep direto pela interface (**+ Create a resource > Template deployment**).
4. **Application Insights**: integre Application Insights ao Django (`opencensus-ext-azure`). Crie um dashboard no Portal com requests/minuto e erros 5xx.
5. **Blue-green deploy**: configure 2 VMs atrás de um **Azure Load Balancer**. O CI/CD alterna deploys entre elas, garantindo zero downtime.

---

## Para continuar aprendendo

- **Django for Beginners / for Professionals** — William S. Vincent
- **Two Scoops of Django** — boas práticas
- **Microsoft Learn — Azure Fundamentals (AZ-900)** — primeira certificação útil e gratuita: https://learn.microsoft.com/training/courses/az-900t00
- **GitHub Actions docs** — https://docs.github.com/actions
- **The Twelve-Factor App** — https://12factor.net (princípios de aplicações cloud-native)

---

## Encerramento

Você começou na Aula 1 criando um venv pela primeira vez e chegou na Aula 8 com CI/CD, HTTPS, banco gerenciado, alertas e backup automático rodando na nuvem. Esse é o mesmo stack que muitas empresas usam em produção — a diferença está apenas em escala e camadas adicionais de redundância.

**Voltar ao índice:** [README principal](../../README.md)
