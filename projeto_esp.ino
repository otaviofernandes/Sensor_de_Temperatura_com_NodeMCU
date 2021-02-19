#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <max6675.h>

//-----------------------------------------------------
LiquidCrystal_I2C lcd(0x27, 16, 2);
//-----------------------------------------------------

int thermoDO = D4;
int thermoCS = D5;
int thermoCLK = D6;
int led_on = D7;
int led_off = D8;
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);


const char* ssid = "***********"; // insiria aqui o nome da rede wifi.
const char* password = "*******"; // insiria aqui a senha de rede.
int od = 0;
int ct = 10;

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

 pinMode(led_on, OUTPUT);
 pinMode(led_off, OUTPUT);
 
 //-----------------------------------------------------
 Wire.begin(D2, D1);
 lcd.init();
 lcd.setBacklight(HIGH);
 lcd.setCursor(0,0);
 lcd.print("Conectando......");
 lcd.setCursor(0,1);
 lcd.print(ssid);
 delay(2000);
 //-----------------------------------------------------

  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (ct > 0){
  Serial.print("Conectando ->");
  delay(500);
  ct = ct - 1;
  Serial.println(ct);
  }
  if (WiFi.status() == WL_CONNECTED){
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
}

void loop(void){
  if(WiFi.status() == WL_CONNECTED){ 
  Serial.println("ONLINE");
  digitalWrite(led_on, HIGH);
  delay(100);
  digitalWrite(led_on, LOW);
  server.handleClient();
  
 //-----------------------------------------------------
  lcd.setBacklight(HIGH);
  lcd.setCursor(0,0);
  lcd.print((WiFi.localIP()));
  lcd.setCursor(0,1);
  String rc = String(thermocouple.readCelsius());
  String rf = String(thermocouple.readFahrenheit());
  lcd.print("Temp:  " + rc + "  C");
  delay(1000);
  lcd.setCursor(0,0);
  lcd.print((WiFi.localIP()));
  lcd.setCursor(0,1);
  lcd.print("Temp:  " + rf + "  F");
  delay(900);
 //-----------------------------------------------------

  }
  else{
  Serial.println("OFFLINE");
  digitalWrite(led_off, HIGH);
  delay(100);
  digitalWrite(led_off, LOW);
  
 //-----------------------------------------------------
  lcd.setBacklight(HIGH);
  lcd.setCursor(0,0);
  lcd.print("----OFF-LINE----");
  lcd.setCursor(0,1);
  String rc = String(thermocouple.readCelsius());
  String rf = String(thermocouple.readFahrenheit());
  lcd.print("Temp:  " + rc + "  C");
  delay(1000);
  lcd.setCursor(0,0);
  lcd.print("----OFF-LINE----");
  lcd.setCursor(0,1);
  lcd.print("Temp:  " + rf + "  F");
  delay(900);
 //-----------------------------------------------------

  }
  od = od + 2;
}
