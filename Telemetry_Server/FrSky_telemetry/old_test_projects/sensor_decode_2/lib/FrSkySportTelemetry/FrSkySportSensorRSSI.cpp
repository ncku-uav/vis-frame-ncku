/*
  FrSky XJT module S.Port output decoder class for Teensy 3.x/LC and 328P based boards (e.g. Pro Mini, Nano, Uno)
  (c) Pawelsky 20160919
  Not for commercial use

  Note that this special sensor class can only be used for decoding.
*/

#include "FrSkySportSensorRSSI.h"

FrSkySportSensorRSSI::FrSkySportSensorRSSI(SensorId id) : FrSkySportSensor(id) { }

uint16_t FrSkySportSensorRSSI::decodeData(uint8_t id, uint16_t appId, uint32_t data)
{
  if((sensorId == id) || (sensorId == FrSkySportSensor::ID_IGNORE))
  {
    switch(appId)
    {
      case 0xF101:
        rssi = (uint8_t)data;
        return appId;
      case 0xF102:
        adc1 = ((uint8_t)data) * (3.3 / 255.0);
        return appId;
      case 0xF103:
        adc2 = ((uint8_t)data) * (3.3 / 255.0);
        return appId;
      case 0xF104:
        rxBatt = ((uint8_t)data) * (3.3 / 255.0) * 4.0;
        return appId;
      case 0xF105:
        swr = (uint8_t)data;
        return appId;
    }
  }
  return SENSOR_NO_DATA_ID;
}

uint8_t FrSkySportSensorRSSI::getRssi() { return rssi; }
float FrSkySportSensorRSSI::getAdc1() { return adc1; }
float FrSkySportSensorRSSI::getAdc2() { return adc2; }
float FrSkySportSensorRSSI::getRxBatt() { return rxBatt; }
uint8_t FrSkySportSensorRSSI::getSwr() { return swr; }
