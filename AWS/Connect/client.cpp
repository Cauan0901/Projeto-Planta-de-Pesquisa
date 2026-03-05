#include "../Connect/client.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

// Definição real dos objetos
WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);