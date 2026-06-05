/*********
  Complete project details at https://randomnerdtutorials.com
  
  This is an example for our Monochrome OLEDs based on SSD1306 drivers. Pick one up today in the adafruit shop! ------> http://www.adafruit.com/category/63_98
  This example is for a 128x32 pixel display using I2C to communicate 3 pins are required to interface (two I2C and one reset).
  Adafruit invests time and resources providing this open source code, please support Adafruit and open-source hardware by purchasing products from Adafruit!
  Written by Limor Fried/Ladyada for Adafruit Industries, with contributions from the open source community. BSD license, check license.txt for more information All text above, and the splash screen below must be included in any redistribution. 
*********/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int alternative_SDA = 12; 
int alternative_SCL = 14; 

void setup() {
  Serial.begin(115200);
  Wire.begin(alternative_SDA, alternative_SCL);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  display.drawPixel(10, 10, WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000);
  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // display.display(). These examples demonstrate both approaches...

  drawScreen_1(); // Animate bitmaps
}

void loop() {
}


// [BEGIN lopaka generated]
static const unsigned char PROGMEM image_Clock_bits[] = {0x03,0xf0,0x00,0x0f,0xfc,0x00,0x1f,0xfe,0x00,0x38,0x07,0x00,0x7d,0xef,0x80,0x7d,0xef,0x80,0xfd,0xef,0xc0,0xfe,0xdf,0xc0,0xff,0x3f,0xc0,0xff,0x3f,0xc0,0xfe,0x9f,0xc0,0xfd,0x4f,0xc0,0x7c,0xaf,0x80,0x7d,0x4f,0x80,0x38,0x07,0x00,0x1f,0xfe,0x00,0x0f,0xfc,0x00,0x03,0xf0,0x00};

static const unsigned char PROGMEM image_weather_temperature_bits[] = {0x1c,0x00,0x22,0x02,0x2b,0x05,0x2a,0x02,0x2b,0x38,0x2a,0x60,0x2b,0x40,0x2a,0x40,0x2a,0x60,0x49,0x38,0x9c,0x80,0xae,0x80,0xbe,0x80,0x9c,0x80,0x41,0x00,0x3e,0x00};

static const unsigned char PROGMEM image_wifi_full_bits[] = {0x01,0xf0,0x00,0x07,0xfc,0x00,0x1e,0x0f,0x00,0x39,0xf3,0x80,0x77,0xfd,0xc0,0xef,0x1e,0xe0,0x5c,0xe7,0x40,0x3b,0xfb,0x80,0x17,0x1d,0x00,0x0e,0xee,0x00,0x05,0xf4,0x00,0x03,0xb8,0x00,0x01,0x50,0x00,0x00,0xe0,0x00,0x00,0x40,0x00,0x00,0x00,0x00};

static const unsigned char PROGMEM image_wifi_not_connected_bits[] = {0x21,0xf0,0x00,0x16,0x0c,0x00,0x08,0x03,0x00,0x25,0xf0,0x80,0x42,0x0c,0x40,0x89,0x02,0x20,0x10,0xa1,0x00,0x23,0x58,0x80,0x04,0x24,0x00,0x08,0x52,0x00,0x01,0xa8,0x00,0x02,0x04,0x00,0x00,0x42,0x00,0x00,0xa1,0x00,0x00,0x40,0x80,0x00,0x00,0x00};


void drawScreen_1(void) {
  for(;;) { // Loop forever...
        display.clearDisplay();
    // wifi_not_connected
    display.drawBitmap(89, 0, image_wifi_not_connected_bits, 19, 16, 1);
    // wifi_full
    display.drawBitmap(89, 0, image_wifi_full_bits, 19, 16, 1);
    // string 3
    display.setTextColor(1);
    display.setTextSize(2);
    display.setTextWrap(false);
    display.setCursor(4, 24);
    display.print("16:50:01");
    // string 4
    display.setCursor(4, 5);
    display.print("31.3");
    // string 5
    display.setCursor(4, 44);
    display.print("CST");
    // weather_temperature
    display.drawBitmap(59, 4, image_weather_temperature_bits, 16, 16, 1);
    // Clock
    display.drawBitmap(110, 24, image_Clock_bits, 18, 18, 1);
    // string 8
    display.setTextSize(1);
    display.setCursor(48, 50);
    display.print("Calorcito");
    // string 9
    display.setCursor(110, 5);
    display.print("Off");
    display.display();
  }
}