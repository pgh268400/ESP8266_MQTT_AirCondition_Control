//DHT_HUMIDITY
#include "DHTesp.h"
DHTesp dht;
String packet;
unsigned long lastSend = 0;
//OTA_SETTING
 
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
 
#ifndef STASSID
#define STASSID "SSID"
#define STAPSK  "PW"
#endif
 
//const char* ssid = STASSID;
//const char* password = STAPSK;
 
 
 
//IR LED
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
 
const uint16_t kIrLed = D2;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).
IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message
 
/////
 
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
 
// MQTT 설정
 
const char* ssid = "SSID";
const char* password = "PW";
const char* mqtt_server = "Server";
const char* mqtt_topic = "UR_TOPIC";
 
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;
 
////////////////////////////////////
 
void setup_wifi() {
 
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  randomSeed(micros());
 
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
 
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
   String msg = "";
  for (int i = 0; i < length; i++) {
    msg +=(char)payload[i];
  }
  Serial.println(msg);
 
 
  if (msg == "power_on") {
  char turn_on[] = "8166817E";   
  unsigned long turn_on_v = strtoul(turn_on, NULL, 16); 
  irsend.sendNEC(turn_on_v);
  Serial.println("send_ok");
  }
 
  
  if (msg == "power_off") {
  char turn_on[] = "8166817E";   
  unsigned long turn_on_v = strtoul(turn_on, NULL, 16); 
  irsend.sendNEC(turn_on_v);
  Serial.println("send_ok");
  }
 
  if (msg == "cold") {
  char turn_on[] = "816619E6";   
  unsigned long turn_on_v = strtoul(turn_on, NULL, 16); 
  irsend.sendNEC(turn_on_v);
  Serial.println("send_ok");
  }
 
    if (msg == "hot") {
  char turn_on[] = "8166E916";   
  unsigned long turn_on_v = strtoul(turn_on, NULL, 16); 
  irsend.sendNEC(turn_on_v);
  Serial.println("send_ok");
  }
  
   if (msg == "reserve_on") {
  char turn_on[] = "8166F906";   
  unsigned long turn_on_v = strtoul(turn_on, NULL, 16); 
  irsend.sendNEC(turn_on_v);
  Serial.println("send_ok");
  }
 
     if (msg == "reserve_off") {
  char turn_on[] = "8166F906";   
  unsigned long turn_on_v = strtoul(turn_on, NULL, 16); 
  irsend.sendNEC(turn_on_v);
  Serial.println("send_ok");
  }
 
  
     if (msg == "dry_on") {
  char turn_on[] = "8166D926";   
  unsigned long turn_on_v = strtoul(turn_on, NULL, 16); 
  irsend.sendNEC(turn_on_v);
  Serial.println("send_ok");
  }
  
       if (msg == "dry_off") {
  char turn_on[] = "8166D926";   
  unsigned long turn_on_v = strtoul(turn_on, NULL, 16); 
  irsend.sendNEC(turn_on_v);
  Serial.println("send_ok");
  }
  
 
}
 
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe(mqtt_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
 
void setup() {
    irsend.begin();
    dht.setup(D5, DHTesp::DHT22); // D5
// OTA SETUP ////////////////////////////////////////
    Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
 
  // Port defaults to 8266
  ArduinoOTA.setPort(8266);
 
  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("myesp8266");
 
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }
 
    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
 
  //SERIAL BAUDRATE 
 
  /*
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  #if ESP8266
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
#else  // ESP8266
  Serial.begin(115200, SERIAL_8N1);
#endif  // ESP8266
*/
  Serial.begin(115200);
 
// MQTT SETUP 
 
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
 
}
 
void Get_th(){
    
  delay(dht.getMinimumSamplingPeriod());
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  packet = "온도 : " + String(temperature) + "*C \n" + "습도 : " + String(humidity) + "%" ; 
  
  client.publish("sensor/th",  (char*) packet.c_str());
  client.publish("sensor/t",   (char*) String(temperature).c_str());
  client.publish("sensor/h",  (char*) String(humidity).c_str());
}
 
void loop() {
  ArduinoOTA.handle();
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
 
    if ( millis() - lastSend > 1000 ) { // Update and send only after 1 seconds
    Get_th();
    lastSend = millis();
  }
 
  }
