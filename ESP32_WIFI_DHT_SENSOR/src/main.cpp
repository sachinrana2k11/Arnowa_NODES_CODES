#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#define DHTTYPE DHT11


String topic_send = "ARNOWA/IN";
const char* ssid = "raspi-webgui";
const char* password = "ChangeMe";
const char* mqttServer = "10.3.141.1";
const int mqttPort = 1883;
const char* mqttUser = "yatinbaluja";
const char* mqttPassword = "raspberry";


uint8_t DHTPin = 4;
long lastSendTime1=0;
int  interval1=5000;
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);
float Temperature;
float Humidity;
void setup_wifi(void);
void reconnect(void);
String fetch_sensor_data(void);
void callback(char*, byte*, unsigned int);
float get_temp(void);
float get_hum(void);

WiFiClient espClient;
PubSubClient client(espClient);
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
  pinMode(DHTPin, INPUT);
  dht.begin();
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
   Temperature = get_temp();
   Humidity = get_hum();
   fetch_sensor_data().toCharArray(payload, 100);
   client.publish("ARNOWA/IN",payload);
   lastSendTime1 = millis();
 }
}

String fetch_sensor_data()
{
 String data = "{\"DeviceID\":\"WIFI_DHT_NODE_1\",\"SenosrType\":\"DHT22\",\"Data\":\{\"temprature\":";
 data += String(Temperature);
 data +=",\"humidity\":";
 data += String(Humidity);
 data +="}\}";
 return data;
}
float get_temp()
{
  float Temperature_temp = dht.readTemperature();
  Serial.println(Temperature_temp);
  if (String(Temperature_temp) == "nan")
  {
    return Temperature;
  }
  Temperature = Temperature_temp;
  //Serial.println(Temperature);
  return Temperature;
}
float get_hum()
{
  float Humidity_temp = dht.readHumidity();
  Serial.println(Humidity_temp);
  if (String(Humidity_temp) == "nan")
  {
    return Humidity;
  }
  Humidity = Humidity_temp;
  //Serial.println(Humidity);
  return Humidity;
}
