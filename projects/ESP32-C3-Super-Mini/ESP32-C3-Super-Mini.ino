/*!
 * @file ESP32-C3-Super-Mini.ino
 * @author @Telematica
 * @brief ESP32 C3 Super Mini project with NTP for time synchronization.
 *
 */
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <esp_system.h>
#include <HTTPClient.h>
#include "NTP.h"
#include "src/secrets/Secrets.h"
#include <SPI.h>
#include <time.h>
// #include <UrlEncode.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <Wire.h>

// #define ARDUINO_USB_CDC_ON_BOOT 1
// #define ARDUINO_USB_MODE 1
#define I2C_SDA 5        // GPIO5 / SDA (Data)
#define I2C_SCL 6        // GPIO5 / SCL / SCK (Clock)
#define LED LED_BUILTIN  // GPI08 BLUE_LED
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// WiFiClientSecure client; // WiFi Client for HTTP requests
// HTTPClient https;

// [BEGIN lopaka generated]
static const unsigned char PROGMEM image_clock_quarters_bits[] = {0x07, 0xc0, 0x19, 0x30, 0x21, 0x08, 0x40, 0x04, 0x41, 0x04, 0x81, 0x02, 0x81, 0x02, 0xe1, 0x0e, 0x80, 0x82, 0x80, 0x42, 0x40, 0x04, 0x40, 0x04, 0x21, 0x08, 0x19, 0x30, 0x07, 0xc0, 0x00, 0x00};
static const unsigned char PROGMEM image_wifi_full_bits_3[] = {0x01, 0xf0, 0x00, 0x07, 0xfc, 0x00, 0x1e, 0x0f, 0x00, 0x39, 0xf3, 0x80, 0x77, 0xfd, 0xc0, 0xef, 0x1e, 0xe0, 0x5c, 0xe7, 0x40, 0x3b, 0xfb, 0x80, 0x17, 0x1d, 0x00, 0x0e, 0xee, 0x00, 0x05, 0xf4, 0x00, 0x03, 0xb8, 0x00, 0x01, 0x50, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00};
static const unsigned char PROGMEM image_wifi_75_bits[] = {0x01, 0xf0, 0x00, 0x06, 0x0c, 0x00, 0x18, 0x03, 0x00, 0x21, 0xf0, 0x80, 0x47, 0xfc, 0x40, 0x8f, 0x1e, 0x20, 0x5c, 0xe7, 0x40, 0x3b, 0xfb, 0x80, 0x17, 0x1d, 0x00, 0x0e, 0xee, 0x00, 0x05, 0xf4, 0x00, 0x03, 0xb8, 0x00, 0x01, 0x50, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00};
static const unsigned char PROGMEM image_wifi_50_bits[] = {0x01, 0xf0, 0x00, 0x06, 0x0c, 0x00, 0x18, 0x03, 0x00, 0x21, 0xf0, 0x80, 0x46, 0x0c, 0x40, 0x88, 0x02, 0x20, 0x50, 0xe1, 0x40, 0x23, 0xf8, 0x80, 0x17, 0x1d, 0x00, 0x0e, 0xee, 0x00, 0x05, 0xf4, 0x00, 0x03, 0xb8, 0x00, 0x01, 0x50, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00};
static const unsigned char PROGMEM image_wifi_3_bars_bits[] = {0x01, 0xf0, 0x00, 0x06, 0x0c, 0x00, 0x18, 0x03, 0x00, 0x21, 0xf0, 0x80, 0x46, 0x0c, 0x40, 0x88, 0x02, 0x20, 0x10, 0xe1, 0x00, 0x23, 0xf8, 0x80, 0x07, 0x1c, 0x00, 0x0c, 0x46, 0x00, 0x05, 0xf4, 0x00, 0x03, 0xb8, 0x00, 0x01, 0x50, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00};
static const unsigned char PROGMEM image_wifi_25_bits[] = {0x01, 0xf0, 0x00, 0x06, 0x0c, 0x00, 0x18, 0x03, 0x00, 0x21, 0xf0, 0x80, 0x46, 0x0c, 0x40, 0x88, 0x02, 0x20, 0x50, 0xe1, 0x40, 0x23, 0x18, 0x80, 0x14, 0x05, 0x00, 0x08, 0xe2, 0x00, 0x05, 0xf4, 0x00, 0x03, 0xb8, 0x00, 0x01, 0x50, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00};
static const unsigned char PROGMEM image_wifi_2_bits[] = {0x01, 0xf0, 0x00, 0x06, 0x0c, 0x00, 0x18, 0x03, 0x00, 0x21, 0xf0, 0x80, 0x46, 0x0c, 0x40, 0x88, 0x02, 0x20, 0x10, 0xe1, 0x00, 0x23, 0x18, 0x80, 0x04, 0x04, 0x00, 0x08, 0x42, 0x00, 0x01, 0xb0, 0x00, 0x02, 0x08, 0x00, 0x00, 0x40, 0x00, 0x00, 0xa0, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00};
static const unsigned char PROGMEM image_wifi_not_connected_bits[] = {0x21, 0xf0, 0x00, 0x16, 0x0c, 0x00, 0x08, 0x03, 0x00, 0x25, 0xf0, 0x80, 0x42, 0x0c, 0x40, 0x89, 0x02, 0x20, 0x10, 0xa1, 0x00, 0x23, 0x58, 0x80, 0x04, 0x24, 0x00, 0x08, 0x52, 0x00, 0x01, 0xa8, 0x00, 0x02, 0x04, 0x00, 0x00, 0x42, 0x00, 0x00, 0xa1, 0x00, 0x00, 0x40, 0x80, 0x00, 0x00, 0x00};

// Internal Constants
static const byte INTERVAL_100MS = 100;
static const unsigned int INTERVAL_5000MS = 5000;

// Task timers and intervals
unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
unsigned long previousMillis3 = 0;

// Counters
unsigned int currentScreen = 0; // 0: Time, 1: SSID/RSSI, 2: Uptime, 3: Public IP and Local IP

// HTTP Server credentials
String localIP;
String publicIP;

// Time variables
String timeStr;
String startupTimeStr;

// Flags
bool firstRun = true;
// bool isScreenOff = false;
// bool lightsOff = false;
// bool enableSerialOutput = true;

/**
 * FUNCTIONS
 * --------------------------------------------------------------------------------
 * --------------------------------------------------------------------------------
 * --------------------------------------------------------------------------------
 */

void blink(unsigned long currentMillis)
{
  // String currentHour = timeStr.substring(11, 13);
  currentMillis = millis(); // Get the current time again (or use the previous one)
  if (currentMillis - previousMillis2 >= INTERVAL_100MS + 500)
  {
    previousMillis2 = currentMillis;

    // Toggle the state of LED2 every 500ms
    if (digitalRead(LED) == LOW)
    {
      digitalWrite(LED, HIGH);
    }
    else
    {
      digitalWrite(LED, LOW);
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
  char timeString[25];                                                      // Buffer to hold formatted string
  strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", &timeinfo); // Format: "YYYY-MM-DD HH:MM:SS"
  Serial.println(timeString);
  timeStr = timeString;
}

void printMemoryUsage()
{
  Serial.print("Total Heap Size: ");
  Serial.println(ESP.getHeapSize());

  Serial.print("Free Heap Memory: ");
  Serial.println(ESP.getFreeHeap());

  Serial.print("Minimum Ever Free Heap: ");
  Serial.println(esp_get_minimum_free_heap_size()); // Requires #include <esp_system.h>
}

void getPublicIP(unsigned long currentMillis)
{
  if (currentMillis - previousMillis3 >= INTERVAL_100MS + 1900)
  {
    previousMillis3 = currentMillis;
  } else {
    return;
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    // 1. Create instances for secure client and HTTP handler
    WiFiClientSecure client;
    HTTPClient https;

    // configTime(0, 0, "pool.ntp.org", "time.nist.gov");
    
    // 2. Assign the Root CA certificate to validate the server
    client.setCACert(rootCACertificateCheckIPAmazon);

    // Note: For quick testing without certificate validation, uncomment below:
    // client.setInsecure();

    Serial.println("\nStarting secure HTTP request...");
    https.setReuse(false);
    https.addHeader("User-Agent", "ESP32-C3");
    https.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);
    https.setTimeout(10000);


    // 3. Initialize the HTTP connection over TLS
    if (https.begin(client, AWS_CHECK_IP_URL))
    {

      // 4. Send HTTP GET Request
      int httpCode = https.GET();

      Serial.printf("[HTTPS] Response code: %d\n", httpCode);

      // 5. Evaluate the Response Code
      if (httpCode > 0)
      {
        Serial.printf("[HTTPS] Response code: %d\n", httpCode);

        if (httpCode == HTTP_CODE_OK)
        {
          String payload = https.getString();
          publicIP = payload;
          Serial.println("[HTTPS] Payload received:");
          Serial.println(payload);
        }
      }
      else
      {
        Serial.printf("[HTTPS] Connection failed, error: %s\n", https.errorToString(httpCode).c_str());
        firstRun = true;
        return;
      }

      // 6. Close the connection resource
      https.end();
    }
    else
    {
      Serial.println("[HTTPS] Unable to connect to server endpoint");
      firstRun = true;
      return;
    }
    firstRun = false;
  }
}

// Screen 1: Date/Time screen
void drawScreen_1(void)
{
  getLT();
  display.clearDisplay();
  // string 2
  display.setTextColor(1);
  display.setTextSize(2);
  display.setTextWrap(false);
  display.setCursor(1, 1);
  display.print(timeStr.substring(0, 10));
  // clock_quarters
  display.drawBitmap(113, 16, image_clock_quarters_bits, 15, 16, 1);
  // string 4
  display.setCursor(1, 17);
  display.print(timeStr.substring(11, 19));
  display.display();
}

// Screen 2: WiFi SSID + RSSI screen
void drawScreen_2(void)
{
  int rssi = WiFi.RSSI();
  String ssid = WiFi.SSID();
  display.clearDisplay();
  Serial.println("SSID: " + ssid);
  Serial.println("RSSI: " + String(rssi) + " dBm");
  const unsigned char *selectedWifiIcon = image_wifi_not_connected_bits;
  if (WiFi.status() == WL_CONNECTED)
  {
    if (rssi >= -50)
    {
      selectedWifiIcon = image_wifi_full_bits_3;
    }
    else if (rssi >= -60)
    {
      selectedWifiIcon = image_wifi_75_bits;
    }
    else if (rssi >= -70)
    {
      selectedWifiIcon = image_wifi_50_bits;
    }
    else if (rssi >= -80)
    {
      selectedWifiIcon = image_wifi_3_bars_bits;
    }
    else if (rssi >= -90)
    {
      selectedWifiIcon = image_wifi_25_bits;
    }
    else
    {
      selectedWifiIcon = image_wifi_2_bits;
    }
  }
  // string 1
  display.setTextColor(1);
  display.setTextWrap(false);
  display.setCursor(1, 4);
  display.setTextSize(1);
  display.print("SSID:");
  // string 2
  display.setCursor(31, 4);
  display.print(ssid.substring(0, 17));
  // string 3
  display.setCursor(1, 20);
  display.print("RSSI:");
  // string 4
  display.setTextSize(2);
  display.setCursor(31, 16);
  display.print(rssi);
  // string 5
  display.setTextSize(1);
  display.setCursor(82, 23);
  display.print("dBm");
  // wifi_full
  display.drawBitmap(107, 16, selectedWifiIcon, 19, 16, 1);
  display.display();
}

// Screen 3: IP
void drawScreen_3(void) {
    display.clearDisplay();
    // string 1
    display.setTextColor(1);
    display.setTextWrap(false);
    display.setTextSize(1);
    display.setCursor(1, 5);
    display.print("PubIP:");
    // string 2
    display.setCursor(37, 5);
    display.print(publicIP);
    // string 3
    display.setCursor(7, 19);
    display.print("LoIP:");
    // string 4
    display.setCursor(37, 19);
    display.print(localIP);
    display.display();
}

// Switch between screens
void showScreen(unsigned long currentMillis)
{
  if (currentMillis - previousMillis1 >= INTERVAL_5000MS)
  {
    previousMillis1 = currentMillis;
    ++currentScreen;
    if (currentScreen > 2)
    {
      currentScreen = 0;
    }
  }
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
  default:
    currentScreen = 0;
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
  // put your setup code here, to run once:
  Serial.begin(115200);

  // delay(2000); // Give the chip 2 seconds to start instead of waiting for USB

  // Set I2C clock speed to 400kHz (Fast Mode) pr 100kHz (Standard Mode)
  Wire.setClock(100000);

  // dht.begin();
  Wire.begin(I2C_SDA, I2C_SCL);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }

  // Clear the buffer
  display.clearDisplay();

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();

  // client.setCACert(rootCACertificateCheckIPAmazon); // Connect to Wi-Fi first, then apply the cert

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

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
  pinMode(LED, OUTPUT);
  // pinMode(BUTTON_PIN, INPUT_PULLUP); // Activa la resistencia interna
  configTime(NTP_GMT_OFFSET_SEC, NTP_DAYLIGHT_OFFSET_SEC, NTP_SERVER);
  startupTimeStr = getFormattedLocalDateTime();
}

void loop()
{
  unsigned long currentMillis = millis();
  Serial.println("firstRun " + String(firstRun));
  if (false)
  {
    getPublicIP(currentMillis);
  }
  blink(currentMillis);
  showScreen(currentMillis);
}
