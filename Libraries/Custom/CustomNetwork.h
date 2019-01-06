#ifndef CustomNetwork_h
#define CustomNetwork_h

#include <Arduino.h>
#include <CustomLogger.h>


class CustomNetwork {
    private:
        CustomLogger logger_;

        const char* wifi_ssid_;
        const char* wifi_passwd_;

        PubSubClient* mqtt_client_;
        const char* mqtt_server_;
        int mqtt_port_;
        const char* mqtt_id_;
        const char* mqtt_user_;
        const char* mqtt_passwd_;


    public:
        CustomNetwork(bool);
        CustomNetwork();
        
        void wifiSetup(const char* ssid, const char* password);
        void wifiConnect();

        void mqttSetup(PubSubClient* mqtt_client, const char* server, int port, const char* id, const char* user, const char* password);
        void mqttConnect();
        bool mqttConnected();
        void mqttLoop();
        void mqttPublish(const char* topic, const char* message);
        void mqttSubscribe(const char* topic);
        void mqttSetCallback(std::function<void(char*, uint8_t*, unsigned int)> callback);
};

#endif
