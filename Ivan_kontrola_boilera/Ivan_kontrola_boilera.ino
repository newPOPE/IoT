#include <OneWire.h>
#include <DS18B20_INT.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,4);

#define DHTTYPE    DHT11
#define DHTPIN 2
#define HIGH_TEMP 26

const int buzzer = 9;

DHT_Unified dht(DHTPIN, DHTTYPE);

#define ONE_WIRE_BUS 3

OneWire     oneWire(ONE_WIRE_BUS);
DS18B20_INT ds18b20(&oneWire);

#define DELAY 1000

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Boi: ??C");
  lcd.setCursor(0, 1);
  lcd.print("Mie: ??C / ??%");

  dht.begin();
  ds18b20.begin();
  ds18b20.setResolution(11);
  Serial.begin(9600);

  pinMode(buzzer, OUTPUT);
}

void loop() {
  lcd.setCursor(15, 0);
  lcd.print("-");

  // handle DHT sensor
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  lcd.setCursor(5, 1);
  lcd.print((int) event.temperature);

  handleBuzzer(event.temperature);

  dht.humidity().getEvent(&event);
  lcd.setCursor(11, 1);
  lcd.print((int) event.relative_humidity);

  ds18b20.requestTemperatures();
  while (!ds18b20.isConversionComplete());  // (BLOCKING!!) wait until sensor is ready
  lcd.setCursor(5, 0);
  lcd.print(ds18b20.getTempC());
  Serial.println(ds18b20.getTempC());
  
  
  //Serial.println(buffer);
  delay(DELAY / 2);
  lcd.setCursor(15, 0);
  lcd.print("|");
  delay(DELAY / 2);
}

void handleBuzzer (float temp) {
  if(temp > HIGH_TEMP) {
    tone(buzzer, 1400);
    delay(1000);
    noTone(buzzer);
    return;
  }
  
  noTone(buzzer);
}
