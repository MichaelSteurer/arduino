#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 sensor219;

void setup(void) 
{  
    Serial.begin(115200);
    sensor219.begin();
    sensor219.setCalibration_16V_400mA();
}

void loop(void) 
{
    float busVoltage = 0;
    float current = 0; // Measure in milli amps
    float power = 0;

    busVoltage = sensor219.getBusVoltage_V();
    current = sensor219.getCurrent_mA();
    power = busVoltage * (current/1000);

    Serial.print(current); 
    Serial.print(" x ");
    Serial.print(busVoltage); 
    Serial.print(" = ");
    Serial.print(power); 
    Serial.println(""); 

  delay(500);
}