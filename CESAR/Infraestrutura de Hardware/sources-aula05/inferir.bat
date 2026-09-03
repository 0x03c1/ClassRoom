@echo off
REM Uso: inferir.bat 01_latch.v
REM Rode antes:  <pasta>\oss-cad-suite\environment.bat
echo === o que o Yosys inferiu de %1 ===
yosys -p "read_verilog %1; proc; opt; stat" 2>nul | findstr /R "\$ cells"
echo.
echo === o que o Verilator reclama de %1 ===
verilator --lint-only -Wall -Wno-DECLFILENAME %1 2>&1 | findstr /R "LATCH BLKSEQ CASEINCOMPLETE"
echo (nada acima = nenhum aviso)
