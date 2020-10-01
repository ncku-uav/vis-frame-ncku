#include <Arduino.h>
#include <ArduinoJson.h>

unsigned long last = 0UL;
int16_t counter = 0;

void JSON_test();
void push_JSON();

DynamicJsonBuffer jsonBuffer;

JsonObject &root = jsonBuffer.createObject();
JsonObject &MQTT = jsonBuffer.createObject();
JsonObject &GPS_j = jsonBuffer.createObject();
JsonObject &Airspeed_j = jsonBuffer.createObject();
JsonObject &ACC_j = jsonBuffer.createObject();
JsonObject &Mag_j = jsonBuffer.createObject();

uint8_t something_updated = 0;

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  //while (mySerial.available() > 0)
  //gps.encode(mySerial.read());
  push_JSON();
}

void JSON_test()
{
}

void push_JSON()
{

  //MQTT onmessage
  // if topic == suchandsuch -> see Thibault telemetry code

  GPS_j["alt"] = counter;
  GPS_j["satcount"] = counter;

  Airspeed_j["calc"] = counter;
  Airspeed_j["tar"] = counter;

  MQTT["GPS"] = GPS_j;
  MQTT["Airspeed"] = Airspeed_j;

  ACC_j["X"] = counter;
  ACC_j["Y"] = counter;
  ACC_j["Z"] = counter;

  MQTT["ACC"] = ACC_j;

  Mag_j["X"] = counter;
  Mag_j["Y"] = counter;
  Mag_j["Z"] = counter;

  MQTT["Mag"] = Mag_j;
  something_updated = 1;

  root["MQTT"] = MQTT;

  if (something_updated == 1)
  {
    root.printTo(Serial);
    Serial.print("\n");
  }

  if (counter == 1000)
  {
    counter = 0;
  }

  counter = counter + 1;
}
