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
    while ( !Serial ) ;  
    gauge.reset();  // Resets MAX1720x
    delay(200);  // Waits for the initial measurements to be made
}

void loop()
{
    Serial.print("Capacity: ");
    Serial.print(gauge.getCapacity());  // Gets the battery's state of charge
    Serial.print(" mAh, TTE: ");
    Serial.print(gauge.getTTE());  // Gets the battery's state of charge
    Serial.print(" s, TTF: ");
    Serial.print(gauge.getTTF());  // Gets the battery's state of charge
    Serial.print(" s, Current: ");
    Serial.print(gauge.getCurrent());  // Gets the battery's state of charge
    Serial.print(" mA, Temperature: ");
    Serial.print(gauge.getTemperature());  // Gets the battery's state of charge
    Serial.print(" degC, SOC: ");
    Serial.print(gauge.getSOC());  // Gets the battery's state of charge
    Serial.print("%, VCELL: ");
    Serial.print(gauge.getVoltage());  // Gets the battery voltage
    Serial.println('mV');
    
    delay(2000);
}