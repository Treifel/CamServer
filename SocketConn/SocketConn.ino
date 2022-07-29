#include <ESPmDNS.h>
#include <WiFi.h>
#include <Preferences.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include "webpage.h"
#include "StepperMaster.h"
#include "Gimbal.h"

#define stepperTimeout 4000

//Device Setup Constants
const char *ssid = "ESP32 Setup";
const char *password = "ESP32Setup";
const char *msg_ssid;
const char *msg_pass;
const int dns_port = 53;
const int http_port = 80;
const int ws_port = 1337;
int stamp;

//Globals
AsyncWebServer server(http_port);
WebSocketsServer webSocket = WebSocketsServer(ws_port);
Preferences prefs;
char msg_buf(40);
int credentialCounter;

//Stepper Init
StepperMaster stepperOne(16, 17, 18, 19); //Horizontal axis
StepperMaster stepperTwo(27, 26, 25, 33); //Vertical axis
Gimbal gimbal(&stepperOne, &stepperTwo, 900);


void gimbalTest() {
  gimbal.rotateTo(-20, 20, 30);
  gimbal.rotateTo(20, 20, 30);
  gimbal.rotateTo(20, -20, 30);
  gimbal.rotateTo(0, -20, 30);
  gimbal.rotateTo(0, 0, 30);
  stepperOne.disable();
  stepperTwo.disable();
}

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
  //Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  //Serial.println("scan done");
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


//########## WebSocket Handling ##########
// Callback: recieving ws message
void onWebSocketEvent(uint8_t client_num, WStype_t type, uint8_t *payload, size_t length) {

  byte incoming[length];
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.print("["); Serial.print(client_num); Serial.println("] Disconnected!");
      Serial.println("------------ Protocol Log End ------------");
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(client_num);
        Serial.print("Client ["); Serial.print(client_num); Serial.print("] connecting with "); Serial.println(ip.toString());
        Serial.println("------------ Protocol Log Start ------------");
      }
      break;
    case WStype_TEXT:

      Serial.print("\n["); Serial.print(client_num); Serial.print("] sent message! ");
      for (int i = 0; i < length; i++) {
        incoming[i] = payload[i];
      }
      Serial.print("\n");
      respond(incoming, length);
      break;

    // For everything else: do nothing
    case WStype_ERROR:
      Serial.println("Unknown ERROR");
      break;
    default:
      Serial.print("Unknown: "); Serial.println(String(type));
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

//Respond function
/*Protocol cmdset:
  0: send available wifi networks
  1: store ssid and pass
  2: clear EEPROM
  3: restart ESP
  4: rotate cam
  5: get stepper position
*/
void respond(byte *payload, int length) {
  char cmd = payload[0];
  String response;
  String px;
  String py;
  String vel;
  String wsSSID;
  String wsPASS;
  int count = 0;

  //Serial.print("Command: "); Serial.print(cmd);
  switch (cmd) {
    case '0': //Return wifi Signals in Range
      Serial.println(" (return wifi signals)");
      response = "0:";
      response += scanWifi();
      webSocket.sendTXT(0, response);
      Serial.println(response);
      break;

    case '1': //Store incoming SSID and password
      {
        //Serial.println(" (Store ssid and password)");
        bool next = false;
        for (int i = 2; i < length; i++) {  //i = 2 -> Protocol: {(cmd):(payload)}
          if (char(payload[i] == ',')) {    //                   {( 0 )(1)(...)}
            next = true;
          } else {
            if (next) wsPASS += char(payload[i]);
            else wsSSID += char(payload[i]);
          }
        }
        webSocket.sendTXT(0, "1:stored");
        storeCreds(wsSSID, wsPASS);
        //Serial.print("SSID: "); Serial.print(wsSSID); Serial.print(", pass: "); Serial.println(wsPASS);
      }
      break;

    case '2': //clear stored wifi credentials
      Serial.println(" (clear stored wifi credentials)");
      prefs.clear();
      prefs.putUInt("counter", 0);
      response = "2:succes";
      webSocket.sendTXT(0, response);
      break;

    case '3':
      webSocket.sendTXT(0, "3:Restarting");
      ESP.restart();
      break;

    case '4':
      //4:posX,posY,Speed
      stamp = millis();
      //Serial.println(" (rotate camera)");
      //Serial.print("RAW: ");
      for (int i = 2; i < length; i++) {
        //Serial.print(char(payload[i]));
        if (char(payload[i]) == ',') {
          count++; //check for
        } else {
          switch (count) {
            case 0:
              px += char(payload[i]);
              break;
            case 1:
              py += char(payload[i]);
              break;
            case 2:
              vel += char(payload[i]);
              break;

            default:
              break;
          }
        }
      }
      //Serial.println("");

      //Serial.print("Rotating to X: "); Serial.print(px.toFloat()); Serial.print(", Y: "); Serial.print(py.toFloat()); Serial.println(".");
      gimbal.rotateTo(py.toFloat(), px.toFloat(), vel.toFloat());
      response = "4:";
      response += px.toFloat(); //get vertical axis Value (x)
      response += ",";
      response += py.toFloat(); //get horizontal axis Value (y)
      webSocket.sendTXT(0, response);

      break;
    case '5':
      Serial.println(" (return stepper positions)");
      response = "5:";
      response += stepperTwo.getAbsoluteAngle(); //get horizontal axis Value (x)
      response += ",";
      response += stepperOne.getAbsoluteAngle(); //get vertical axis Value (y)
      webSocket.sendTXT(0, response);
      break;
    default: //command could not be recognized
      Serial.println("(Unknown command!)");
      webSocket.sendTXT(0, "100:Unkown Command");
      break;
  }
}

bool storeCreds(String ssid, String pass) {
  prefs.putString("ssid", ssid.c_str());
  prefs.putString("pass", pass.c_str());
  int temp = prefs.getUInt("counter") + 1;
  prefs.putUInt("counter", temp);
}


void showEEPROM() {
  credentialCounter = prefs.getUInt("counter", 0);
  if (credentialCounter > 0) {
    Serial.print("Found "); Serial.print(credentialCounter); Serial.println(" stored WiFi Credentials:");
    String eepromSSID = prefs.getString("ssid");
    String eepromPASS = prefs.getString("pass");
    Serial.print("SSID: "); Serial.println(eepromSSID);
    Serial.print("Pass: "); Serial.println(eepromPASS);
  } else {
    Serial.print("Found "); Serial.print(credentialCounter); Serial.println(" stored WiFi Credentials");
  }

}

void stepperTimeoutCheck() {
  if (stamp + stepperTimeout <= millis()) {
    stepperOne.disable();
    stepperTwo.disable();
  }
}

void tryWifi() {
  String eepromSSID;
  String eepromPASS;
  int wifiTimeoutLimit = 10; //timeout in seconds
  for (int i = 0; i < credentialCounter; i++) {

    if (WiFi.status() != WL_CONNECTED) {
      eepromSSID = prefs.getString("ssid");
      eepromPASS = prefs.getString("pass");
      WiFi.begin(eepromSSID.c_str(), eepromPASS.c_str());

      int timeStamp = millis() + wifiTimeoutLimit*1000;
      Serial.print("Trying to connect to "); Serial.print(eepromSSID);
      while ((WiFi.status() != WL_CONNECTED) && (timeStamp >= millis())) {
         //if (millis() % 1000 == 0) Serial.print(" .");
      }
    }

  }
}

//----------------------------------- µ-Controller loop -----------------------------------
void setup() {
  Serial.begin(115200);

// ------------- WIFI Setup -------------
  //Check store wifi credentials
  prefs.begin("Credentials", false);
  showEEPROM();
  if (credentialCounter > 0) {
    tryWifi();
  }


  if (WiFi.status() != WL_CONNECTED) { //if there is no working wifi start AP
    if (!WiFi.softAP(ssid, password)) {
      while (true) {
        if ((millis() / 1000) % 2 == 0) Serial.println("SoftAP failiure!");
      }
    }
    //initmDNS();
    delay(2000);
    Serial.print("AP running at "); Serial.println(WiFi.softAPIP());
  }else{
    Serial.print("ESP connected to "); Serial.print(WiFi.SSID()); Serial.print(" running on "); Serial.println(WiFi.localIP());
  }

  Serial.println("Starting website host and websocket communication");
  //There must be a WiFi connection beyond this point (AP or Router)

  // ------------- WebSocket and Server setup -------------
  server.on("/", HTTP_GET, onIndexRequest);
  server.onNotFound(onPageNotFound);

  server.begin();
  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);
}



void loop() {
  webSocket.loop();
  stepperTimeoutCheck();
}
