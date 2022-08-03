#include "BluetoothSerial.h"
#include "utils.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

volatile bool isPairEnabled = true;
BluetoothSerial SerialBT;
String BTData;

void disableBluetoothService() {
    isPairEnabled = false;
}

void executeCommand(String cmd, String value) {
    value.trim();
    if (cmd == "ssid") {
        writeToEEPROM(value, "", ssid_s, ssid_e, false);
    } else if (cmd == "password") {
        writeToEEPROM(value, "", pwd_s, pwd_e, false);
    } else if (cmd == "reset") {
        resetESP();
    } else if (cmd == "disable") {
        disableBluetoothService();
    } else if (cmd == "getip") {
        SerialBT.println(getIP());
    }
}

void parseAndExecCommand(String command) {
    command.trim();
    int actionIndex = command.indexOf("=");
    if (actionIndex == -1) {
        executeCommand(command, "");
        return;
    }
    executeCommand(command.substring(0, actionIndex), command.substring(actionIndex + 1, command.length()));
}

void bluetoothService(void* pvParameters) {
    SerialBT.begin("codingloop");
    while (isPairEnabled) {
        if (SerialBT.available()) {
            BTData = SerialBT.readString();
            parseAndExecCommand(BTData);
        }
        vTaskDelay(1000);
    }
    SerialBT.end();
    Serial.write("Ending pairmode");
    vTaskDelete(NULL);
}
