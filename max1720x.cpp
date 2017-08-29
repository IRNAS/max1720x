/**
 * Name: max1720x
 * Author: Luka Mustafa - Institute IRNAS Race { info@irnas.eu }
 * Version: 1.0
 * Description: A library for interfacing the MAXIM MAX17201/MAX17205
 * 				Li+ fuel gauges.
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

/*
 *  Function:     double max1720x::getVoltage()
 *  Description:  gets the lowest cell voltage and returns it in mV
 */
double max1720x::getVoltage()
{
	Wire.beginTransmission(MAX1720X_ADDR);
	Wire.write(MAX1720X_VCELL_ADDR);
	Wire.endTransmission(false);
	Wire.requestFrom(MAX1720X_ADDR, (int)2,HIGH);         //send stop
  uint16_t combined = Wire.read()|(Wire.read()<<8);     // LSB or-ed with MSB
  double voltage = combined;                            //combine registers
  return voltage*0.078125;                              //calculate actual value and return in mV
}

/*
 *  Function:     double max1720x::getCurrent()
 *  Description:  gets the voltage between two pins and calculates it with 0.0015625 mV/Ohm to get mA
 */
double max1720x::getCurrent()
{
	Wire.beginTransmission(MAX1720X_ADDR);
	Wire.write(MAX1720X_CURENT_ADDR);
	Wire.endTransmission(false);
	Wire.requestFrom(MAX1720X_ADDR, (int)2,HIGH);         //send stop
  int16_t combined = Wire.read()|(Wire.read()<<8);      // LSB or-ed with MSB
  double current = (double)combined*0.0015625/0.010;    //calculate actual value as 0.0015625 mV/Ohm
  return current;                                   
}

/*
 *  Function:     double max1720x::getAvgCurrent()
 *  Description:  gets the average from the average register and calculates it with 0.0015625 mV/Ohm to get mA
 */
double max1720x::getAvgCurrent()
{
 Wire.beginTransmission(MAX1720X_ADDR);
 Wire.write(MAX1720X_AVGCURENT_ADDR);
 Wire.endTransmission(false);
 Wire.requestFrom(MAX1720X_ADDR, (int)2, HIGH);
 int16_t combined  = Wire.read()|(Wire.read()<<8);      // LSB or-ed with MSB
 double current = (double)combined*0.0015625/0.010;
 return current;                                        //calculate actual value as 0.0015625 mV/Ohm
}

/*
 *  Function:     double max1720x::getMaxCurrent()
 *  Description:  gets the maxmin current register and returns max
 */
double max1720x::getMaxCurrent()
{
  /*
    .0004mV / Rsense resolution
    0.0004V / 0.010ohm resolution
    0.04 resolution
    that is 40mA resolution!
   */
   
  Wire.beginTransmission(MAX1720X_ADDR);
  Wire.write(MAX1720X_MAXMIN_CURENT_ADDR);
  Wire.endTransmission(false);
  Wire.requestFrom(MAX1720X_ADDR, (int)2,HIGH);         //send stop
  int8_t minimum     = Wire.read();                     // getting max
  int8_t maximum     = Wire.read();                     // getting min

  return ((double)maximum * 0.04 * 1000);               // calculate it with 
}

/*
 *  Function:     double max1720x::getMinCurrent()
 *  Description:  gets the maxmin current register and returns min
 */
double max1720x::getMinCurrent()
{
  /*
    .0004mV / Rsense resolution
    0.0004V / 0.010ohm resolution
    0.04 resolution
    that is 40mA resolution!
   */
   
  Wire.beginTransmission(MAX1720X_ADDR);
  Wire.write(MAX1720X_MAXMIN_CURENT_ADDR);
  Wire.endTransmission(false);
  Wire.requestFrom(MAX1720X_ADDR, (int)2,HIGH);         //send stop
  int8_t minimum     = Wire.read();                     // getting max
  int8_t maximum     = Wire.read();                     // getting min

  return ((double)minimum * 0.04 * 1000);               // calculate it with
}

/*
 *  Function:     double max1720x::getTemperature()
 *  Descritpion:  gets the temperature from the temperature register
 */
double max1720x::getTemperature()
{
	Wire.beginTransmission(MAX1720X_ADDR);
	Wire.write(MAX1720X_TEMP_ADDR);
	Wire.endTransmission(false);
	Wire.requestFrom(MAX1720X_ADDR, (int)2,HIGH);         //send stop
  int16_t combined = Wire.read()|(Wire.read()<<8);      // LSB or-ed with MSB
  double temperature = (double)combined/256;            // to get the right value
  return temperature;
}

/*
 *  Function:     double max1720x::getSOC()
 *  Description:  Returns the relative state of charge of the connected LiIon Polymer battery as a percentage of the full capacity w/ resolution 1/256%
 */
double max1720x::getSOC()
{
	Wire.beginTransmission(MAX1720X_ADDR);
	Wire.write(MAX1720X_REPSOC_ADDR);
	Wire.endTransmission(false);
	Wire.requestFrom(MAX1720X_ADDR, (int)2);
  uint16_t combined = Wire.read()|(Wire.read()<<8);     // LSB or-ed with MSB
  double soc = combined;                                //combine registers
	return soc/256;                                       //calculate actual value and return in %
}

/*
 *  Function: double max1720x::getCapacity()
 *  Description: RepCap or reported capacity is a filtered version of the AvCap register that prevents large jumps in the reported value caused by changes in the application such as abrupt changes in temperature or load current. 
 */
double max1720x::getCapacity()
{
	Wire.beginTransmission(MAX1720X_ADDR);
	Wire.write(MAX1720X_REPCAP_ADDR);
	Wire.endTransmission(false);
	Wire.requestFrom(MAX1720X_ADDR, (int)2);
  uint16_t combined = Wire.read()|(Wire.read()<<8);     // LSB or-ed with MSB
	double capacity = (double)combined*0.005/0.01;        //calculate actual value as 0.005 mVh/Ohm
  return capacity;
}

/*
 *  Function:     double max1720x::getTTE()
 *  Description:  The TTE register holds the estimated time to empty for the application under present temperature and load conditions 
 */
double max1720x::getTTE()
{
	Wire.beginTransmission(MAX1720X_ADDR);
	Wire.write(MAX1720X_TTE_ADDR);
	Wire.endTransmission(false);
	Wire.requestFrom(MAX1720X_ADDR, (int)2);
  uint16_t combined = Wire.read()|(Wire.read()<<8);     // LSB or-ed with MSB
	double capacity = (double)combined*5.625;             // calculate actual value as value*5.625s
  return capacity;
}

/*
 *  Function:     double max1720x::getTTF()
 *  Description:  The TTF register holds the estimated time to full for the application under present conditions. 
 */
double max1720x::getTTF()
{
	Wire.beginTransmission(MAX1720X_ADDR);
	Wire.write(MAX1720X_TTF_ADDR);
	Wire.endTransmission(false);
	Wire.requestFrom(MAX1720X_ADDR, (int)2);
  uint16_t combined = Wire.read()|(Wire.read()<<8);     // LSB or-ed with MSB
	double capacity = (double)combined*5.625;             //calculate actual value as value*5.625s
  return capacity;
}

/*
 *  Function:     uint8_t max1720x::getBatteryStatus()
 *  Description:  reads the status register and gets the 4th bit which is battery status
 */
uint8_t max1720x::getBatteryStatus()
{
	Wire.beginTransmission(MAX1720X_ADDR);
	Wire.write(MAX1720X_STATUS_ADDR);
	Wire.endTransmission(false);
	Wire.requestFrom(MAX1720X_ADDR, (int)2);
  uint16_t combined = Wire.read()|(Wire.read()<<8);     // LSB or-ed with MSB
	return (combined >> 3) & 1;                           // gets that bit and returns it (0 or 1)
}

/*
 *  Function:     uint8_t max1720x::resetMaxMinAvgCurrent()
 *  Description:  Resets the maxmin avg current register by writing 0x807F into it
 */
uint8_t max1720x::resetMaxMinAvgCurrent()
{
  Wire.beginTransmission(MAX1720X_ADDR);
  Wire.write(0x1C);                                     // go to register
  Wire.write((0x807F >> 0) & 0xFF);                     // write first half
  Wire.write((0x807F >> 8) & 0xFF);                     // write second half
  return Wire.endTransmission();
}

/*
 *  Function:     void max1720x::setCurrentAvgTime(int input_value)
 *  Description:  sets the time for avg current defined by the table
 */
void max1720x::setCurrentAvgTime(int input_value)
{
  // the formula is calculated by 45 * 2^(value-7) = x seconds

  // table of seconds
  // 0 -> 0,35s    3 -> 2,8s    6 -> 22,5s  9  -> 3min   12 -> 24min   15 -> 3,2h
  // 1 -> 0,703s   4 -> 5,65s   7 -> 45s    10 -> 6min   13 -> 48min
  // 2 -> 1,40     5 -> 11,12s  8 -> 90s    11 -> 12min  14 -> 1,6h

  Wire.beginTransmission(MAX1720X_ADDR);
  Wire.write(0x29);
  Wire.endTransmission(false);
  Wire.requestFrom(MAX1720X_ADDR, (int)2);
  uint16_t combined = Wire.read()|(Wire.read()<<8);      // LSB or-ed with MSB
  combined = combined & 0xFFF0;                          // with this we are clearing the 4 bits we want to set after
  
  /*
   * with (value & 0x000F) we make it copy the good values but set everything else to 0 
   * with current_reg | (value & 0x000F) we make it "copy" the other values from the current reg to the value
   */
  uint16_t new_reg = combined | (input_value & 0x000F);  
  delay(100);
  Wire.beginTransmission(MAX1720X_ADDR);
  Wire.write(0x29);                                      // go to register
  Wire.write((new_reg >> 0) & 0xFF);                     // first half
  Wire.write((new_reg >> 8) & 0xFF);                     // second half
  Wire.endTransmission();
  
}

/*
 *  Function: uint8_t max1720x::reset()
 *  Description: resets the chip
 */
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

