#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

const char ssid[] = "RaspberrypicoW";  // SSID
const char pass[] = "password";   // password

const IPAddress ip(192, 168, 1, 100);      // IPアドレス
const IPAddress subnet(255, 255, 255, 0); // サブネットマスク

WebServer server(80);

int Mode = 0;
String ledBt = "off";

void handleRoot() {
  String temp ="<!DOCTYPE html> \n<html lang=\"ja\">";
  temp +="<head>";
  temp +="<meta charset=\"utf-8\">";
  temp +="<title>picoW</title>";
  temp +="<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  temp +="<style>";
  temp +=".container{";
  temp +="  max-width: 500px;";
  temp +="  margin: auto;";
  temp +="  text-align: center;";
  temp +="  font-size: 1.2rem;";
  temp +="}";
  temp +="span,.pm{";
  temp +="  display: inline-block;";
  temp +="  border: 1px solid #ccc;";
  temp +="  width: 50px;";
  temp +="  height: 30px;";
  temp +="  vertical-align: middle;";
  temp +="  margin-bottom: 20px;";
  temp +="}";
  temp +="span{";
  temp +="  width: 120px;";
  temp +="}";
  temp +="button{";
  temp +="  width: 100px;";
  temp +="  height: 40px;";
  temp +="  font-weight: bold;";
  temp +="  margin-bottom: 20px;";
  temp +="}";
  temp +="button.on{ background:lime; color:white; }";
  temp +=".column-2{ max-width:250px; margin:auto; text-align:center; display:flex; justify-content:space-between; flex-wrap:wrap; }";
  temp +="</style>";
  temp +="</head>";
  
  temp +="<body>";
  temp +="<div class=\"container\">";
  temp +="<h3>picoW</h3>";
  
  //ボタン
  //temp +="<div class=\"column-2\">";
  temp +="<button class=\"" + ledBt + "\" type=\"button\" ><a href=\"/Led\">LED</a></button><br>";
  //temp +="</div>";
  
  temp +="</div>";
  temp +="</body>";
  server.send(200, "text/HTML", temp);
}

void Led() {
  if(ledBt == "off"){
    ledBt = "on";    
    Mode = 1;
  }else{
    ledBt = "off";
    Mode = 0;
  }
  handleRoot();
}

void setup(void) {
  pinMode(LED_BUILTIN, OUTPUT);  
  Serial.begin(115200);
  delay(50);
  
  WiFi.mode(WIFI_AP);
  
  WiFi.softAPConfig(ip, ip, subnet);
  WiFi.softAP(ssid, pass);
  delay(50);
  
  
  IPAddress myIP = WiFi.softAPIP();  // WiFi.softAPIP()でWiFi起動
  Serial.print("SoftAPのIPアドレス: ");
  Serial.println(myIP);

  server.on("/", handleRoot);
  server.on("/Led", Led);
  
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();

  if(Mode){
    digitalWrite(LED_BUILTIN, HIGH);
  }else{
    digitalWrite(LED_BUILTIN, LOW);
  }
}