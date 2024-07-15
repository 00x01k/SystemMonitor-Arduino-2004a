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
