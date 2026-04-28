# Guia de Setup — Antes do Laboratório

> Este guia deve ser seguido **antes** do dia do laboratório. Estimativa: 30-45 min.

## 1. Windows

### Ferramentas (download e instalação)

| Software                                       | Link                                                                          | Tamanho |
| ---------------------------------------------- | ----------------------------------------------------------------------------- | ------- |
| CPU-Z                                          | <https://www.cpuid.com/softwares/cpu-z.html>                                  | ~3 MB   |
| HWiNFO64                                       | <https://www.hwinfo.com/download/>                                            | ~5 MB   |
| CrystalDiskMark                                | <https://crystalmark.info/en/software/crystaldiskmark/>                       | ~5 MB   |
| AIDA64 Trial                                   | <https://www.aida64.com/downloads>                                            | ~50 MB  |
| Cinebench R23                                  | Microsoft Store ou site da Maxon                                              | ~600 MB |
| LatencyMon                                     | <https://www.resplendence.com/latencymon>                                     | ~1 MB   |
| Sysinternals Suite (Process Explorer + RAMMap) | <https://learn.microsoft.com/en-us/sysinternals/downloads/sysinternals-suite> | ~50 MB  |
| Python 3.10+                                   | <https://www.python.org/downloads/>                                           | ~30 MB  |

### Após instalar Python

```powershell
pip install numpy psutil
```

### Verificação rápida no Linux

```powershell
python --version
python -c "import numpy, psutil; print('OK')"
```

---

## 2. Linux (Ubuntu / Debian)

### Pacotes apt

```bash
sudo apt update
sudo apt install -y \
  hwloc dmidecode pciutils lshw \
  sysbench fio stress-ng hdparm \
  htop numactl \
  linux-tools-common linux-tools-generic \
  build-essential python3-pip
```

> **Nota sobre `perf`:** o pacote `linux-tools-generic` instala o `perf` genérico. Se sua versão de kernel for específica, instale também `linux-tools-$(uname -r)`.

### Pacotes Python

```bash
pip3 install --user numpy psutil
```

### Verificação rápida

```bash
lscpu | head -5
sysbench --version
fio --version
python3 -c "import numpy, psutil; print('OK')"
```

---

## 3. Clonar este repositório

```bash
git clone <URL_DO_REPO> infra-hardware-lab
cd infra-hardware-lab
```

## 4. Tornar os scripts executáveis (Linux)

```bash
chmod +x docs/aula1-anatomia-cpu-pipeline-multicore/scripts/*.sh
chmod +x docs/aula4-barramentos-io-interrupcoes/scripts/*.sh
```

## 5. Teste inicial — rode o script da Aula 1

```bash
python3 docs/aula1-anatomia-cpu-pipeline-multicore/scripts/teste_paralelismo.py
```

Se ele rodar e exibir uma tabela com tempos e speedup, você está pronto para o laboratório.

---

## Problemas comuns

| Problema                             | Solução                                                                                             |
| ------------------------------------ | --------------------------------------------------------------------------------------------------- |
| `sudo: perf: command not found`      | `sudo apt install linux-tools-$(uname -r)`                                                          |
| Python `ModuleNotFoundError: psutil` | `pip3 install --user psutil` ou use `python -m pip install psutil`                                  |
| Cinebench muito lento                | Em "File → Advanced → Custom Test Duration: Off", rodar versão mínima                               |
| `fio` permission denied              | Rode com sudo ou ajuste o caminho de `--filename`                                                   |
| Script Python trava no Windows       | Garanta que está usando Python 3.10+ e que o `multiprocessing` está com `set_start_method("spawn")` |
