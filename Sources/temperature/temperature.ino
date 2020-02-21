// #include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include <CustomNetwork.h>
#include <CustomHelpers.h>

#include "constants.h"


WiFiClient wifi_client_;

// DHT dht_(DATA_PIN, DHT22);
OneWire ourWire(DATA_PIN);
DallasTemperature sensors(&ourWire);

CustomLogger logger_(DEBUG);
CustomNetwork custom_network_(DEBUG);


//-----------------------------------------------------------------------------
void setup() {
    custom_network_.wifiSetup(WIFI_SSID, WIFI_PASSWORD);
    // dht_.begin();
}


//-----------------------------------------------------------------------------
void loop() {
    // float h = dht_.readHumidity();
    // float t = dht_.readTemperature();

    String mac = custom_network_.getMacAddress();

    float h = 0;
    sensors.requestTemperatures();
    float t = sensors.getTempCByIndex(0);

    if (isnan(t) || isnan(h)) {
        logger_.debugln("Failed to read from DHT sensor - restart!");
        delay(2 * 1000);
        return;
    }

    h = (int)(h * 100) / 100.0;
    t = (int)(t * 100) / 100.0;

    String payload = String(
        "{"
        "  'n': '" + mac + "',"
        "  't': '" + String(TOKEN) + "',"
        "  'p': {"
        "    't':" + String(t) + "," 
        "    'h':" + String(h) + ""
        "  }"
        "}"
    );
    payload.replace("'", "\"");
    logger_.debugln(String("payload: " + payload).c_str());

    custom_network_.wifiPost(REQUEST_URL, payload.c_str());
    
    ESP.deepSleep(30 * DEEPSLEEP_MINUTE);
    delay(100);
}
