#ifndef Constants_h
#define Constants_h

#include <pins_arduino.h>
#include <c_types.h>

const char* WIFI_SSID = "";
const char* WIFI_PASSWORD = "";

const char* MQTT_SERVER = "";
const int MQTT_PORT = 10651;
const char* MQTT_ID = "";
const char* MQTT_USER = "";
const char* MQTT_PASSWORD = "";

const char* TOPIC_IN = "";
const char* TOPIC_OUT = "";

const int IR_SEND_PIN = D0;

const bool DEBUG = true;

#endif
