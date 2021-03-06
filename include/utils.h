#ifndef Utils_H
#define Utils_H

#include "Arduino.h"
#include "EEPROM.h"
#include "configs.h"

String readFromEEPROM(byte start, byte end) {
    String readValue = "";
    byte reader;
    for (int i = start; i < end; i++) {
        if ((reader = EEPROM.read(i)) == 0) {
            break;
        }
        readValue.concat((char)reader);
    }
    return readValue;
}

boolean writeToEEPROM(String newValue, String oldValue, byte startByte, byte endByte, bool verify) {
    Serial.println("Writing value");
    Serial.println(newValue);
    // Verify value existing in EEPROM and old-value passed are same
    if (verify) {
        String oldValueStored = readFromEEPROM(startByte, endByte);
        if (oldValue != oldValueStored) {
            return false;
        }
    }

    byte i, j;

    // Clear the existing value
    for (i = startByte; i < endByte; i++) {
        EEPROM.write(i, 0);
    }

    // Write the new value
    byte newValueEndIndex = newValue.length() + startByte;

    for (i = startByte, j = 0; i < newValueEndIndex; i++, j++) {
        EEPROM.write(i, newValue[j]);
    }
    EEPROM.commit();
    return true;
}

String routerSSID() {
    return readFromEEPROM(ssid_s, ssid_e);
}

String routerPassword() {
    return readFromEEPROM(pwd_s, pwd_e);
}

String getIP() {
    return readFromEEPROM(ip_s, ip_e);
}

void resetESP() {
    ESP.restart();
}

#endif