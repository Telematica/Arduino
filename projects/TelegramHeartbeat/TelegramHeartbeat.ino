/*!
 * @file TelegramHeartbeat.ino
 * @author @Telematica
 * @brief A simple IoT device that monitors temperature and humidity, displays the data on an OLED screen, and sends periodic updates to a Telegram bot.
 *
 * Using ESP8266MOD NodeMCU Hardware, WiFi library for WiFi connectivity and HTTP requests,
 * Adafruit_SSD1306 for OLED display, DHT for temperature and humidity sensor,
 * and NTP for time synchronization.
 *
 */
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <time.h>
#include <UrlEncode.h>
#include <WiFiClientSecure.h>
#include <Wire.h>
#include "src/secrets/Secrets.h"
#include "pitches.h"
#include "NTP.h"

#define ALTERNATIVE_I2C_SCL 14 // GPIO14 D5
#define ALTERNATIVE_I2C_SDA 12 // GPIO12 D6
#define BUTTON_PIN 13          // GPIO13 D7
#define BUZZER_PIN 4           // GPIO4  D2
#define DHTPIN 5               // GPIO5  D1
#define DHTTYPE DHT11          // DHT 11 sensor type
#define LED1 LED_BUILTIN       // Module LED (Blue): Connected to GPIO2 (labeled D4 on the board).
#define LED2 16                // Board LED: Connected to GPIO16 (labeled D0 on the board).
#define OLED_RESET -1          // Reset pin # (or -1 if sharing Arduino reset pin)
#define REQUEST_INTERVAL 60000 // Telegram Request fired every 60s
#define SCREEN_HEIGHT 64       // OLED display height, in pixels
#define SCREEN_WIDTH 128       // OLED display width, in pixels

ADC_MODE(ADC_VCC); // Place this at the very top of your sketch (outside of setup)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
DHT dht(DHTPIN, DHTTYPE);                                                 // DHT Temperature and Humidity Sensor setup
WiFiClient wifiClient;                                                    // WiFi Client for HTTP requests

// [BEGIN lopaka generated]
static const unsigned char PROGMEM image_weather_temperature_bits[] = {0x1c, 0x00, 0x22, 0x02, 0x2b, 0x05, 0x2a, 0x02, 0x2b, 0x38, 0x2a, 0x60, 0x2b, 0x40, 0x2a, 0x40, 0x2a, 0x60, 0x49, 0x38, 0x9c, 0x80, 0xae, 0x80, 0xbe, 0x80, 0x9c, 0x80, 0x41, 0x00, 0x3e, 0x00};
static const unsigned char PROGMEM image_wifi_full_bits[] = {0x01, 0xf0, 0x00, 0x07, 0xfc, 0x00, 0x1e, 0x0f, 0x00, 0x39, 0xf3, 0x80, 0x77, 0xfd, 0xc0, 0xef, 0x1e, 0xe0, 0x5c, 0xe7, 0x40, 0x3b, 0xfb, 0x80, 0x17, 0x1d, 0x00, 0x0e, 0xee, 0x00, 0x05, 0xf4, 0x00, 0x03, 0xb8, 0x00, 0x01, 0x50, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00};
static const unsigned char PROGMEM image_wifi_not_connected_bits[] = {0x21, 0xf0, 0x00, 0x16, 0x0c, 0x00, 0x08, 0x03, 0x00, 0x25, 0xf0, 0x80, 0x42, 0x0c, 0x40, 0x89, 0x02, 0x20, 0x10, 0xa1, 0x00, 0x23, 0x58, 0x80, 0x04, 0x24, 0x00, 0x08, 0x52, 0x00, 0x01, 0xa8, 0x00, 0x02, 0x04, 0x00, 0x00, 0x42, 0x00, 0x00, 0xa1, 0x00, 0x00, 0x40, 0x80, 0x00, 0x00, 0x00};
static const unsigned char PROGMEM image_wifi_not_connected_bits2[] = {0x0c, 0x03, 0xff, 0x00, 0x00, 0x0c, 0x03, 0xff, 0x00, 0x00, 0x03, 0x3c, 0x00, 0xf0, 0x00, 0x03, 0x3c, 0x00, 0xf0, 0x00, 0x00, 0xc0, 0x00, 0x0f, 0x00, 0x00, 0xc0, 0x00, 0x0f, 0x00, 0x0c, 0x33, 0xff, 0x00, 0xc0, 0x0c, 0x33, 0xff, 0x00, 0xc0, 0x30, 0x0c, 0x00, 0xf0, 0x30, 0x30, 0x0c, 0x00, 0xf0, 0x30, 0xc0, 0xc3, 0x00, 0x0c, 0x0c, 0xc0, 0xc3, 0x00, 0x0c, 0x0c, 0x03, 0x00, 0xcc, 0x03, 0x00, 0x03, 0x00, 0xcc, 0x03, 0x00, 0x0c, 0x0f, 0x33, 0xc0, 0xc0, 0x0c, 0x0f, 0x33, 0xc0, 0xc0, 0x00, 0x30, 0x0c, 0x30, 0x00, 0x00, 0x30, 0x0c, 0x30, 0x00, 0x00, 0xc0, 0x33, 0x0c, 0x00, 0x00, 0xc0, 0x33, 0x0c, 0x00, 0x00, 0x03, 0xcc, 0xc0, 0x00, 0x00, 0x03, 0xcc, 0xc0, 0x00, 0x00, 0x0c, 0x00, 0x30, 0x00, 0x00, 0x0c, 0x00, 0x30, 0x00, 0x00, 0x00, 0x30, 0x0c, 0x00, 0x00, 0x00, 0x30, 0x0c, 0x00, 0x00, 0x00, 0xcc, 0x03, 0x00, 0x00, 0x00, 0xcc, 0x03, 0x00, 0x00, 0x00, 0x30, 0x00, 0xc0, 0x00, 0x00, 0x30, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const unsigned char PROGMEM image_wifi_1_bits[] = {0x00, 0x03, 0xff, 0x00, 0x00, 0x00, 0x03, 0xff, 0x00, 0x00, 0x00, 0x3c, 0x00, 0xf0, 0x00, 0x00, 0x3c, 0x00, 0xf0, 0x00, 0x03, 0xc0, 0x00, 0x0f, 0x00, 0x03, 0xc0, 0x00, 0x0f, 0x00, 0x0c, 0x03, 0xff, 0x00, 0xc0, 0x0c, 0x03, 0xff, 0x00, 0xc0, 0x30, 0x3c, 0x00, 0xf0, 0x30, 0x30, 0x3c, 0x00, 0xf0, 0x30, 0xc0, 0xc0, 0x00, 0x0c, 0x0c, 0xc0, 0xc0, 0x00, 0x0c, 0x0c, 0x03, 0x00, 0xfc, 0x03, 0x00, 0x03, 0x00, 0xfc, 0x03, 0x00, 0x0c, 0x0f, 0x03, 0xc0, 0xc0, 0x0c, 0x0f, 0x03, 0xc0, 0xc0, 0x00, 0x30, 0x00, 0x30, 0x00, 0x00, 0x30, 0x00, 0x30, 0x00, 0x00, 0xc0, 0x30, 0x0c, 0x00, 0x00, 0xc0, 0x30, 0x0c, 0x00, 0x00, 0x03, 0xcf, 0x00, 0x00, 0x00, 0x03, 0xcf, 0x00, 0x00, 0x00, 0x0c, 0x00, 0xc0, 0x00, 0x00, 0x0c, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0xcc, 0x00, 0x00, 0x00, 0x00, 0xcc, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const unsigned char PROGMEM image_wifi_2_bars_bits[] = {0x00, 0x03, 0xff, 0x00, 0x00, 0x00, 0x03, 0xff, 0x00, 0x00, 0x00, 0x3c, 0x00, 0xf0, 0x00, 0x00, 0x3c, 0x00, 0xf0, 0x00, 0x03, 0xc0, 0x00, 0x0f, 0x00, 0x03, 0xc0, 0x00, 0x0f, 0x00, 0x0c, 0x03, 0xff, 0x00, 0xc0, 0x0c, 0x03, 0xff, 0x00, 0xc0, 0x30, 0x3c, 0x00, 0xf0, 0x30, 0x30, 0x3c, 0x00, 0xf0, 0x30, 0xc0, 0xc0, 0x00, 0x0c, 0x0c, 0xc0, 0xc0, 0x00, 0x0c, 0x0c, 0x33, 0x00, 0xfc, 0x03, 0x30, 0x33, 0x00, 0xfc, 0x03, 0x30, 0x0c, 0x0f, 0x03, 0xc0, 0xc0, 0x0c, 0x0f, 0x03, 0xc0, 0xc0, 0x03, 0x30, 0x00, 0x33, 0x00, 0x03, 0x30, 0x00, 0x33, 0x00, 0x00, 0xc0, 0xfc, 0x0c, 0x00, 0x00, 0xc0, 0xfc, 0x0c, 0x00, 0x00, 0x33, 0xff, 0x30, 0x00, 0x00, 0x33, 0xff, 0x30, 0x00, 0x00, 0x0f, 0xcf, 0xc0, 0x00, 0x00, 0x0f, 0xcf, 0xc0, 0x00, 0x00, 0x03, 0x33, 0x00, 0x00, 0x00, 0x03, 0x33, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const unsigned char PROGMEM image_wifi_3_bars_bits[] = {0x00, 0x03, 0xff, 0x00, 0x00, 0x00, 0x03, 0xff, 0x00, 0x00, 0x00, 0x3c, 0x00, 0xf0, 0x00, 0x00, 0x3c, 0x00, 0xf0, 0x00, 0x03, 0xc0, 0x00, 0x0f, 0x00, 0x03, 0xc0, 0x00, 0x0f, 0x00, 0x0c, 0x03, 0xff, 0x00, 0xc0, 0x0c, 0x03, 0xff, 0x00, 0xc0, 0x30, 0x3c, 0x00, 0xf0, 0x30, 0x30, 0x3c, 0x00, 0xf0, 0x30, 0xc0, 0xc0, 0x00, 0x0c, 0x0c, 0xc0, 0xc0, 0x00, 0x0c, 0x0c, 0x33, 0x00, 0xfc, 0x03, 0x30, 0x33, 0x00, 0xfc, 0x03, 0x30, 0x0c, 0x0f, 0xff, 0xc0, 0xc0, 0x0c, 0x0f, 0xff, 0xc0, 0xc0, 0x03, 0x3f, 0x03, 0xf3, 0x00, 0x03, 0x3f, 0x03, 0xf3, 0x00, 0x00, 0xfc, 0xfc, 0xfc, 0x00, 0x00, 0xfc, 0xfc, 0xfc, 0x00, 0x00, 0x33, 0xff, 0x30, 0x00, 0x00, 0x33, 0xff, 0x30, 0x00, 0x00, 0x0f, 0xcf, 0xc0, 0x00, 0x00, 0x0f, 0xcf, 0xc0, 0x00, 0x00, 0x03, 0x33, 0x00, 0x00, 0x00, 0x03, 0x33, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const unsigned char PROGMEM image_wifi_4_bars_bits[] = {0x00, 0x03, 0xff, 0x00, 0x00, 0x00, 0x03, 0xff, 0x00, 0x00, 0x00, 0x3c, 0x00, 0xf0, 0x00, 0x00, 0x3c, 0x00, 0xf0, 0x00, 0x03, 0xc0, 0x00, 0x0f, 0x00, 0x03, 0xc0, 0x00, 0x0f, 0x00, 0x0c, 0x03, 0xff, 0x00, 0xc0, 0x0c, 0x03, 0xff, 0x00, 0xc0, 0x30, 0x3f, 0xff, 0xf0, 0x30, 0x30, 0x3f, 0xff, 0xf0, 0x30, 0xc0, 0xff, 0x03, 0xfc, 0x0c, 0xc0, 0xff, 0x03, 0xfc, 0x0c, 0x33, 0xf0, 0xfc, 0x3f, 0x30, 0x33, 0xf0, 0xfc, 0x3f, 0x30, 0x0f, 0xcf, 0xff, 0xcf, 0xc0, 0x0f, 0xcf, 0xff, 0xcf, 0xc0, 0x03, 0x3f, 0x03, 0xf3, 0x00, 0x03, 0x3f, 0x03, 0xf3, 0x00, 0x00, 0xfc, 0xfc, 0xfc, 0x00, 0x00, 0xfc, 0xfc, 0xfc, 0x00, 0x00, 0x33, 0xff, 0x30, 0x00, 0x00, 0x33, 0xff, 0x30, 0x00, 0x00, 0x0f, 0xcf, 0xc0, 0x00, 0x00, 0x0f, 0xcf, 0xc0, 0x00, 0x00, 0x03, 0x33, 0x00, 0x00, 0x00, 0x03, 0x33, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const unsigned char PROGMEM image_wifi_full_bits_2[] = {0x00, 0x03, 0xff, 0x00, 0x00, 0x00, 0x03, 0xff, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xf0, 0x00, 0x00, 0x3f, 0xff, 0xf0, 0x00, 0x03, 0xfc, 0x00, 0xff, 0x00, 0x03, 0xfc, 0x00, 0xff, 0x00, 0x0f, 0xc3, 0xff, 0x0f, 0xc0, 0x0f, 0xc3, 0xff, 0x0f, 0xc0, 0x3f, 0x3f, 0xff, 0xf3, 0xf0, 0x3f, 0x3f, 0xff, 0xf3, 0xf0, 0xfc, 0xff, 0x03, 0xfc, 0xfc, 0xfc, 0xff, 0x03, 0xfc, 0xfc, 0x33, 0xf0, 0xfc, 0x3f, 0x30, 0x33, 0xf0, 0xfc, 0x3f, 0x30, 0x0f, 0xcf, 0xff, 0xcf, 0xc0, 0x0f, 0xcf, 0xff, 0xcf, 0xc0, 0x03, 0x3f, 0x03, 0xf3, 0x00, 0x03, 0x3f, 0x03, 0xf3, 0x00, 0x00, 0xfc, 0xfc, 0xfc, 0x00, 0x00, 0xfc, 0xfc, 0xfc, 0x00, 0x00, 0x33, 0xff, 0x30, 0x00, 0x00, 0x33, 0xff, 0x30, 0x00, 0x00, 0x0f, 0xcf, 0xc0, 0x00, 0x00, 0x0f, 0xcf, 0xc0, 0x00, 0x00, 0x03, 0x33, 0x00, 0x00, 0x00, 0x03, 0x33, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const unsigned char PROGMEM image_weather_temperature_bits_big[] = {0x03, 0xf0, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x0c, 0x0c, 0x00, 0x0c, 0x0c, 0x0c, 0x00, 0x0c, 0x0c, 0xcf, 0x00, 0x33, 0x0c, 0xcf, 0x00, 0x33, 0x0c, 0xcc, 0x00, 0x0c, 0x0c, 0xcc, 0x00, 0x0c, 0x0c, 0xcf, 0x0f, 0xc0, 0x0c, 0xcf, 0x0f, 0xc0, 0x0c, 0xcc, 0x3c, 0x00, 0x0c, 0xcc, 0x3c, 0x00, 0x0c, 0xcf, 0x30, 0x00, 0x0c, 0xcf, 0x30, 0x00, 0x0c, 0xcc, 0x30, 0x00, 0x0c, 0xcc, 0x30, 0x00, 0x0c, 0xcc, 0x3c, 0x00, 0x0c, 0xcc, 0x3c, 0x00, 0x30, 0xc3, 0x0f, 0xc0, 0x30, 0xc3, 0x0f, 0xc0, 0xc3, 0xf0, 0xc0, 0x00, 0xc3, 0xf0, 0xc0, 0x00, 0xcc, 0xfc, 0xc0, 0x00, 0xcc, 0xfc, 0xc0, 0x00, 0xcf, 0xfc, 0xc0, 0x00, 0xcf, 0xfc, 0xc0, 0x00, 0xc3, 0xf0, 0xc0, 0x00, 0xc3, 0xf0, 0xc0, 0x00, 0x30, 0x03, 0x00, 0x00, 0x30, 0x03, 0x00, 0x00, 0x0f, 0xfc, 0x00, 0x00, 0x0f, 0xfc, 0x00, 0x00};
static const unsigned char PROGMEM image_network_www_bits[] = {0x03, 0xc0, 0x0d, 0xb0, 0x32, 0x4c, 0x24, 0x24, 0x44, 0x22, 0x7f, 0xfe, 0x88, 0x11, 0x88, 0x11, 0x88, 0x11, 0x88, 0x11, 0x7f, 0xfe, 0x44, 0x22, 0x24, 0x24, 0x32, 0x4c, 0x0d, 0xb0, 0x03, 0xc0};

// Internal Constants
static const byte INTERVAL_100MS = 100;
static const unsigned int INTERVAL_2000MS = 2000;

// Task timers and intervals
// @todo Change millis() logic to avoid 49 days limit
unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
unsigned long previousMillis3 = 0;

// Counters
unsigned int currentScreen = 0; // 0: Time, 1: Temp/Humidity, 2: IP Address, 3: SSID/RSSI

// HTTP Server credentials
String localIP;
String publicIP;

// Sensors variables
String temperature;
String humidity;

// Time variables
String timeStr;
String startupTimeStr;

// Flags
bool firstRun = true;
bool isScreenOff = false;
bool lightsOff = false;
bool enableSerialOutput = true;

/**
 * FUNCTIONS
 * --------------------------------------------------------------------------------
 * --------------------------------------------------------------------------------
 * --------------------------------------------------------------------------------
 */

void blink(unsigned long currentMillis)
{
  if (lightsOff)
  {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    return;
  }

  String currentHour = timeStr.substring(11, 13);

  if (currentHour.toInt() < 7)
  {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    return;
  }

  // --- Task 1 ---
  if (currentMillis - previousMillis1 >= INTERVAL_100MS)
  {
    previousMillis1 = currentMillis;

    // Toggle the state of LED 1
    if (digitalRead(LED1) == LOW)
    {
      digitalWrite(LED1, HIGH);
    }
    else
    {
      digitalWrite(LED1, LOW);
    }
  }

  // --- Task 2 ---
  currentMillis = millis(); // Get the current time again (or use the previous one)

  if (currentMillis - previousMillis2 >= INTERVAL_100MS + 500)
  {
    previousMillis2 = currentMillis;

    // Toggle the state of LED 2
    if (digitalRead(LED2) == LOW)
    {
      digitalWrite(LED2, HIGH);
    }
    else
    {
      digitalWrite(LED2, LOW);
    }
  }
}

String getFormattedLocalDateTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return "(Error)";
  }
  char timeString[25];                                                      // Buffer to hold formatted string
  strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", &timeinfo); // Format: "YYYY-MM-DD HH:MM:SS"
  return timeString;
}

String getUptime()
{
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

void getLT()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
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

void readSensors()
{
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

void printMemoryUsage()
{
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

// @todo Use secure request (HTTPS)
void getPublicIP()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    WiFiClientSecure client;
    HTTPClient https;
    X509List cert(rootCACertificate);
    client.setTrustAnchors(&cert);

    if (https.begin(client, AWS_CHECK_IP_URL))
    {
      int httpCode = https.GET();
      if (httpCode > 0)
      {
        Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
        String payload = https.getString();
        Serial.println(payload);
        publicIP = payload;
      }
      else
      {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }
      https.end();
    }
  }
}

// @todo Use secure request (HTTPS)
void telegramRequest(unsigned long currentMillis)
{
  if (firstRun || (currentMillis - previousMillis3 >= REQUEST_INTERVAL))
  {
    previousMillis3 = currentMillis;
    if (WiFi.status() == WL_CONNECTED)
    {
      WiFiClientSecure client;
      HTTPClient https;
      X509List cert(rootCACertificateTelegram);
      client.setTrustAnchors(&cert);
      // std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
      // @todo - Implement certificate validation using BearSSL::X509List and client->setTrustAnchors()
      // Use this to skip certificate validation
      // client->setInsecure();
      https.setReuse(false);
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

      if (https.begin(client, url))
      {
        int httpCode = https.GET();
        if (httpCode > 0)
        {
          Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
          String payload = https.getString();
          Serial.println(payload);
        }
        else
        {
          Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
        }
        https.end();
      }
    }
    firstRun = false;
  }
}

void nokiaRingtone()
{
  int noteDurations[] = {300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 600};

  // Nokia Ringtone (E5 D5 F#4 G#4 - C#5 B4 D4 E4 - B4 A4 C#4 E4 A4)
  int nokiaRingtone[] = {NOTE_E5, NOTE_D5, NOTE_FS4, NOTE_GS4, NOTE_CS5, NOTE_B4, NOTE_D4, NOTE_E4, NOTE_B4, NOTE_A4, NOTE_CS4, NOTE_E4, NOTE_A4};

  for (int i = 0; i < 13; i++)
  {
    tone(BUZZER_PIN, nokiaRingtone[i], noteDurations[i]);

    // Pause between notes
    delay(noteDurations[i] * 1.30);
    noTone(BUZZER_PIN);
  }
}

void playAlarm(unsigned long currentMillis)
{
  // timeStr Format: "YYYY-MM-DD HH:MM:SS"
  String currentMinute = timeStr.substring(14, 16);
  String currentHour = timeStr.substring(11, 13);
  Serial.println("currentMinute: " + String(currentMinute));
  Serial.println("currentHour: " + String(currentHour));
  if ((currentMinute == "00" && currentHour == "07") || (currentMinute == "00" && currentHour == "08"))
  {
    Serial.println("Playing alarm...");
    nokiaRingtone();
  }
  delay(20);
}

void readButton()
{
  int buttonState = digitalRead(BUTTON_PIN);
  Serial.println("Button state: " + String(buttonState));

  if (buttonState == LOW)
  {
    ++currentScreen;
    if (currentScreen > 4)
    {
      currentScreen = 0;
    }
    /*
    if (currentScreen == 2)
    {
      lightsOff = true;
      isScreenOff = true;
      display.ssd1306_command(SSD1306_DISPLAYOFF); // Send sleep command (0xAE)
    }
    if (currentScreen > 2)
    {
      lightsOff = false;
      isScreenOff = false;
      display.ssd1306_command(SSD1306_DISPLAYON); // Send wake command (0xAF)
      display.clearDisplay();
      display.display();
      currentScreen = 0;
    }*/
    delay(200); // Debounce delay
  }
}

/**
 * SCREEN DRAWING FUNCTIONS
 * --------------------------------------------------------------------------------
 * --------------------------------------------------------------------------------
 * --------------------------------------------------------------------------------
 */

// Screen 1: Temperature and Humidity + Date/Time + WiFi RSSI screen
void drawScreen_1(void)
{
  if (lightsOff)
  {
    display.ssd1306_command(SSD1306_DISPLAYOFF); // Send sleep command (0xAE)
    isScreenOff = true;
    return;
  }
  else
  {
    display.ssd1306_command(SSD1306_DISPLAYON); // Send wake command (0xAF)
    isScreenOff = false;
  }
  String currentMinute = timeStr.substring(14, 16);
  String currentHour = timeStr.substring(11, 13);
  Serial.println("currentMinute: " + String(currentMinute));
  Serial.println("currentHour: " + String(currentHour));
  if (currentHour.toInt() < 7 && !isScreenOff)
  {
    display.ssd1306_command(SSD1306_DISPLAYOFF); // Send sleep command (0xAE)
    isScreenOff = true;
    return;
  }
  else if (currentHour.toInt() >= 7 && isScreenOff)
  {
    display.ssd1306_command(SSD1306_DISPLAYON); // Send wake command (0xAF)
    isScreenOff = false;
  }

  display.clearDisplay();
  getLT();
  readSensors();
  if (WiFi.status() == WL_CONNECTED)
  {
    // wifi_full
    display.drawBitmap(89, 0, image_wifi_full_bits, 19, 16, 1);
  }
  else
  {
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
  // string 8
  display.setTextSize(1);
  display.setCursor(48, 50);
  // string 9
  display.setCursor(110, 5);
  if (WiFi.status() == WL_CONNECTED)
  {
    display.print("On");
  }
  else
  {
    display.print("Off");
  }
  display.display();
}

// Screen 2: WiFi SSID + RSSI screen
void drawScreen_2(void)
{
  if (lightsOff)
  {
    display.ssd1306_command(SSD1306_DISPLAYOFF); // Send sleep command (0xAE)
    isScreenOff = true;
    return;
  }
  else
  {
    display.ssd1306_command(SSD1306_DISPLAYON); // Send wake command (0xAF)
    isScreenOff = false;
  }
  int rssi = WiFi.RSSI();
  String ssid = WiFi.SSID();
  display.clearDisplay();
  if (rssi >= -50)
  {
    display.drawBitmap(88, 3, image_wifi_full_bits_2, 38, 32, 1);
  }
  else if (rssi < -50 && rssi >= -60)
  {
    display.drawBitmap(88, 3, image_wifi_4_bars_bits, 38, 32, 1);
  }
  else if (rssi < -60 && rssi >= -70)
  {
    display.drawBitmap(88, 3, image_wifi_3_bars_bits, 38, 32, 1);
  }
  else if (rssi < -70 && rssi >= -80)
  {
    display.drawBitmap(88, 3, image_wifi_2_bars_bits, 38, 32, 1);
  }
  else if (rssi < -80 && rssi >= -90)
  {
    display.drawBitmap(88, 3, image_wifi_1_bits, 38, 32, 1);
  }
  else if (rssi < -90 || WiFi.status() != WL_CONNECTED)
  {
    display.drawBitmap(88, 3, image_wifi_not_connected_bits2, 38, 32, 1);
  }

  // string 2
  display.setTextColor(1);
  display.setTextSize(1);
  display.setTextWrap(false);
  display.setCursor(2, 5);
  display.print("SSID:");
  // string 3
  display.setCursor(2, 28);
  display.print("RSSI:");
  // string 4
  display.setCursor(2, 15);
  display.print(ssid.substring(0, 15)); // ABCDEFGHIJKLMN
  // string 5
  display.setTextSize(3);
  display.setCursor(-5, 39);
  display.print(rssi);
  // string 6
  display.setCursor(72, 39);
  display.print("dBM");
  display.display();
}

// Screen 3: Temperatura and Humidity screen
void drawScreen_3(void)
{
  if (lightsOff)
  {
    display.ssd1306_command(SSD1306_DISPLAYOFF); // Send sleep command (0xAE)
    isScreenOff = true;
    return;
  }
  else
  {
    display.ssd1306_command(SSD1306_DISPLAYON); // Send wake command (0xAF)
    isScreenOff = false;
  }
  display.clearDisplay();
  readSensors();
  // string 1
  display.setTextColor(1);
  display.setTextWrap(false);
  display.setCursor(0, 1);
  display.setTextSize(1);
  display.print("Temperature:");
  // weather_temperature
  display.drawBitmap(96, 29, image_weather_temperature_bits_big, 32, 32, 1);
  // string 3
  display.setCursor(0, 36);
  display.print("Humidity:");
  // string 4
  display.setTextSize(2);
  display.setCursor(0, 14);
  display.print(temperature + " C");
  // string 5
  display.setCursor(0, 49);
  display.print(humidity + " %");
  display.display();
}

// Screen 4: IP and Voltage screen
void drawScreen_4(void)
{
  float voltage = ESP.getVcc(); // / 1000.0 // Get voltage in millivolts and convert to volts
  display.clearDisplay();
  // string 1
  display.setTextSize(1);
  display.setTextColor(1);
  display.setTextWrap(false);
  display.setCursor(1, 1);
  display.print("PubIP:");
  // string 2
  display.setCursor(38, 10);
  display.print(publicIP);
  // string 5
  display.setCursor(1, 47);
  display.print("Voltage:");
  // string 3
  display.setCursor(1, 22);
  display.print("Lo IP:");
  // string 4
  display.setCursor(38, 31);
  display.print(localIP);
  // string 6
  display.setCursor(50, 42);
  display.print(voltage / 1000.0);
  // string 7
  display.setCursor(98, 42);
  display.print("V");
  // string 8
  display.setCursor(50, 53);
  display.print(voltage);
  // string 9
  display.setCursor(92, 53);
  display.print("mV");
  display.drawBitmap(110, 44, image_network_www_bits, 16, 16, 1);
  display.display();
}

void drawScreen_5(void)
{
  display.clearDisplay();
  // string 1
  display.setTextColor(1);
  display.setTextWrap(false);
  display.setCursor(1, 4);
  display.print("Free Heap: ");
  // string 2
  display.setCursor(116, 5);
  display.print("KB");
  // string 4
  display.setCursor(72, 5);
  display.print(ESP.getFreeHeap() / 1024);
  // string 4
  display.setCursor(1, 20);
  display.print("Fragmentation: ");
  // string 5
  display.setCursor(72, 32);
  display.print(ESP.getHeapFragmentation());
  // string 6
  display.setCursor(116, 30);
  display.print("%");
  // string 7
  display.setCursor(1, 43);
  display.print("Max Free Block: ");
  // string 8
  display.setCursor(72, 54);
  display.print(ESP.getMaxFreeBlockSize());
  // string 9
  display.setCursor(116, 54);
  display.print("B");
  display.display();
}

// Switch between screens
void showScreen()
{
  switch (currentScreen)
  {
  case 0:
    drawScreen_1();
    break;
  case 1:
    drawScreen_2();
    break;
  case 2:
    drawScreen_3();
    break;
  case 3:
    drawScreen_4();
    break;
  case 4:
    drawScreen_5();
    break;
  default:
    drawScreen_1();
    break;
  }
}

/**
 * SETUP AND LOOP
 * --------------------------------------------------------------------------------
 * --------------------------------------------------------------------------------
 * --------------------------------------------------------------------------------
 */

void setup()
{
  Serial.begin(115200);
  dht.begin();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Wire.begin(ALTERNATIVE_I2C_SDA, ALTERNATIVE_I2C_SCL);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  delay(100);             // Wait for the display to power on completely
  display.clearDisplay(); // Clear the buffer
  display.display();

  Serial.printf("\n");
  Serial.printf("Conectando a Wi-Fi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println(".");
  }

  Serial.print("Conectado con la IP: ");
  Serial.println(WiFi.localIP());
  localIP = WiFi.localIP().toString();
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Activa la resistencia interna
  configTime(NTP_GMT_OFFSET_SEC, NTP_DAYLIGHT_OFFSET_SEC, NTP_SERVER);
  startupTimeStr = getFormattedLocalDateTime();
}

void loop()
{
  unsigned long currentMillis = millis();
  readButton();
  blink(currentMillis);
  showScreen();
  playAlarm(currentMillis);
  telegramRequest(currentMillis);
}
