#include <Arduino.h>
#include "SimpleBLE.h"
#define ANALOG_PIN_0 36
int analog_value = 0;
int dutyCycle = 0;
String node_name = "BLE1_LDR";
long lastSendTime1=0;
int  interval1 = 5000;
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
SimpleBLE ble;
int fetch_sensor_data(void);
void onButton(void);
int Light_Intensity;

void onButton(){
    Light_Intensity = fetch_sensor_data();
    String out = node_name;
    out +=",";
    out +=Light_Intensity;
    Serial.print("BROADCASTING - LIGHT INTENSITY-:");
    Serial.println(out);
    ble.begin(out);
}
void setup() {
  Serial.begin(9600);
}

void loop() {
  if (millis() - lastSendTime1 > interval1)
 {
   onButton();
   lastSendTime1 = millis();
 }
}

int fetch_sensor_data()
{
  analog_value = int(analogRead(ANALOG_PIN_0));
  dutyCycle = map(analog_value, 0, 4095, 100, 0);
  return dutyCycle;
}
