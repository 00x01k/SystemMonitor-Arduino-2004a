# SystemMonitor-Arduino-2004a
# Система мониторинга с использованием Arduino и Python

Этот проект предназначен для мониторинга загрузки ЦП, использования ОЗУ и температуры процессора системы и отображения информации на ЖК-дисплее, подключенном к Arduino через интерфейс I2C.

## Требования к оборудованию
- Arduino (например, Arduino Uno)
- ЖК-дисплей (20x4) с интерфейсом I2C
- Соединительные провода

## Требования к программному обеспечению
- Python 3.x
- Arduino IDE
- Библиотека `psutil` для Python

## Настройка

### Подключение оборудования
1. Подключите SDA пин ЖК-дисплея к пину A5 на Arduino.
2. Подключите SCL пин ЖК-дисплея к пину A4 на Arduino.
3. Подключите VCC и GND ЖК-дисплея к соответствующим пинам питания на Arduino.

### Arduino
1. Откройте Arduino IDE.
2. Установите библиотеку LiquidCrystal_I2C через библиотечный менеджер Arduino IDE.
3. Загрузите скетч `SystemMonitor.ino` на Arduino:
    ```cpp
    #include <Wire.h>
    #include <LiquidCrystal_I2C.h>

    // Параметры LCD дисплея (измените адрес и размеры экрана, если необходимо)
    LiquidCrystal_I2C lcd(0x27, 20, 4);  // Адрес экрана 0x27, 20 столбцов и 4 строки

    void setup() {
      Wire.begin();  // Инициализация библиотеки Wire (для I2C)
      lcd.init();  // Инициализация LCD
      lcd.backlight();  // Включение подсветки LCD
      lcd.begin(20, 4);  // Установка размеров экрана (20 столбцов и 4 строки)

      Serial.begin(9600);  // Инициализация последовательной связи
    }

    void loop() {
      // Чтение данных из последовательного порта (от Python скрипта)
      if (Serial.available() > 0) {
        String data = Serial.readStringUntil('\n');  // Чтение данных до символа новой строки
        lcd.clear();  // Очистка экрана LCD
        
        // Вывод данных о CPU (первая строка)
        lcd.setCursor(0, 0);  // Установка курсора в начало первой строки
        lcd.print("CPU: ");
        lcd.print(data.substring(0, 3));  // Вывод использования CPU (первые 3 символа)
        lcd.print("%");

        // Вывод данных о RAM (третья строка)
        lcd.setCursor(0, 2);  // Установка курсора в начало третьей строки
        lcd.print("RAM: ");
        lcd.print(data.substring(4, 8));  // Вывод использования RAM (следующие 4 символа)
        lcd.print("%");

        // Вывод данных о температуре процессора (четвертая строка)
        lcd.setCursor(0, 3);  // Установка курсора в начало четвертой строки
        lcd.print("CPU Temp: ");
        lcd.print(data.substring(9, 13));  // Вывод температуры CPU (следующие 4 символа)
        lcd.print(" C");

        delay(1000);  // Задержка 1 секунда
      }
    }
    ```

### Python
1. Установите необходимые библиотеки для Python:
    ```sh
    pip install psutil pyserial
    ```
2. Создайте файл `system_monitor.py` и вставьте в него следующий код:
    ```python
    import psutil
    import serial
    import time

    # Настройки
    SERIAL_PORT = '/dev/ttyUSB0'  # Замените на ваш порт Arduino, например, '/dev/ttyUSB0'
    BAUD_RATE = 9600

    # Подключение к Arduino
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)

    def get_cpu_temperature():
        # Получение температуры CPU с использованием psutil
        try:
            temperature = psutil.sensors_temperatures()['coretemp'][0].current  # Изменено на 'coretemp'
            return temperature
        except Exception as e:
            print(f"Failed to get CPU temperature: {e}")
            return 0.0

    while True:
        # Получение данных о системе
        cpu_percent = psutil.cpu_percent()
        ram_percent = psutil.virtual_memory().percent

        # Получение температуры CPU
        cpu_temp = get_cpu_temperature()

        # Формирование строки данных для отправки на Arduino
        data_to_send = f"{cpu_percent:.1f},{ram_percent:.1f},{cpu_temp:.1f}"

        # Вывод данных в консоль
        print(f"CPU: {cpu_percent:.1f}%, RAM: {ram_percent:.1f}%, CPU Temp: {cpu_temp:.1f}°C")

        # Отправка данных на Arduino
        ser.write(data_to_send.encode() + b'\n')

        time.sleep(1)  # Задержка в 1 секунду
    ```

## Использование
1. Убедитесь, что Arduino подключен к вашему компьютеру.
2. Запустите скрипт Python для начала передачи данных на Arduino:
    ```sh
    python system_monitor.py
    ```
3. ЖК-дисплей будет отображать использование ЦП, использование ОЗУ и температуру процессора.

## Лицензия
Этот проект лицензирован под лицензией MIT.
