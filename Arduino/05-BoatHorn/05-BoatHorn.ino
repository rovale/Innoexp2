#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Secrets.h>

const char mqttClientId[] =     "Innoexp2Esp8266Rovale";

const char commandTopic[] =     "evs/innoexp2/esp8266_rovale/command";
const char horningTopic[] =     "evs/innoexp2/esp8266_rovale/horning";

const char statusTopic[] =      "evs/innoexp2/esp8266_rovale/status";
const char uptimeTopic[] =      "evs/innoexp2/esp8266_rovale/uptime";
const char rssiTopic[] =        "evs/innoexp2/esp8266_rovale/rssi";

WiFiClient wiFiClient;
PubSubClient client(wiFiClient);

unsigned long lastReconnectAttemptAt = 0;
unsigned long lastHealthMessageAt = 0;
unsigned long disconnects = 0;

void connectToNetwork() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.print(ssid);

  WiFi.begin(ssid, wiFiPassword);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("connected at address ");
  Serial.println(WiFi.localIP());
}

void publish(const char* topic, String message){
    Serial.print(topic);
    Serial.print(": ");
    Serial.println(message);

    char messageCharArray[message.length() + 1];
    message.toCharArray(messageCharArray, message.length() + 1);
    
    client.publish(topic, messageCharArray, true);    
}

void onReceive(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  DynamicJsonBuffer jsonBuffer(1024);
  JsonObject& root = jsonBuffer.parseObject(payload);
  if (root.success()) {
    String commandName = root["name"];
    Serial.println(commandName);
    if (commandName == "turnBoatHornOn") {
      turnBoatHornOn();
    }
    else if (commandName == "turnBoatHornOff") {
      turnBoatHornOff();
    }
    else {
      Serial.println("Unknown command.");
    }
  }
  else {
    Serial.println("Unable to parse payload.");  
  }
}

void turnBoatHornOn(){
  digitalWrite(2, HIGH);
  publish(horningTopic, "true");
}

void turnBoatHornOff(){
  digitalWrite(2, LOW);
  publish(horningTopic, "false");
}

boolean connectToMqqtBroker() {
  Serial.print("Connecting to MQTT broker...");
  //if (client.connect(mqttClientId, mqttUsername, mqttPassword, statusTopic, 1, true, "offline")) {
  if (client.connect(mqttClientId, statusTopic, 1, true, "offline")) {
    Serial.println("connected");
    client.publish(statusTopic, "online", true);
    client.subscribe(commandTopic, 1);
  } else {
    Serial.print("failed, rc=");
    Serial.println(client.state());
  }

  return client.connected();
}

void setup() {
  pinMode(2, OUTPUT);
  Serial.begin(9600);
  connectToNetwork();
  client.setServer(mqttServer, 1883);
  client.setCallback(onReceive);
}

void loop() {
  unsigned long currentMillis = millis();

  if (!client.connected()) {
    if (currentMillis - lastReconnectAttemptAt >= 5000) {
      lastReconnectAttemptAt = currentMillis;

      if (connectToMqqtBroker()) {
        lastReconnectAttemptAt = 0;
      }     
    }
  } else {
    client.loop();

    if (currentMillis - lastHealthMessageAt >= 60000) {
      lastHealthMessageAt = currentMillis;
      publish(uptimeTopic, String(currentMillis / 1000));
  
      int rssi = WiFi.RSSI();
      publish(rssiTopic, String(rssi));
    }
  }  
}
