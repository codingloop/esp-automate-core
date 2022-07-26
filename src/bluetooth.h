#include "Arduino.h"
#include "BluetoothSerial.h"


volatile bool isPairEnabled;

void disableBluetoothService() {
    isPairEnabled = false;
}

void bluetoothService( void * pvParameters ) {
    if (digitalRead(T4)) {
        isPairEnabled = true;
        attachInterrupt(digitalPinToInterrupt(T4), disableBluetoothService, ONLOW);
        SerialBT.begin("codingloop");
    }
//   Serial.print("Task2 running on core ");
//   Serial.println(xPortGetCoreID());
  while (isPairEnabled) {
        if(SerialBT.available())
        {
            BTData = SerialBT.read();
            Serial.write(BTData);
        }
  }
  SerialBT.end();
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