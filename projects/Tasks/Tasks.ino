#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "src/secrets/WiFiCredentials.h"

#define DHTPIN 5
#define DHTTYPE DHT11
#define LED1 LED_BUILTIN
#define LED2 16

// HIVEMQ MQTT Broker credentials
const char* mqtt_broker = "broker.hivemq.com";
const char* mqtt_topic = "ucol/diplomado/ceron";
const char* mqtt_client_id = "cliente_ceron";
const int mqtt_port = 1883;

// Task timers and intervals
unsigned long previousMillis1 = 0;
unsigned long interval1 = 100;  // Task 1 interval (e.g., 200 milliseconds)

unsigned long previousMillis2 = 0;
unsigned long interval2 = 5000;  // Task 2 interval (e.g., 5000 milliseconds)

unsigned long previousMillis3 = 0;

// DHT Temperature and Humidity Sensor setup
DHT dht(DHTPIN, DHTTYPE);

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void reconnect() {
  while (!mqttClient.connected()) {
    Serial.println("Conectando con el broker MQTT...");

    if (mqttClient.connect(mqtt_client_id)) {
      Serial.println("Conectado al broker MQTT.");
    } else {
      Serial.print("Error: ");
      Serial.println(mqttClient.state());
      Serial.println("Reintento en 5 segundos...");
      delay(5000);
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(SSID, WIFI_PASSWORD);
  Serial.printf("\n");
  Serial.printf("Conectando a Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(".");
  }

  Serial.print("Conectado con la IP: ");
  Serial.println(WiFi.localIP());
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
}

void blink(unsigned long currentMillis) {
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

  if (currentMillis - previousMillis2 >= interval1) {
    previousMillis2 = currentMillis;

    // Toggle the state of LED 2
    if (digitalRead(LED2) == LOW) {
      digitalWrite(LED2, HIGH);
    } else {
      digitalWrite(LED2, LOW);
    }
  }
}

void readSensorsAndPublish(unsigned long currentMillis) {
  if (currentMillis - previousMillis3 >= interval2) {
    previousMillis3 = currentMillis;

    // Read Sensors and Publish to HiveMQ
    float t = dht.readTemperature();
    float h = dht.readHumidity();

    Serial.printf("Temperatura %f C, humedad %f %\n", t, h);

    char json[200];
    sprintf(json, "{\"temperature\": %f,\"humidity\": %f}", t, h);
    Serial.println(json);
    mqttClient.publish(mqtt_topic, json);
  }
}

void loop() {
  unsigned long currentMillis = millis();

  mqttClient.setServer(mqtt_broker, mqtt_port);

  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();
  // put your main code here, to run repeatedly:
  blink(currentMillis);
  readSensorsAndPublish(currentMillis);
}
