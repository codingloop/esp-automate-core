#ifndef Configs_H
#define Configs_H

#include "pins_arduino.h"

// EEPROM bytes settings

// SSID and password
static const uint8_t ssid_s = 0;
static const uint8_t ssid_e = 30;
static const uint8_t pwd_s = 31;
static const uint8_t pwd_e = 60;

// current ip
static const uint8_t ip_s = 61;
static const uint8_t ip_e = 76;

#endif
