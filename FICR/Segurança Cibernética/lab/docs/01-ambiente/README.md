# 01 · Preparação do Ambiente

> [← Anterior: Introdução](../00-introducao/README.md) · [Voltar ao índice](../../README.md) · [Próximo: Reconhecimento Passivo →](../02-recon-passivo/README.md)

---

## Objetivos

- Configurar uma estação de pentest segura e reprodutível
- Entender o papel de cada ferramenta de anonimato
- Organizar notas e evidências desde o início

---

## 1. Sistema Operacional

| Ferramenta | Quando usar |
|---|---|
| **Kali Linux** | Padrão da disciplina — +600 ferramentas pré-instaladas |
| **Parrot OS Security** | Alternativa mais leve, bom para notebooks modestos |
| **BlackArch** | Arch-based, +2800 ferramentas, para usuários avançados |
| **Commando VM** | Windows para pentest em ambientes Active Directory |

### Instalação recomendada (Kali)

```bash
# Em VM (VirtualBox/VMware) — NUNCA como SO principal em máquina pessoal com dados sensíveis
# ISO oficial: https://www.kali.org/get-kali/

# Após instalar:
sudo apt update && sudo apt full-upgrade -y
sudo apt install -y kali-linux-large   # meta-pacote com 95% do que usaremos
sudo apt install -y pipx git tmux neovim
pipx ensurepath
```

### Snapshots

**Sempre** tire snapshot da VM antes de:
- Instalar novas ferramentas experimentais
- Iniciar um engagement real
- Rodar exploits com payloads customizados

---

## 2. Anonimato Operacional

> **Importante:** anonimato não é sinônimo de impunidade. É uma camada de proteção operacional para o pentester, não licença para agir sem autorização.

| Ferramenta | Função |
|---|---|
| **ProxyChains** | Encadeia proxies SOCKS4/5/HTTP para qualquer ferramenta CLI |
| **Tor** | Rede de anonimato — útil em OSINT passivo |
| **Mullvad / ProtonVPN** | VPNs sem log, camada adicional |
| **WireGuard** | Túnel VPN moderno e rápido — útil para conectar à infra do cliente |

### Configuração básica do ProxyChains

```bash
# /etc/proxychains4.conf
# Comente "strict_chain" e descomente "dynamic_chain"
# Adicione proxies no final:
socks5 127.0.0.1 9050   # Tor local

# Uso:
proxychains4 nmap -sT -Pn alvo.com.br
```

---

## 3. Multiplexador de Terminal (sessões persistentes)

Sessões SSH caem. Scans de 12 horas não podem depender de uma janela de terminal aberta.

```bash
# Tmux básico
tmux new -s pentest           # cria sessão
# Ctrl+b, d                   # destaca (detach)
tmux ls                       # lista sessões
tmux attach -t pentest        # reconecta

# Splits
# Ctrl+b, %                   # split vertical
# Ctrl+b, "                   # split horizontal
# Ctrl+b, setas               # navegar
```

Alternativa: **GNU Screen** (mais antigo, mais simples, ainda útil).

---

## 4. Gerenciamento de Notas e Evidências

Um pentest sem notas é um pentest perdido. Escolha **uma** ferramenta e comprometa-se com ela.

| Ferramenta | Perfil |
|---|---|
| **CherryTree** | Árvore hierárquica, screenshots inline, ideal para iniciantes |
| **Obsidian** | Markdown com links bidirecionais — ótimo para mapear relações |
| **Joplin** | Open-source, com sincronização criptografada |
| **Ghostwriter** | Plataforma web colaborativa para equipes Red Team |

### Estrutura de pastas sugerida

```
~/engagements/
└── cliente-X-2026Q2/
    ├── 00-escopo/
    │   ├── contrato.pdf
    │   ├── roe.md
    │   └── contatos.md
    ├── 01-recon/
    │   ├── subdomains.txt
    │   ├── shodan.json
    │   └── screenshots/
    ├── 02-scans/
    │   ├── nmap-tcp-full.xml
    │   └── nuclei-results.jsonl
    ├── 03-findings/
    │   ├── SQLi-001/
    │   │   ├── evidencia-request.http
    │   │   ├── evidencia-response.png
    │   │   └── readme.md
    │   └── ...
    ├── 04-loot/
    │   └── (credenciais, hashes, tokens)
    └── 05-relatorio/
        └── final-v1.0.pdf
```

---

## 5. C2 Frameworks (para tópicos avançados)

Não usaremos em aulas iniciais, mas é importante conhecer:

| Framework | Licença | Perfil |
|---|---|---|
| **Sliver** | Open-source | Alternativa moderna ao Cobalt Strike |
| **Mythic** | Open-source | Arquitetura modular, multi-payload |
| **Havoc** | Open-source | Interface moderna, pós-exploração focada |
| **Covenant** | Open-source | .NET, antigo mas didático |
| **Cobalt Strike** | Comercial | Padrão da indústria Red Team |

---

## 6. Checklist de Ambiente Pronto

- [ ] Kali atualizado (`apt full-upgrade`)
- [ ] VM com snapshot limpa
- [ ] Tmux configurado
- [ ] CherryTree/Obsidian com template de engagement
- [ ] VPN do cliente testada
- [ ] Chave SSH gerada para o engagement
- [ ] Git configurado para versionar notas

---

## Exercício

Crie uma VM Kali e documente, no formato do template acima, a preparação para um engagement fictício chamado `cliente-dojo-2026`. Entregue o diretório zipado com pelo menos os arquivos `roe.md` e `contatos.md` preenchidos.

---

[← Anterior: Introdução](../00-introducao/README.md) · [Voltar ao índice](../../README.md) · [Próximo: Reconhecimento Passivo →](../02-recon-passivo/README.md)
