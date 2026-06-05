#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <UrlEncode.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "time.h"
#include "src/secrets/Secrets.h"
#include "pitches.h"

#define DHTPIN 5
#define DHTTYPE DHT11
#define LED1 LED_BUILTIN
#define LED2 16
#define NTP_SERVER "pool.ntp.org"
#define GMT_OFFSET_SEC -21600
#define DAYLIGHT_OFFSET_SEC 0
#define REQUEST_INTERVAL 60000
#define BUZZER_PIN 4 // GPIO4

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int alternative_SDA = 12; 
int alternative_SCL = 14; 
bool isScreenOff = false;

// [BEGIN lopaka generated]
static const unsigned char PROGMEM image_Clock_bits[] = {0x03,0xf0,0x00,0x0f,0xfc,0x00,0x1f,0xfe,0x00,0x38,0x07,0x00,0x7d,0xef,0x80,0x7d,0xef,0x80,0xfd,0xef,0xc0,0xfe,0xdf,0xc0,0xff,0x3f,0xc0,0xff,0x3f,0xc0,0xfe,0x9f,0xc0,0xfd,0x4f,0xc0,0x7c,0xaf,0x80,0x7d,0x4f,0x80,0x38,0x07,0x00,0x1f,0xfe,0x00,0x0f,0xfc,0x00,0x03,0xf0,0x00};
static const unsigned char PROGMEM image_weather_temperature_bits[] = {0x1c,0x00,0x22,0x02,0x2b,0x05,0x2a,0x02,0x2b,0x38,0x2a,0x60,0x2b,0x40,0x2a,0x40,0x2a,0x60,0x49,0x38,0x9c,0x80,0xae,0x80,0xbe,0x80,0x9c,0x80,0x41,0x00,0x3e,0x00};
static const unsigned char PROGMEM image_wifi_full_bits[] = {0x01,0xf0,0x00,0x07,0xfc,0x00,0x1e,0x0f,0x00,0x39,0xf3,0x80,0x77,0xfd,0xc0,0xef,0x1e,0xe0,0x5c,0xe7,0x40,0x3b,0xfb,0x80,0x17,0x1d,0x00,0x0e,0xee,0x00,0x05,0xf4,0x00,0x03,0xb8,0x00,0x01,0x50,0x00,0x00,0xe0,0x00,0x00,0x40,0x00,0x00,0x00,0x00};
static const unsigned char PROGMEM image_wifi_not_connected_bits[] = {0x21,0xf0,0x00,0x16,0x0c,0x00,0x08,0x03,0x00,0x25,0xf0,0x80,0x42,0x0c,0x40,0x89,0x02,0x20,0x10,0xa1,0x00,0x23,0x58,0x80,0x04,0x24,0x00,0x08,0x52,0x00,0x01,0xa8,0x00,0x02,0x04,0x00,0x00,0x42,0x00,0x00,0xa1,0x00,0x00,0x40,0x80,0x00,0x00,0x00};

// Task timers and intervals
unsigned long previousMillis1 = 0;
unsigned long interval1 = 100;  // Task 1 interval (e.g., 200 milliseconds)
unsigned long previousMillis2 = 0;
unsigned long interval2 = 2000;  // Task 2 interval (e.g., 5000 milliseconds)
unsigned long previousMillis3 = 0;
unsigned long previousMillis4 = 0;

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
String startupTimeStr;

// Flags
bool firstRun = true;

// DHT Temperature and Humidity Sensor setup
DHT dht(DHTPIN, DHTTYPE);

WiFiClient wifiClient;

void blink(unsigned long currentMillis) {
  String currentHour = timeStr.substring(11, 13);
  Serial.println("currentHour: " + String(currentHour));
  if (currentHour.toInt() < 7) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    return;
  }

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

  if (currentMillis - previousMillis2 >= interval1 + 500) {
    previousMillis2 = currentMillis;

    // Toggle the state of LED 2
    if (digitalRead(LED2) == LOW) {
      digitalWrite(LED2, HIGH);
    } else {
      digitalWrite(LED2, LOW);
    }
  }
}

String getFormattedLocalDateTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return "(Error)";
  }
  char timeString[25]; // Buffer to hold formatted string
  // Format: "YYYY-MM-DD HH:MM:SS"
  strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", &timeinfo);
  // Serial.println(timeString);
  return timeString;
}

String getUptime() {
  unsigned long uptimeSeconds = millis() / 1000;
  unsigned long days = uptimeSeconds / 86400;
  unsigned long hours = (uptimeSeconds % 86400) / 3600;
  unsigned long minutes = (uptimeSeconds % 3600) / 60;
  unsigned long seconds = uptimeSeconds % 60;

  char uptimeString[20];
  sprintf(uptimeString, "%01lu days, %02lu:%02lu:%02lu", days, hours, minutes, seconds);
  Serial.println(uptimeString);
  return uptimeString;
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
  if (firstRun || (currentMillis - previousMillis3 >= REQUEST_INTERVAL)) {
    previousMillis3 = currentMillis;
    if (WiFi.status() == WL_CONNECTED) {
      std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
      // Use this to skip certificate validation
      client->setInsecure();
      getLT();
      getPublicIP();
      readSensors();
      String rawData = "Startup Datetime: ";
      rawData += startupTimeStr;
      rawData += "\n\n";
      rawData += "Last Heartbeat Datetime: ";
      rawData += timeStr;
      rawData += "\n\n";
      rawData += "Uptime: ";
      rawData += getUptime();
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
    firstRun = false;
  }
}

void nokiaRingtone() {
  int noteDurations[] = { 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 600 };

  // Nokia Ringtone (E5 D5 F#4 G#4 - C#5 B4 D4 E4 - B4 A4 C#4 E4 A4)
  int nokiaRingtone[] = { 
    NOTE_E5, NOTE_D5, NOTE_FS4, NOTE_GS4, 
    NOTE_CS5, NOTE_B4, NOTE_D4, NOTE_E4, 
    NOTE_B4, NOTE_A4, NOTE_CS4, NOTE_E4, NOTE_A4 
  };
  
  for (int i = 0; i < 13; i++) {
    tone(BUZZER_PIN, nokiaRingtone[i], noteDurations[i]);
    
    // Pause between notes
    delay(noteDurations[i] * 1.30); 
    noTone(BUZZER_PIN);
  }
}

void playAlarm(unsigned long currentMillis) {
  // Format: "YYYY-MM-DD HH:MM:SS"
  String currentMinute = timeStr.substring(14, 16);
  String currentHour = timeStr.substring(11, 13);
  Serial.println("currentMinute: " + String(currentMinute));
  Serial.println("currentHour: " + String(currentHour));
  if ((currentMinute == "00" && currentHour == "07") || (currentMinute == "00" && currentHour == "08")) {
    Serial.println("Playing alarm...");
    nokiaRingtone();
  }
  delay(20);
}


void drawScreen_1(void) {
  String currentMinute = timeStr.substring(14, 16);
  String currentHour = timeStr.substring(11, 13);
  Serial.println("currentMinute: " + String(currentMinute));
  Serial.println("currentHour: " + String(currentHour));
  if (currentHour.toInt() < 7 && !isScreenOff) {
    display.ssd1306_command(SSD1306_DISPLAYOFF); // Send sleep command (0xAE)
    isScreenOff = true;
    return;
  } else if (currentHour.toInt() >= 7 && isScreenOff) {
    display.ssd1306_command(SSD1306_DISPLAYON);  // Send wake command (0xAF)
    isScreenOff = false;
  }

  //for(;;) { // Loop forever...
    display.clearDisplay();
    getLT();
    readSensors();
    if (WiFi.status() == WL_CONNECTED) {
      // wifi_full
      display.drawBitmap(89, 0, image_wifi_full_bits, 19, 16, 1);
    } else {
      // wifi_not_connected
      display.drawBitmap(89, 0, image_wifi_not_connected_bits, 19, 16, 1);
    }

    Serial.println("Temp: " + timeStr);
    // string 3
    display.setTextColor(1);
    display.setTextSize(2);
    display.setTextWrap(false);
    display.setCursor(4, 24);
    display.print(timeStr.substring(0, 10));
    // string 3.1
    display.setTextColor(1);
    display.setTextSize(2);
    display.setTextWrap(false);
    display.setCursor(4, 44);
    // Format: "YYYY-MM-DD HH:MM:SS"
    display.print(timeStr.substring(11, 19));
    // string 4
    display.setCursor(4, 5);
    display.print(temperature);
    // string 5
    // display.setCursor(4, 44);
    // display.print("CST");
    // weather_temperature
    display.setTextWrap(true);
    display.drawBitmap(69, 0, image_weather_temperature_bits, 16, 16, 1);
    // Clock
    // display.drawBitmap(110, 24, image_Clock_bits, 18, 18, 1);
    // string 8
    display.setTextSize(1);
    display.setCursor(48, 50);

    /*
    if (temperature.toInt() > 25) {
      display.print("Calorcito");
    } else {
      display.print("Cool!");
    }
    */
    // string 9
    display.setCursor(110, 5);
    if (WiFi.status() == WL_CONNECTED) {
      display.print("On");
    } else {
      display.print("Off");
    }
    display.display();
  //}
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dht.begin();
  Wire.begin(alternative_SDA, alternative_SCL);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  // delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

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
  pinMode(BUZZER_PIN, OUTPUT);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  startupTimeStr = getFormattedLocalDateTime();
}

void loop() {
  unsigned long currentMillis = millis();
  blink(currentMillis);
  telegramRequest(currentMillis);
  playAlarm(currentMillis);
  drawScreen_1();
}
