# ESP8266MOD

## TODOs

- Add RSSI (Received Signal Strength Indicator)
- Switch Screens to show RSSI, Temp/Humidity and turn off lights (LEDs and OLED display)

## Telegram Heartbeart Project Sensors and peripherals wiring

### DHT11 Sensor Wiring

|ESP8266    | <=>   | DHT11           |
| :---      | :---: | ---:            |
|3V3        | =>    | Vcc (Middle PIN)|
|GND        | =>    | - PIN (GND).    |
|S (Signal) | =>    | D1              |

### Passive Buzzer Wiring

|ESP8266    | <=>   | Passive Buzzer   |
| :---      | :---: | ---:             |
|3V3        | =>    | Vcc (Middle PIN) |
|GND        | =>    | - PIN (GND)      |
|S (Signal) | =>    | D2               |

### OLED Display Wiring

|ESP8266    | <=>   | OLED Display 128x64 |
| :---      | :---: | ---:                |
|GND        | =>    | GND                 |
|3V3        | =>    | Vcc                 |
|SCL        | =>    | D5                  |
|SDA        | =>    | D6                  |

## Reference Links

- https://lopaka.app/editor/40422/82950
- https://www.google.com/search?q=esp8266+async+request+client+code+https&sca_esv=b0606a0ae3ff7c6b&biw=1344&bih=750&sxsrf=ANbL-n68Wmz_rArWBZKoLQu4kz2UW8DbYw%3A1780688367299&ei=7yUjav71Eb25qtsP95rd6Qk&ved=0ahUKEwi-qdqM7fCUAxW9nGoFHXdNN50Q4dUDCBA&uact=5&oq=esp8266+async+request+client+code+https&gs_lp=Egxnd3Mtd2l6LXNlcnAiJ2VzcDgyNjYgYXN5bmMgcmVxdWVzdCBjbGllbnQgY29kZSBodHRwczIFECEYoAEyBRAhGKABMgUQIRifBTIFECEYnwUyBRAhGJ8FSJwXUOANWOIUcAF4AZABAJgBnQGgAbMFqgEDMS41uAEDyAEA-AEBmAIHoALfBcICChAAGEcY1gQYsAOYAwCIBgGQBgiSBwMyLjWgB5AysgcDMS41uAfXBcIHBTAuMS42yAcegAgB&sclient=gws-wiz-serp
- https://www.google.com/search?q=esp8266+async+request+client+code&sca_esv=b0606a0ae3ff7c6b&biw=1866&bih=1042&sxsrf=ANbL-n5ic2oG-gxXKS9NJIEa3Z4BrPfaEA%3A1780688267981&ei=iyUjauLNO-SoqtsPzs2kwAo&oq=esp8266+async+request+code&gs_lp=Egxnd3Mtd2l6LXNlcnAiGmVzcDgyNjYgYXN5bmMgcmVxdWVzdCBjb2RlKgIIADIIECEYoAEYwwQyCBAhGKABGMMEMggQIRigARjDBEiPIFCPAljfD3ACeAGQAQCYAcQBoAH2BaoBAzQuM7gBA8gBAPgBAZgCCaACqgbCAgoQABhHGNYEGLADwgIFEAAY7wXCAggQABiABBiiBMICChAhGAoYoAEYwwSYAwCIBgGQBgiSBwU1LjMuMaAHoBuyBwUzLjMuMbgHmwbCBwUwLjQuNcgHIoAIAQ&sclient=gws-wiz-serp
- https://github.com/khoih-prog/AsyncHTTPRequest_Generic
- https://robots-argentina.com.ar/referencia-de-distribucion-de-pines-ESP8266-que-pines-GPIO-usar.htm

### MOTOR FN5015LS
