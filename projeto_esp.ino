#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <max6675.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int thermoDO = D4;
int thermoCS = D5;
int thermoCLK = D6;
int led_on = D7;
int led_off = D8;
int conexao = 0;
int cont = 0;
int od = 0;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);


const char* ssid = "**********"; // insira aqui o nome da rede wifi.
const char* password = "***********"; // insira aqui a senha de rede.


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
 
 
  Serial.begin(115200);
  //Serial.println("MAX6675 test");
  // wait for MAX chip to stabilize
  //delay(500);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  //Serial.println("");

  // Wait for connection
  while ((WiFi.status() != WL_CONNECTED) && (cont < 10)) {
    delay(500);
    Serial.print(".");
    cont ++;
  }
  
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


 Wire.begin(D1, D2);
 lcd.init();
 lcd.setBacklight(HIGH);
 lcd.setCursor(0,0);
 lcd.print("Conectando");
 lcd.setCursor(0,1);
 lcd.print(ssid);
 delay(2000);

if (WiFi.status() != WL_CONNECTED){
  conexao = 0;
  }
else{
  conexao = 1;
  }
}

void loop(void){
 lcd.setCursor(0,0);
 lcd.print("                ");
 lcd.setCursor(0,1);
 lcd.print("                ");
 
 if (conexao == 1){
 Serial.print("ON-Line");
 Serial.println("");
 digitalWrite(led_on, HIGH);
 delay(100);
 digitalWrite(led_on, LOW);
  
 server.handleClient();
 lcd.setCursor(0,0);
 lcd.print((WiFi.localIP()));
 lcd.setCursor(0,1);
 String rc = String(thermocouple.readCelsius());
 String rf = String(thermocouple.readFahrenheit());

 lcd.setCursor(0,1);
 lcd.print(rc + " C");
 delay(1000);
 lcd.setCursor(0,1);
 lcd.print(rf + " F");
 delay(900);
}

 else{
 Serial.print("Off-Line");
 Serial.println("");
 digitalWrite(led_off, HIGH);
 delay(100);
 digitalWrite(led_off, LOW);
 lcd.setCursor(0,0);
 lcd.print("OFF-LINE        ");
 String rc = String(thermocouple.readCelsius());
 String rf = String(thermocouple.readFahrenheit());
 lcd.setCursor(0,1);
 lcd.print(rc + " C");
 delay(1000);
 lcd.setCursor(0,1);
 lcd.print(rf + " F");
 delay(900);
 }
  od = od + 2;
}
