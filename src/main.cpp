#include "Arduino.h"
#include "bluetooth.h"

#define ledPIN 2

int counter = 0;
bool on = true;
TaskHandle_t bleServer;


void handleBleServer( void * pvParameters ){
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());
  for (;;) {
    delay(1000);
    Serial.print("I am thread runner");
  }
}

void managePairMode() {
  attachInterrupt(digitalPinToInterrupt(T4), bluetoothService, ONHIGH);

  xTaskCreatePinnedToCore(
                    handleBleServer,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &bleServer,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */     
}


void setup() {
  pinMode(ledPIN, OUTPUT);
  Serial.begin(115200);
  managePairMode();
             
  delay(500); 

  // //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  // xTaskCreatePinnedToCore(
  //                   Task2code,   /* Task function. */
  //                   "Task2",     /* name of task. */
  //                   10000,       /* Stack size of task */
  //                   NULL,        /* parameter of the task */
  //                   1,           /* priority of the task */
  //                   &Task2,      /* Task handle to keep track of created task */
  //                   1);          /* pin task to core 1 */
  //   delay(500); 
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