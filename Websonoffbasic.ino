//Sonoff RF R2 POWER V1.0

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define LED 13 //GPIO-13
#define BUTTON 0//GPIO-0
#define RELAY  12 //GPIO-12

bool LEDstatus = HIGH;


#ifndef STASSID
#define STASSID "MISSDID"
#define STAPSK  "MIPASS"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);


void setup() {
  Serial.begin(9600);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED, OUTPUT);
  
  Serial.println("Inicio de la wifi......");
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
  server.on("/", handle_OnConnect);
  server.on("/on", []() {
    LEDstatus=LOW;
    server.send(200, "text/html", SendHTML(LEDstatus)); 
  });
  server.on("/off", []() {
    LEDstatus=HIGH;
    server.send(200, "text/html", SendHTML(LEDstatus)); 
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started"); 
 
}

// the loop function runs over and over again forever
void loop() {
  server.handleClient();
  if(LEDstatus)
    digitalWrite(LED, HIGH);
  else
    digitalWrite(LED, LOW);
}
void handle_OnConnect() {
  server.send(200, "text/html", SendHTML(LEDstatus)); 
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
String SendHTML(uint8_t onOff){
  String str = "<!DOCTYPE html>\n";
  str +="<html>\n";
  str +="<head>\n";
  str +="<title>LED Control</title>\n";
  str +="</head>\n";
  str +="<body>\n";
  str +="<h1>LED</h1>\n";
  str +="<p>Click on and off.</p>\n";
  str +="<form method=\"get\">\n";
  if(onOff)
    str +="<input type=\"button\" value=\"ON\" onclick=\"window.location.href='/on'\">\n";
  else
    str +="<input type=\"button\" value=\"OFF\" onclick=\"window.location.href='/off'\">\n";
  str +="</form>\n";
  str +="</body>\n";
  str +="</html>\n";
  return str;
}
