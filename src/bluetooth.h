#include "BluetoothSerial.h"
#include "utils.h"

volatile bool isPairEnabled = true;
BluetoothSerial SerialBT;
String BTData;

void disableBluetoothService() {
    isPairEnabled = false;
}

void executeCommand(String cmd, String value) {
    value.trim();
    Serial.println("Received cmd and value");
    Serial.println(cmd);
    Serial.println(value);
    if (cmd == "ssid") {
        writeToEEPROM(value, "", 0, 30, false);
    } else if (cmd == "password") {
        writeToEEPROM(value, "", 31, 60, false);
    } else if (cmd == "reset") {
        resetESP();
    } else if (cmd == "disable") {
        disableBluetoothService();
        Serial.write("disabling pairmode");
    }
}

void parseAndExecCommand(String command) {
    command.trim();
    int actionIndex = command.indexOf("=");
    if (actionIndex == -1) {
       executeCommand(command, ""); 
    }
    executeCommand(command.substring(0, actionIndex), command.substring(actionIndex+1, command.length()));
}

void bluetoothService( void * pvParameters ) {
    // writeToEEPROM("Shivanugraha-office", "", 0, 30, false);
    // writeToEEPROM("Harigadde@39", "", 31, 60, false);
   
    SerialBT.begin("codingloop");
    while (isPairEnabled) {
        if(SerialBT.available())
        {
            BTData = SerialBT.readString();
            Serial.println(BTData.c_str());
            parseAndExecCommand(BTData);
        }
  }
  SerialBT.end();
   Serial.write("Ending pairmode");
    vTaskDelete(NULL);
}
