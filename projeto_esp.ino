#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <max6675.h>

int thermoDO = D4;
int thermoCS = D5;
int thermoCLK = D6;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);


const char* ssid = "********"; // insiria aqui o nome da rede wifi.
const char* password = "**********"; // insiria aqui a senha de rede.
int od = 0;

ESP8266WebServer server(80);

void handleRoot() {
  String result = String(thermocouple.readCelsius());
  String ordem = String(od);
  server.send(200, "text/plain", ordem + " " + result);
}

void handleNotFound(){
  String message = "File Not Found\n";
  server.send(404, "text/plain", message);
}

void setup(void){
  
  Serial.println("MAX6675 test");
  // wait for MAX chip to stabilize
  delay(500);
  
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
  delay(5000);
  float r = (thermocouple.readCelsius());
  od = od + 5;
 Serial.print(String(od) + " ");
Serial.print(r);
Serial.print(" Celsius");
Serial.println();
}