# Configuração do Ambiente — Raylib

Antes de começar as aulas, você precisa instalar a biblioteca Raylib e um compilador C na sua máquina. Este guia cobre os três sistemas operacionais mais comuns.

> **Importante:** vá até o final do guia da sua plataforma e rode o **teste de instalação**. Se o teste falhar, você ainda não está pronto para a Aula 1.

---

## Índice

- [macOS](#macos)
- [Linux (Ubuntu, Debian, Mint, Pop!_OS)](#linux-ubuntu-debian-mint-popos)
- [Linux (Arch, Manjaro)](#linux-arch-manjaro)
- [Linux (Fedora)](#linux-fedora)
- [Windows](#windows)
- [Teste de instalação](#teste-de-instalação)
- [Problemas comuns](#problemas-comuns)

---

## macOS

### 1. Instale o Homebrew

O Homebrew é o gerenciador de pacotes do macOS. Se você ainda não tem, abra o **Terminal** e rode:

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

### 2. Instale as ferramentas de compilação

```bash
xcode-select --install
```

Se aparecer uma janela pedindo para instalar, aceite. Se já estiver instalado, vai dizer que já existe — pode ignorar.

### 3. Instale a Raylib

```bash
brew install raylib
```

### 4. Descubra onde o Homebrew instalou

No terminal, rode:

```bash
brew --prefix raylib
```

- Se aparecer `/opt/homebrew/...` → você está em um Mac **Apple Silicon** (M1, M2, M3, M4).
- Se aparecer `/usr/local/...` → você está em um Mac **Intel**.

Guarde essa informação — você vai usar nos scripts de compilação.

---

## Linux (Ubuntu, Debian, Mint, Pop!_OS)

### 1. Atualize os pacotes e instale o compilador

```bash
sudo apt update
sudo apt install build-essential
```

### 2. Instale a Raylib

A versão dos repositórios oficiais costuma ser suficiente para esta disciplina:

```bash
sudo apt install libraylib-dev
```

> Se sua distro tiver uma versão muito antiga da Raylib (anterior à 4.0), siga as instruções de compilação manual em <https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux>.

---

## Linux (Arch, Manjaro)

```bash
sudo pacman -S base-devel raylib
```

---

## Linux (Fedora)

```bash
sudo dnf install gcc make
sudo dnf install raylib raylib-devel
```

---

## Windows

No Windows, o caminho mais simples é usar o **w64devkit** ou o **MSYS2**. Vamos com o **w64devkit** por ser mais leve e portátil.

### 1. Baixe o w64devkit

Acesse <https://github.com/skeeto/w64devkit/releases> e baixe o arquivo `w64devkit-x.y.z.exe` mais recente (auto-extrator).

### 2. Extraia para `C:\w64devkit`

Execute o arquivo baixado e escolha extrair direto para `C:\`. Isso vai criar a pasta `C:\w64devkit`.

### 3. Adicione ao PATH

1. Abra o **Iniciar** e digite "variáveis de ambiente" → clique em **Editar as variáveis de ambiente do sistema**.
2. Clique em **Variáveis de Ambiente...**.
3. Em **Variáveis do sistema**, encontre `Path`, selecione e clique em **Editar**.
4. Clique em **Novo** e adicione: `C:\w64devkit\bin`
5. Clique **OK** em todas as janelas.

### 4. Teste o compilador

Abra um **novo** PowerShell ou CMD (precisa ser novo para reconhecer o PATH atualizado) e rode:

```cmd
gcc --version
```

Se aparecer a versão do GCC, está funcionando.

### 5. Baixe a Raylib pré-compilada

1. Acesse <https://github.com/raysan5/raylib/releases>
2. Baixe o arquivo `raylib-X.Y_win64_mingw-w64.zip` (versão mais recente).
3. Extraia para `C:\raylib`. Você deve ter uma estrutura tipo:
   ```
   C:\raylib\
   ├── include\        (contém raylib.h)
   ├── lib\            (contém libraylib.a)
   └── ...
   ```

### 6. Atualize os scripts

Nos exemplos das aulas, o `compile.bat` já está configurado esperando a Raylib em `C:\raylib`. Se você escolheu outro caminho, ajuste o arquivo `.bat`.

---

## Teste de instalação

Crie um arquivo chamado `teste.c` em qualquer pasta com este conteúdo:

```c
#include "raylib.h"

int main(void) {
    InitWindow(640, 360, "Teste de Instalacao - Raylib");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(BLACK);
            DrawText("Funcionou! :)", 200, 160, 40, GREEN);
            DrawText("Pressione ESC para sair", 180, 220, 20, RAYWHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
```

### Compilando no macOS (Apple Silicon)

```bash
gcc teste.c -o teste \
  -I/opt/homebrew/include \
  -L/opt/homebrew/lib \
  -lraylib \
  -framework CoreVideo -framework IOKit -framework Cocoa \
  -framework GLUT -framework OpenGL

./teste
```

### Compilando no macOS (Intel)

```bash
gcc teste.c -o teste \
  -I/usr/local/include \
  -L/usr/local/lib \
  -lraylib \
  -framework CoreVideo -framework IOKit -framework Cocoa \
  -framework GLUT -framework OpenGL

./teste
```

### Compilando no Linux

```bash
gcc teste.c -o teste -lraylib -lm
./teste
```

### Compilando no Windows (w64devkit)

```cmd
gcc teste.c -o teste.exe -IC:\raylib\include -LC:\raylib\lib -lraylib -lopengl32 -lgdi32 -lwinmm
teste.exe
```

Se uma janela preta abriu com o texto "Funcionou!" em verde, **parabéns, está tudo certo**. Pode partir para a [Aula 1](./docs/aula1-primeira-janela/README.md).

---

## Problemas comuns

### "fatal error: 'raylib.h' file not found"

O compilador não encontrou o cabeçalho da Raylib. Verifique:

- **macOS:** você usou as flags `-I/opt/homebrew/include` (Apple Silicon) ou `-I/usr/local/include` (Intel)?
- **Linux:** instalou o pacote `libraylib-dev` (e não só `libraylib`)?
- **Windows:** o caminho `-IC:\raylib\include` está correto e o arquivo `raylib.h` existe nessa pasta?

### "undefined reference to `InitWindow`" (ou outra função)

O compilador achou o header mas não a biblioteca em si. Confira:

- Você incluiu `-lraylib` no comando?
- No macOS, você incluiu os `-framework` no final?
- No Windows, você incluiu `-lopengl32 -lgdi32 -lwinmm`?

### A janela abre e fecha imediatamente no Windows

Você executou de dentro de um IDE que fecha o terminal ao terminar? Tente rodar pelo **CMD** ou **PowerShell** manualmente.

### "command not found: gcc" no macOS

Você não instalou as ferramentas de linha de comando do Xcode. Rode:

```bash
xcode-select --install
```

### Permissão negada ao rodar `./teste` no Linux/macOS

Dê permissão de execução:

```bash
chmod +x teste
./teste
```

---

Se ainda tiver problemas, traga prints para o professor ou poste no grupo da turma. **Não pule para a Aula 1 sem o teste funcionando.**
