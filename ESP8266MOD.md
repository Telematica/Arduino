# ESP8266MOD NodeMCU V3

## Specs

### Core Hardware Specifications

* Microcontroller: Tensilica 32-bit RISC CPU (ESP8266EX) (Single Core)
* Clock Speed: 80 MHz or 160 MHz
* Flash Memory: 4 MB
* RAM: 96 KB data RAM + 64 KB instruction RAM
* USB-to-Serial: CH340G chip
* Antenna: Integrated PCB trace antenna
* Core Version: 3.1.2

### Wi-Fi Specifications

* Wireless Standard: IEEE 802.11 b/g/n
* Frequency Range: 2.4 GHz - 2.5 GHz
* Modes: Station (Client), SoftAP (Access Point), and Station + SoftAP
* Security: WEP, WPA, WPA2

### Power Specifications

* Operating Voltage: 3.3V
* Input Voltage (VIN): 4.5V to 9V (via USB or external supply)
* Current Draw: ~70 mA (up to 200 mA max during transmission); < 200 μ A in deep sleep

### Pinout & Interfaces

* GPIOs: 16 pins
* ADC: 1 channel (10-bit resolution, 0-1V range)
* Protocols: UART, SPI, I2C (software implementation), and I2S
* PWM: Available on all GPIO pins

### Physical Dimensions

* Size: ~58 mm × 31 mm × 13 mm (slightly wider than the V2 for broader breadboard compatibility)
* Weight: 10 grams

### Misc Specs

* Operating temperature: from -40°C to +125°C

## Constraints

1) Board Labels vs. GPIO Numbers (Most Common)

    * Many boards (like the NodeMCU or Wemos D1 Mini) have silkscreen labels like D1, D2, or D3. These do not correspond to the actual GPIO numbers required in the code.

    * __The Fix__: If you want to use pin D5, you must declare pinMode(D5, OUTPUT); or use the direct GPIO mapping (e.g., GPIO 14).

2) Pin Constraints at Boot

    * The ESP8266 has specific pins that dictate how the chip boots up. If these pins are pulled HIGH or LOW by an attached component (like a relay or button) when the board powers on, the board may fail to start or act as if the pinMode is broken:

    * GPIO 0: Must NOT be pulled LOW at boot (prevents normal boot).
    * GPIO 2: Must NOT be pulled LOW at boot.
    * GPIO 15: Must NOT be pulled HIGH at boot.
    * GPIO 16: Does not have standard INPUT_PULLUP capabilities.
    * [Reference](https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/)

3) Pins That Blink/Output Voltage on Startup

    * Several pins output a 3.3V signal briefly when the ESP8266 restarts or boots.
    * Affected pins: GPIO 1, 3, 9, 10, 16.
    * __The Fix__: Avoid connecting relays or delicate logic to these pins if you do not want them to rapidly trigger during a board reset.

4) ESP8266 Doesn't have INPUT_PULLDOWN

    * If you are trying to configure a pin to read LOW using an internal pull-down resistor, the ESP8266 does not natively support this.

## Active Operation

* Idle (Wi-Fi on): ~70 - 80mA ([Source](https://maciejmalecki.github.io/blog/power-supply-for-bare-esp8266))
* Wi-Fi Transmitting/Receiving: ~170mA ([Source](https://www.youtube.com/watch?v=kwxaw43YukQ&t=469))
* Boot-up & Peak Spikes: 150mA to well over 300mA, sometimes up to 400mA for a few milliseconds. Because of this, reliable operation requires a power supply or voltage regulator capable of handling at least 500mA. ([Source](https://maciejmalecki.github.io/blog/power-supply-for-bare-esp8266))
* Deep Sleep: 10µA to 20µA for the bare chip, though development boards (like NodeMCU) consume around 10mA in sleep mode due to extra onboard components. ([Source](https://www.reddit.com/r/esp8266/comments/nudzmb/i_am_measuring_the_current_consumption_of_a/))

### I/O Limits

* Maximum GPIO Output: Up to 12mA per pin. ([Source](https://tttapa.github.io/ESP8266/Chap04%20-%20Microcontroller.html))

### Key Considerations

* Voltage Range: The chip operates strictly on 3.3V. Attempting to run it directly from a 5V source (like an Arduino Uno) will permanently damage the module. ([Source](https://www.instructables.com/ESP8266-Pro-Tips/))
* Current Spikes: A 500mA to 1A power source and a large bypass capacitor (e.g., 330μF to 1000μF) are highly recommended to smooth out power peaks and prevent unpredictable restarts. ([Source](https://forum.arduino.cc/t/how-to-power-my-esp-via-vin-pin/1269281))

## Reference

* [ESP8266 Pinout Reference](https://lastminuteengineers.com/esp8266-pinout-reference/)
* [ESP8266 Pinout Reference: Which GPIO pins should you use?](https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/)
* [](https://www.hackster.io/kingpyndiah11/temperature-sensor-with-buzzer-alert-sfm-27-1-using-telegram-40aa29)
* [ESP8266 0.96 inch OLED Display with Arduino IDE](https://randomnerdtutorials.com/esp8266-0-96-inch-oled-display-with-arduino-ide/)
* [](https://www.google.com/search?q=esp8266+programmatically+turn+off&oq=esp8266+programmatically+turn+off&gs_lcrp=EgZjaHJvbWUyBggAEEUYOTIHCAEQIRigATIHCAIQIRigATIHCAMQIRigAdIBCTEzODQ2ajBqN6gCALACAA&sourceid=chrome&ie=UTF-8)
* [](https://www.google.com/search?q=esp32+send+commands+to+alexa&oq=esp32+send+commands+to+alexa&gs_lcrp=EgZjaHJvbWUyBggAEEUYOTIHCAEQIRigATIHCAIQIRigATIHCAMQIRigATIHCAQQIRigATIHCAUQIRifBTIHCAYQIRifBTIHCAcQIRifBTIHCAgQIRifBTIHCAkQIRifBdIBCDY1MzRqMGo3qAIAsAIA&sourceid=chrome&ie=UTF-8)
* [Espalexa allows you to easily control your ESP with the Alexa voice assistant.](https://github.com/Aircoookie/Espalexa)
* [Core Hardware Specifications](https://www.make-it.ca/nodemcu-details-specifications/)
* [Manual (PDF)](https://handsontec.com/dataspecs/module/esp8266-V13.pdf)
* [](https://store-usa.arduino.cc/products/nodemcu-esp8266)
* [](https://mischianti.org/nodemcu-v3-high-resolution-pinout-and-specs/)