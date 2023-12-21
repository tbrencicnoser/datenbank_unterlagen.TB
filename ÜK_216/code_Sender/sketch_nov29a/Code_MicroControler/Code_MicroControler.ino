//------------------------------------------------------------------------------------------
#include <WiFi.h>
#include <PubSubClient.h>
#define LIGHTSENSORPIN 32  //Ambient light sensor reading
const char* device_id = "BifkastenTAK";
const char* ssid = "GuestWLANPortal";  //noser
//const char* ssid = "Tomas's iPhone";  //hotspot
const char* mqtt_server = "noseryoung.ddns.net";
const char* topic1 = "zuerich/briefkasten/in";
WiFiClient espClient;
PubSubClient client(espClient);
void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1983);
  pinMode(LIGHTSENSORPIN, INPUT);
}
//Noser Welan:

void setup_wifi() {
  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("done!");
}

// Hotspot Tomas:
/*
void setup_wifi() {
  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid, "passwort"); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("done!");
}*/

void reconnect() {
  Serial.print("Attempting MQTT connection...");
  while (!client.connected()) {
    if (client.connect(device_id)) {
      Serial.println("done!");
      client.subscribe(topic1);
    } else {
      delay(500);
      Serial.print(".");
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  float reading = analogRead(LIGHTSENSORPIN);  //Read light level
  Serial.println(reading);

  char tempBuffer[16];
  sprintf(tempBuffer, "%f", reading);
  client.publish(topic1, tempBuffer);


  delay(1000);
}
