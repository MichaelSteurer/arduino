#include <DHT.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <CustomNetwork.h>
#include <CustomHelpers.h>

#include "constants.h"


WiFiClient wifi_client_;
PubSubClient mqtt_client_(wifi_client_);
DHT dht_(DHTPIN, DHTTYPE);

CustomLogger logger_(DEBUG);
CustomNetwork custom_network_(DEBUG);


//-----------------------------------------------------------------------------
void setup()
{
    custom_network_.wifiSetup(WIFI_SSID, WIFI_PASSWORD);
    custom_network_.wifiConnect();

    custom_network_.mqttSetup(&mqtt_client_, MQTT_SERVER, MQTT_PORT, MQTT_ID, MQTT_USER, MQTT_PASSWORD);
    custom_network_.mqttConnect();

    dht_.begin();
}


//-----------------------------------------------------------------------------
void loop()
{
    if (!custom_network_.mqttConnected())
        custom_network_.mqttConnect();
    custom_network_.mqttLoop();

    float t = dht_.readTemperature();

    String payload = String("{\"payload\":{\"Temp\":" + String(t) + "}}");
    custom_network_.mqttPublish(TOPIC, payload.c_str());

    if (isnan(t))
    {
        logger_.debugln("Failed to read from DHT sensor!");
    }
    logger_.debugln(String("payload: " + payload).c_str());

    delay(10 * 60 * 1000);
}
