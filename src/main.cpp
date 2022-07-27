#include "Arduino.h"
#include "bluetooth.h"
#include <WebServer.h>
#include <WiFiClient.h>
#include <root.h>

#define ledPIN 2

int counter = 0;
bool on = true;
TaskHandle_t bleServer;

const char* ssid     = "Ishu";
const char* password = "ishuharigadde";

WebServer server(80);


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
  pinMode(ledPIN, INPUT_PULLUP);
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
}

void loop() {
  server.handleClient();
}
