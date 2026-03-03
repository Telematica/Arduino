#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHTPIN 5

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(5000);
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  Serial.printf("Temperatura: %f C, humedad: %f %\n", t, h);
}
