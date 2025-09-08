# Cloud Computing - Termos em Inglês e Português com Exemplos

Este guia apresenta os principais conceitos de **Computação em Nuvem (Cloud Computing)** em inglês e português, com explicações e exemplos práticos.

---

## Conceitos Fundamentais

- **Cloud Computing** (Computação em Nuvem): Entrega de serviços de TI (servidores, armazenamento, banco de dados, redes, software, etc.) pela internet ("a nuvem").

- **On-Premises** (No Local / Infraestrutura Própria): Modelo em que os recursos de TI estão dentro da empresa, sob controle direto.

- **Public Cloud** (Nuvem Pública): Recursos fornecidos por terceiros (ex: AWS, Azure, Google Cloud).

- **Private Cloud** (Nuvem Privada): Nuvem operada exclusivamente para uma organização.

- **Hybrid Cloud** (Nuvem Híbrida): Combinação de nuvem pública, privada e infraestrutura local.

---

## Modelos de Serviço

**IaaS - Infrastructure as a Service** (Infraestrutura como Serviço): Serviços básicos de infraestrutura (máquinas virtuais, redes, armazenamento).

  Exemplo: Criar uma máquina virtual na AWS EC2.

  ```bash
    aws ec2 run-instances --image-id ami-123456 --instance-type t2.micro
  ```

**PaaS - Platform as a Service** (Plataforma como Serviço): Fornece ambiente pronto para desenvolvimento e deploy de aplicações.

  Exemplo: Deploy no AWS Elastic Beanstalk.

  ```bash
  eb create meu-app
  ```

**SaaS - Software as a Service** (Software como Serviço): Aplicativos prontos, acessados via navegador.

  Exemplo: Google Workspace, Microsoft 365.

---

## Conceitos de Infraestrutura

**Virtual Machine (VM)** (Máquina Virtual): Emulação de um computador dentro da nuvem.

  ```bash
  gcloud compute instances create minha-vm --zone=us-central1-a
  ```

**Container** (Contêiner): Unidade leve de software que empacota código e dependências.

  ```bash
  docker run -d -p 8080:80 nginx
  ```

**Serverless Computing** (Computação Serverless / Sem Servidor): Execução de código sem gerenciar servidores.

  Exemplo: AWS Lambda, Google Cloud Functions.

  ```python
  def handler(event, context):
      return "Hello Cloud!"
  ```

**Scalability** (Escalabilidade): Capacidade de aumentar ou diminuir recursos conforme a demanda.

**Elasticity** (Elasticidade): Ajuste automático de recursos para atender à carga de trabalho.

---

## Redes e Segurança

**VPC - Virtual Private Cloud** (Nuvem Privada Virtual): Rede isolada dentro da nuvem.

  ```bash
  aws ec2 create-vpc --cidr-block 10.0.0.0/16
  ```

**Load Balancer** (Balanceador de Carga): Distribui tráfego entre múltiplos servidores.

**IAM - Identity and Access Management** (Gerenciamento de Identidade e Acesso): Controle de permissões de usuários e serviços.

  ```bash
  aws iam create-user --user-name aluno
  ```

**Encryption** (Criptografia): Proteção de dados em trânsito e em repouso.

---

## Armazenamento e Banco de Dados

**Object Storage** (Armazenamento de Objetos): Armazena dados como objetos (arquivos + metadados).

  Exemplo: AWS S3, Google Cloud Storage.

  ```bash
  aws s3 cp arquivo.txt s3://meu-bucket/
  ```

**Block Storage** (Armazenamento em Blocos): Disco virtual usado por VMs.

**Database as a Service (DBaaS)** (Banco de Dados como Serviço): Banco de dados gerenciado na nuvem.

  Exemplo: AWS RDS, Azure SQL Database.

---

## Ferramentas e Práticas

**CLI - Command Line Interface** (Interface de Linha de Comando): Ferramenta para gerenciar nuvem via terminal.

  Exemplo: `aws cli`, `gcloud`, `az`.

**Infrastructure as Code (IaC)** (Infraestrutura como Código): Automação de recursos da nuvem com código.

  Exemplo: Terraform, AWS CloudFormation.

  ```hcl
  resource "aws_instance" "vm" {
    ami           = "ami-123456"
    instance_type = "t2.micro"
  }
  ```

**Monitoring** (Monitoramento): Acompanhamento de métricas e logs.

  Exemplo: AWS CloudWatch, Prometheus, Grafana.

**DevOps** (DevOps): Integração entre desenvolvimento e operações, com pipelines de CI/CD.

---

## Normas e Conformidade

**GDPR** (RGPD - Regulamento Geral de Proteção de Dados): Lei europeia de proteção de dados.
**HIPAA** (HIPAA - Lei de Proteção de Dados de Saúde, EUA).
**ISO/IEC 27001** (ISO/IEC 27001 - Gestão da Segurança da Informação).
**SOC 2** (SOC 2 - Certificação de segurança e privacidade para provedores de serviços).

---

## Backup e Recuperação

**Backup** (Cópia de Segurança): Duplicação de dados para prevenção de perda.

  ```bash
  aws s3 sync /meus-dados s3://backup-bucket
  ```

**Disaster Recovery (DR)** (Recuperação de Desastres): Estratégia para restaurar sistemas após falhas graves.

**High Availability (HA)** (Alta Disponibilidade): Arquitetura que reduz tempo de inatividade por falhas.

---

## Exemplos Práticos Rápidos

* Criar um bucket no AWS S3:

  ```bash
  aws s3 mb s3://meu-novo-bucket
  ```

* Deploy simples no Google Cloud App Engine:

  ```bash
  gcloud app deploy
  ```

* Função Serverless no Azure Functions:

  ```bash
  func init MeuApp --python
  func new --name HelloWorld --template "HTTP trigger"
  ```
