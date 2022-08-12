#include <EEPROM.h>
#include <WebServer.h>
#include <WiFiClient.h>
#include <bluetooth.h>
#include <utils.h>

WebServer server(80);

void startBluetoothService() {
    xTaskCreate(bluetoothService, "BlueetoothService", 10000, NULL, tskIDLE_PRIORITY, NULL);
}

// Web server endpoints
void handleRoot() {
    server.send(200, "text/html", "Home");
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

void readAnalogValue() {
    server.send(200, "text/html", String(analogRead(A6)));
}

// ENd web server endpoints

void setup() {
    Serial.begin(115200);
    EEPROM.begin(512);
    pinMode(A6, INPUT_PULLDOWN);

    startBluetoothService();

    WiFi.mode(WIFI_STA);
    WiFi.begin(routerSSID().c_str(), routerPassword().c_str());

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    writeToEEPROM(WiFi.localIP().toString(), "", ip_s, ip_e, false);

    server.on("/", handleRoot);
    server.on("/analog/a6", readAnalogValue);
    server.onNotFound(handleNotFound);
    server.begin();
}

void loop() {
    server.handleClient();
}
