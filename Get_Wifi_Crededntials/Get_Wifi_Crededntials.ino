#ifndef MAIN
#define DEBUG 1
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

String ssid = "testi_kn";
String password = "testi_ss";
int globaalimuuttuja = 0;

ESP8266WebServer server(80); //Server on port 80

void setup(void){
  #if DEBUG
  Serial.begin(115200);
  #endif
  getWifiCred();
}

void loop(void){
}


// What server does when connected
void handle_OnConnect_AP() {
 server.send(200, "text/html", MAIN_HTML()); //Send web page
}

// What happens when you press "OK"
void handle_OK() {
 ssid = server.arg("ssid"); 
 password = server.arg("password"); 
 globaalimuuttuja = 1;
 #if DEBUG
 Serial.print("Local ssid:");
 Serial.println(ssid);
 Serial.print("Local password:");
 Serial.println(password);
 #endif
 String s = "<a href='/'> Go Back </a>";
 server.send(200, "text/html", s); //Send web page to return
}

// What happens if page is not found
void handle_NotFound(){
  server.send(404, "text/plain", "Not found, too bad \n 404");
}
#endif



#define DEBUGCREDENTIALS 1
void getWifiCred(){
  //SSID and Password for the Access point
  char* ssidAP = "Mirror";
  char* passwordAP = "12345678";
  //IP Address details for the Access point
  IPAddress local_ip(192,168,1,1);
  IPAddress gateway(192,168,1,1);
  IPAddress subnet(255,255,255,0);

   // Set up the device as a soft access point
  WiFi.softAP(ssidAP, passwordAP);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  // Tell how to respond to different requests 
  server.on("/", handle_OnConnect_AP);
  server.on("/action_page", handle_OK);
  
  //Start server
  server.begin();
  #if DEBUG
  Serial.println("HTTP server started");
  Serial.print("SSID:");
  Serial.println(ssidAP);
  Serial.print("PASSWORD:");
  Serial.println(passwordAP);
  Serial.print("Local IP:");
  Serial.println(local_ip);
  #endif

  // Handle web client requests until the wifi credential have been acquired
  while(globaalimuuttuja == 0){
    server.handleClient(); 
  }
  
  #if DEBUG
  Serial.println("getWifiCred End");
  Serial.print(ssid);
  Serial.println(password);
  #endif
}

//The main wep page stored in a variable to make handling of the HTML code easier
String MAIN_HTML(){
  String sivu = "<!DOCTYPE html>";
  sivu += "<html>";
  sivu += "<body>";

  sivu += "<h2>Mirror<h2>";
  sivu += "<h3> Wifi credentials</h3>";

  sivu += "<form action=\"/action_page\">";
  sivu += "SSID:<br>";
  sivu += "<input type=\"text\" name=\"ssid\" value=\"Mokkula-1234\">";
  sivu += "<br>";
  sivu += "Password:<br>";
  sivu += "<input type=\"text\" name=\"password\" value=\"12345678\">";
  sivu += "<br><br>";
  sivu += "<input type=\"submit\" value=\"OK\">";
  sivu += "</form> ";

  sivu += "</body>";
  sivu += "</html>";
  return sivu;
}
