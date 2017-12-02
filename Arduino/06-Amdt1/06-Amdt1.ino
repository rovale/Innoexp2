#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Secrets.h>

const char mqttClientId[] =     "Innoexp2Amdt1";

const char commandTopic[] =     "evs/innoexp2/amdt1/command";
const char movementTopic[] =    "evs/innoexp2/amdt1/sensor/movement";

const char statusTopic[] =      "evs/innoexp2/amdt1/status";
const char uptimeTopic[] =      "evs/innoexp2/amdt1/uptime";
const char rssiTopic[] =        "evs/innoexp2/amdt1/rssi";

WiFiClient wiFiClient;
PubSubClient client(wiFiClient);

int lastMovement = LOW;
const int movementPin = D3;
unsigned long lastReconnectAttemptAt = 0;
unsigned long lastSystemMessageAt = 0;
unsigned long lastMovementMessageAt = 0;
unsigned long disconnects = 0;
unsigned long movementCount;

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

void publish(const char* topic, String message, boolean retained){
    Serial.print(topic);
    Serial.print(": ");
    Serial.println(message);

    char messageCharArray[message.length() + 1];
    message.toCharArray(messageCharArray, message.length() + 1);
    
    client.publish(topic, messageCharArray, retained);    
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
    if (commandName == "doSomething") {
      doSomething();
    }
    else if (commandName == "doSomethingElse") {
      doSomethingElse();
    }
    else {
      Serial.println("Unknown command.");
    }
  }
  else {
    Serial.println("Unable to parse payload.");  
  }
}

void doSomething(){
}

void doSomethingElse(){
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
  pinMode(movementPin, INPUT);
  Serial.begin(115200);
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

    if (currentMillis - lastSystemMessageAt >= 60000) {
      lastSystemMessageAt = currentMillis;
      publish(uptimeTopic, String(currentMillis / 1000), true);
  
      int rssi = WiFi.RSSI();
      publish(rssiTopic, String(rssi), true);
    }

    byte currentMovement = digitalRead(movementPin);
    if (currentMovement != lastMovement) {
      movementCount++;
      lastMovement = currentMovement;
    }

    if (currentMillis - lastMovementMessageAt >= 2500) {
      lastMovementMessageAt = currentMillis;
      publish(movementTopic, String(movementCount), false);
      movementCount = 0;
    }    
  }  
}
