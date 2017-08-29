/**
 * This is an example demonstrating the use of the max1720x library
 * Print out the information to serial monitor at 115200 baud
 */

#include <Wire.h>
#include <max1720x.h>

max1720x gauge;

void setup()
{
    Serial.begin(115200); // Initializes serial port
    // Waits for serial port to connect. Needed for Leonardo only
    while ( !Serial );  
    gauge.reset();  // Resets MAX1720x
    delay(200);  // Waits for the initial measurements to be made
    gauge.resetMaxMinAvgCurrent();
    delay(200);
    gauge.setCurrentAvgTime(4);
    delay(200);
}

void loop()
{
    Serial.println("-------------------------------");
    Serial.print("Capacity: ");
    Serial.print(gauge.getCapacity());  // Gets the battery's state of charge
    Serial.println(" mAh");
    Serial.print("TTE: ");
    Serial.print(gauge.getTTE());  // Gets the battery's state of charge
    Serial.println(" s"); 
    Serial.print("TTF: ");
    Serial.print(gauge.getTTF());  // Gets the battery's state of charge
    Serial.println(" s");
    Serial.print("Current: ");
    Serial.print(gauge.getCurrent());  // Gets the battery's state of charge
    Serial.println(" mA");
    Serial.print("Average current: ");
    Serial.print(gauge.getAvgCurrent());  // Gets the battery's state of charge
    Serial.println(" mA");
    Serial.print("Max current: ");
    Serial.print(gauge.getMaxCurrent());
    Serial.println(" mA");
    Serial.print("Min Current: ");
    Serial.print(gauge.getMinCurrent());
    Serial.println(" mA");
    Serial.print("Temperature: ");
    Serial.print(gauge.getTemperature());  // Gets the battery's state of charge
    Serial.println(" degC");
    Serial.print("SOC: ");
    Serial.print(gauge.getSOC());  // Gets the battery's state of charge
    Serial.println("%");
    Serial.print("VCELL: ");
    Serial.print(gauge.getVoltage());  // Gets the battery voltage
    Serial.println("mV");
    Serial.print("Battery status: ");
    Serial.print(gauge.getBatteryStatus() ? "Battery not present" : "Battery present");
    Serial.println("");
    delay(2000);
}

