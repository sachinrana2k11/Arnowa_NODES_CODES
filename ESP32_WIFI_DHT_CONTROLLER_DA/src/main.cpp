#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
long lastSendTime1=0;
int  interval1=5000;
const char* ssid = "Vulp";
const char* password = "admin12345a";
const char* mqttServer = "ec2-35-168-210-54.compute-1.amazonaws.com";
const int mqttPort = 1883;
const char* mqttUser = "sachinrana2k18";
const char* mqttPassword = "admin12345a";
void setup_wifi(void);
void reconnect(void);
void callback(char*, byte*, unsigned int);
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char payload[100];
int value = 0;
const int ledPin1 = 2;
const int ledPin2 = 18;
const int ledPin3 = 19;
const int ledPin4 = 21;
void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqttServer, 1883);
  client.setCallback(callback);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
  digitalWrite(ledPin1, HIGH);
  digitalWrite(ledPin2, HIGH);
  digitalWrite(ledPin3, HIGH);
  digitalWrite(ledPin4, HIGH);
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

void callback(char* topic, byte* message, unsigned int length) {
//  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }

  if (String(topic) == "arnowa/cmd") {
    //Serial.print("Changing output to ");
    if(messageTemp == "1"){
      //Serial.println("on");
      digitalWrite(ledPin1, LOW);
    }
    if(messageTemp == "2"){
      //Serial.println("off");
      digitalWrite(ledPin1, HIGH);
    }
    if(messageTemp == "3"){
      //Serial.println("on");
      digitalWrite(ledPin2, LOW);
    }
    if(messageTemp == "4"){
      //Serial.println("off");
      digitalWrite(ledPin2, HIGH);
    }
    if(messageTemp == "5"){
      //Serial.println("on");3
      digitalWrite(ledPin3, LOW);
    }
    if(messageTemp == "6"){
      //Serial.println("off");
      digitalWrite(ledPin3, HIGH);
    }
    if(messageTemp == "7"){
      //Serial.println("on");
      digitalWrite(ledPin4, LOW);
    }
    if(messageTemp == "8"){
      //Serial.println("off");
      digitalWrite(ledPin4, HIGH);
    }
  }
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
   client.publish("test","pinging");
   lastSendTime1 = millis();
 }
}
