#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <UrlEncode.h>
#include "time.h"
#include "src/secrets/Secrets.h"

#define DHTPIN 5
#define DHTTYPE DHT11
#define LED1 LED_BUILTIN
#define LED2 16
#define NTP_SERVER "pool.ntp.org"
#define GMT_OFFSET_SEC -21600
#define DAYLIGHT_OFFSET_SEC 0
#define MIN_INTERVAL 60000

// Task timers and intervals
unsigned long previousMillis1 = 0;
unsigned long interval1 = 100;  // Task 1 interval (e.g., 200 milliseconds)
unsigned long previousMillis2 = 0;
unsigned long interval2 = 10000;  // Task 2 interval (e.g., 5000 milliseconds)
unsigned long previousMillis3 = 0;

// HTTP Server credentials
String localIP;

// NTP Server credentials
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -21600; // UTC -6
const int daylightOffset_sec = 0; // Set to 3600 if in Daylight Savings

// Time
String temperature;
String humidity;
String timeStr;
String publicIP;

// DHT Temperature and Humidity Sensor setup
DHT dht(DHTPIN, DHTTYPE);

WiFiClient wifiClient;

void blink(unsigned long currentMillis) {
  // --- Task 1 ---
  if (currentMillis - previousMillis1 >= interval1) {
    previousMillis1 = currentMillis;

    // Toggle the state of LED 1
    if (digitalRead(LED1) == LOW) {
      digitalWrite(LED1, HIGH);
    } else {
      digitalWrite(LED1, LOW);
    }
  }

  // --- Task 2 ---
  currentMillis = millis();  // Get the current time again (or use the previous one)

  if (currentMillis - previousMillis2 >= interval1) {
    previousMillis2 = currentMillis;

    // Toggle the state of LED 2
    if (digitalRead(LED2) == LOW) {
      digitalWrite(LED2, HIGH);
    } else {
      digitalWrite(LED2, LOW);
    }
  }
}

void getLT() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  char timeString[25]; // Buffer to hold formatted string
  // Format: "YYYY-MM-DD HH:MM:SS"
  strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", &timeinfo);
  Serial.println(timeString);
  timeStr = timeString;
  /*
  Serial.println("Time variables");
  char timeHour[3];
  strftime(timeHour,3, "%H", &timeinfo);
  Serial.println(timeHour);
  char timeWeekDay[10];
  strftime(timeWeekDay,10, "%A", &timeinfo);
  Serial.println(timeWeekDay);
  Serial.println();
  */
}

void readSensors() {
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    char json[200];
    temperature = t;
    humidity = h;
    Serial.printf("Temperatura %f C, humedad %f %\n", t, h);
    sprintf(json, "{\"temperature\": %f,\"humidity\": %f}", t, h);
    Serial.println(json);
    Serial.println(timeStr);
}

void printMemoryUsage() {
  Serial.print("Free Heap: ");
  Serial.print(ESP.getFreeHeap() / 1024); // KB
  Serial.println(" KB");

  Serial.print("Fragmentation: ");
  Serial.print(ESP.getHeapFragmentation());
  Serial.println(" %");

  Serial.print("Max Free Block: ");
  Serial.print(ESP.getMaxFreeBlockSize() / 1024);
  Serial.println(" KB");
}

void getPublicIP() {
  if (WiFi.status() == WL_CONNECTED) {
    std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
    // Use this to skip certificate validation
    client->setInsecure();
    HTTPClient https;
    if (https.begin(*client, AWS_CHECK_IP_URL)) {
      int httpCode = https.GET();
      if (httpCode > 0) {
        Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
        String payload = https.getString();
        Serial.println(payload);
        publicIP = payload;
      } else {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }
      https.end();
    }
  }
}

void telegramRequest(unsigned long currentMillis) {
  if (currentMillis - previousMillis3 >= MIN_INTERVAL) {
    previousMillis3 = currentMillis;
    if (WiFi.status() == WL_CONNECTED) {
      std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
      // Use this to skip certificate validation
      client->setInsecure();
      getLT();
      getPublicIP();
      readSensors();
      String rawData = "Alive Date: ";
      rawData += timeStr;
      rawData += "\n\n";
      rawData += "Public IP: ";
      rawData += publicIP;
      rawData += "\n";
      rawData += "Local IP: ";
      rawData += localIP;
      rawData += "\n\n";
      rawData += "Temperature in Living Room: ";
      rawData += temperature;
      rawData += " °C";
      rawData += "\n\n";
      rawData += "Humidity in Living Room: ";
      rawData += humidity;
      rawData += "%";
      rawData += "\n\n";
      rawData += "Device: ESP8266MCU";

      String text = urlEncode(rawData);
      String url = TELEGRAM_API_URL;
      url += TELEGRAM_API_KEY;
      url += "/editMessageText";
      url += "?";
      url += "chat_id=";
      url += TELEGRAM_USER_ID;
      url += "&";
      url += "message_id=";
      url += TELEGRAM_MESSAGE_ID;
      url += "&"; 
      url += "text=";
      url += text;
      HTTPClient https;

      if (https.begin(*client, url)) {
        int httpCode = https.GET();
        if (httpCode > 0) {
          Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
          String payload = https.getString();
          Serial.println(payload);
        } else {
          Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
        }
        https.end();
      }
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(SSID, WIFI_PASSWORD);
  Serial.printf("\n");
  Serial.printf("Conectando a Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(".");
  }

  Serial.print("Conectado con la IP: ");
  Serial.println(WiFi.localIP());
  localIP = WiFi.localIP().toString();
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop() {
  unsigned long currentMillis = millis();
  blink(currentMillis);
  telegramRequest(currentMillis);
}
