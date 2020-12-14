
#include <ESP8266WiFi.h> // NodeMCu
#include <Ticker.h>
#include <AsyncMqttClient.h> // MQTT client

// Wifi credentials 
#define WIFI_SSID "lassila"
#define WIFI_PASSWORD "56310460"

// MQTT Broker connect info
#define MQTT_IP IPAddress(192, 168, 1, 236)
#define MQTT_PORT 1883

// MQTT Topics where the test data will be submited to
#define MQTT_PUB_TEMP "esp/testaus"


// Variables to hold variables,TODO fix to not use global variables, bad practice
bool mqttconnected = 0;
bool wificonnected = 0;
bool messagepub = 0;
int counter = 0;

AsyncMqttClient mqttObj;
Ticker mqttReconnectTimer; // Timer to determine how often to try to reconnect mqtt incase of disconnect
Ticker wifiReconnectTimer; // Timer to determine how often to try to reconnect wifi incase of disconnect

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;

void setup() {
  Serial.begin(115200);
  Serial.println();
  mqttconnected = 0;
  wificonnected = 0;
  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);
  
  mqttObj.onConnect(onMqttConnect);
  mqttObj.onDisconnect(onMqttDisconnect);
  mqttObj.onSubscribe(onMqttSubscribe);
  mqttObj.onPublish(onMqttPublish);
  mqttObj.setServer(MQTT_IP, MQTT_PORT);
  
  connectToWifi();
}

void loop() {
  while(mqttconnected == 1 && wificonnected == 1){
    
    // Publish an MQTT message on topic esp/testaus
    uint16_t packetIdPub1 = mqttObj.publish(MQTT_PUB_TEMP, 1, true, String(counter).c_str());
    #if MQTTPUBDEBUG       
    Serial.printf("Publishing on topic %s at QoS 1, packetId: %i ", MQTT_PUB_TEMP, packetIdPub1);
    Serial.printf("Message: %.2f \n", counter);
    #endif
    delay(500);
    counter = counter + 1;
  }
}




void connectToWifi() {
  #if MQTT_PROT_DEBUG
  Serial.println("Connecting to Wi-Fi...");
  #endif
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  #if MQTT_PROT_DEBUG
  Serial.println("Connected to Wi-Fi.");
  #endif
  wificonnected = 1;
  connectToMqtt();
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  #if MQTT_PROT_DEBUG
  Serial.println("Disconnected from Wi-Fi.");
  #endif
  wificonnected = 0;
  mqttReconnectTimer.detach(); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
  wifiReconnectTimer.once(2, connectToWifi);
}

void connectToMqtt() {
  #if MQTT_PROT_DEBUG
  Serial.println("Connecting to MQTT...");
  #endif
  mqttObj.connect();
}

void onMqttConnect(bool sessionPresent) {
  #if MQTT_PROT_DEBUG
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
  #endif
  mqttconnected = 1;
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  #if MQTT_PROT_DEBUG
  Serial.println("Disconnected from MQTT.");
  #endif
  if (WiFi.isConnected()) {
    mqttReconnectTimer.once(2, connectToMqtt);
  }
  mqttconnected = 0;
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
  #if MQTT_PROT_DEBUG
  Serial.println("Subscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
  Serial.print("  qos: ");
  Serial.println(qos);
  #endif
}

void onMqttPublish(uint16_t packetId) {
  #if MQTT_PROT_DEBUG
  Serial.print("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
  #endif
}
