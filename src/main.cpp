#include <EEPROM.h>
#include <WebServer.h>
#include <WiFiClient.h>
#include <bluetooth.h>
#include <utils.h>

WebServer server(80);

void startBluetoothService() {
    xTaskCreate(bluetoothService, "BlueetoothService", 10000, NULL, tskIDLE_PRIORITY, NULL);
}

// void handleRoot() {
//   String q1=server.arg("pin");
//   if (q1=="high") {
//     digitalWrite(ledPIN, HIGH);
//   } else if (q1=="low") {
//     digitalWrite(ledPIN, LOW);
//   }
//   server.send(200, "text/html", MAIN_page);
// }

void changedata() {
    String q1 = server.arg("pin");
    server.send(200, "text", "Invalid data provided");
}

void handleNotFound() {
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
}

void setup() {
    Serial.begin(115200);
    EEPROM.begin(512);
    startBluetoothService();

    WiFi.mode(WIFI_STA);
    WiFi.begin(routerSSID().c_str(), routerPassword().c_str());

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println(WiFi.localIP());
    writeToEEPROM(WiFi.localIP().toString(), "", ip_s, ip_e, false);

    // server.on("/", handleRoot);
    server.onNotFound(handleNotFound);
    server.begin();
}

void loop() {
    server.handleClient();
}
