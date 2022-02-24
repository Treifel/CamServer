//AP - WebSocket
#include <WiFi.h>
#include "esp_wifi.h"
#include <WebServer.h>
#include "webpage.h"


/* Put your SSID & Password */
const char* ssid = "ESP32";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

/* Put IP Address details

  IPAddress local_ip(192,168,1,1);
  IPAddress gateway(192,168,1,1);
  IPAddress subnet(255,255,255,0);
*/

WebServer server(80);

uint8_t LED1pin = 27;
bool LED1status = LOW;

uint8_t LED2pin = 5;
bool LED2status = LOW;

String devices[10];
int currentDevice = 0; //0 default -> 0 ist the ESP this Programm is running on

void setup() {
  Serial.begin(115200);
  pinMode(LED1pin, OUTPUT);
  pinMode(LED2pin, OUTPUT);

  WiFi.softAP(ssid, password);
  //WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);

  server.on("/", handle_OnConnect);
  server.on("/dev", handle_led1on);
  server.on("/#dev/1", handle_devChange);
  server.on("/led2on", handle_led2on);
  server.on("/led2off", handle_led2off);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.print("Running on "); Serial.println(WiFi.softAPIP());
  Serial.println("HTTP server started");
}

wifi_sta_list_t device_list;
tcpip_adapter_sta_list_t adapter_list;

void loop() {
  server.handleClient();
  if (LED1status)
  {
    digitalWrite(LED1pin, HIGH);
  }
  else
  {
    digitalWrite(LED1pin, LOW);
  }

  if (LED2status)
  {
    digitalWrite(LED2pin, HIGH);
  }
  else
  {
    digitalWrite(LED2pin, LOW);
  }


  //updateDevices(devices, 4000);



}


void handle_devChange() {
  Serial.println("Device was changed");
}
void updateDevices(String * macAddressn, int checkInterval)
{
  static int timestamp = checkInterval;
  //list connected devices
  if (millis() - timestamp > checkInterval)
  {
    wifi_sta_list_t wifi_sta_list;
    tcpip_adapter_sta_list_t adapter_sta_list;

    memset(&wifi_sta_list, 0, sizeof(wifi_sta_list));
    memset(&adapter_sta_list, 0, sizeof(adapter_sta_list));

    esp_wifi_ap_get_sta_list(&wifi_sta_list);
    tcpip_adapter_get_sta_list(&wifi_sta_list, &adapter_sta_list);



    Serial.print("Currently "); Serial.print(adapter_sta_list.num); Serial.println(" devices connected");
    Serial.print("Time: "); Serial.println(timestamp / 100.0);

    if (adapter_sta_list.num > 0) Serial.println("\n\nInfo on Devices: ");
    for (int i = 0; i < adapter_sta_list.num; i++) {

      tcpip_adapter_sta_info_t station = adapter_sta_list.sta[i];

      Serial.print("station nr ");
      Serial.println(i);

      Serial.print("MAC: ");

      String mac = "";
      for (int i = 0; i < 6; i++) {

        mac += String(station.mac[i], HEX);
        if (i != 5) mac += ":";
      }
      Serial.print(mac);

      Serial.print("\nIP: ");
      Serial.println(ip4addr_ntoa(&(station.ip)));
      Serial.println("//--------------------//");
    }

    Serial.println("----------------------------");
    timestamp = millis();
  }
}







void handle_OnConnect() {
  LED1status = LOW;
  LED2status = LOW;
  Serial.println("GPIO4 Status: OFF | GPIO5 Status: OFF");
  server.send(200, "text/html", webpageCode);
  //server.send(200, "text/html", SendHTML(LED1status,LED2status));
}

void handle_led1on() {
  Serial.println("success");
}

void handle_led1off() {
  LED1status = LOW;
  Serial.println("GPIO4 Status: OFF");
  server.send(200, "text/html", SendHTML(false, LED2status));
}

void handle_led2on() {

  Serial.println("Communication with other esp");

}

void handle_led2off() {
  LED2status = LOW;
  Serial.println("GPIO5 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status, false));
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t led1stat, uint8_t led2stat) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>LED Control</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += ".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".button-on {background-color: #3498db;}\n";
  ptr += ".button-on:active {background-color: #2980b9;}\n";
  ptr += ".button-off {background-color: #34495e;}\n";
  ptr += ".button-off:active {background-color: #2c3e50;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>ESP32 Web Server</h1>\n";
  ptr += "<h3>Using Access Point(AP) Mode</h3>\n";

  if (led1stat)
  {
    ptr += "<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n";
  }
  else
  {
    ptr += "<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/led1on\">ON</a>\n";
  }

  if (led2stat)
  {
    ptr += "<p>LED2 Status: ON</p><a class=\"button button-off\" href=\"/led2off\">OFF</a>\n";
  }
  else
  {
    ptr += "<p>LED2 Status: OFF</p><a class=\"button button-on\" href=\"/led2on\">ON</a>\n";
  }

  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
