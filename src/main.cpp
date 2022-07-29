#include "Arduino.h"
#include "bluetooth.h"
#include <WebServer.h>
#include <WiFiClient.h>
#include <root.h>
#include<Ticker.h>

#define ledPIN 2
#define iPin T5


int counter = 0;
bool on = true;
volatile int c = 0;
TaskHandle_t bleServer;

const char* ssid     = "Shivanugraha-Office";
const char* password = "Harigadde@39";
Ticker t1;

WebServer server(80);

portMUX_TYPE synch = portMUX_INITIALIZER_UNLOCKED;



void triggerBLEServer(){
  // Serial.print("Task2 running on core ");
  // Serial.println(xPortGetCoreID());
  portENTER_CRITICAL(&synch);
  digitalWrite(ledPIN, digitalRead(iPin));
  c+=1;
  
  // xTaskCreatePinnedToCore(
  //                   bluetoothService,   /* Task function. */
  //                   "Task1",     /* name of task. */
  //                   10000,       /* Stack size of task */
  //                   NULL,        /* parameter of the task */
  //                   1,           /* priority of the task */
  //                   &bleServer,      /* Task handle to keep track of created task */
  //                   1);          /* pin task to core 0 */  

  portEXIT_CRITICAL(&synch);
}

void managePairMode() {
  attachInterrupt(digitalPinToInterrupt(iPin), triggerBLEServer, HIGH);
}

void printCount() {
  Serial.println(c);
  Serial.println(digitalRead(iPin));

}


void handleRoot() {
  String q1=server.arg("pin");
  if (q1=="high") {
    digitalWrite(ledPIN, HIGH);
  } else if (q1=="low") {
    digitalWrite(ledPIN, LOW);
  }
  server.send(200, "text/html", MAIN_page);
}

void changedata()
{
  String q1=server.arg("pin");
  server.send(200,"text","Invalid data provided");
}

void handleNotFound() {
  digitalWrite(ledPIN, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(ledPIN, 0);
}


void setup() {
  pinMode(ledPIN, OUTPUT);
  pinMode(iPin, INPUT_PULLDOWN);
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    server.on("/", handleRoot);
    server.onNotFound(handleNotFound);
    server.begin();

    
  managePairMode();
  digitalWrite(T5, LOW);
  Serial.println(digitalRead(T5));
  t1.attach(3, printCount);
}

void loop() {
  server.handleClient();
}
