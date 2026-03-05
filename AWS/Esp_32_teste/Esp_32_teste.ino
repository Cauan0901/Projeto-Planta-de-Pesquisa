//Hariharnath Paduchuru Working Demo
#include "secrets.h"

#define AWS_IOT_PUBLISH_TOPIC   "Esp_32_teste/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "Esp_32_teste/sub"

#include "../Connect/connectAWS.h"

//Dentro do connect AWS estão basucamente todas as libraries de wi-fi necessárias e a de pubsub

int numb = 0;
 
void publishMessage()
{
  StaticJsonDocument<200> doc;
  doc["number"] = numb;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client
 
  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}
 
void messageHandler(char* topic, byte* payload, unsigned int length)
{
  Serial.print("incoming: ");
  Serial.println(topic);
 
  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
  const char* message = doc["message"];
  Serial.println(message);
}

void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to WiFi");
  WiFi.begin("Virus", "proverbios321", 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println(" Connected!");
  connectAWS();
}

void loop() {
  numb++;
  publishMessage();
  client.loop();
  delay(5000);

}