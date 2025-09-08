# Aula Final: Deploy Completo do Projeto Full Stack no AWS (Upload do App JS via EC2)

## **Objetivo**
Nesta aula, você aprenderá a:
- Criar e configurar uma instância **EC2** para hospedar toda a aplicação full stack.
- Realizar o **upload do aplicativo JavaScript** (back-end Node.js + Express e front-end React) diretamente para a instância EC2.
- Instalar os pré-requisitos (Node.js, NPM, etc.) na instância EC2.
- Iniciar os servidores do **back-end** e do **front-end** para um deploy completo.

---

## **Pré-requisitos**
- Conta ativa na **AWS**.
- **Node.js e NPM** instalados localmente (para build e testes).
- Repositório do projeto versionado no **GitHub**.
- Arquivo de chave **(.pem)** para acesso SSH à instância EC2.

---

# **📌 Parte 1: Criação e Configuração da Instância EC2**

1. **Criação da Instância:**
   - Acesse o **AWS Management Console** e vá para o serviço **EC2**.
   - Clique em **Launch Instance** e escolha a AMI **Amazon Linux 2**.
   - Selecione um **t2.micro** (ou similar, conforme sua necessidade e disponibilidade no Free Tier).
   - Configure o **Security Group** para liberar as portas necessárias:
     - **22** (SSH)
     - **80** (HTTP)
     - **5000** (ou outra porta para o back-end, se preferir)
   - Gere e baixe o par de chaves (**.pem**) para acesso SSH.

2. **Conectando à Instância:**
   - Após a criação, copie o **IP público** da instância.
   - Conecte-se via SSH:
     ```bash
     ssh -i "seu-arquivo.pem" ec2-user@IP_DA_INSTANCIA
     ```

3. **Atualização do Sistema e Instalação do Node.js:**
   ```bash
   sudo apt update -y
   curl -sL https://rpm.nodesource.com/setup_16.x | sudo bash -
   sudo apt install -y nodejs
   ```
   - Verifique as versões instaladas:
     ```bash
     node -v
     npm -v
     ```

---

# **📌 Parte 2: Upload e Configuração do Back-end**

1. **Transferência dos Arquivos do Back-end:**
   - No seu ambiente local, acesse a pasta do back-end:
     ```bash
     cd aula-backend
     ```
   - Utilize o comando `scp` para enviar os arquivos para a instância EC2:
     ```bash
     scp -i "seu-arquivo.pem" -r ./aula-backend ec2-user@IP_DA_INSTANCIA:/home/ec2-user
     ```
   - Ou utilize o comando `git clone` para enviar os arquivos para a instância EC2:
      ```bash
      git clone https://github.com/seu_usuario/nome_do_repo.git
      ```

2. **Instalação das Dependências e Configuração:**
   - Na instância EC2, navegue até a pasta do back-end:
     ```bash
     cd aula-backend
     ```
   - Instale as dependências:
     ```bash
     npm install
     ```
   - Configure as variáveis de ambiente necessárias para o funcionamento da API. Por exemplo, você pode definir as variáveis diretamente no terminal ou criar um arquivo `.env`:
     ```bash
     export PORT=5000
     export MONGO_URI="mongodb+srv://SEU_USUARIO:SUA_SENHA@SEU_CLUSTER.mongodb.net/filmesDB?retryWrites=true&w=majority"
     ```
     
3. **Iniciando o Servidor do Back-end:**
   - Inicie a aplicação:
     ```bash
     node server.js
     ```
   - **Dica:** Para manter o servidor rodando em background, considere o uso do [PM2](https://pm2.keymetrics.io/):
     ```bash
     npm install -g pm2
     pm2 start server.js --name backend
     ```

4. **Teste do Back-end:**
   - No navegador ou via ferramenta de testes (como o Postman), acesse:
     ```bash
     http://IP_DA_INSTANCIA:5000/filmes
     ```
   - Confirme que a API está respondendo corretamente.

---

# **📌 Parte 3: Upload e Configuração do Front-end**

1. **Build e Transferência dos Arquivos do Front-end:**
   - No seu ambiente local, acesse a pasta do front-end:
     ```bash
     cd aula-frontend
     ```
   - Gere a versão de produção do React:
     ```bash
     npm run build
     ```
   - Transfira a pasta do front-end (incluindo a pasta `build`) para a instância EC2:
     ```bash
     scp -i "seu-arquivo.pem" -r ./aula-frontend ec2-user@IP_DA_INSTANCIA:/home/ec2-user
     ```
   - Ou utilize o comando `git clone` para enviar os arquivos para a instância EC2:
      ```bash
      git clone https://github.com/seu_usuario/nome_do_repo.git
      ```


2. **Instalação e Configuração para Servir o Front-end:**
   - Na instância EC2, navegue até a pasta do front-end:
     ```bash
     cd aula-frontend
     ```
   - Instale o pacote `serve`, que será utilizado para hospedar os arquivos estáticos:
     ```bash
     npm install -g serve
     ```
   - Inicie o servidor para servir a build do React na porta 80:
     ```bash
     serve -s build -l 80
     ```
   - Caso a porta 80 já esteja em uso ou se preferir outra porta, ajuste-a e lembre-se de liberar essa porta no Security Group.

3. **Teste do Front-end:**
   - No navegador, acesse:
     ```bash
     http://IP_DA_INSTANCIA
     ```
   - Verifique se a aplicação React carrega corretamente e se as chamadas à API (back-end) estão funcionando.

---

# **📌 Parte 4: Testando a Aplicação Completa**

1. **Acesse a aplicação** via navegador utilizando o IP público da instância EC2.
2. **Realize os testes integrados:**
   - Verifique o carregamento dos filmes pela API.
   - Execute operações de CRUD (Adicionar, Editar e Remover filmes) para confirmar que o back-end e o front-end estão devidamente integrados.

---

# **📌 Resumo dos Comandos**

## **Na Máquina Local (Upload dos Arquivos)**
```bash
# Upload do Back-end
cd aula-backend
scp -i "seu-arquivo.pem" -r ./aula-backend ec2-user@IP_DA_INSTANCIA:/home/ec2-user

# Build e Upload do Front-end
cd aula-frontend
npm run build
scp -i "seu-arquivo.pem" -r ./aula-frontend ec2-user@IP_DA_INSTANCIA:/home/ec2-user
```

## **GitHub para a instância (Clone dos Arquivos)**

```bash
# Upload do Back-end e Front-end
git clone https://github.com/seu_usuario/nome_do_repo.git
```

## **Na Instância EC2**
```bash
# Atualizar o sistema e instalar Node.js
sudo yum update -y
curl -sL https://rpm.nodesource.com/setup_16.x | sudo bash -
sudo yum install -y nodejs

# Back-end
cd aula-backend
npm install
export PORT=5000
export MONGO_URI="mongodb+srv://SEU_USUARIO:SUA_SENHA@SEU_CLUSTER.mongodb.net/filmesDB?retryWrites=true&w=majority"
# Inicie o servidor (use pm2 para rodar em background, se preferir)
node server.js

# Front-end
cd ../aula-frontend
npm install -g serve
serve -s build -l 80
```

---

# **🎯 Conclusão**
Neste tutorial, realizamos o **upload do app JS** – tanto o back-end quanto o front-end – diretamente para uma instância **EC2**. Após criar a instância pelo AWS Management Console e conectar via SSH, instalamos os pré-requisitos, fizemos o upload dos arquivos do projeto e iniciamos os servidores do back-end (Node.js/Express) e do front-end (React). Assim, você obtém um deploy completo e funcional do projeto full stack na AWS utilizando apenas a instância EC2.
