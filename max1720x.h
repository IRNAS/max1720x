/**
 * Name: max1720x
 * Author: Luka Mustafa - Institute IRNAS Race { info@irnas.eu }
 * Version: 1.0
 * Description: A library for interfacing the MAXIM MAX17201/MAX17205
 * 				Li+ fuel gauges.
 * Source: https://github.com/pAIgn10/LiFuelGauge
 * License: Copyright (c) 2017 Nick Lamprianidis 
 *          This library is licensed under the GPL license
 *          http://www.opensource.org/licenses/mit-license.php
 * Inspiration: The library is inspired by: https://github.com/pAIgn10/LiFuelGauge
 * Filename: max1720x.h
 * File description: Definitions and methods for the max1720x library
 */

#ifndef max1720x
#define max1720x_h

#include <Arduino.h>
#include <Wire.h>

#define MAX1720X_ID_LEN       (8)

#define MAX1720X_RSENSE_VAL   (0.01f)

// MAX1720X register addresses
const int MAX1720X_ADDR = 0x36;
const int MAX1720X_ADDR_HIGH = 0x0B;

const int MAX1720X_STATUS_ADDR = 0x00; // Contains alert status and chip status
const int MAX1720X_VCELL_ADDR = 0x09; // Lowest cell voltage of a pack, or the cell voltage for a single cell
const int MAX1720X_VCELL1_ADDR = 0xD8; // Cell #1 voltage
const int MAX1720X_VCELL2_ADDR = 0xD7; // Cell #2 voltage
const int MAX1720X_VCELL3_ADDR = 0xD6; // Cell #3 voltage
const int MAX1720X_VCELL4_ADDR = 0xD5; // Cell #4 voltage
const int MAX1720X_REPSOC_ADDR = 0x06; // Reported state of charge
const int MAX1720X_REPCAP_ADDR = 0x05; // Reported remaining capacity
const int MAX1720X_TEMP_ADDR = 0x08; // Temperature
const int MAX1720X_TEMP1_ADDR = 0x34; // Temperature #1
const int MAX1720X_TEMP0_ADDR = 0x35; // Temperature Int
const int MAX1720X_TEMP2_ADDR = 0x3B; // Temperature #2
const int MAX1720X_CURENT_ADDR = 0x0A; // Battery current
const int MAX1720X_TTE_ADDR = 0x11; // Time to empty
const int MAX1720X_TTF_ADDR = 0x20; // Time to full
const int MAX1720X_CAPACITY_ADDR = 0x10; // Full capacity estimation
const int MAX1720X_VBAT_ADDR = 0xDA; // Battery pack voltage
const int MAX1720X_AVCELL_ADDR = 0x17; // Battery cycles
const int MAX1720X_COMMAND_ADDR = 0x60; // Command register
const int MAX1720X_CONFIG2_ADDR = 0xbb; // Command register
const int MAX1720X_ID_ADDR = 0xbc; // ID, first register
const int MAX1702X_PACKCFG_ADDR = 0xb5; // nPackCfg register
const int MAX1702X_NHIBCFG = 0xB4; 

// Class for interfacing the MAX1720X Li+ fuel gauges
class max1720x
{
public:
	max1720x();
    double getVoltage();
    double getSOC();
    double getTemperature();
    double getCurrent();
    void   getCurrentRaw();
    double getCapacity();
    uint16_t getCapacityRaw();
    double getTTE();
    double getTTF();
    double getCellVoltage(uint8_t);
    double getPackVoltage();
    double getCellTemperature(uint8_t);
    void getID(uint8_t*);
    void hibernate();
    uint8_t reset();
    void setPackCfg(uint16_t);
private:
    uint16_t getStatus();
};

#endif  // max1720x
