@echo off
setlocal
cd /d "%~dp0"
set "PATH=C:\msys64\ucrt64\bin;%PATH%"
mingw32-make test
set "resultado=%errorlevel%"
echo.
if "%resultado%"=="0" (
    echo Testes concluidos com sucesso.
) else (
    echo Falha na compilacao ou nos testes. Confira as mensagens acima.
)
pause
exit /b %resultado%
