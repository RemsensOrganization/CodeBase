# GPS CLI Parser

Модуль для парсинга командной строки GPS-приложения на Qt.

## Описание

Класс `GpsCliParser` обрабатывает аргументы командной строки для GPS-устройств:
- Путь к лог-файлу
- COM-порт
- Скорость передачи данных (baud rate)  
- Флаг сохранения данных нулевого уровня
- Формат вывода данных (CSV/JSON)

## Параметры командной строки

```

Usage: gps_app [options]
GPS CLI module

Options:
-h, --help           Показать справку
-v, --version        Показать версию
-p, --path <path>    Путь к файлу (по умолчанию: gps_log.txt)
-c, --com <port>     COM порт (по умолчанию: /dev/pts/1)
-b, --baud <rate>    Скорость (по умолчанию: 9600)
-z, --zerolevel      Сохранять данные нулевого уровня (по умолчанию: false)
-f, --format <fmt>   Формат (CSV или JSON, по умолчанию: CSV)

```

## Примеры использования

```bash
# Показать справку
./gps_app --help

# Стандартный запуск
./gps_app

# Кастомные параметры
./gps_app -p /tmp/gps_data.csv -c /dev/ttyUSB0 -b 115200 -f JSON -z

# Windows
gps_app.exe -p C:\logs\gps.txt -c COM3 -b 9600
```



