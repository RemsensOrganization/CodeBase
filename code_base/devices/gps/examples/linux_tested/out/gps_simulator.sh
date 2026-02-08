#!/bin/bash

GPS_DATA_FILE="gps_data.nmea"
COM_PORT="/tmp/gpsCOM"

echo "GPS emulator starts..."
echo "NMEA source: $GPS_DATA_FILE"
echo "COM-port: $COM_PORT"

# Проверка файла
if [ ! -f "$GPS_DATA_FILE" ]; then
    echo "Файл $GPS_DATA_FILE не найден!"
    exit 1
fi

# Проверка бинарника
if [ ! -x "gps_linux" ]; then
    echo "gps_linux не найден или не исполняемый!"
    exit 1
fi

# Подаём строки по одной с задержкой
while true; do
  while read line; do
    echo "$line"
    sleep 1   # задержка 1 секунда между строками
  done < "$GPS_DATA_FILE"
done | socat -d -d - PTY,link=$COM_PORT,raw,echo=0 &
SOCAT_PID=$!

# Убиваем socat при выходе
trap "kill $SOCAT_PID" EXIT

# Ждём чуть-чуть, чтобы socat успел создать порт
sleep 3
ls -l $COM_PORT

# Запускаем приложение
./gps_linux -c "$COM_PORT"
