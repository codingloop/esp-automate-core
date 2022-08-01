#include "BluetoothSerial.h"
#include "utils.h"

volatile bool isPairEnabled;
BluetoothSerial SerialBT;
String BTData;

void disableBluetoothService() {
    isPairEnabled = false;
}

void executeCommand(String cmd, String value) {
    value.trim();

    if (cmd == "ssid") {
        writeToEEPROM(value, "", 0, 30, false);
    } else if (cmd == "password") {
        writeToEEPROM(value, "", 31, 60, false);
    } else if (cmd == "reset") {
        resetESP();
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
            Serial.write(BTData);
            parseAndExecCommand(BTData);
        }
  }
  SerialBT.end();
   Serial.write("Ending pairmode");
}

// #include "BluetoothSerial.h"

/* Check if Bluetooth configurations are enabled in the SDK */
/* If not, then you have to recompile the SDK */
// #if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
// #error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
// #endif

// #define ledPIN 2
// BluetoothSerial SerialBT;
// byte BTData;

// void setup() {
//   pinMode(ledPIN, OUTPUT);
//   Serial.begin(115200);
//   SerialBT.begin("codingloop");
//   SerialBT.end();
//   Serial.println("Bluetooth Started! Ready to pair...");
// }

// void loop() {
//   if(SerialBT.available())
//   {
//     BTData = SerialBT.read();
//     Serial.write(BTData);
//   }

//   /* If received Character is 1, then turn ON the LED */
//   /* You can also compare the received data with decimal equivalent */
//   /* 48 for 0 and 49 for 1 */
//   /* if(BTData == 48) or if(BTData == 49) */
//   if(BTData == '1')
//   {
//     digitalWrite(ledPIN, HIGH);
//   }
  
//   /* If received Character is 0, then turn OFF the LED */
//   if(BTData == '0')
//   {
//     digitalWrite(ledPIN, LOW);
//   }

//   Serial.println(touchRead(T1));
//   delay(1000);
// }