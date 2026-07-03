#include <Adafruit_GFX.h>
#include <Adafruit_GC9A01A.h>
#include <BLEDevice.h>
#include "NTP.h"
#include "src/secrets/Secrets.h"
#include <SPI.h>
#include <time.h>
#include <WiFi.h>

// Define custom SPI pins for ESP32-C3 Super Mini
#define TFT_SCLK 4
#define TFT_MOSI 6
#define TFT_RST 8
#define TFT_DC 9
#define TFT_CS 10

// Initialize the display using software SPI to support custom C3 pins
Adafruit_GC9A01A tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

// 16-bit Framebuffer Canvas (~115KB RAM, comfortably fits inside C3's 400KB SRAM)
GFXcanvas16 canvas(240, 240);

// Simulated Clock State Variables
int hours = 14; // 24hr format internally
int minutes = 45;
int seconds = 0;
unsigned long lastTick = 0;

// Time variables
String timeStr;

// some extra colors
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define ORANGE 0xFBE0
#define GREY 0x84B5
#define BORDEAUX 0xA000

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
  hours = timeinfo.tm_hour;
  minutes = timeinfo.tm_min;
  seconds = timeinfo.tm_sec;
}

void setup(void)
{
  // Explicitly Disable BLE
  Serial.println("Disabling BLE...");
  BLEDevice::deinit(true);

  delay(2000); // Give the chip 2 seconds to start instead of waiting for USB

  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(BLACK);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.printf("\n");
  Serial.printf("Conectando a Wi-Fi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println(".");
  }

  configTime(NTP_GMT_OFFSET_SEC, NTP_DAYLIGHT_OFFSET_SEC, NTP_SERVER);
}

void loop()
{
  // Simple timekeeping loop (updates every 1000 milliseconds)
  if (millis() - lastTick >= 500)
  {
    lastTick = millis();
    /*
    seconds++;
    if (seconds >= 60)
    {
      seconds = 0;
      minutes++;
    }
    if (minutes >= 60)
    {
      minutes = 0;
      hours++;
    }
    if (hours >= 24)
    {
      hours = 0;
    }
    */
    getLT();
    // Redraw memory buffer frame
    renderFancyDigitalClock();
  }
}

void renderFancyDigitalClock()
{
  // 1. Draw UI Background Elements
  canvas.fillScreen(0x0841); // Sleek Dark Charcoal Grey (0x0841)

  // Decorative internal frame rings
  canvas.drawCircle(120, 120, 118, 0x18C3); // Subtle muted inner frame border

  // 2. Draw a dotted progress ring for seconds along the perimeter
  for (int i = 0; i < 60; i++)
  {
    float angle = (i * 6.0 - 90.0) * 3.14159 / 180.0;
    int dotX = 120 + cos(angle) * 110;
    int dotY = 120 + sin(angle) * 110;

    // If the tick marks a past second, highlight it neon orange; else leave it dark grey
    uint16_t dotColor = (i <= seconds) ? 0xFD20 : 0x31A6;
    canvas.fillCircle(dotX, dotY, 2, dotColor);
  }

  // 3. Render Large Custom Digital Typography
  canvas.setTextColor(0xFFFF); // Bright White Text
  canvas.setTextSize(4);       // Big default block font

  // Format Time String (HH:MM)
  char timeBuffer[6];
  sprintf(timeBuffer, "%02d:%02d", hours, minutes);

  // Draw centered text string (adjusted offset for scale 4 font sizes)
  canvas.setCursor(60, 95);
  canvas.print(timeBuffer);

  // 4. Render Bottom Info Bracket (AM/PM and Seconds Counter)
  canvas.setTextSize(2);
  canvas.setTextColor(0x7BEF); // Light Accent Gray
  canvas.setCursor(85, 140);

  if (hours >= 12)
  {
    canvas.print("PM ");
  }
  else
  {
    canvas.print("AM ");
  }

  // Print dynamic trailing zero seconds explicitly
  if (seconds < 10)
    canvas.print("0");
  canvas.print(seconds);

  // 5. Blast the finalized memory state to the display chip instantly
  tft.drawRGBBitmap(0, 0, canvas.getBuffer(), 240, 240);
}