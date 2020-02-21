#include <CustomNetwork.h>


//-----------------------------------------------------------------------------
CustomNetwork::CustomNetwork(bool debug) : logger_(debug) {}

//-----------------------------------------------------------------------------
CustomNetwork::CustomNetwork() : logger_(false) {}

//-----------------------------------------------------------------------------
void CustomNetwork::wifiSetup(const char* ssid, const char* password) {
    this->wifi_ssid_ = ssid;
    this->wifi_passwd_ = password;
}

//-----------------------------------------------------------------------------
bool CustomNetwork::wifiConnect() {
    return wifiConnect(10);
}

//-----------------------------------------------------------------------------
bool CustomNetwork::wifiConnect(int timeout_seconds) {
    this->logger_.debugln("");
    this->logger_.debug("Attempting to connect to Wifi ");
    this->logger_.debugln(this->wifi_ssid_);

    int password_length = sizeof(this->wifi_passwd_) - 1;
    if (!password_length) {
        WiFi.begin(this->wifi_ssid_);
    } else {
        WiFi.begin(this->wifi_ssid_, this->wifi_passwd_);
    }

    int seconds = 0;
    while (WiFi.status() != WL_CONNECTED && seconds <= timeout_seconds) {
        seconds++;
        delay(1000);
        this->logger_.debug(".");
    }

    bool connected = false;
    if (WiFi.status() == WL_CONNECTED) {
        connected = true;

        String msg = "connected with IP " + WiFi.localIP();
        this->logger_.debugln(msg.c_str());
    } else {
        this->logger_.debugln("could not connect");
    }

    return connected;
}

//-----------------------------------------------------------------------------
void CustomNetwork::wifiDisconnect() {
    WiFi.disconnect();
    this->logger_.debugln("Disconnect from WIFI network");
}

//-----------------------------------------------------------------------------
String CustomNetwork::getMacAddress() {
    String mac_address = WiFi.macAddress();

    this->logger_.debug("Retrieved Mac Address: ");
    this->logger_.debugln(mac_address.c_str());

    return mac_address;
}

//-----------------------------------------------------------------------------
int CustomNetwork::wifiPost(const char* address, const char* payload) {
    this->logger_.debugln("");
    this->logger_.debug("POST data to ");
    this->logger_.debugln(address);

    for(int i = 0; i < 4; i++) {
        if (WiFi.status() != WL_CONNECTED) {
            wifiConnect();
            delay(4 * 1000);
        } else {
            break;
        }
    }


    WiFiClientSecure *client = new WiFiClientSecure;

    /*
    We need to disable the ssl fingerprint verification in 
    the library of WiFiClientSecureBearSSL.cpp. Otherwise we
    need to set the 20 bytes SHA1 fingerprint of the certificate. 
    Due to the short-time Let's Encrypt certs, this is rather impractical.

    ------->8------->8------->8------->8------->8-------
        // Handle SHA1 fingerprint matching
        ...
        // if (xc->match_fingerprint && memcmp(res, xc->match_fingerprint, sizeof(res))) {
        //     return BR_ERR_X509_NOT_TRUSTED;
        // }
    -------8<-------8<-------8<-------8<-------8<-------
    */

    const uint8_t fingerprint[20] = {};
    client->setFingerprint(fingerprint);

    int resp_code = 0;
    for(int i = 0; i < 4; i++) {
        HTTPClient http;
        http.begin(*client, address);
        http.addHeader("Content-Type", "application/json");
        resp_code = http.POST(payload);
        String resp_payload = http.getString();
        http.end();

        if (resp_code > 0) {
            this->logger_.debug("[HTTPS] Response: ");
            this->logger_.debugln(resp_payload.c_str());
            break;
        } else {
            this->logger_.debug("[HTTPS] GET... failed, error: ");
            this->logger_.debugln(http.errorToString(resp_code).c_str());
        }
        delay(4 * 1000);
    }

    return resp_code;
}

//-----------------------------------------------------------------------------
void CustomNetwork::mqttSetup(PubSubClient* mqtt_client, const char* server, int port, const char* id, const char* user, const char* password) {
    this->mqtt_client_ = mqtt_client;
    this->mqtt_server_ = server;
    this->mqtt_port_ = port;
    this->mqtt_id_ = id;
    this->mqtt_user_ = user;
    this->mqtt_passwd_ = password;

    this->mqtt_client_->setServer(this->mqtt_server_, this->mqtt_port_);
}

//-----------------------------------------------------------------------------
void CustomNetwork::mqttConnect() {
    if (WiFi.status() != WL_CONNECTED) {
        wifiConnect();
    }

    while (!mqtt_client_->connected()) {
        this->logger_.debugln("");
        this->logger_.debug("Attempting to connect to MQTT Server ");
        this->logger_.debugln(this->mqtt_server_);

        if (mqtt_client_->connect(this->mqtt_id_, this->mqtt_user_, this->mqtt_passwd_)) {
            this->logger_.debugln("connected");
        } else {
            this->logger_.debug("failed, rc=");
            this->logger_.debug(String(mqtt_client_->state()).c_str());
            this->logger_.debugln(" try again in 5 seconds");
            delay(5000);
        }
    }
}

//-----------------------------------------------------------------------------
void CustomNetwork::mqttDisconnect() {
    this->logger_.debugln("Disconnect from MQTT service");
    this->mqtt_client_->disconnect();
}

//-----------------------------------------------------------------------------
bool CustomNetwork::mqttConnected() {
    return this->mqtt_client_->connected();
}

//-----------------------------------------------------------------------------
void CustomNetwork::mqttLoop() {
    this->mqtt_client_->loop();
}

//-----------------------------------------------------------------------------
void CustomNetwork::mqttPublish(const char* topic, const char* message) {
    this->mqttConnect();
    this->mqtt_client_->publish(topic, message);
}

//-----------------------------------------------------------------------------
void CustomNetwork::mqttSubscribe(const char* topic) {
    this->mqtt_client_->subscribe(topic);
}

void CustomNetwork::mqttSetCallback(std::function<void(char *, uint8_t *, unsigned int)> callback) {
    this->mqtt_client_->setCallback(callback);
}
