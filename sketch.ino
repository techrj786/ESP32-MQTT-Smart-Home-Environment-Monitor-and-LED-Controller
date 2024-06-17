#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

const char* ssid = "Wokwi-GUEST";

const char* mqtt_server = "mqtt.pedalsupclients.xyz";
const int mqtt_port = 1883;

const char* clientID = "esp32_client"; // Base client ID for ESP32
const char* led1_clientID = "led1";
const char* led2_clientID = "led2";
const char* led3_clientID = "led3";
const char* led4_clientID = "led4";

const char* subscribe_topic = "t2";
const char* publish_topic = "sensor_data";

WiFiClient espClient;
PubSubClient client(espClient);

#define DHTPIN 4  // Change to the appropriate GPIO pin for DHT22
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

const int LED1_PIN = 16;  // GPIO pin for LED 1
const int LED2_PIN = 17; // GPIO pin for LED 2
const int LED3_PIN = 18; // GPIO pin for LED 3
const int LED4_PIN = 19; // GPIO pin for LED 4

int led1_brightness = 0;  // Initial brightness values (0-255)
int led2_brightness = 0;
int led3_brightness = 0;
int led4_brightness = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED4_PIN, OUTPUT);

  analogWrite(LED1_PIN, led1_brightness);
  analogWrite(LED2_PIN, led2_brightness);
  analogWrite(LED3_PIN, led3_brightness);
  analogWrite(LED4_PIN, led4_brightness);

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  client.subscribe(subscribe_topic);

  dht.begin();

  // Attempt to connect
  reconnect();
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

String normalizeString(String str) {
  str.toLowerCase();
  str.trim();
  str.replace(" ", "");
  str.replace("_", "");
  return str;
}

bool matchClientID(String receivedID, const char* expectedID) {
  receivedID = normalizeString(receivedID);
  return (receivedID == expectedID);
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);

  int separatorIndex = message.indexOf(',');
  if (separatorIndex != -1) {
    String receivedID = message.substring(0, separatorIndex);
    String operation = message.substring(separatorIndex + 1);

    receivedID = normalizeString(receivedID);
    operation = normalizeString(operation);

    if (matchClientID(receivedID, led1_clientID)) {
      if (operation.startsWith("brightness")) {
        int brightness = operation.substring(10).toInt();
        led1_brightness = map(brightness, 0, 100, 0, 255); // Map brightness percentage (0-100) to PWM range (0-255)
        analogWrite(LED1_PIN, led1_brightness);
      } else if (operation == "on") {
        analogWrite(LED1_PIN, 255); // Full brightness
      } else if (operation == "off") {
        analogWrite(LED1_PIN, 0); // Off
      }
    } else if (matchClientID(receivedID, led2_clientID)) {
      if (operation.startsWith("brightness")) {
        int brightness = operation.substring(10).toInt();
        led2_brightness = map(brightness, 0, 100, 0, 255);
        analogWrite(LED2_PIN, led2_brightness);
      } else if (operation == "on") {
        analogWrite(LED2_PIN, 255);
      } else if (operation == "off") {
        analogWrite(LED2_PIN, 0);
      }
    } else if (matchClientID(receivedID, led3_clientID)) {
      if (operation.startsWith("brightness")) {
        int brightness = operation.substring(10).toInt();
        led3_brightness = map(brightness, 0, 100, 0, 255);
        analogWrite(LED3_PIN, led3_brightness);
      } else if (operation == "on") {
        analogWrite(LED3_PIN, 255);
      } else if (operation == "off") {
        analogWrite(LED3_PIN, 0);
      }
    } else if (matchClientID(receivedID, led4_clientID)) {
      if (operation.startsWith("brightness")) {
        int brightness = operation.substring(10).toInt();
        led4_brightness = map(brightness, 0, 100, 0, 255);
        analogWrite(LED4_PIN, led4_brightness);
      } else if (operation == "on") {
        analogWrite(LED4_PIN, 255);
      } else if (operation == "off") {
        analogWrite(LED4_PIN, 0);
      }
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    Serial.print("MQTT server: ");
    Serial.println(mqtt_server);
    Serial.print("MQTT port: ");
    Serial.println(mqtt_port);

    if (client.connect(clientID)) {
      Serial.println("connected");
      client.subscribe(subscribe_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void loop() {
  client.loop(); // Ensure client receives MQTT messages

  if (!client.connected()) {
    reconnect();
  }
}
