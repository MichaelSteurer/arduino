// 0: DHT
// 1: DALLAS
#define SENSOR 1

#if SENSOR == 0
#include <DHT.h>
#elif SENSOR == 1
#include <OneWire.h>
#include <DallasTemperature.h>
#endif

#include <CustomNetwork.h>
#include <CustomHelpers.h>

#include "constants.h"

ADC_MODE(ADC_VCC);

WiFiClient wifi_client_;


#if SENSOR == 0

    DHT dht_(DATA_PIN, DHT22);

#elif SENSOR == 1

    OneWire ourWire(DATA_PIN);
    DallasTemperature sensors(&ourWire);

#endif

CustomLogger logger_(DEBUG);
CustomNetwork custom_network_(DEBUG);


//-----------------------------------------------------------------------------
void setup() {
    custom_network_.wifiSetup(WIFI_SSID, WIFI_PASSWORD);

#if SENSOR == 0
    dht_.begin();
#endif
}


//-----------------------------------------------------------------------------
void loop() {
    logger_.debugln("I'm awake, but I'm going into deep sleep mode");

#if SENSOR == 0
    float t = dht_.readTemperature();
    float h = dht_.readHumidity();
#elif SENSOR == 1
    sensors.requestTemperatures();
    float t = sensors.getTempCByIndex(0);
    float h = 0;
#endif

    if (isnan(t) || isnan(h)) {
        logger_.debugln("Failed to read from DHT sensor - restart!");
        delay(2 * 1000);
        return;
    }

    h = (int)(h * 100) / 100.0; // two digits
    t = (int)(t * 100) / 100.0; // two digits

    String mac = custom_network_.getMacAddress();
    float b = ESP.getVcc(); // set ADC_MODE(ADC_VCC); on top

    String payload = String(
        "{"
        "  'n': '" + mac + "',"
        "  't': '" + String(TOKEN) + "',"
        "  'p': {"
        "    't':" + String(t) + "," 
        "    'b':" + String(b) + "," 
        "    'h':" + String(h) + ""
        "  }"
        "}"
    );
    payload.replace("'", "\"");
    logger_.debugln(String("payload: " + payload).c_str());

    custom_network_.wifiPost(REQUEST_URL, payload.c_str());
    
    // on the nodeMCU we need to connect RST with GPIO16!
    ESP.deepSleep(60 * 60 * 1e6); // 1 second -> 1e6
    delay(100);

    // delay(60 * 1e3); // 1 seconds -> 1e3
}
