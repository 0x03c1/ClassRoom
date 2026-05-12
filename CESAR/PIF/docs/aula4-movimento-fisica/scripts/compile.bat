@echo off
REM Script de compilacao para Windows (w64devkit)
REM Assume que a Raylib esta instalada em C:\raylib

cd /d "%~dp0\.."

echo ^>^> Compilando exemplo.c...
gcc exemplo.c -o exemplo.exe ^
    -IC:\raylib\include ^
    -LC:\raylib\lib ^
    -lraylib -lopengl32 -lgdi32 -lwinmm

if errorlevel 1 (
    echo ^>^> Erro na compilacao.
    pause
    exit /b 1
)

echo ^>^> Compilacao OK. Executando...
exemplo.exe
pause
