#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "src/WiFiCredentials.h"

// Credenciales de la red Wi-Fi
const char* ssid = SSID;
const char* password = WIFI_PASSWORD;

const char* mqtt_broker = "broker.hivemq.com";
const char* mqtt_topic = "ucol/diplomado/ceron";
const char* mqtt_client_id = "cliente_ceron";
const int mqtt_port = 1883;

#define DHTPIN 5
#define DHTTYPE DHT11

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

  WiFi.begin(ssid, password);
  Serial.printf("\n");
  Serial.printf("Conectando a Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(".");
  }

  Serial.print("Conectado con la IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  mqttClient.setServer(mqtt_broker, mqtt_port);

  if (!mqttClient.connected()) {
    reconnect();
  }

  mqttClient.loop();
  // put your main code here, to run repeatedly:
  delay(5000);
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  Serial.printf("Temperatura %f C, humedad %f %\n", t, h);

  char json[200];
  sprintf(json, "{\"temperature\": %f,\"humidity\": %f}", t, h);
  Serial.println(json);
  mqttClient.publish(mqtt_topic, json);
}
