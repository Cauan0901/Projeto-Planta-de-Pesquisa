#ifndef MQTT_CONFIG_H
#define MQTT_CONFIG_H

#include <WiFiClientSecure.h>
#include <PubSubClient.h>

// O 'extern' diz: "Essas variáveis existem, mas serão criadas no arquivo .cpp"
extern WiFiClientSecure net;
extern PubSubClient client;


#endif