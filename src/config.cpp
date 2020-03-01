#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
constexpr uint16_t sleepTimeInSeconds = 300;
bool wifiConnection = false;
bool mqttConnection = false;
uint32_t mqtt_delay = 100;
uint16_t sleep_delay = 10000;