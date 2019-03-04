#include <IRremoteESP8266.h>
#include <IRutils.h>
#include <IRsend.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <CustomNetwork.h>
#include <CustomHelpers.h>

#include "constants.h"

WiFiClient wifi_client_;
PubSubClient mqtt_client_(wifi_client_);

IRsend irsend_(IR_SEND_PIN);

CustomLogger custom_logger_(DEBUG);
CustomNetwork custom_network_(DEBUG);
CustomHelpers custom_helpers_(DEBUG);

//-----------------------------------------------------------------------------
void setup()
{
    custom_network_.wifiSetup(WIFI_SSID, WIFI_PASSWORD);
    custom_network_.mqttSetup(&mqtt_client_, MQTT_SERVER, MQTT_PORT, MQTT_ID, MQTT_USER, MQTT_PASSWORD);

    irsend_.begin();
}

//-----------------------------------------------------------------------------
void mqtt_callback(char *topic, byte *payload, unsigned int length)
{
    custom_logger_.debug("Message arrived [");
    custom_logger_.debug(topic);
    custom_logger_.debug("]: ");

    for (int i = 0; i < length; i++)
        custom_logger_.debug(((char*)&payload[i]));
    custom_logger_.debugln("");

    int nec = custom_helpers_.convert_hexstring_to_int(payload);
    if (nec)
        irsend_.sendNEC(nec, 32);
}

//-----------------------------------------------------------------------------
void loop()
{
    if (!custom_network_.mqttConnected()) {
        custom_network_.mqttConnect();
        custom_network_.mqttSetCallback(mqtt_callback);
        custom_network_.mqttSubscribe(TOPIC_IN);
    }

    custom_network_.mqttLoop();
}
