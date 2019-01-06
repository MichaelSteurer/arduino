#ifndef Constants_h
#define Constants_h

#include <pins_arduino.h>
#include <c_types.h>

const char* WIFI_SSID = "";
const char* WIFI_PASSWORD = "";

const char* MQTT_SERVER = "";
const int MQTT_PORT = 1883;
const char* MQTT_ID = "";
const char* MQTT_USER = NULL;
const char* MQTT_PASSWORD = NULL;

const char* TOPIC = "";

uint8_t DHTPIN = D5;
uint8_t DHTTYPE = DHT22;

const bool DEBUG = true;

#endif
