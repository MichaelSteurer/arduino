#include <Arduino.h>

#include <CustomLogger.h>


//-----------------------------------------------------------------------------
CustomLogger::CustomLogger(bool enabled) {
    Serial.begin(115200);
    while (!Serial) {
        delay(50);
    }

    this->enabled_ = enabled;
}

//-----------------------------------------------------------------------------
void CustomLogger::debug(const char* msg) {
    if(this->enabled_) {
        Serial.print(msg);
    }
}

//-----------------------------------------------------------------------------
void CustomLogger::debug(const char* msg[]) {
    int array_length = sizeof(msg);
    Serial.print(array_length);
    for (int i=0; i < array_length; i++) {
        debug(msg[i]);
    }
}

//-----------------------------------------------------------------------------
void CustomLogger::debugln(const char* msg) {
    if(this->enabled_) {
        Serial.println(msg);
    }
}
