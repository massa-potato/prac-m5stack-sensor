#include <M5Stack.h>
#include <DHT.h>

const int PIN_DHT = 5;
DHT dht(PIN_DHT, DHT11);

void setup() {
  M5.begin();
  dht.begin();  
}

uint16_t analogRead_value = 0;

void loop() {
  delay(3000);

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();  
  analogRead_value = analogRead(36);

  M5.Lcd.clear(BLACK);
  if ( (temperature > 28) or (analogRead_value < 300) ) {
    M5.Lcd.fillScreen(TFT_RED);
  } 

  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.drawString("M5STACK SENSORS:", 0, 0, 4);

  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.drawString("Humidity:", 0, 30, 4);
  M5.Lcd.drawFloat(humidity, 0, 0, 60, 4);
  M5.Lcd.drawString("%", 50, 60, 4);
  
  M5.Lcd.setTextColor(GREEN);
  M5.Lcd.drawString("Temperature:", 0, 90, 4);
  M5.Lcd.drawFloat(temperature, 0, 0, 120, 4);
  M5.Lcd.drawString("C", 50, 120, 4);
 
  M5.Lcd.setTextColor(BLUE);
  M5.Lcd.drawString("Moisture:", 0, 150, 4);
  M5.Lcd.drawFloat(analogRead_value, 0, 0, 180, 4);
}
