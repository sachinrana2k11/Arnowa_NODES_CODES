#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#define ANALOG_PIN_0 36
int analog_value = 0;
int dutyCycle = 0;
String topic_send = "ARNOWA/IN";
const char* ssid = "Vulp";
const char* password = "1234567v";
const char* mqttServer = "ec2-35-168-210-54.compute-1.amazonaws.com";
const int mqttPort = 1883;
const char* mqttUser = "sachinrana2k18";
const char* mqttPassword = "admin12345a";
long lastSendTime1=0;
int  interval1=5000;
// Initialize DHT sensor.
void setup_wifi(void);
void reconnect(void);
void callback(char*, byte*, unsigned int);
int  get_lux(void);
String fetch_sensor_data(void);
WiFiClient espClient;
PubSubClient client(espClient);
int Light_Intensity;
long lastMsg = 0;
char payload[100];
int value = 0;
void callback(char* topic, byte* message, unsigned int length) {
//  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
}
void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqttServer, 1883);
  client.setCallback(callback);
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void reconnect() {
  while (!client.connected()) {

    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client", mqttUser,mqttPassword)) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("arnowa/cmd");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
 if (!client.connected()) {
   setup_wifi();
    reconnect();
  }
  client.loop();
  if (millis() - lastSendTime1 > interval1)
 {
   Light_Intensity = get_lux();
   fetch_sensor_data().toCharArray(payload, 100);
   client.publish("ARNOWA/IN",payload);
   lastSendTime1 = millis();
 }
}

String fetch_sensor_data()
{
 String data = "{\"DeviceID\":\"WIFI_LDR_NODE_1\",\"SenosrType\":\"LDR\",\"Data\":\{\"LUX\":";
 data += String(Light_Intensity);
 data +="}\}";
 Serial.println(data);
 return data;
}
int get_lux()
{
  analog_value = int(analogRead(ANALOG_PIN_0));
  dutyCycle = map(analog_value, 0, 4095, 100, 0);
  return dutyCycle;
}
