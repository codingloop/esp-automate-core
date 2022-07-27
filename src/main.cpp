#include "Arduino.h"
#include "bluetooth.h"

#define ledPIN 2

int counter = 0;
bool on = true;
TaskHandle_t bleServer;


void triggerBLEServer(){
  // Serial.print("Task2 running on core ");
  // Serial.println(xPortGetCoreID());
  
  xTaskCreatePinnedToCore(
                    bluetoothService,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &bleServer,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 0 */     
}

void managePairMode() {
  attachInterrupt(digitalPinToInterrupt(ledPIN), triggerBLEServer, ONHIGH);
}


void setup() {
  pinMode(ledPIN, OUTPUT);
  Serial.begin(115200);
  managePairMode();
             
  delay(500); 
}

void loop() {
  if (on) {
    on = false;
  } else {
    on = true;
  }
  counter++;
  if (counter > 10) {
    counter = 0;
  }
  // Serial.print(vTaskGetInfo(bleServer));
  digitalWrite(ledPIN, on);
  delay(1000);
}