#include <M5Stack.h>
#include <DHT.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>

#define WIFI_SSID "（自身のSSIDを記述）"
#define WIFI_PASSWORD "（自身のWiFiパスワードを記述）"

// スプレッドシートへデータを飛ばすWebhook URL
const char *host = "（GASでデプロイしたURLを記述）";

// JSON設定
StaticJsonDocument<255> json_request;
char buffer[255];

// dht設定
const int PIN_DHT = 5;
DHT dht(PIN_DHT, DHT11);

// センサー値
float humidity;                // 湿度
float temperature;             // 温度
uint16_t moisture = 0;         // 土壌水分


/* setup関数 */
void setup() {
  M5.begin();
  dht.begin();
  
  // Wi-Fi接続
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  WiFi.setSleep(false); // btnA連打バグへの対応
  Serial.print("connectiong");
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();

  // WiFi Connected
  Serial.println("¥nWiFi Connected.");
  Serial.println(WiFi.localIP());
  M5.Lcd.setTextSize(3);
  M5.Lcd.println("WiFi Connected:");
  M5.Lcd.println(WiFi.localIP());
  M5.Lcd.println("");
  
}

/* センサーの値送信 */
void sendValues(){
  json_request["humidity"] = humidity;
  json_request["temperature"] = temperature;
  json_request["moisture"] = moisture;
  serializeJson(json_request, buffer, sizeof(buffer));

  HTTPClient http;
  http.begin(host);
  http.addHeader("Content-Type", "application/json");
  int status_code = http.POST((uint8_t*)buffer, strlen(buffer));
  Serial.println(status_code);
  if(status_code > 0){
    if(status_code == HTTP_CODE_FOUND){
      String payload = http.getString();
      Serial.println(payload);

      M5.Lcd.setCursor(10, 100);
      M5.Lcd.fillScreen(BLACK);
      M5.Lcd.setTextColor(WHITE);
      M5.Lcd.setTextSize(3);
      M5.Lcd.println("Send Done!");
    }  
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s¥n", http.errorToString(status_code).c_str());
  }
  http.end();

}


/* loop関数 */
void loop(){

  // 各センサの値を読み込む
  delay(3000);
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();  
  moisture = analogRead(36);

  // 画面への出力
  M5.Lcd.clear(BLACK);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(1);
  M5.Lcd.drawString("M5STACK SENSORS:", 0, 0, 4);

  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.drawString("Humidity:", 0, 30, 4); // 湿度
  M5.Lcd.drawFloat(humidity, 0, 0, 60, 4);
  M5.Lcd.drawString("%", 50, 60, 4);
  
  M5.Lcd.setTextColor(GREEN);
  M5.Lcd.drawString("Temperature:", 0, 90, 4); // 温度
  M5.Lcd.drawFloat(temperature, 0, 0, 120, 4);
  M5.Lcd.drawString("C", 50, 120, 4);
 
  M5.Lcd.setTextColor(BLUE);
  M5.Lcd.drawString("Moisture:", 0, 150, 4); // 土壌水分
  M5.Lcd.drawFloat(moisture, 0, 0, 180, 4);

  // センサーの値を送る
  sendValues();

}
