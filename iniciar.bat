@echo off
setlocal
cd /d "%~dp0"
set "PATH=C:\msys64\ucrt64\bin;%PATH%"
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
if errorlevel 1 goto erro
cmake --build build --target Bomberman --parallel 4
if errorlevel 1 goto erro
"%~dp0build\Bomberman.exe"
if errorlevel 1 goto erro
exit /b 0
:erro
echo.
echo Nao foi possivel iniciar o jogo. Confira o erro acima.
pause
exit /b 1
