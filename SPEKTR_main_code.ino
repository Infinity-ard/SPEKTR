#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// -----------------------------
// Определение пинов управления реле
// -----------------------------
const int rlyPin1 = 2;  // Реле катушки 1
const int rlyPin2 = 3;  // Реле катушки 2
const int rlyPin3 = 4;  // Реле катушки 3
const int rlyPin4 = 5;  // Реле катушки 4
const int rlyPin5 = 6;  // Реле катушки 5

// -----------------------------
// Определение пинов сенсоров
// -----------------------------
const int sensorPin1 = 7;  // Сенсор для катушки 1
const int sensorPin2 = 8;  // Сенсор для катушки 2
const int sensorPin3 = 9;  // Сенсор для катушки 3
const int sensorPin4 = 10; // Сенсор для катушки 4
const int sensorPin5 = 11; // Сенсор для катушки 5

// -----------------------------
// Определение пинов управления
// -----------------------------
const int shtButton = 12;  // Кнопка выстрела
const int chrgTrans = 13;  // Управление зарядом конденсаторов

// -----------------------------
// Глобальные переменные для управления состоянием
// -----------------------------
bool charging = false;     // Флаг состояния зарядки
bool readyToFire = false;  // Флаг готовности к выстрелу

// -----------------------------
// Инициализация LCD-дисплея
// -----------------------------
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Адрес 0x27, дисплей 16x2 символа

// -----------------------------
// Функция начальной настройки
// -----------------------------
void setup() {
    // Инициализация LCD
    lcd.init();                      // Инициализация
    lcd.backlight();                 // Включение подсветки

    // Сообщение о запуске прошивки
    centerText("Loading firmware", 0);  // Центрированный текст на строке 0
    centerText("Please wait", 1);       // Центрированный текст на строке 1
    delay(2000);                        // Задержка для отображения текста

    // Настройка режимов пинов реле
    pinMode(rlyPin1, OUTPUT);  // Реле 1
    pinMode(rlyPin2, OUTPUT);  // Реле 2
    pinMode(rlyPin3, OUTPUT);  // Реле 3
    pinMode(rlyPin4, OUTPUT);  // Реле 4
    pinMode(rlyPin5, OUTPUT);  // Реле 5

    // Настройка режимов пинов сенсоров
    pinMode(sensorPin1, INPUT);  // Сенсор 1
    pinMode(sensorPin2, INPUT);  // Сенсор 2
    pinMode(sensorPin3, INPUT);  // Сенсор 3
    pinMode(sensorPin4, INPUT);  // Сенсор 4
    pinMode(sensorPin5, INPUT);  // Сенсор 5

    // Настройка режимов кнопки и трансформатора
    pinMode(shtButton, INPUT);  // Кнопка выстрела
    pinMode(chrgTrans, OUTPUT); // Управление зарядом

    // Сообщение о версии прошивки
    lcd.clear();                          // Очистка экрана
    centerText("Firmware version", 0);    // Текст на строке 0
    centerText("24112024", 1);            // Версия на строке 1
    delay(2000);                          // Задержка
    lcd.clear();                          // Очистка экрана

    // Начальная разрядка конденсаторов
    dischargeCapacitors();
}

// -----------------------------
// Основной цикл программы
// -----------------------------
void loop() {
    lcd.clear();                         // Очистка экрана
    centerText("Waiting actions", 0);    // Сообщение о готовности

    // Проверяем состояние кнопки выстрела
    if (digitalRead(shtButton) == LOW) {
        // Если зарядка не начата и выстрел не готов
        if (!charging && !readyToFire) {
            startCharging();             // Начинаем зарядку
        } else if (readyToFire) {
            fire();                      // Выполняем выстрел
        }
    }
}

// -----------------------------
// Функция разрядки конденсаторов
// -----------------------------
void dischargeCapacitors() {
    lcd.clear();                          // Очистка экрана
    centerText("Discharging caps", 0);    // Сообщение о разрядке

    // Включаем все реле для разрядки
    digitalWrite(rlyPin1, HIGH);
    digitalWrite(rlyPin2, HIGH);
    digitalWrite(rlyPin3, HIGH);
    digitalWrite(rlyPin4, HIGH);
    digitalWrite(rlyPin5, HIGH);

    delay(100);  // Короткая задержка для выполнения разрядки

    // Выключаем реле
    digitalWrite(rlyPin1, LOW);
    digitalWrite(rlyPin2, LOW);
    digitalWrite(rlyPin3, LOW);
    digitalWrite(rlyPin4, LOW);
    digitalWrite(rlyPin5, LOW);

    // Завершаем разрядку
    delay(1000);                           // Задержка для завершения
    lcd.clear();                           // Очистка экрана
    centerText("Preparation done", 0);     // Сообщение о завершении
    delay(1000);                           // Задержка
}

// -----------------------------
// Функция начала зарядки конденсаторов
// -----------------------------
void startCharging() {
    charging = true;                      // Устанавливаем флаг зарядки
    lcd.clear();                          // Очистка экрана
    lcd.setCursor(0, 0);                  // Устанавливаем курсор
    lcd.print("Charging caps");           // Сообщение о зарядке

    int totalTime = 3000;                 // Общее время зарядки
    int stepTime = totalTime / 5;         // Время на один шаг

    digitalWrite(chrgTrans, HIGH);        // Запуск зарядки

    for (int i = 0; i <= 5; i++) {        // Цикл прогресса
        lcd.setCursor(0, 1);              // Курсор на второй строке
        lcd.print("Charging: [");
        for (int j = 0; j < 5; j++) {     // Рисуем шкалу
            if (j < i) {
                lcd.print("#");
            } else {
                lcd.print("-");
            }
        }
        lcd.print("]");
        if (i < 5) delay(stepTime);       // Задержка между шагами
    }

    digitalWrite(chrgTrans, LOW);         // Завершение зарядки
    charging = false;                     // Сбрасываем флаг зарядки
    readyToFire = true;                   // Устанавливаем флаг готовности
    lcd.clear();                          // Очистка экрана
    centerText("Charging done", 0);       // Сообщение о завершении
    delay(1000);
    lcd.clear();
    centerText("Ready to fire", 0);       // Готовность к выстрелу
}

// -----------------------------
// Функция выполнения выстрела
// -----------------------------
void fire() {
    lcd.clear();                          // Очистка экрана
    centerText("Firing", 0);              // Сообщение о выстреле

    if (digitalRead(sensorPin1) == LOW) {
        digitalWrite(rlyPin1, HIGH);
        delay(100);                       // Время активации реле
        digitalWrite(rlyPin1, LOW);
    }

    // Аналогичные блоки для остальных сенсоров...

    readyToFire = false;                  // Сброс флага готовности
    lcd.clear();                          // Очистка экрана
    centerText("Fire complete", 0);       // Сообщение о завершении
    delay(1000);

    dischargeCapacitors();                // Разрядка после выстрела
}

// -----------------------------
// Функция центрирования текста
// -----------------------------
void centerText(const char* text, int row) {
    int len = strlen(text);              // Длина текста
    int pos = (16 - len) / 2;            // Вычисление позиции
    lcd.setCursor(pos, row);             // Установка курсора
    lcd.print(text);                     // Вывод текста
}
