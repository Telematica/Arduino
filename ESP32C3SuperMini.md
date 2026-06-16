# ESP32 C3 Super Mini

## Specs

### Core Hardware Specifications

* __Processor__: 32-bit RISC-V single-core processor, running up to 160 MHz
* __Memory__: 400 KB SRAM, 384 KB ROM, and 4 MB of built-in Flash memory
* __Wireless__: IEEE 802.11 b/g/n (2.4 GHz) up to 150 Mbps and Bluetooth: Bluetooth 5 (Low Energy) and Bluetooth Mesh
* __Power__: 3.3V operating voltage, with support for 3.3V to 6V external power inputs
* __Ultra-Low Power__: Deep sleep power consumption as low as ~~ 43 μ A (can be further reduced by removing the onboard power LED)
* [Source](https://dl.artronshop.co.th/ESP32-C3%20SuperMini%20datasheet.pdf)

### Peripherals & I/O

* __GPIO Pins__: 11 programmable GPIO pins
* __Analog Inputs__: 4 ADC (Analog-to-Digital Converter) channels
* __Communication Interfaces__: 1 x I²C, 1 x SPI, and 2 x UART ports
* __PWM__: Up to 6 channels for LED control and motor driving
* __USB Interface__: Native USB Type-C port supporting USB Serial/JTAG controller (no external bridge chip like CP2102 needed)
* __Hardware Accelerators__: Supports encryption hardware for AES-128/256, RSA, HMAC, SHA-2, digital signatures, and secure startup
* [Source](https://dl.artronshop.co.th/ESP32-C3%20SuperMini%20datasheet.pdf)

### Physical Dimensions

* Form Factor: 22.5 mm × 18 mm (thumb-sized)
* Weight: ~ 3.05 g

## Constraints

The ESP32-C3 Super Mini breaks out 11 programmable GPIO pins (numbered 0 to 10 on the board). While all 11 pins work for basic tasks, GPIO 8 and 9 should be used with caution because they are strapping pins connected to the onboard LED and BOOT button.

A more specific, categorized breakdown helps you decide exactly how to use them without causing boot loops or interfering with built-in functions:

🌟 1. Safest Pins to Use (General GPIO, PWM, ADC)

* GPIO 0, GPIO 1, GPIO 2, GPIO 3, GPIO 4, GPIO 5, GPIO 6, GPIO 7
* Best for: Standard sensors, relays, buttons, or PWM (LED dimming). Pins 0 through 5 are also RTC pins, meaning they can be used to wake the board from deep sleep.
* Note: GPIO 2 is a strapping pin. It is fine to use, but make sure your attached component doesn't pull it LOW during boot (which would prevent the board from starting up correctly).
* [Source](https://www.studiopieters.nl/esp32-c3-super-mini-pinout/)

⚠️ 2. Pins with Restrictions

* GPIO 8: Connected to the onboard blue LED. You can use this as a normal pin, but it will flash with your activity. Avoid pulling it LOW during boot.
* GPIO 9: Connected to the BOOT button. Pulling this LOW during boot will put the board into firmware flashing mode. It is best to avoid using this pin if you need a circuit to work immediately upon powering on.
* [Source](https://www.studiopieters.nl/esp32-c3-super-mini-pinout/)

🚫 3. Pins to Generally Avoid

* GPIO 10: Often erratic during boot and heavily tied to internal board mechanisms. Most community builders suggest leaving it disconnected to prevent stability issues.
* [Source](https://www.reddit.com/r/AskElectronics/comments/1jd1zew/which_gpio_pins_is_ok_to_use_on_the_esp32c3/)

🔌 Dedicated Communication Pins

If you need protocols like I2C, SPI, or Serial, the Super Mini assigns them by default:

* UART (Serial): GPIO 20 (RX) and GPIO 21 (TX).
* I2C: GPIO 8 (SDA) and GPIO 9 (SCL) (Remember the BOOT button/LED caution above).
* SPI: GPIO 4 (SCK), GPIO 5 (MISO), GPIO 6 (MOSI), and GPIO 7 (CS).

## Reference

* [ESP32-C3 repeatedly disconecting from usb](https://esp32.com/viewtopic.php?t=36173)
