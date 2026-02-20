@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

rem === Настройки под вашу сеть ===
set "IFACE=Ethernet"
set "NET=192.168.1"
set "START_HOST=100"
set "END_HOST=200"
set "MASK=255.255.255.0"
set "GATEWAY=192.168.1.11"

set "FREEIP="

echo Поиск свободного IP в диапазоне %NET%.%START_HOST%-%NET%.%END_HOST%...

for /L %%H in (%START_HOST%,1,%END_HOST%) do (
    set "CANDIP=%NET%.%%H"
    echo Проверяю !CANDIP! ...
    ping -n 1 -w 300 !CANDIP! >nul
    if !errorlevel! NEQ 0 (
        echo Найден кандидат: !CANDIP!
        set "FREEIP=!CANDIP!"
        goto :after_loop
    )
)

:after_loop
if not defined FREEIP (
    echo Не удалось найти свободный IP в заданном диапазоне.
    goto :eof
)

echo Настраиваю интерфейс %IFACE% на %FREEIP% ...

netsh interface ip set address name="%IFACE%" static %FREEIP% %MASK% %GATEWAY% 1

echo Готово. Используется IP: %FREEIP%
endlocal
