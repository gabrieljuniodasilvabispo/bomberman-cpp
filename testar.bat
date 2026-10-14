@echo off
setlocal
cd /d "%~dp0"
set "PATH=C:\msys64\ucrt64\bin;%PATH%"
mingw32-make test
set "resultado=%errorlevel%"
echo.
echo Na fase TDD inicial, falhas por comportamento nao implementado sao esperadas.
pause
exit /b %resultado%
