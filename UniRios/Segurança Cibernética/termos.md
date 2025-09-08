# Termos de Segurança Cibernética em Inglês e Português com Exemplos de Uso

## Conceitos Fundamentais

- **Cybersecurity** (Segurança Cibernética): Conjunto de práticas, processos e tecnologias usadas para proteger sistemas, redes e dados contra ataques digitais.

- **Threat** (Ameaça): Qualquer potencial evento ou ação que possa comprometer a segurança de sistemas ou dados.

- **Vulnerability** (Vulnerabilidade): Uma fraqueza em um sistema ou software que pode ser explorada por uma ameaça.

- **Exploit** (Exploração): Um código ou técnica que tira proveito de uma vulnerabilidade para realizar um ataque.

- **Risk** (Risco): Combinação da probabilidade de uma ameaça explorar uma vulnerabilidade e do impacto resultante.

## Tipos de Ataques

- **Phishing** (Phishing): Tentativa de enganar usuários para que revelem informações sensíveis, geralmente por e-mail ou mensagens falsas.
  
  Exemplo: Um e-mail falso pedindo para "atualizar sua senha do banco".

- **Malware** (Malware): Qualquer software malicioso projetado para danificar ou obter acesso não autorizado a sistemas.
  
  Exemplo: Vírus, worms, trojans, ransomware.

- **Ransomware** (Ransomware): Tipo de malware que criptografa os arquivos da vítima e exige pagamento para liberá-los.
  
  Exemplo: WannaCry (2017).

- **Denial of Service (DoS)** (Negação de Serviço): Ataque que sobrecarrega um sistema ou rede, tornando-o indisponível.
  
  Exemplo: Enviar milhares de requisições a um servidor até que ele caia.

- **Man-in-the-Middle (MitM)** (Homem no Meio): Ataque onde o invasor intercepta e possivelmente altera a comunicação entre duas partes.
  
  Exemplo: Captura de tráfego em redes Wi-Fi públicas.

- **SQL Injection** (Injeção de SQL): Exploração de falhas em consultas SQL para manipular ou acessar dados não autorizados.
  
  ```sql
  SELECT * FROM usuarios WHERE id = '' OR '1'='1';
  ```

## Defesas e Boas Práticas

**Firewall** (Firewall): Dispositivo ou software que monitora e controla o tráfego de rede com base em regras de segurança.

  Exemplo: Bloquear conexões de IPs suspeitos.

**Antivirus** (Antivírus): Software projetado para detectar e remover malware.

**Encryption** (Criptografia): Técnica de transformar dados em formato ilegível para proteger sua confidencialidade.

  ```bash
  openssl enc -aes-256-cbc -in arquivo.txt -out arquivo.enc
  ```

**Authentication** (Autenticação): Processo de verificar a identidade de um usuário ou sistema.

  Exemplo: Login com usuário e senha.

**Multi-Factor Authentication (MFA)** (Autenticação de Múltiplos Fatores): Exige mais de uma forma de verificação.

  Exemplo: Senha + Código enviado por SMS.

**Access Control** (Controle de Acesso): Definição de permissões para garantir que apenas usuários autorizados possam acessar recursos.

## Conceitos Avançados

**Intrusion Detection System (IDS)** (Sistema de Detecção de Intrusos): Monitora a rede para detectar atividades maliciosas.

**Intrusion Prevention System (IPS)** (Sistema de Prevenção de Intrusos): Além de detectar, também bloqueia ataques.

**Zero-Day** (Dia-Zero): Vulnerabilidade desconhecida pelo fabricante e ainda sem correção disponível.

**Patch** (Correção): Atualização de software para corrigir falhas de segurança ou bugs.

**Incident Response** (Resposta a Incidentes): Conjunto de procedimentos para detectar, responder e recuperar de ataques cibernéticos.

**Security Policy** (Política de Segurança): Conjunto de diretrizes e regras que definem como a segurança deve ser implementada e mantida.

**Penetration Test (Pentest)** (Teste de Penetração): Simulação de ataque para avaliar a segurança de um sistema.

  Exemplo: Utilizar **Metasploit** para verificar vulnerabilidades em um servidor.

**Social Engineering** (Engenharia Social): Manipulação psicológica de pessoas para obter informações ou acesso a sistemas.

  Exemplo: Alguém ligando fingindo ser do suporte técnico pedindo senha.

## Normas e Certificações

**ISO/IEC 27001** (ISO/IEC 27001): Norma internacional para gestão de segurança da informação.

**GDPR** (RGPD - Regulamento Geral de Proteção de Dados): Lei europeia de proteção de dados pessoais.

**HIPAA** (HIPAA - Lei de Portabilidade e Responsabilidade de Seguros de Saúde): Legislação dos EUA para proteção de dados de saúde.

**SOC 2** (SOC 2): Certificação que avalia controles de segurança e privacidade em provedores de serviços.
