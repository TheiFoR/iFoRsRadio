@echo off
setlocal

REM Путь к исполняемому файлу
set "EXE_PATH=build\iFoRsRadio\iFoRsRadio.exe"

REM Проверка, существует ли файл
if not exist "%EXE_PATH%" (
    echo Файл %EXE_PATH% не найден.
    exit /b 1
)

REM Запуск 30 экземпляров
for /L %%i in (1,1,30) do (
    echo Запуск экземпляра %%i
    start "" "%EXE_PATH%"
    REM timeout /nobreak /t 1 >nul
)

echo Все 30 экземпляров запущены.
endlocal
pause
