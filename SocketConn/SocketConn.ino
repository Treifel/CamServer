#include <ESPmDNS.h>
#include <WiFi.h>
#include <Preferences.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include "webpage.h"
#include "StepperMaster.h"
#include "Gimbal.h"

#define stepperTimeout 4000

//AP SSID and Pass
const char *ssid = "ESP32 Setup";
const char *password = "ESP32Setup";


//dns setup
const int dns_port = 53;
const int http_port = 80;
const int ws_port = 1337;
int stamp;


//Webserver init
AsyncWebServer server(http_port);
WebSocketsServer webSocket = WebSocketsServer(ws_port);

//Create EEPROM namespace objects
Preferences credentials;
Preferences wifiMeta;
Preferences favPos;
char msg_buf(40);
int credentialCounter;


//Stepper init
StepperMaster stepperOne(16, 17, 18, 19);  //Horizontal axis
StepperMaster stepperTwo(27, 26, 25, 33);  //Vertical axis
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
void initmDNS() {
  String dnsName;
  if (wifiMeta.getString("mDNS")) dnsName = wifiMeta.getString("mDNS");
  else dnsName = "cam";  //default Name if nothing is stored

  if (MDNS.begin(dnsName.c_str())) {
    Serial.print("mDNS hostet under http://");
    Serial.print(dnsName);
    Serial.println(".local");

  } else {
    while (1) {
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

  String cmdRaw;
  byte incoming[length];

  for (int i = 0; i < length; i++) {
    cmdRaw += (char)payload[i];
  }
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.print("[");
      Serial.print(client_num);
      Serial.println("] Disconnected!");
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(client_num);
        Serial.print("Client [");
        Serial.print(client_num);
        Serial.print("] connecting with ");
        Serial.println(ip.toString());
      }
      break;
    case WStype_TEXT:

      Serial.print("\n[");
      Serial.print(client_num);
      Serial.print("] ");
      Serial.print(cmdRaw);
      for (int i = 0; i < length; i++) {
        incoming[i] = payload[i];
      }
      Serial.print("\n");
      respond(incoming, length, client_num);
      break;

    // For everything else: do nothing
    case WStype_ERROR:
      Serial.println("Unknown ERROR");
      break;
    default:
      Serial.print("Unknown WS_TYPE: ");
      Serial.println(String(type));
      break;
  }
}

//Callback send homepage
void onIndexRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.print("[");
  Serial.print(String(remote_ip));
  Serial.print("] HTTP GET request of ");
  Serial.println(request->url());
  request->send(200, "text/html", webpageCode);
}

//Callback not found
void onPageNotFound(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.print("[");
  Serial.print(String(remote_ip));
  Serial.print("] HTTP GET request of ");
  Serial.println(request->url());
  request->send(404, "text/plan", "Page not found!\n    404");
}

//Respond function
/*Protocol cmdset:
  0: recieve available wifi networks
  1: store ssid and pass
  2: clear EEPROM
  3: reboot ESP
  4: rotate cam
  5: get stepper position
  6: send Wifi mDNS and static ip
  7: get fav pos coords
*/
void respond(byte *payload, int length, uint8_t client_num) {
  char cmd = payload[0];
  String data;
  for (int i = 2; i < length; i++) {  //cmd+":" --> [0, 1, data, data, ...]
    data += char(payload[i]);
  }
  Serial.print("payload: ");
  Serial.println(data);

  //Rework data Extraction!
  String response;
  String px;
  String py;
  String vel;
  String wsSSID;
  String wsPASS;
  String ip;
  String mDNS;
  int count = 0;
  int index;

  Serial.print("Command: ");
  Serial.print(cmd);
  switch (cmd) {
    case '0':  //Return wifi Signals in Range
      Serial.println(" (return wifi signals)");
      response = "0:";
      response += scanWifi();
      webSocket.sendTXT(client_num, response);
      //Serial.println(response);
      break;

    case '1':  //Store incoming SSID and password
      {
        Serial.println(" (Store ssid and password)");
        storeCreds(data);
        webSocket.sendTXT(client_num, "1:credentials_stored");
        //Serial.print("SSID: "); Serial.print(wsSSID); Serial.print(", pass: "); Serial.println(wsPASS);
      }
      break;

    case '2':  //clear stored wifi credentials
      Serial.println(" (clear stored wifi credentials)");
      credentials.clear();
      credentials.putUInt("counter", 0);
      response = "2:credentials_cleared";
      webSocket.sendTXT(client_num, response);
      break;

    case '3':
      Serial.println(" (Restart ESP)");
      webSocket.sendTXT(client_num, "3:Restarting");
      ESP.restart();
      break;

    case '4':
      //4:posX,posY,Speed
      stamp = millis();
      Serial.println(" (rotate camera)");
      py = splitString(data, ',', 0);
      px = splitString(data, ',', 1);
      vel = splitString(data, ',', 2);


      gimbal.rotateTo(py.toFloat(), px.toFloat(), vel.toFloat());
      response = "4:";
      response += stepperOne.getAbsoluteAngle();  //get vertical axis Value (x)
      response += ",";
      response += stepperTwo.getAbsoluteAngle();  //get horizontal axis Value (y)
      webSocket.broadcastTXT(response);

      break;
    case '5':
      Serial.println(" (return stepper positions)");
      response = "5:";
      response += stepperTwo.getAbsoluteAngle();  //get horizontal axis Value (x)
      response += ",";
      response += stepperOne.getAbsoluteAngle();  //get vertical axis Value (y)
      webSocket.sendTXT(client_num, response);
      break;
    case '6':
      Serial.println(" (store static IP and mDNS name)");
      storeWifiMeta(data);
      response = "6:wifiMeta_stored";
      webSocket.sendTXT(client_num, response);
      break;
    case '7':
      break;
    default:  //command could not be recognized
      Serial.println("(Unknown command!)");
      webSocket.sendTXT(client_num, "100:Unkown Command");
      break;
  }
}

//EEPROM store functions
bool storeCreds(String payload) {  //Format: ssid,pass
  String ssid = splitString(payload, ',', 0);
  String pass = splitString(payload, ',', 1);

  credentials.putString("ssid", ssid.c_str());
  credentials.putString("pass", pass.c_str());
  int temp = credentials.getUInt("counter") + 1;
  credentials.putUInt("counter", temp);
}

bool storePosition(String payload) {
  String name = splitString(payload, ',', 0);
  String posX = splitString(payload, ',', 1);
  String posY = splitString(payload, ',', 2);

  String val = String(posX);
  val += ":";
  val += String(posY);
  favPos.putString(name.c_str(), val);
}

bool storeWifiMeta(String payload) {  //webhost checks IP validity (javascript Regex)
  String staticIP = splitString(payload, ',', 0);
  String mDNS = splitString(payload, ',', 1);

  wifiMeta.putString("staticIP", staticIP);
  wifiMeta.putString("mDNS", mDNS);
}

void showEEPROM() {
  //show wifi credentials
  Serial.println("------ NVS ------");
  credentialCounter = credentials.getUInt("counter", 0);
  if (credentialCounter > 0) {
    Serial.print("Found ");
    Serial.print(credentialCounter);
    Serial.println(" stored WiFi Credentials:");
    String eepromSSID = credentials.getString("ssid");
    String eepromPASS = credentials.getString("pass");
    Serial.print("SSID: ");
    Serial.println(eepromSSID);
    Serial.print("Pass: ");
    Serial.println(eepromPASS);
  } else {
    Serial.print("Found ");
    Serial.print(credentialCounter);
    Serial.println(" stored WiFi Credentials");
  }

  //show wifi Metadata
  String staticIP = wifiMeta.getString("staticIP");
  String mDNS = wifiMeta.getString("mDNS");

  if (staticIP != NULL || mDNS != NULL) {
    Serial.println("Found WiFi metadata: ");
    if (staticIP != NULL) {
      Serial.print("StaticIP: ");
      Serial.println(staticIP);
    }
    if (mDNS != NULL) {
      Serial.print("mDNS: ");
      Serial.println(mDNS);
    }
  }

  //list saved Positions
  Serial.println("-----------------");
}

void stepperTimeoutCheck() {
  if (stamp + stepperTimeout <= millis()) {
    stepperOne.disable();
    stepperTwo.disable();
  }
}

String splitString(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}



bool staticIPSetup() {


  // Default
  IPAddress gateway;

  IPAddress subnet;
  IPAddress primaryDNS;    // optional
  IPAddress secondaryDNS;  // optional

  String eepromSSID = credentials.getString("ssid");
  String eepromPASS = credentials.getString("pass");

  Serial.println("Fetching WiFi data to setup static IP...");
  delay(50);


  //Serial.print("Subnet Mask: ");
  subnet = WiFi.subnetMask();
  //Serial.println(subnet);

  //Serial.print("Gateway IP: ");
  gateway = WiFi.gatewayIP();
  //Serial.println(gateway);

  //Serial.print("DNS 1: ");
  primaryDNS = WiFi.dnsIP(0);
  //Serial.println(primaryDNS);

  //Serial.print("DNS 2: ");
  secondaryDNS = WiFi.dnsIP(2);
  //Serial.println(secondaryDNS);


  //construc staticIP from EEPROM
  String IP = wifiMeta.getString("staticIP");
  int ip_number[4];

  for (int i = 0; i < 4; i++) {
    String temp = splitString(IP, '.', i);
    ip_number[i] = temp.toInt();
  }
  IPAddress local_IP(ip_number[0], ip_number[1], ip_number[2], ip_number[3]);

  //Restart WiFi with static ip
  WiFi.disconnect();
  delay(200);
  WiFi.mode(WIFI_STA);
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  delay(500);
  WiFi.begin(eepromSSID.c_str(), eepromPASS.c_str());  //conenct to WiFi with static IP
  while (WiFi.status() != WL_CONNECTED) {


    //Serial.println(WiFi.status());
  }

  return true;
}

//connecting to wifi with timeout
void tryWifi() {

  Serial.println("Connecting...");
  String eepromSSID;
  String eepromPASS;
  int wifiTimeoutLimit = 10;  //timeout in seconds
  for (int i = 0; i < credentialCounter; i++) {

    if (WiFi.status() != WL_CONNECTED) {
      eepromSSID = credentials.getString("ssid");
      eepromPASS = credentials.getString("pass");
      WiFi.begin(eepromSSID.c_str(), eepromPASS.c_str());  //start with dynamic IP

      int timeStamp = millis() + wifiTimeoutLimit * 1000;
      while ((WiFi.status() != WL_CONNECTED) && (timeStamp >= millis())) {
        // Serial.print(WiFi.status()); Serial.print(": "); Serial.print(timeStamp); Serial.print(" >= "); Serial.println(millis());
        //Serial.println(WiFi.status());//Serial.print(" .");
      }
    }

    //set up static IP
    staticIPSetup();
  }
}

//----------------------------------- µ-Controller loop -----------------------------------
void setup() {
  Serial.begin(115200);

  //Setup EEPROM | creating namespaces if they dont exist
  //Create Wifi Namespace
  credentials.begin("Credentials", false);
  //Create Wifi Meta Namespace (static ip and mDNS name)
  wifiMeta.begin("wifiMeta", false);
  //Create position Namespace
  favPos.begin("FavPositions", false);
  showEEPROM();
  if (credentialCounter > 0) {
    tryWifi();
  }

  //if there is no working wifi start AP
  if (WiFi.status() != WL_CONNECTED) {
    if (!WiFi.softAP(ssid, password)) {
      while (true) {
        if ((millis() / 1000) % 2 == 0) Serial.println("SoftAP failiure!");
      }
    }
    delay(2000);
    Serial.print("AP running at ");
    Serial.println(WiFi.softAPIP());
  } else {
    Serial.print("ESP connected to ");
    Serial.print(WiFi.SSID());
    Serial.print(" running on ");
    Serial.println(WiFi.localIP());
  }

  //setting up DNS Name to connect
  initmDNS();

  Serial.println("Webhost ready");

  //There must be a WiFi connection beyond this point (AP or Router)
  server.on("/", HTTP_GET, onIndexRequest);
  server.onNotFound(onPageNotFound);

  server.begin();
  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);
  Serial.println("------------ Protocol Log Start ------------");
}



void loop() {
  webSocket.loop();
  stepperTimeoutCheck();
}