#define MAIN
#define INTERVAL  10000 // How often to publish sensor data (ms)
#define TEMPDEBUG 0 // Used to print debug info for "lampotila_sensori" function
#define MQTT_PROT_DEBUG 1 //Misc. prints relating to MQTT data transfer
#define MQTTPUBDEBUG 1 // Prints about puplished messages

// Used ready made libraries
#include "DHT.h" // Temp.data sensor
#include <ESP8266WiFi.h> // NodeMCu
#include <Ticker.h>
#include <AsyncMqttClient.h> // MQTT client

// Init for temp/hum sensor
#define DHTPIN 14  // The pin that temperature sensor uses for data transfer
#define DHTTYPE DHT11   // Type of sensor used for temp. and hum. measurements  
DHT dht(DHTPIN, DHTTYPE);

// Wifi credentials, TODO implement "get_wifi_credentials.h"
#define WIFI_SSID "lassila"
#define WIFI_PASSWORD "56310460"

// MQTT Broker connect info
#define MQTT_IP IPAddress(192, 168, 1, 236)
#define MQTT_PORT 1883

// MQTT Topics where the temperature data will be submited to, TODO rethink publish structure, maybe good enuf?
#define MQTT_PUB_TEMP "esp/temp"
#define MQTT_PUB_HUM "esp/hum"

// Variables to hold sensor readings,TODO fix to not use global variables, bad practice
float temp;
float hum;

unsigned long previousMillis = 0;   // Stores last time temperature was published

AsyncMqttClient mqttObj;
Ticker mqttReconnectTimer; // Timer to determine how often to try to reconnect mqtt incase of disconnect
Ticker wifiReconnectTimer; // Timer to determine how often to try to reconnect wifi incase of disconnect

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;

void setup() {
  Serial.begin(115200);
  Serial.println();
  dht.begin();
  
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
  // if the time between last sent message and current time is >= given interval publish a new message
  unsigned long currentMillis = millis(); 
  if (currentMillis - previousMillis >= INTERVAL) {
    previousMillis = currentMillis; // Store current time to time the next loop
    
    // New sensor readings
    do{
    tempMeasure(temp,hum);
    }while(temp <= -273 || temp >= 273);
    
    // Publish an MQTT message on topic esp/temp
    uint16_t packetIdPub1 = mqttObj.publish(MQTT_PUB_TEMP, 1, true, String(temp).c_str());
    #if MQTTPUBDEBUG       
    Serial.printf("Publishing on topic %s at QoS 1, packetId: %i ", MQTT_PUB_TEMP, packetIdPub1);
    Serial.printf("Message: %.2f \n", temp);
    #endif

    // UNCOMMENT INCASE HUMIDITY DATA IS NEEDED
    // Publish an MQTT message on topic esp/hum
    uint16_t packetIdPub2 = mqttObj.publish(MQTT_PUB_HUM, 1, true, String(hum).c_str());
    #if MQTTPUBDEBUG                            
    Serial.printf("Publishing on topic %s at QoS 1, packetId %i: ", MQTT_PUB_HUM, packetIdPub2);
    Serial.printf("Message: %.2f \n", hum);
    #endif
  }
}




void connectToWifi() {
  #if MQTT_PROT_DEBUG
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  #endif
}

void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  #if MQTT_PROT_DEBUG
  Serial.println("Connected to Wi-Fi.");
  #endif
  connectToMqtt();
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  #if MQTT_PROT_DEBUG
  Serial.println("Disconnected from Wi-Fi.");
  #endif
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
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  #if MQTT_PROT_DEBUG
  Serial.println("Disconnected from MQTT.");
  #endif
  if (WiFi.isConnected()) {
    mqttReconnectTimer.once(2, connectToMqtt);
  }
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
