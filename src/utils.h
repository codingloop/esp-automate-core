#include "Arduino.h"
#include "EEPROM.h"

String readFromEEPROM(byte i1,byte i2)
{
  int l=i2;
  String c;
  int count=0;
  byte re;
  for(int i=i1;i<l;i++)
  {
    if((re=EEPROM.read(i))==0)
    {
      break;
    }
    c[count]=(char)re;
    count++;
  }
  c.trim();
  return c;
}

boolean writeToEEPROM(String newValue,String oldValue,byte startByte,byte endByte, bool verify)
{
    // Verify value existing in EEPROM and old-value passed are same
    if (verify) {
        String oldValueStored = readFromEEPROM(startByte, endByte);
        if (oldValue != oldValueStored) {
            return false;
        }
    }

    byte i, j;

    // Clear the existing value
    for(i=startByte;i<endByte;i++)
    {
        EEPROM.write(i,0);
    }

    // Write the new value
    byte newValueEndIndex = newValue.length() + startByte;

    for(i=startByte, j=0; i<newValueEndIndex; i++,j++)
    {
      EEPROM.write(i, newValue[j]);
    }
    EEPROM.commit();
}