# Azure API Management — Guia Prático

Configuração de uma instância do Azure API Management (APIM) conectada à API FastAPI implantada no App Service. O objetivo é expor a API por meio de um gateway centralizado, aplicando políticas de segurança, controle de acesso e documentação unificada.

---

## Índice

- [O que é o Azure API Management](#o-que-é-o-azure-api-management)
- [Arquitetura da solução](#arquitetura-da-solução)
- [Pré-requisitos](#pré-requisitos)
- [Parte 1 — Criar a instância do APIM](#parte-1--criar-a-instância-do-apim)
- [Parte 2 — Importar a API do App Service](#parte-2--importar-a-api-do-app-service)
- [Parte 3 — Configurar operações](#parte-3--configurar-operações)
- [Parte 4 — Aplicar políticas](#parte-4--aplicar-políticas)
- [Parte 5 — Criar produtos e assinaturas](#parte-5--criar-produtos-e-assinaturas)
- [Parte 6 — Testar pelo portal do desenvolvedor](#parte-6--testar-pelo-portal-do-desenvolvedor)
- [Parte 7 — Monitoramento](#parte-7--monitoramento)
- [Referência de conceitos](#referência-de-conceitos)
- [Solução de problemas](#solução-de-problemas)
- [Limpeza de recursos](#limpeza-de-recursos)

---

## O que é o Azure API Management

O Azure API Management é um serviço de gateway para APIs. Ele fica posicionado entre os consumidores (aplicações clientes, front-ends, parceiros) e os serviços de back-end (como a API FastAPI no App Service), centralizando responsabilidades que não deveriam estar no código da aplicação.

### Por que usar um gateway de API

Em arquiteturas sem gateway, cada API precisa implementar individualmente autenticação, limitação de requisições, logs, versionamento e CORS. Isso gera duplicação de código e dificulta a governança. Com o APIM, essas responsabilidades são delegadas ao gateway.

```
Cliente (browser, mobile, parceiro)
        |
        v
+-------------------------+
|  Azure API Management   |  <-- autenticação, rate limit, logs, CORS, transformação
+-------------------------+
        |
        v
+-------------------------+
|  Azure App Service      |  <-- apenas a lógica de negócio (FastAPI)
+-------------------------+
        |
        v
+-------------------------+
|  Banco de dados         |  <-- persistência (futuro)
+-------------------------+
```

### Componentes principais do APIM

| Componente          | Descrição                                                               |
| ------------------- | ----------------------------------------------------------------------- |
| Gateway             | Ponto de entrada que recebe as requisições e as repassa ao back-end     |
| Portal do Publisher | Interface de administração onde APIs e políticas são configuradas       |
| Portal do Developer | Portal público onde desenvolvedores consultam e testam as APIs          |
| Produtos            | Agrupamentos de APIs expostos com um conjunto de políticas e permissões |
| Assinaturas         | Chaves de acesso geradas por produto, entregues aos consumidores        |
| Políticas           | Regras aplicadas às requisições e respostas (XML declarativo)           |

---

## Arquitetura da solução

Ao final deste guia, a arquitetura será a seguinte:

```
URL pública do APIM
https://<nome-apim>.azure-api.net/tarefas

        |
        v
  APIM Gateway
  - Valida a chave de assinatura (Ocp-Apim-Subscription-Key)
  - Aplica rate limit (10 requisições por minuto)
  - Injeta cabeçalho de correlação
  - Encaminha para o back-end

        |
        v
  App Service (back-end interno)
  https://fastapi-demo-<seunome>.azurewebsites.net
  - FastAPI processa a requisição
  - Retorna a resposta

        |
        v
  APIM Gateway
  - Remove cabeçalhos internos
  - Retorna a resposta ao cliente
```

---

## Pré-requisitos

- API FastAPI implantada no Azure App Service e respondendo em sua URL pública (conforme o guia anterior)
- Conta Azure for Students ativa
- Acesso ao portal em [portal.azure.com](https://portal.azure.com)

> **Observação sobre tempo de provisionamento:** A instância do APIM no plano Consumption leva entre 1 e 5 minutos para ser criada. No plano Developer, pode levar até 40 minutos. Este guia utiliza o plano Consumption, que é o mais rápido e adequado para estudos.

---

## Parte 1 — Criar a instância do APIM

### 1.1 Abrir o serviço no portal

1. No portal, pesquise por **API Management services** na barra de pesquisa superior.
2. Clique em **API Management services** nos resultados.
3. Clique em **Create**.

### 1.2 Preencher os dados básicos (aba Basics)

| Campo               | Valor recomendado                              |
| ------------------- | ---------------------------------------------- |
| Subscription        | Azure for Students                             |
| Resource group      | `rg-fastapi-demo` (mesmo grupo da API)         |
| Region              | Brazil South                                   |
| Resource name       | `apim-tarefas-<seunome>` (único globalmente)   |
| Organization name   | Nome da sua instituição ou projeto             |
| Administrator email | Seu e-mail institucional                       |
| Pricing tier        | Consumption (sem custo fixo, paga por chamada) |

> **Sobre o plano Consumption:** Diferentemente dos planos Developer e Standard, o plano Consumption não tem custo fixo mensal. Você paga apenas pelas chamadas realizadas, com 1 milhão de chamadas gratuitas por mês. É o plano mais adequado para desenvolvimento e para o Azure for Students.

### 1.3 Revisar e criar

1. Clique em **Review + create**.
2. Revise os dados exibidos.
3. Clique em **Create**.

A implantação será iniciada. Aguarde a notificação de conclusão antes de prosseguir.

**O que você deve ver ao concluir:** A página de visão geral da instância APIM, com o campo **Gateway URL** preenchido com algo como `https://apim-tarefas-<seunome>.azure-api.net`.

Guarde essa URL. É o endereço público do gateway.

---

## Parte 2 — Importar a API do App Service

O APIM pode importar APIs automaticamente a partir da especificação OpenAPI gerada pelo FastAPI. Isso elimina a necessidade de cadastrar cada operação manualmente.

### 2.1 Verificar a URL do App Service antes de continuar

Este passo é obrigatório. Sem a URL da sua aplicação em mãos, não é possível preencher o formulário de importação.

1. Abra uma nova aba no navegador.
2. Acesse o portal, pesquise **App Services** e clique na sua Web App (`fastapi-demo-<seunome>`).
3. Na página **Overview**, copie o valor do campo **Default domain**. O formato será:
   ```
   fastapi-demo-<seunome>.azurewebsites.net
   ```
4. Monte a URL completa da aplicação adicionando `https://` na frente:
   ```
   https://fastapi-demo-<seunome>.azurewebsites.net
   ```
5. Abra essa URL no navegador. Você deve ver o JSON de resposta da rota raiz:
   ```json
   {"status": "ok", "docs": "/docs"}
   ```

   Se a página não carregar ou retornar erro, a API não está em execução. Resolva isso antes de prosseguir — o APIM precisa alcançar o App Service para importar as operações.

### 2.2 Entender o que é o /openapi.json

O FastAPI gera automaticamente uma especificação OpenAPI da sua API. Essa especificação é um arquivo JSON que descreve todos os endpoints, parâmetros, modelos de dados e respostas. O APIM lê esse arquivo para criar as operações sem que você precise cadastrá-las uma a uma.

Para visualizar a especificação da sua API, acesse no navegador:
```
https://fastapi-demo-<seunome>.azurewebsites.net/openapi.json
```

Você verá um JSON com a estrutura completa da API. Não é necessário entender o conteúdo agora — apenas confirme que a página carrega sem erro. Essa é a URL que será colada no APIM.

Para visualizar a documentação interativa gerada a partir dessa especificação, acesse:
```
https://fastapi-demo-<seunome>.azurewebsites.net/docs
```

### 2.3 Acessar a seção de APIs no APIM

1. Volte para a aba do portal com a instância do APIM.
2. No menu lateral esquerdo, clique em **APIs**.
3. Você verá uma tela com várias opções de origem para criar uma API. Procure o bloco intitulado **Create from definition** (ou **Definir a partir de**).
4. Dentro desse bloco, clique em **OpenAPI**.

### 2.4 Preencher o formulário "Create from OpenAPI specification"

A tela que abre é exatamente a do screenshot. Clique em **Full** (ao lado de Basic, no topo) para exibir todos os campos disponíveis.

Preencha cada campo conforme descrito abaixo:

---

**OpenAPI specification** (campo obrigatório, marcado com asterisco vermelho)

Cole a URL do `/openapi.json` da sua aplicação:
```
https://fastapi-demo-<seunome>.azurewebsites.net/openapi.json
```

Após colar a URL, pressione **Tab** ou clique fora do campo. O APIM fará uma requisição à URL e preencherá automaticamente os campos **Display name** e **Name** com os valores definidos no `main.py`. Se o preenchimento automático não ocorrer, verifique se a URL está correta e se a aplicação está respondendo.

> **Nota:** Nesta versão do portal, o botão **Load** pode não estar visível como um botão separado. O carregamento ocorre automaticamente ao sair do campo da URL, ou ao clicar em **Create**. Caso os campos não sejam preenchidos automaticamente, preencha-os manualmente conforme a tabela abaixo.

---

**Include required query parameters in operation templates** (checkbox)

Deixe marcado. Isso instrui o APIM a incluir parâmetros de query string obrigatórios nos templates das operações importadas.

---

**Display name** (campo obrigatório)

Nome exibido na interface do APIM e no portal do desenvolvedor. Preencha com:
```
API de Tarefas
```

---

**Name** (campo obrigatório)

Identificador interno usado em URLs e referências dentro do APIM. Deve conter apenas letras minúsculas, números e hífens. Preencha com:
```
api-tarefas
```

---

**Description** (campo opcional)

Descrição exibida no portal do desenvolvedor. Preencha com:
```
API de gerenciamento de tarefas implantada no Azure App Service.
```

---

**URL scheme**

Selecione **Both**. Isso permite que o APIM aceite requisições tanto via HTTP quanto HTTPS. Em produção, selecione apenas HTTPS.

---

**API URL suffix** (campo importante)

O sufixo é o caminho adicionado após a URL base do gateway. Ele separa esta API de outras que possam existir na mesma instância do APIM.

Preencha com:
```
tarefas
```

Com esse sufixo, a URL base do gateway é `https://apim-tarefas-<seunome>.azure-api.net` e o endpoint de listagem ficará disponível em:
```
https://apim-tarefas-<seunome>.azure-api.net/tarefas/tasks
```

O campo **Base URL** abaixo do sufixo é apenas informativo — ele mostra a URL resultante conforme você digita o sufixo.

---

**Tags** (campo opcional)

Deixe em branco por enquanto.

---

**Products** (campo importante)

Produtos são agrupamentos que controlam o acesso à API. Sem associar a API a um produto, ela não ficará acessível para consumidores externos.

Clique no campo e selecione **Unlimited**.

> **O que é o produto Unlimited:** É um produto padrão criado automaticamente pelo APIM que permite acesso sem restrições de cota. É adequado para testes e desenvolvimento. Na Parte 5 deste guia, você criará um produto com políticas de acesso mais controladas.

---

**Version this API?** (checkbox)

Deixe desmarcado. Versionamento de API é um tópico avançado que não será abordado nesta prática.

---

### 2.5 Criar a API

Após preencher todos os campos, clique em **Create**.

**O que você deve ver:** A API importada aparecerá na lista com suas operações já cadastradas automaticamente:
- `GET /` — Health check
- `GET /tasks` — Listar tarefas
- `GET /tasks/{task_id}` — Buscar tarefa por ID
- `POST /tasks` — Criar tarefa

Se a lista de operações aparecer vazia, o APIM não conseguiu ler o `/openapi.json`. Nesse caso, consulte a seção [Solução de problemas](#solução-de-problemas) ao final deste guia.

---

## Parte 3 — Configurar operações

Após a importação, verifique se as operações foram importadas corretamente e configure o endereço do back-end.

### 3.1 Verificar o back-end

1. Com a API selecionada na lista, clique na aba **Settings** (dentro da API, não no menu lateral).
2. Localize o campo **Web service URL**.
3. Confirme que contém a URL base do seu App Service:
   ```
   https://fastapi-demo-<seunome>.azurewebsites.net
   ```
4. Se estiver incorreto, corrija e clique em **Save**.

### 3.2 Revisar as operações importadas

1. Clique na aba **Design**.
2. Você verá a lista de operações no painel esquerdo:
   - `GET /tasks` — Listar tarefas
   - `GET /tasks/{task_id}` — Buscar tarefa por ID
   - `POST /tasks` — Criar tarefa

3. Clique em cada operação para verificar se os parâmetros e o corpo da requisição foram importados corretamente.

---

## Parte 4 — Aplicar políticas

Políticas são regras declarativas escritas em XML que o APIM aplica nas requisições (inbound), nas respostas (outbound) ou em caso de erro (on-error). São o recurso mais poderoso do APIM.

### 4.1 Entender a estrutura de uma política

```xml
<policies>
    <inbound>
        <!-- Aplicado ANTES de encaminhar ao back-end -->
        <!-- Exemplos: autenticação, rate limit, transformação de requisição -->
        <base />
    </inbound>
    <backend>
        <!-- Controla como a requisição chega ao back-end -->
        <base />
    </backend>
    <outbound>
        <!-- Aplicado DEPOIS que o back-end responde -->
        <!-- Exemplos: transformação de resposta, remoção de cabeçalhos -->
        <base />
    </outbound>
    <on-error>
        <!-- Executado quando ocorre um erro em qualquer etapa -->
        <base />
    </on-error>
</policies>
```

### 4.2 Aplicar políticas globais na API

1. Com a API selecionada, clique na aba **Design**.
2. No painel central, clique em **All operations** (para aplicar a todas as operações).
3. Na seção **Inbound processing**, clique no ícone de código `</>` (Policy code editor).
4. Substitua o conteúdo pelo seguinte:

```xml
<policies>
    <inbound>
        <base />

        <!-- Unica opcao de rate limiting suportada no Consumption -->
        <rate-limit calls="10" renewal-period="60" />

        <set-header name="X-Correlation-Id" exists-action="skip">
            <value>@(context.RequestId.ToString())</value>
        </set-header>

        <set-header name="X-Gateway" exists-action="override">
            <value>azure-apim</value>
        </set-header>
    </inbound>

    <backend>
        <base />
    </backend>

    <outbound>
        <base />

        <set-header name="X-Powered-By" exists-action="delete" />
        <set-header name="Server" exists-action="delete" />

        <set-header name="X-Content-Type-Options" exists-action="override">
            <value>nosniff</value>
        </set-header>
        <set-header name="X-Frame-Options" exists-action="override">
            <value>DENY</value>
        </set-header>
    </outbound>

    <on-error>
        <base />
    </on-error>
</policies>
```

5. Clique em **Save**.

> **Se aparecer erro de validacao ao salvar:** Confirme que copiou o XML completo, incluindo a tag de abertura `<policies>` e o fechamento `</policies>`. O editor do APIM valida o XML antes de salvar e rejeita conteudo incompleto.

### 4.3 Aplicar política de CORS (necessária para front-ends)

1. Ainda na aba **Design**, com **All operations** selecionado.
2. Na seção **Inbound processing**, clique em **Add policy**.
3. Selecione **Allow cross-origin resource sharing (CORS)**.
4. Configure:
   - **Allowed origins:** `*` (em produção, especifique o domínio do front-end)
   - **Allowed methods:** GET, POST, OPTIONS
   - **Allowed headers:** `*`
5. Clique em **Save**.

---

## Parte 5 — Criar produtos e assinaturas

No APIM, os consumidores da API não acessam diretamente as operações. Eles precisam se inscrever em um **produto**, que agrupa APIs e define as condições de uso.

### 5.1 O que é um produto

Um produto é um pacote de APIs com uma política de acesso associada. Exemplos de produtos comuns:

| Produto  | Uso típico                                      |
| -------- | ----------------------------------------------- |
| Free     | Acesso limitado para testes (sem SLA)           |
| Standard | Acesso para parceiros com rate limit moderado   |
| Premium  | Acesso para clientes pagantes com SLA garantido |

### 5.2 Criar um produto para desenvolvimento

1. No menu lateral do APIM, clique em **Products**.
2. Clique em **Add**.
3. Preencha:

   | Campo                 | Valor                                         |
   | --------------------- | --------------------------------------------- |
   | Display name          | Desenvolvimento                               |
   | Id                    | desenvolvimento                               |
   | Description           | Acesso para desenvolvedores e testes internos |
   | Published             | Sim                                           |
   | Requires subscription | Sim                                           |
   | Requires approval     | Nao                                           |

4. Na seção **APIs**, clique em **Add API** e selecione **API de Tarefas**.
5. Clique em **Create**.

### 5.3 Obter a chave de assinatura

1. No menu lateral, clique em **Subscriptions**.
2. Clique em **Add subscription**.
3. Preencha:

   | Campo        | Valor                         |
   | ------------ | ----------------------------- |
   | Name         | sub-desenvolvimento-teste     |
   | Display name | Assinatura de Desenvolvimento |
   | Scope        | Product                       |
   | Product      | Desenvolvimento               |

4. Clique em **Create**.
5. Na lista de assinaturas, clique nos três pontos `...` ao lado da assinatura criada.
6. Clique em **Show/hide keys**.
7. Copie o valor de **Primary key**.

Essa chave será enviada em cada requisição no cabeçalho `Ocp-Apim-Subscription-Key`.

---

## Parte 6 — Testar a API

Esta seção apresenta três formas de testar a API. Recomenda-se executar os testes nas três formas para familiarizar-se com cada ferramenta.

| Ferramenta            | Tipo      | Indicada para                                      |
| --------------------- | --------- | -------------------------------------------------- |
| Portal do APIM (Test) | Interface | Verificação rápida sem instalar nada               |
| Bruno                 | Interface | Testes visuais com suporte a variáveis de ambiente |
| curl                  | Terminal  | Automação, scripts e ambientes sem interface       |

---

### 6.1 Testar pelo portal do APIM (sem instalar nada)

Esta é a forma mais rápida e não requer nenhuma ferramenta adicional.

1. No menu lateral do APIM, clique em **APIs**.
2. Selecione **API de Tarefas**.
3. Clique na operação `GET /tasks`.
4. Clique na aba **Test** (no painel central, ao lado de Design e Settings).
5. Role até a seção **Headers** e confirme que o campo `Ocp-Apim-Subscription-Key` já está preenchido automaticamente com a chave da assinatura.
6. Clique em **Send**.

**O que você deve ver:** No painel inferior, a resposta com status `200 OK` e o JSON com a lista de tarefas.

Para testar a criação de uma tarefa (`POST /tasks`):

1. Clique na operação `POST /tasks` no painel esquerdo.
2. Clique na aba **Test**.
3. Na seção **Request body**, mantenha o tipo `application/json` e substitua o conteúdo por:
   ```json
   {
     "title": "Tarefa criada pelo portal",
     "description": "Teste via aba Test do APIM",
     "completed": false
   }
   ```
4. Clique em **Send**.

**O que você deve ver:** Status `201 Created` e o JSON da tarefa criada com o ID gerado.

---

### 6.2 Testar com Bruno (VS Code)

O Bruno é uma extensão gratuita e open source para o VS Code que permite testar APIs com suporte completo a variáveis de ambiente, sem necessidade de licença ou conta.

> **Por que Bruno e não Thunder Client ou Postman:** O Thunder Client requer licença paga para usar variáveis de ambiente. O Postman exige criação de conta. O Bruno oferece ambas as funcionalidades de forma completamente gratuita e funciona offline.

#### Instalação

1. Abra o VS Code.
2. Clique no ícone de extensões na barra lateral esquerda (ou pressione `Ctrl+Shift+X`).
3. Pesquise por **Bruno**.
4. Clique em **Install** na extensão publicada por **Bruno**.
5. Após instalar, um ícone do Bruno aparecerá na barra lateral esquerda (um hexágono laranja).

#### Criar uma coleção

No Bruno, as requisições são organizadas em **coleções**, que são pastas salvas no disco. Isso permite versionar os testes junto com o código no Git.

1. Clique no ícone do Bruno na barra lateral.
2. Clique em **Create Collection**.
3. Preencha o nome como `APIM - Tarefas`.
4. Escolha uma pasta no seu computador para salvar a coleção (por exemplo, dentro da pasta do projeto).
5. Clique em **Create**.

A coleção aparecerá no painel do Bruno.

#### Configurar variáveis de ambiente

1. Com a coleção `APIM - Tarefas` visível no painel, clique nos três pontos `...` ao lado do nome da coleção.
2. Selecione **Settings**.
3. Clique na aba **Environments**.
4. Clique em **Add Environment** e nomeie como `desenvolvimento`.
5. Clique em **Add Variable** e preencha a primeira variável:
   - **Name:** `base_url`
   - **Value:** `https://apim-tarefas-<seunome>.azure-api.net/tarefas`
6. Clique em **Add Variable** novamente e preencha a segunda variável:
   - **Name:** `subscription_key`
   - **Value:** cole aqui sua chave primária copiada na Parte 5
7. Clique em **Save**.
8. No painel do Bruno, selecione o ambiente **desenvolvimento** no seletor que aparece no canto superior da coleção.

As variáveis são referenciadas nas requisições com a sintaxe `{{nome_da_variavel}}`.

#### Teste 1 — Listar todas as tarefas (GET)

1. Clique nos três pontos `...` ao lado da coleção e selecione **New Request**.
2. Preencha:
   - **Name:** `GET - Listar tarefas`
   - **Method:** `GET`
   - **URL:** `{{base_url}}/tasks`
3. Clique em **Create**.
4. Na tela da requisição, clique na aba **Headers** e adicione:
   - **Name:** `Ocp-Apim-Subscription-Key`
   - **Value:** `{{subscription_key}}`
5. Clique em **Send** (o botão de seta no canto superior direito da requisição).

**O que você deve ver:** No painel inferior, status `200 OK` e o JSON com a lista de tarefas.

#### Teste 2 — Buscar tarefa por ID (GET)

1. Crie uma nova requisição na coleção.
2. Preencha:
   - **Name:** `GET - Buscar tarefa por ID`
   - **Method:** `GET`
   - **URL:** `{{base_url}}/tasks/1`
3. Adicione o header `Ocp-Apim-Subscription-Key: {{subscription_key}}`.
4. Clique em **Send**.

**O que você deve ver:** Status `200 OK` e o JSON apenas da tarefa com ID 1.

#### Teste 3 — Criar uma tarefa (POST)

1. Crie uma nova requisição na coleção.
2. Preencha:
   - **Name:** `POST - Criar tarefa`
   - **Method:** `POST`
   - **URL:** `{{base_url}}/tasks`
3. Adicione o header `Ocp-Apim-Subscription-Key: {{subscription_key}}`.
4. Clique na aba **Body**, selecione **JSON** e cole:
   ```json
   {
     "title": "Tarefa via Bruno",
     "description": "Criada com o cliente open source no VS Code",
     "completed": false
   }
   ```
5. Clique em **Send**.

**O que você deve ver:** Status `201 Created` e o JSON da tarefa criada com o ID gerado.

#### Teste 4 — Requisição sem chave (deve falhar)

1. Crie uma nova requisição na coleção.
2. Preencha:
   - **Name:** `GET - Sem autenticacao (deve retornar 401)`
   - **Method:** `GET`
   - **URL:** `{{base_url}}/tasks`
3. **Não adicione** nenhum header.
4. Clique em **Send**.

**O que você deve ver:** Status `401 Unauthorized` com a mensagem:
```json
{
  "statusCode": 401,
  "message": "Access denied due to missing subscription key..."
}
```

Isso confirma que o gateway está rejeitando requisições sem autenticação antes mesmo de encaminhá-las ao App Service.

---

### 6.3 Testar com curl (terminal)

> **Nota para usuários Windows:** O `curl` está disponível nativamente no Windows 10 a partir da versão 1803 (abril de 2018) e no Windows 11. Para verificar se está disponível, abra o **Prompt de Comando** (`cmd`) e execute:
> ```
> curl --version
> ```
> Se retornar a versão, está disponível. Caso contrário, use o Bruno (seção 6.2) ou o portal do APIM (seção 6.1).

> **Atenção ao usar PowerShell:** O PowerShell tem um comando interno chamado `curl` que é na verdade um alias para `Invoke-WebRequest`, com sintaxe completamente diferente do curl real. Para evitar confusão, use sempre o **Prompt de Comando (cmd)** para os comandos abaixo. Para abrir o cmd: pressione `Win + R`, digite `cmd` e pressione Enter.

Substitua os valores marcados com `<>` pelos seus antes de executar.

**Listar todas as tarefas:**
```cmd
curl -X GET "https://apim-tarefas-<seunome>.azure-api.net/tarefas/tasks" -H "Ocp-Apim-Subscription-Key: <sua-chave>"
```

**Buscar tarefa por ID:**
```cmd
curl -X GET "https://apim-tarefas-<seunome>.azure-api.net/tarefas/tasks/1" -H "Ocp-Apim-Subscription-Key: <sua-chave>"
```

**Criar uma tarefa:**
```cmd
curl -X POST "https://apim-tarefas-<seunome>.azure-api.net/tarefas/tasks" -H "Ocp-Apim-Subscription-Key: <sua-chave>" -H "Content-Type: application/json" -d "{\"title\": \"Tarefa via curl\", \"description\": \"Criada pelo terminal\", \"completed\": false}"
```

> **Por que as aspas estão diferentes:** No Windows (cmd), as aspas duplas dentro do JSON precisam ser escapadas com `\"`. No Linux e macOS, usa-se aspas simples para envolver o JSON e aspas duplas por dentro, sem necessidade de escape.

**Testar sem a chave (deve retornar 401):**
```cmd
curl -X GET "https://apim-tarefas-<seunome>.azure-api.net/tarefas/tasks"
```

---

### 6.4 Testar o rate limit

Este teste verifica se a política de rate limit configurada na Parte 4 está funcionando.

**Via Bruno:** Abra a requisição `GET - Listar tarefas` e clique em **Send** mais de 10 vezes seguidas em menos de 1 minuto. Observe o status mudar de `200` para `429` no painel de resposta a partir da 11ª requisição.

**Via curl no Prompt de Comando:**

```cmd
for /L %i in (1,1,12) do curl -s -o NUL -w "Requisicao %i: %%{http_code}\n" "https://apim-tarefas-<seunome>.azure-api.net/tarefas/tasks" -H "Ocp-Apim-Subscription-Key: <sua-chave>"
```

**O que você deve ver:** As primeiras 10 requisições retornam `200`. A partir da 11ª, o APIM retorna `429 Too Many Requests` sem encaminhar nada ao App Service. Aguarde 60 segundos para o contador resetar.

---

## Parte 7 — Monitoramento

### 7.1 Visualizar métricas no portal

1. Na página da instância APIM, clique em **Metrics** no menu lateral (dentro de Monitoring).
2. Clique em **Add metric** e adicione:
   - **Requests** — total de requisições recebidas
   - **Failed requests** — requisições com erro
   - **Gateway capacity** — uso do gateway (plano Consumption não exibe esta métrica)
3. Ajuste o intervalo de tempo para os últimos 30 minutos.
4. Clique em **Pin to dashboard** para fixar o gráfico no painel principal.

### 7.2 Analisar requisições individuais

1. No menu lateral, clique em **APIs**.
2. Selecione sua API.
3. Clique na aba **Analytics** (ou acesse via menu lateral em Monitoring > Analytics).
4. Você verá:
   - Volume de requisições por operação
   - Latência média por operação
   - Taxa de erros
   - Distribuição geográfica das requisições

### 7.3 Ativar o Application Insights (opcional)

Para logs detalhados de cada requisição:

1. No menu lateral, vá em **Monitoring** > **Application Insights**.
2. Clique em **Enable**.
3. Crie ou selecione uma instância do Application Insights.
4. Defina o **Sampling** em 100% para capturar todas as requisições (adequado para volumes baixos de desenvolvimento).
5. Clique em **Save**.

A partir desse ponto, cada requisição ao APIM gera um registro detalhado no Application Insights com headers, corpo, tempo de resposta e eventuais erros.

---

## Referência de conceitos

### Gateway URL vs. Back-end URL

| Endereço                                               | Descrição                                                  |
| ------------------------------------------------------ | ---------------------------------------------------------- |
| `https://apim-tarefas-<seunome>.azure-api.net/tarefas` | URL pública do gateway. Deve ser usada pelos consumidores. |
| `https://fastapi-demo-<seunome>.azurewebsites.net`     | URL do App Service. Nunca deve ser exposta diretamente.    |

### Políticas mais utilizadas

| Política                    | Elemento XML                  | Finalidade                                         |
| --------------------------- | ----------------------------- | -------------------------------------------------- |
| Rate limiting               | `rate-limit-by-key`           | Limitar requisições por chave, IP ou usuário       |
| Autenticação JWT            | `validate-jwt`                | Validar tokens JWT (Azure AD, Auth0, etc.)         |
| Transformação de requisição | `set-header`, `set-body`      | Modificar cabeçalhos e corpo antes de enviar       |
| Cache                       | `cache-lookup`, `cache-store` | Armazenar respostas para reduzir carga no back-end |
| IP filtering                | `ip-filter`                   | Permitir ou bloquear faixas de endereços IP        |
| CORS                        | `cors`                        | Configurar permissões de origem cruzada            |
| Rewrite URL                 | `rewrite-uri`                 | Alterar o caminho antes de encaminhar ao back-end  |
| Mock response               | `mock-response`               | Retornar resposta simulada sem chamar o back-end   |

### Planos do APIM

| Plano       | Custo fixo       | SLA    | Indicado para                             |
| ----------- | ---------------- | ------ | ----------------------------------------- |
| Consumption | Nenhum (por uso) | 99,95% | Desenvolvimento, testes, cargas variáveis |
| Developer   | ~US$ 50/mês      | Nenhum | Desenvolvimento com mais recursos         |
| Basic       | ~US$ 140/mês     | 99,95% | Produção com volume baixo                 |
| Standard    | ~US$ 700/mês     | 99,95% | Produção com volume médio                 |
| Premium     | ~US$ 2800/mês    | 99,99% | Produção enterprise, multi-região         |

---

## Solução de problemas

### Operações não aparecem após importar pelo OpenAPI

**Causa:** O APIM não conseguiu ler o arquivo `/openapi.json` da aplicação durante o import, geralmente porque a URL estava incorreta ou a aplicação não estava respondendo no momento.

**Verificação:**
1. Abra a URL abaixo diretamente no navegador e confirme que retorna um JSON (não uma página de erro):
   ```
   https://fastapi-demo-<seunome>.azurewebsites.net/openapi.json
   ```
2. Se retornar erro 503 ou página em branco, o App Service não está em execução. Acesse o Log stream da Web App para identificar o problema de inicialização.
3. Se a URL carregar corretamente mas o APIM ainda não importar as operações, delete a API criada, aguarde 1 minuto e repita o processo da Parte 2.

---

### Erro 401 — Subscription key inválida ou ausente

**Causa:** A requisição não contém o cabeçalho `Ocp-Apim-Subscription-Key` ou a chave está incorreta.

**Verificação:**
1. Confirme que a chave foi copiada corretamente (sem espaços).
2. Verifique se a assinatura está associada ao produto correto.
3. Verifique se o produto está associado à API.

---

### Erro 404 — Not Found no gateway

**Causa:** O sufixo da API ou o caminho da operação está incorreto.

**Verificação:**
1. No APIM, vá em **APIs** > selecione a API > aba **Settings**.
2. Confirme o valor em **API URL suffix**.
3. A URL completa do endpoint deve ser: `https://<gateway>/tarefas/tasks` (sufixo + caminho FastAPI).

---

### Erro 502 — Bad Gateway

**Causa:** O APIM conseguiu receber a requisição, mas não conseguiu se comunicar com o App Service.

**Verificação:**
1. Confirme que o App Service está em execução: acesse a URL direta do App Service no navegador.
2. Verifique a **Web service URL** nas configurações da API no APIM.
3. Verifique os logs do App Service em **Log stream** no portal.

---

### Erro 429 — Too Many Requests

**Causa:** O rate limit configurado na política foi atingido.

**Comportamento esperado:** Esse erro é intencional. Aguarde o período de renovação (60 segundos na configuração deste guia) e tente novamente.

---

## Limpeza de recursos

Para remover todos os recursos criados neste guia e no guia anterior:

1. No portal, vá em **Resource groups**.
2. Clique em `rg-fastapi-demo`.
3. Clique em **Delete resource group**.
4. Digite `rg-fastapi-demo` no campo de confirmação.
5. Clique em **Delete**.

A exclusão remove em uma única operação o APIM, o App Service, o App Service Plan e todos os recursos associados ao grupo.

> **Observacao:** A exclusão de uma instância do APIM pode levar alguns minutos para ser concluída no plano Consumption.
