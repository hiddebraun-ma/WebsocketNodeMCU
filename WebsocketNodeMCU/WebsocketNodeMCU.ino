#include <Arduino_JSON.h>
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
// See: https://github.com/Links2004/arduinoWebSockets

// Include secrets for the WIFI network
#include "wifi_secrets.h"

WebSocketsServer webSocketServer = WebSocketsServer(81);
char str[60];

void handleWebSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocketServer.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

        // send message to client
        webSocketServer.sendTXT(num, "Connected!");
      }
      break;
    case WStype_TEXT:
      {
        String _payload = String((char *) &payload[0]);
        JSONVar commandObject = JSON.parse(_payload);
        Serial.printf("WS: %s\n", payload);

        if(commandObject.hasOwnProperty("command")){
          doCommand(num, commandObject);
        }
      }
      break;
    case WStype_BIN:
      Serial.printf("[%u] get binary length: %u\n", num, length);
      hexdump(payload, length);

      // send message to client
      // webSocket.sendBIN(num, payload, length);
      break;
  }

}

void doCommand(uint8_t num, JSONVar commando) {
  String cmd = (const char*) commando["command"];
  sprintf(str, "Running command: %s", cmd);
  webSocketServer.sendTXT(num, str);
  
  if(cmd == "toggle-led"){
      Serial.println("Toggling Built-In Led");
      int ledStatus = digitalRead(LED_BUILTIN);
      digitalWrite(LED_BUILTIN, !ledStatus);
  }
}

void setup(void) {

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  Serial.println();
  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  digitalWrite(LED_BUILTIN, HIGH);

  webSocketServer.begin();
  webSocketServer.onEvent(handleWebSocketEvent);
}

void loop(void) {
  webSocketServer.loop();
}
