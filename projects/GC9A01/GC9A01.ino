#include <Adafruit_GFX.h>
#include <Adafruit_GC9A01A.h>
#include <BLEDevice.h>
#include "NTP.h"
#include "src/secrets/Secrets.h"
#include <SPI.h>
#include <time.h>
#include <WiFi.h>

#define TFT_WIDTH  240
#define TFT_HEIGHT 240

// TUS PINES ORIGINALES (Cables como los tenías al principio)
#define TFT_SCLK 4
#define TFT_MOSI 6
#define TFT_RST 8
#define TFT_DC 9
#define TFT_CS 10

// Inicializamos usando SPI por Software (Seguro y comprobado que te funciona)
Adafruit_GC9A01A tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

// Variables de tiempo
int hours = 0; 
int minutes = 0;
int seconds = 0;
int lastSecond = -1; // Para rastrear cuándo cambia el tiempo

#define BLACK 0x0000
#define WHITE 0xFFFF
#define BG_COLOR 0x0841 // Charcoal Grey del fondo

void getLT()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return;
  
  hours = timeinfo.tm_hour;
  minutes = timeinfo.tm_min;
  seconds = timeinfo.tm_sec;
}

// Función que dibuja el fondo estático UNA SOLA VEZ
void drawStaticUI()
{
  tft.fillScreen(BG_COLOR); 
  tft.drawCircle(120, 120, 118, 0x18C3); // Borde interno
}

// Función que actualiza SOLAMENTE los números
void updateDynamicClock()
{
  // 1. Hora y Minutos (Tamaño 4)
  tft.setTextSize(4);
  
  // EL TRUCO: Al pasar dos colores (Texto, Fondo), el texto nuevo borra al viejo sin parpadear
  tft.setTextColor(WHITE, BG_COLOR); 
  tft.setCursor(60, 95);
  
  char timeBuffer[6];
  sprintf(timeBuffer, "%02d:%02d", hours, minutes);
  tft.print(timeBuffer);

  // 2. Segundos y AM/PM (Tamaño 2)
  tft.setTextSize(2);
  tft.setTextColor(0x7BEF, BG_COLOR); // Light Accent Gray sobre el fondo
  tft.setCursor(85, 140);

  if (hours >= 12) {
    tft.print("PM ");
  } else {
    tft.print("AM ");
  }

  char secBuffer[3];
  sprintf(secBuffer, "%02d", seconds);
  tft.print(secBuffer);
}

void setup(void)
{
  Serial.begin(115200);
  
  Serial.println("Disabling BLE...");
  BLEDevice::deinit(true);
  delay(1000); 

  // Iniciar pantalla
  tft.begin();
  tft.setRotation(3);
  
  // Dibujamos la interfaz base estática
  drawStaticUI();

  // Conexión WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Conectando WiFi...");
  }

  configTime(NTP_GMT_OFFSET_SEC, NTP_DAYLIGHT_OFFSET_SEC, NTP_SERVER);
}

void loop()
{
  getLT(); // Consultar el reloj interno

  // Si el segundo cambió, actualizamos solo el texto en pantalla
  if (seconds != lastSecond)
  {
    updateDynamicClock();
    lastSecond = seconds;
  }
  
  delay(20); // Pequeña pausa para no saturar el procesador
}