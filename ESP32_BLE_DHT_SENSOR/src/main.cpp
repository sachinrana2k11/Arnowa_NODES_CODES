#include <Arduino.h>
#include "SimpleBLE.h"
#include <DHT.h>
#include <Adafruit_Sensor.h>
#define DHTTYPE DHT11
String node_name = "BLE1_DHT";
int time_to_send = 5000;

long lastSendTime1 = 0;
int interval1 = 5000;

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

uint8_t DHTPin = 4;
SimpleBLE ble;
String fetch_sensor_data(void);
float get_temp(void);
float get_hum(void);

DHT dht(DHTPin, DHTTYPE);
float Temperature;
float Humidity;

void onButton()
{
  Temperature = get_temp();
  Humidity = get_hum();
  String out = node_name;
  out += ",";
  out += Temperature;
  out += ",";
  out += Humidity;
  Serial.println(out);
  ble.begin(out);
}

void setup()
{
  Serial.begin(9600);
  pinMode(DHTPin, INPUT);
  dht.begin();
  Serial.setDebugOutput(true);
  pinMode(0, INPUT_PULLUP);
  Serial.print("ESP32 SDK: ");
  Serial.println(ESP.getSdkVersion());
  ble.begin("ESP32 SimpleBLE");
  Serial.println("Press the button to change the device's name");
}

void loop()
{
  if (millis() - lastSendTime1 > interval1)
  {
    onButton();
    lastSendTime1 = millis();
  }
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
