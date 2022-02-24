#include <ESPmDNS.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include "webpage.h"



//Device Setup Constants
const char *ssid = "ESP32 Setup";
const char *password = "ESP32Setup";
const char *msg_ssid;
const char *msg_pass;
const int dns_port = 53;
const int http_port = 80;
const int ws_port = 1337;

//Globals
AsyncWebServer server(http_port);
WebSocketsServer webSocket = WebSocketsServer(ws_port);
char msg_buf(40);

/*########################## FUNCTIONS ##############################*/

//mDNS
void initmDNS()
{
  const char *dnsName = "camsetup";
  if (MDNS.begin(dnsName))
  {
    Serial.println("mDNS responder started");
    Serial.print("My name is: ");
    Serial.println(dnsName);

  }
  else
  {
    while (1)
    {
      Serial.println("Error setting up MDNS responder");
      delay(1000);
    }
  }
}

//WiFi scan
String scanWifi() {
  String result;
  Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
    result = "";
  } else {
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      result += WiFi.SSID(i);
      if (i != n - 1) result += ",";
      delay(10);
    }
  }
  return result;
}

// Callback: recieving ws message
void onWebSocketEvent(uint8_t client_num, WStype_t type, uint8_t *payload, size_t length) {
  /*Commandset for Arduino:
    0: send available wifi networks
    1: store ssid and pass
    2: resume
  */
  byte incoming[length];
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.print("["); Serial.print(client_num); Serial.println("] Disconnected!");
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(client_num);
        Serial.print("Client "); Serial.print(client_num); Serial.print(" connecting with "); Serial.println(ip.toString());
      }
      break;
    case WStype_TEXT:

      Serial.print("["); Serial.print(client_num); Serial.print("] sent message: ");
      for (int i = 0; i < length; i++) {
        incoming[i] = payload[i];
      }
      Serial.print("\n");
      respond(incoming, length);
      break;
      
    // For everything else: do nothing
    default:
      break;
  }


}

//Callback send homepage
void onIndexRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.print("[");  Serial.print(String(remote_ip)); Serial.print("] HTTP GET request of "); Serial.println(request->url());
  request->send(200, "text/html", webpageCode);
}

//Callback not found
void onPageNotFound(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.print("["); Serial.print(String(remote_ip)); Serial.print("] HTTP GET request of "); Serial.println(request->url());
  request->send(404, "text/plan", "Page not found!\n    404");
}


void respond(byte *payload, int length){
  Serial.print("Message: ");
  char cmd = payload[0];
  char message[length -2];

  Serial.print("Response: ");
  for (int i = 2; i < length; i++){
    Serial.print((char)payload[i]);
  }
  Serial.println("");
  String networks = "0:";
  networks += scanWifi();
  Serial.println(networks);
  webSocket.sendTXT(0, networks);
 }





void setup() {
  Serial.begin(115200);
  if (!WiFi.softAP(ssid, password)) {
    while (true) {
      if ((millis() / 1000) % 2 == 0) Serial.println("SoftAP failiure!");
    }
  }
  //initmDNS();
  delay(2000);
  Serial.print("AP running at "); Serial.println(WiFi.softAPIP());

  server.on("/", HTTP_GET, onIndexRequest);
  server.onNotFound(onPageNotFound);

  server.begin();
  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);
}
void loop() {
  webSocket.loop();
}
