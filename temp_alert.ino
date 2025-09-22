#include <Arduino_LED_Matrix.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

ArduinoLEDMatrix matrix;

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 16, 2);  

int threshold = 30; // Threshold temperature °C

// Happy face
const uint32_t happy[3] = {
  0x1f820449, 0x240250a4, 0xf22041f8
};

// Sad face
const uint32_t sadd[3] = {
  0x1f820449, 0x24025fa5, 0x0a2041f8
};

void setup() {
  Serial.begin(9600);
  dht.begin();
  matrix.begin();

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Temp Alert Sys");
}

void loop() {
  float temperature = dht.readTemperature();

  if (isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    lcd.setCursor(0, 1);
    lcd.print("Sensor Error   ");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  // Show temperature on LCD
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print((char)223); // degree symbol
  lcd.print("C   ");

if (temperature > threshold) {
  // Blink sad face
  for (int i = 0; i < 3; i++) {
    matrix.loadFrame(sadd);
    delay(500);
    matrix.clear();
    delay(500);
  }
} else {
  // Show happy face
  matrix.loadFrame(happy);
  delay(1000);
}

}
