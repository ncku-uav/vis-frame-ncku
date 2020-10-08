#include <UIPEthernet.h>
#include "PubSubClient.h"
#include <ArduinoJson.h>

// Ethernet Setup
uint8_t mac[6] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
byte ip[] = {192, 168, 0, 254};
IPAddress server(192, 168, 0, 101);

//Ethernet And MQTT Client
EthernetClient ethClient;
PubSubClient mqttClient(ethClient);

//MQTT callback function
void callback(char *topic, byte *payload, unsigned int length)
{
  StaticJsonBuffer<175> jsonBuffer; //static buffer prooved to be more stable and is chose with some margin
  JsonObject &root = jsonBuffer.createObject();
  JsonObject &MQTT = root.createNestedObject("MQTT");

  //Built MQTT Message
  MQTT["Topic"] = topic;
  payload[length] = '\0';
  MQTT["Payload"] = (char *)payload;

  root.printTo(Serial);
  Serial.println();

}

void reconnect()
{
  // Solange wiederholen bis Verbindung wiederhergestellt ist
  while (!mqttClient.connected())
  {
    //Serial.print("Versuch des MQTT Verbindungsaufbaus...");

    //Verbindungsversuch:
    if (mqttClient.connect("arduinoClient"))
    {
      Serial.println("MQTT erfolgreich verbunden!");

      // und meldet sich bei inTopic für eingehende Nachrichten an:
      mqttClient.subscribe("data/#");
    }
    else
    { // Im Fehlerfall => Fehlermeldung und neuer Versuch
      Serial.println("MQTT Verbindungsfehler, erneuter Versuch in 5s...");
      //Serial.print(mqttClient.state());
      //Serial.println(" Nächster Versuch in 5 Sekunden");
      // 5 Sekunden Pause vor dem nächsten Versuch
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(115200);

  mqttClient.setServer(server, 1883); // Adresse des MQTT-Brokers
  mqttClient.setCallback(callback);   // Handler für eingehende Nachrichten
  // Ethernet-Verbindung aufbauen
  Ethernet.begin(mac, ip);
  // Kleine Pause um Ethernetverbindung aufzubauen
  delay(3000);
}

void loop()
{
  // Solange probieren bis es klappt:
  if (!mqttClient.connected())
  {
    reconnect();
  }
  mqttClient.loop();
}
