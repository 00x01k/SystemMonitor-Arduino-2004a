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
