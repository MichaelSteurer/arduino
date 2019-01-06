#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <Arduino.h>
#include <CustomNetwork.h>
#include <CustomHelpers.h>


//-----------------------------------------------------------------------------
CustomNetwork::CustomNetwork(bool debug) : logger_(debug) {}

//-----------------------------------------------------------------------------
CustomNetwork::CustomNetwork() : logger_(false) {}

//-----------------------------------------------------------------------------
void CustomNetwork::wifiSetup(const char* ssid, const char* password)
{
    this->wifi_ssid_ = ssid;
    this->wifi_passwd_ = password;
}

//-----------------------------------------------------------------------------
void CustomNetwork::wifiConnect()
{
    this->logger_.debugln("");
    this->logger_.debug("Attempting to connect to Wifi ");
    this->logger_.debugln(this->wifi_ssid_);

    WiFi.begin(this->wifi_ssid_, this->wifi_passwd_);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        this->logger_.debug(".");
    }

    this->logger_.debug("connected with IP ");

    this->logger_.debugln(const_cast<char*>(WiFi.localIP().toString().c_str()));
}

void CustomNetwork::mqttSetup(PubSubClient* mqtt_client, const char* server, int port, const char* id, const char* user, const char* password)
{
    this->mqtt_client_ = mqtt_client;
    this->mqtt_server_ = server;
    this->mqtt_port_ = port;
    this->mqtt_id_ = id;
    this->mqtt_user_ = user;
    this->mqtt_passwd_ = password;

    this->mqtt_client_->setServer(this->mqtt_server_, this->mqtt_port_);
}

//-----------------------------------------------------------------------------
void CustomNetwork::mqttConnect()
{
    if (WiFi.status() != WL_CONNECTED) {
        wifiConnect();
    }

    while (!mqtt_client_->connected())
    {
        this->logger_.debugln("");
        this->logger_.debug("Attempting to connect to MQTT Server ");
        this->logger_.debugln(this->mqtt_server_);

        if (mqtt_client_->connect(this->mqtt_id_, this->mqtt_user_, this->mqtt_passwd_))
        {
            this->logger_.debugln("connected");
        }
        else
        {
            this->logger_.debug("failed, rc=");
            this->logger_.debug(String(mqtt_client_->state()).c_str());
            this->logger_.debugln(" try again in 5 seconds");
            delay(5000);
        }
    }
}

//-----------------------------------------------------------------------------
bool CustomNetwork::mqttConnected()
{
    return this->mqtt_client_->connected();
}

//-----------------------------------------------------------------------------
void CustomNetwork::mqttLoop()
{
    this->mqtt_client_->loop();
}

//-----------------------------------------------------------------------------
void CustomNetwork::mqttPublish(const char* topic, const char* message) 
{
    this->mqtt_client_->publish(topic, message);
}

//-----------------------------------------------------------------------------
void CustomNetwork::mqttSubscribe(const char* topic) 
{
    this->mqtt_client_->subscribe(topic);
}


void CustomNetwork::mqttSetCallback(std::function<void(char*, uint8_t*, unsigned int)> callback)
{
    this->mqtt_client_->setCallback(callback);

}
