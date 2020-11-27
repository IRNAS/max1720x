/**
 * Name: max1720x
 * Author: Luka Mustafa - Institute IRNAS Race { info@irnas.eu }
 * Version: 1.0
 * Description: A library for interfacing the MAXIM MAX17201/MAX17205
 *         Li+ fuel gauges.
 * Source: https://github.com/pAIgn10/max1720x
 * License: Copyright (c) 2017 Nick Lamprianidis 
 *          This library is licensed under the GPL license
 *          http://www.opensource.org/licenses/mit-license.php
 * Inspiration: The library is inspired by: https://github.com/pAIgn10/max1720x
 * Filename: max1720x.cpp
 * File description: Definitions and methods for the max1720x library
 */

#include "max1720x.h"

// Initializes variables and the Wire library
max1720x::max1720x() { 
  Wire.begin(); 
}

// Returns a measurement of the voltage of the connected LiIon Polymer battery
double max1720x::getVoltage()
{
  Wire.beginTransmission(MAX1720X_ADDR);
  Wire.write(MAX1720X_VCELL_ADDR);
  Wire.endTransmission(false);
  Wire.requestFrom(MAX1720X_ADDR, (int)2, HIGH); //send stop
  uint16_t combined = Wire.read()|(Wire.read()<<8); // LSB or-ed with MSB
  double voltage = combined; //combine registers
  return voltage*0.078125; // //calculate actual value and return in mV
}

double max1720x::getCurrent()
{
  Wire.beginTransmission(MAX1720X_ADDR);
  Wire.write(MAX1720X_CURENT_ADDR);
  Wire.endTransmission(false);
  Wire.requestFrom(MAX1720X_ADDR, (int)2, HIGH); //send stop
  int16_t combined = Wire.read()|(Wire.read()<<8); // LSB or-ed with MSB
  double current = (double)combined * 0.0015625 / MAX1720X_RSENSE_VAL;
  return current;//calculate actual value as 0.0015625 mV/Ohm
}

void max1720x::getCurrentRaw()
{
  char msgString[32];
  Wire.beginTransmission(MAX1720X_ADDR);
  Wire.write(MAX1720X_CURENT_ADDR);
  Wire.endTransmission(false);
  Wire.requestFrom(MAX1720X_ADDR, (int)2, HIGH); //send stop
  uint16_t combined = Wire.read()|(Wire.read()<<8); // LSB or-ed with MSB
  double current = (double)combined * (0.0015625 / MAX1720X_RSENSE_VAL);
  Serial.println(F("getCurrent:"));
  dtostrf(current, 5, 2, msgString);
  Serial.println(msgString);
}

double max1720x::getTemperature()
{
  Wire.beginTransmission(MAX1720X_ADDR);
  Wire.write(MAX1720X_TEMP_ADDR);
  Wire.endTransmission(false);
  Wire.requestFrom(MAX1720X_ADDR, (int)2, HIGH); //send stop
  int16_t combined = Wire.read()|(Wire.read()<<8); // LSB or-ed with MSB
  double temperature = (double)combined/256;
  return temperature;
}

// Returns the relative state of charge of the connected LiIon Polymer battery
// as a percentage of the full capacity w/ resolution 1/256%
double max1720x::getSOC()
{
  Wire.beginTransmission(MAX1720X_ADDR);
  Wire.write(MAX1720X_REPSOC_ADDR);
  Wire.endTransmission(false);
  Wire.requestFrom(MAX1720X_ADDR, (int)2);
  uint16_t combined = Wire.read()|(Wire.read()<<8); // LSB or-ed with MSB
  double soc = combined; //combine registers
  return soc/256; //calculate actual value and return in %
}

// RepCap or reported capacity is a filtered version of the AvCap register that prevents large jumps in the reported value caused by changes in the application such as abrupt changes in temperature or load current. 
double max1720x::getCapacity()
{
  Wire.beginTransmission(MAX1720X_ADDR);
  Wire.write(MAX1720X_REPCAP_ADDR);
  Wire.endTransmission(false);
  Wire.requestFrom(MAX1720X_ADDR, (int)2);
  uint16_t combined = Wire.read()|(Wire.read()<<8); // LSB or-ed with MSB
  // convert from VH to AH using Rsense resistor
  //Serial.write((byte*)&combined,2);
  double capacity = (double)combined / MAX1720X_RSENSE_VAL;
  return capacity;
}

uint16_t max1720x::getCapacityRaw()
{
  Wire.beginTransmission(MAX1720X_ADDR);
  Wire.write(MAX1720X_REPCAP_ADDR);
  Wire.endTransmission(false);
  Wire.requestFrom(MAX1720X_ADDR, (int)2);
  uint16_t combined = Wire.read()|(Wire.read()<<8); // LSB or-ed with MSB
  // convert from VH to AH using Rsense resistor
  Serial.write((byte*)&combined,2);
  Serial.println(F("getCapacity RAW no division with R-SENSE"));
  Serial.print(combined, DEC);
  Serial.println(F(" "));
  return combined;
}

// The TTE register holds the estimated time to empty for the application under present temperature and load conditions 
double max1720x::getTTE()
{
  Wire.beginTransmission(MAX1720X_ADDR);
  Wire.write(MAX1720X_TTE_ADDR);
  Wire.endTransmission(false);
  Wire.requestFrom(MAX1720X_ADDR, (int)2);
  uint16_t combined = Wire.read()|(Wire.read()<<8); // LSB or-ed with MSB
  double capacity = (double)combined*5.625;
  return capacity;//calculate actual value as value*5.625s
}

// The TTF register holds the estimated time to full for the application under present conditions. 
double max1720x::getTTF()
{
  Wire.beginTransmission(MAX1720X_ADDR);
  Wire.write(MAX1720X_TTF_ADDR);
  Wire.endTransmission(false);
  Wire.requestFrom(MAX1720X_ADDR, (int)2);
  uint16_t combined = Wire.read()|(Wire.read()<<8); // LSB or-ed with MSB
  double capacity = (double)combined*5.625;
  return capacity;//calculate actual value as value*5.625s
}

// Status Register (000h) The Status register maintains all flags related to alert thresholds and battery insertion or removal.
uint16_t max1720x::getStatus()
{
  Wire.beginTransmission(MAX1720X_ADDR);
  Wire.write(MAX1720X_STATUS_ADDR);
  Wire.endTransmission(false);
  Wire.requestFrom(MAX1720X_ADDR, (int)2);
  uint16_t combined = Wire.read()|(Wire.read()<<8); // LSB or-ed with MSB
  return combined;
}

// Returns a measurement of the voltage of the chosen cell
double max1720x::getCellVoltage(uint8_t cell)
{
  Wire.beginTransmission(MAX1720X_ADDR);
  switch (cell) {
    case 2:
      Wire.write(MAX1720X_VCELL2_ADDR);
      break;
    case 3:
      Wire.write(MAX1720X_VCELL2_ADDR);
      break;
    case 4:
      Wire.write(MAX1720X_VCELL4_ADDR);
      break;
    case 1:
    default:
      Wire.write(MAX1720X_VCELL1_ADDR);
      break;
  }
  Wire.endTransmission(false);
  Wire.requestFrom(MAX1720X_ADDR, (int)2, HIGH); //send stop
  uint16_t combined = Wire.read()|(Wire.read()<<8); // LSB or-ed with MSB
  double voltage = combined; //combine registers
  return voltage*0.078125; // //calculate actual value and return in mV
}

// Returns a measurement of the voltage for pack of cells
double max1720x::getPackVoltage(void)
{
  Wire.beginTransmission(MAX1720X_ADDR);
  Wire.write(MAX1720X_VBAT_ADDR);
  Wire.endTransmission(false);
  Wire.requestFrom(MAX1720X_ADDR, (int)2, HIGH); //send stop
  uint16_t combined = Wire.read()|(Wire.read()<<8); // LSB or-ed with MSB
  double voltage = combined; //combine registers
  return voltage*0.00125; //calculate actual value and return in V
}

// Returns a measurement of the temperature of the chosen cell
double max1720x::getCellTemperature(uint8_t cell)
{
  Wire.beginTransmission(MAX1720X_ADDR_HIGH);
  switch (cell)
  {
    case 1:
      Wire.write(MAX1720X_TEMP1_ADDR);
      break;
    case 2:
      Wire.write(MAX1720X_TEMP2_ADDR);
      break;
    case 0:
    default:
      Wire.write(MAX1720X_TEMP0_ADDR);
      break;
  }
  Wire.endTransmission(false);
  Wire.requestFrom(MAX1720X_ADDR_HIGH, (int)2, HIGH); //send stop
  int16_t combined = Wire.read()|(Wire.read()<<8); // LSB or-ed with MSB
  double temperature = (double)combined/10 - 273;
  return temperature;
}

// Returns unique 64-bit ID
void max1720x::getID(uint8_t *id)
{
  for (uint8_t i = 0; i < MAX1720X_ID_LEN / 2; i++)
  {
    Wire.beginTransmission(MAX1720X_ADDR_HIGH);
    Wire.write(MAX1720X_ID_ADDR + i); 
    Wire.endTransmission(false);
    Wire.requestFrom(MAX1720X_ADDR_HIGH, (int)2, HIGH);
    *id++ = Wire.read();
    *id++ = Wire.read();
  }
}

// Reset procedure
uint8_t max1720x::reset()
{
  Wire.beginTransmission(MAX1720X_ADDR);
  Wire.write(MAX1720X_COMMAND_ADDR);
  Wire.write(0x0f);
  Wire.write(0x00);
  Wire.endTransmission();
  delay(50);
  Wire.beginTransmission(MAX1720X_ADDR);
  Wire.write(MAX1720X_CONFIG2_ADDR);
  Wire.write(0x01);
  Wire.write(0x00);
  return Wire.endTransmission();
}

void max1720x::setPackCfg(uint16_t val)
{
  Wire.beginTransmission(MAX1720X_ADDR_HIGH);
  Wire.write(MAX1702X_PACKCFG_ADDR);  
  Wire.write((uint8_t)(val & 0xFF));
  Wire.write((uint8_t)(val >> 8));
  Wire.endTransmission();
}

void max1720x::hibernate()
{
  unsigned int val=0x890B;
  Wire.beginTransmission(MAX1720X_ADDR_HIGH);
  Wire.write(MAX1702X_NHIBCFG);  
  Wire.write((uint8_t)(val & 0xFF));
  Wire.write((uint8_t)(val >> 8));
  Wire.endTransmission();
}
