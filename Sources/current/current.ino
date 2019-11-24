#include <Wire.h>
#include <Adafruit_INA219.h>
#include <PubSubClient.h>

#include <CustomNetwork.h>
#include <CustomHelpers.h>

#include "constants.h"


Adafruit_INA219 sensor219;

WiFiClient wifi_client_;
PubSubClient mqtt_client_(wifi_client_);

CustomLogger logger_(DEBUG);
CustomNetwork custom_network_(DEBUG);


void setup(void) {  
    custom_network_.wifiSetup(WIFI_SSID, WIFI_PASSWORD);
    custom_network_.mqttSetup(&mqtt_client_, MQTT_SERVER, MQTT_PORT, MQTT_ID, MQTT_USER, MQTT_PASSWORD);

    sensor219.begin();
    sensor219.setCalibration_16V_400mA();
}


void loop(void) {
    float busVoltage = sensor219.getBusVoltage_V();
    float current = sensor219.getCurrent_mA();

    String payload = String(current) + "; " + String(busVoltage);
    logger_.debugln(payload.c_str());

    custom_network_.mqttPublish(TOPIC, payload.c_str());
}