#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Пины подключения
#define Sensor1Pin 2
#define Sensor2Pin 3
#define Sensor3Pin 4
#define Sensor4Pin 5
#define TempPin 6
#define TriggerPin 7
#define ResetPin 8

// Настройка дисплея
LiquidCrystal_I2C Lcd(0x27, 16, 2);

// Настройка температурного датчика
OneWire OneWireBus(TempPin);
DallasTemperature Sensors(&OneWireBus);

void setup() {
  pinMode(Sensor1Pin, INPUT);
  pinMode(Sensor2Pin, INPUT);
  pinMode(Sensor3Pin, INPUT);
  pinMode(Sensor4Pin, INPUT);
  pinMode(TriggerPin, INPUT);
  pinMode(ResetPin, OUTPUT);
  
  Lcd.begin(16, 2);
  Lcd.setBacklight(true);

  Sensors.begin(); // Инициализация температурного датчика
}

void loop() {
  // Проверка температуры
  CheckTemperature();

  // Проверка курка
  if (digitalRead(TriggerPin) == LOW) {
    // Проверка пуль при нажатии на курок
    CheckBullets();
    delay(1000);  // Задержка после выстрела
  }
}

void CheckTemperature() {
  Sensors.requestTemperatures(); // Запрос температуры
  float Temp = Sensors.getTempCByIndex(0); // Получение температуры

  if (Temp > 40.0) {
    Lcd.clear();
    Lcd.print("ERROR 2015"); // Ошибка превышения температуры
  }
}

void CheckBullets() {
  bool BulletDetected[4];
  BulletDetected[0] = digitalRead(Sensor1Pin) == LOW;
  BulletDetected[1] = digitalRead(Sensor2Pin) == LOW;
  BulletDetected[2] = digitalRead(Sensor3Pin) == LOW;
  BulletDetected[3] = digitalRead(Sensor4Pin) == LOW;

  // Проверка на регистрацию пули
  bool BulletFound = false;
  for (int i = 0; i < 4; i++) {
    if (BulletDetected[i]) {
      BulletFound = true;
      break;
    }
  }

  if (!BulletFound) {
    Lcd.clear();
    Lcd.print("ERROR 21418"); // Ошибка: не удалось зарегистрировать пулю
  }

  // Проверка на вылет пули
  if (BulletDetected[0] && BulletDetected[1] && BulletDetected[2] && !BulletDetected[3]) {
    Lcd.clear();
    Lcd.print("ERROR 241213"); // Ошибка вылета пули
  }
}

void StopProgram() {
  Lcd.clear();
  Lcd.print("Stop program");
  delay(1000); // Задержка перед сбросом

  digitalWrite(ResetPin, LOW); // Подавить сигнал на сброс
  delay(1000); // Задержка для сброса

  Lcd.clear();
  Lcd.print("Reset program");
  delay(1000); // Задержка перед повторным запуском
  digitalWrite(ResetPin, HIGH); // Выключить сброс
}
