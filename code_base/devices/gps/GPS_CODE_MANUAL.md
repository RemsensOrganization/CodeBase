# GPS Device

Qt-модуль для чтения NMEA из последовательного порта, парсинга в `GpsData`, уведомления подписчиков и (опционально) логирования данных в файл.  
Основной класс — `GPSDevice`, который внутри использует `GPSReceiver` (получение строк), `GPSParser` (парсинг) и `GpsDataBase`+`GpsPortAutoDetector` (автоопределение порта)


## 1. Быстрый старт

Для работы с GPS Device вам понадобится дирректория `src` данного проекта.

### - Подключение в qmake

Добавь `.pri` в свой `.pro`:

```pri
include(src/solutions/gps_device/gps_device.pri)
```

Для GUI-приложения также нужно:

```pri
QT += gui widgets
```

### - Минимальный пример

```cpp
#include "gps_device.h"

int main(int argc, char** argv)
{
    GPSDevice device;

    // Запуск с настройками по умолчанию (см. ниже раздел "Запуск")
    device.start();

    // ...
    device.stop();
}
```

`GPSDevice::start()` запускает приём данных в отдельном потоке, а `stop()` запрашивает остановку и ждёт завершения потока.

---

## 2. Запуск и остановка

У `GPSDevice` есть три варианта запуска:

```cpp
// 1) По умолчанию (автоопределение порта GPS при помощи файла gps_database.json,
// в который внесен список типовых GPS-приемников. Его можно дописать вручную)
device.start();

// 2) Явно указать порт (baud rate будет "default baudRate")
device.start("COM3");                // Windows пример
// device.start("/dev/ttyUSB0");     // Linux пример

// 3) Явно указать порт и baud rate
device.start("COM3", QSerialPort::Baud9600);
```

Проверка запущен ли приём:

```cpp
if (device.isRunning()) { /* ... */ }
```

Остановка:

```cpp
device.stop();
```

---

## 3. Подключение отображения (View)

Для UI/виджетов (или любой другой визуализации/вывода) используется интерфейс `IGpsView`.

Подключение:

```cpp
device.attachView(view);
```

Что делает `attachView()`:
- подписывает view на сигналы `gpsDataUpdated(GpsData)` и `gpsStatusChanged(QString)`;
- в качестве context-объекта у `connect` используется `view->asQObject()`, чтобы соединения автоматически разорвались при уничтожении view.

### - Пример: подключить простой виджет

Если у тебя есть реализация `IGpsView` (например `GpsSimpleWidget`), то использование выглядит так:

```cpp
auto* view = new GpsSimpleWidget(parent);
GPSDevice device;

device.attachView(view);
device.start("COM3", QSerialPort::Baud9600);
```

---

## 4. Логирование в файл

### - Сырые строки (origin / zero-level)

```cpp
device.writeOriginGpsDataToFile("your/fullpath/to/origin.txt");
//или
device.writeOriginGpsDataToFile();
```

По умолчанию файл называется `gpsDataOrigin.txt` и создается в дирректории с исполняемым файлом.

Повторный вызов с тем же путём ничего не делает (переподключение не выполняется).
Если повторно вызвать с другим путем, то произойдет переключение на лету целевого файла для записи.

### - Форматированные данные (CSV/JSON)

```cpp
device.writeFormattedGpsDataToFile(logger::saveFormat::csv,  "your/fullpath/to/formatted.txt");
device.writeFormattedGpsDataToFile(logger::saveFormat::jsonIndented, "your/fullpath/to/formatted.txt.txt"); //другой формат
//или
device.writeFormattedGpsDataToFile(logger::saveFormat::jsonIndented );
```

По умолчанию файл называется `gpsDataFormatted.txt` и создается в дирректории с исполняемым файлом.

Повторный вызов с тем же путём ничего не делает (переподключение не выполняется).
Если повторно вызвать с другим путем, то произойдет переключение на лету целевого файла для записи.

---

## 5. Сигналы и статус

`GPSDevice` отдаёт два сигнала:

- `gpsStatusChanged(const QString& msg)` — человекочитаемый статус.
- `gpsDataUpdated(const GpsData& data)` — новые распарсенные данные.

---

## 6. EmitMode: принцип работы

У `GPSDevice` в конструктор можно передать параметр `EmitMode`.
`EmitMode` определяет правило, по которому `GPSDevice` решает, когда выдавать наружу очередные данные через сигнал: `emit gpsUpdated(data)`.

Режимы
 - `EmitMode::BothValid` (по умолчанию)  

`gpsDataUpdated(data)` эмитится только когда данные считаются полными и согласованными (есть и GPRMC, и GPGGA).
Это более строгий режим: обновления приходят реже, но результат обычно стабильнее для логики/отображения.

 - `EmitMode::AnyValid`  

`gpsDataUpdated(data)` эмитится уже когда доступна хотя бы часть валидных данных ( GPRMC / GPGGA).
Это более мягкий режим: обновления приходят чаще, но возможны ситуации, когда данные неполные.
Будут сигналы с одинаковыми данными, т.к. GPRMC и GPGGA частично дублируют друг друга


```cpp
enum class EmitMode {
    BothValid,  // только если GPRMC и GPGGA валидные и время совпадает
    AnyValid    // если хотя бы одно валидное
};
```
---

## 7. Проекты-примеры
QT-проекты, показывающие применение GPS Device на практике находятся в папке [examples](./examples/)  
Имеются проект с виджетом для отображения данных GPS, проект без отображения и проект с интерфейсом команднйо строки

---

## 8. API (кратко)

### - Public

- `explicit GPSDevice(EmitMode mode = EmitMode::BothValid, QObject* parent = nullptr)`
- `bool isRunning() const`
- `void attachView(IGpsView* view)`

### - Slots

- `void start()`
- `void start(const QString& portName)`
- `void start(const QString& portName, QSerialPort::BaudRate baudRate)`
- `void stop()`
- `void writeFormattedGpsDataToFile(logger::saveFormat format, const QString& fileFullPath = kFormattedDataFileName)`
- `void writeOriginGpsDataToFile(const QString& fileFullPath = kOriginDataFileName)`

### - Signals

- `void gpsStatusChanged(const QString& msg)`
- `void gpsDataUpdated(const GpsData& data)`
