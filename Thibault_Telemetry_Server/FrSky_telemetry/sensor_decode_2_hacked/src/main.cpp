// Uncomment the #define below to enable internal polling of data.
// Use only when there is no device in the S.Port chain (e.g. S.Port capable FrSky receiver) that normally polls the data.
//#define POLLING_ENABLED

#include "FrSkySportSensor.h"
#include "FrSkySportSensorAss.h"
#include "FrSkySportSensorFcs.h"
#include "FrSkySportSensorFlvss.h"
#include "FrSkySportSensorGps.h"
#include "FrSkySportSensorRpm.h"
#include "FrSkySportSensorSp2uart.h"
#include "FrSkySportSensorVario.h"
#include "FrSkySportSingleWireSerial.h"
#include "FrSkySportSensorRSSI.h"

#include "FrSkySportDecoder.h"
#include "SoftwareSerial.h"

#include "ArduinoJson-v5.13.5.h"

#define RSSI_EN 1
#define ASS_EN 1
#define FCS_EN 1
#define FLVSS_EN 1
#define GPS_EN 1
#define RPM_EN 1
#define SP2UART_EN 1
#define VARIO_EN 1
#define SPEC1_EN 1
//#define SPEC2_EN 1
//#define SPEC3_EN 1

/* CREATES JSON STREAN WITH FOLLOWING FORMAT
{
	"frsky": {
		"RSSI": {
			"rssi": "0",
			"adc1": "0.00",
			"adc2": "0.00",
			"rxBatt": "0.00",
			"swr": "0"
		},
		"ASS": {
			"airspeed": "0.00"
		},
		"FCS": {
			"current": "0.00",
			"voltage": "0.00"
		},
		"FLVSS": {
			"V0": "0.00",
			"V1": "0.00",
			"V2": "0.00",
			"V3": "0.00",
			"V4": "0.00",
			"V5": "0.00"
		},
		"GPS": {
			"lat": "0.00",
			"lon": "0.00",
			"alt": "0.00",
			"speed": "0.00",
			"cog": "0.00",
			"year": "2000",
			"month": "0",
			"day": "0",
			"hour": "0",
			"min": "0",
			"sec": "0"
		},
		"RPM": {
			"rpm": "0",
			"t1": "0",
			"t2": "0"
		},
		"SP2UART": {
			"adc3": "0.00",
			"adc4": "0.00"
		},
		"VARIO": {
			"alt": "0.00",
			"vsi": "0.00"
		},
		"SPEC1": {
			"V0": "0.00",
			"V1": "0.00",
			"V2": "0.00",
			"V3": "0.00",
			"V4": "0.00",
			"V5": "0.00"
		}
	}
}
*/


#ifdef RSSI_EN
FrSkySportSensorRSSI rssi;
#endif
#ifdef ASS_EN
FrSkySportSensorAss ass;
#endif                                                 // Create ASS sensor with default ID
#ifdef FCS_EN
FrSkySportSensorFcs fcs;                               // Create FCS-40A sensor with default ID (use ID8 for FCS-150A)
#endif
#ifdef FLVSS_EN
FrSkySportSensorFlvss flvss;                          // Create FLVSS sensor with default ID
#endif
#ifdef GPS_EN
FrSkySportSensorGps gps(FrSkySportSensor::ID28);                               // Create GPS sensor with default ID
#endif
#ifdef RPM_EN
FrSkySportSensorRpm rpm;                               // Create RPM sensor with default ID
#endif
#ifdef SP2UART_EN
FrSkySportSensorSp2uart sp2uart;                       // Create SP2UART Type B sensor with default ID
#endif
#ifdef VARIO_EN
FrSkySportSensorVario vario;                           // Create Variometer sensor with default ID
#endif
#ifdef SPEC1_EN
FrSkySportSensorFlvss spec1(FrSkySportSensor::ID15);  // Create FLVSS sensor with given ID
#endif
#ifdef SPEC2_EN
FrSkySportSensorFlvss spec2(FrSkySportSensor::ID15);  // Create FLVSS sensor with given ID
#endif
#ifdef SPEC3_EN
FrSkySportSensorFlvss spec3(FrSkySportSensor::ID15);  // Create FLVSS sensor with given ID
#endif

#ifdef POLLING_ENABLED
  FrSkySportDecoder decoder(true);                     // Create decoder object with polling
#else
  FrSkySportDecoder decoder;                           // Create decoder object without polling
#endif

uint32_t currentTime;
uint16_t decodeResult;

void setup()
{
  decoder.begin(FrSkySportSingleWireSerial::SOFT_SERIAL_PIN_2
    #ifdef RSSI_EN
      ,&rssi
    #endif
    #ifdef ASS_EN
      ,&ass
    #endif
    #ifdef FCS_EN
      ,&fcs
    #endif
    #ifdef FLVSS_EN
      ,&flvss
    #endif
    #ifdef GPS_EN
      ,&gps
    #endif
    #ifdef RPM_EN
      ,&rpm
    #endif
    #ifdef SP2UART_EN
      ,&sp2uart
    #endif
    #ifdef VARIO_EN
      ,&vario
    #endif
    #ifdef SPEC1_EN
      ,&spec1
    #endif
    #ifdef SPEC2_EN
      ,&spec2
    #endif
    #ifdef SPEC3_EN
      ,&spec3
    #endif

    );

  Serial.begin(115200);
}

void loop()
{
  // Read and decode the telemetry data, note that the data will only be decoded for sensors
  // that that have been passed to the begin method. Print the AppID of the decoded data.
  decodeResult = decoder.decode();
  //if(decodeResult != SENSOR_NO_DATA_ID) { Serial.print("Decoded data with AppID 0x"); Serial.print(decodeResult, HEX); }

  currentTime = millis();

  uint8_t something_updated = 0;
  DynamicJsonBuffer jsonBuffer;

  JsonObject& root = jsonBuffer.createObject();
  JsonObject& frsky_j = jsonBuffer.createObject();

  #ifdef RSSI_EN
  if(rssi.data_available && currentTime%2000 == 0)
  {
    JsonObject& RSSI_j = jsonBuffer.createObject();
    // Get current/voltage sensor (FCS) data
    RSSI_j["rssi"] = rssi.getRssi();
    RSSI_j["adc1"] = rssi.getAdc1();
    RSSI_j["adc2"] = rssi.getAdc2();
    RSSI_j["rxBatt"] = rssi.getRxBatt();
    RSSI_j["swr"] = rssi.getSwr();

    frsky_j["RSSI"] = RSSI_j;
    something_updated = 1;
  }

  #endif

  #ifdef ASS_EN
  if(ass.data_available)
  {
    JsonObject& ASS_j = jsonBuffer.createObject();
    // Get airspeed sensor (ASS) data
    ASS_j["airspeed"] = ass.getSpeed(); // Airspeed in km/h
    //Serial.print("{\"sensortype\":\"ASS\",");

    frsky_j["ASS"] = ASS_j;
    something_updated = 1;
  }
  #endif

  #ifdef FCS_EN
  if(fcs.data_available)
  {
    JsonObject& FCS_j = jsonBuffer.createObject();

    // Get current/voltage sensor (FCS) data
    FCS_j["current"] = fcs.getCurrent();
    FCS_j["voltage"] = fcs.getVoltage();

    frsky_j["FCS"] = FCS_j;
    something_updated = 1;
  }
  #endif

  #ifdef FLVSS_EN
  if(flvss.data_available)
  {
    JsonObject& FLVSS_j = jsonBuffer.createObject();

    // Get LiPo voltage sensor (FLVSS) data (each cell1-cell6 voltage in volts)
    FLVSS_j["V1"] = flvss.getCell1();
    FLVSS_j["V2"] = flvss.getCell2();
    FLVSS_j["V3"] = flvss.getCell3();
    FLVSS_j["V4"] = flvss.getCell4();
    FLVSS_j["V5"] = flvss.getCell5();
    FLVSS_j["V6"] = flvss.getCell6();

    frsky_j["FLVSS"] = FLVSS_j;
    something_updated = 1;
  }
  #endif

  #ifdef GPS_EN
  if(gps.data_available)
  {
    JsonObject& GPS_j = jsonBuffer.createObject();

    // Get GPS data
    GPS_j["lat"] = gps.getLat();
    GPS_j["lon"] = gps.getLon();
    GPS_j["alt"] = gps.getAltitude();
    GPS_j["speed"] = gps.getSpeed();
    GPS_j["cog"] = gps.getCog();
    GPS_j["year"] = gps.getYear();
    GPS_j["month"] = gps.getMonth();
    GPS_j["day"] = gps.getDay();
    GPS_j["hour"] = gps.getHour();
    GPS_j["min"] = gps.getMinute();
    GPS_j["sec"] = gps.getSecond();

    frsky_j["GPS"] = GPS_j;
    something_updated = 1;
  }
  #endif

  #ifdef RPM_EN
  if(rpm.data_available)
  {
    JsonObject& RPM_j = jsonBuffer.createObject();

    // Get RPM/temperature sensor data
    RPM_j["rpm"] = rpm.getRpm();
    RPM_j["t1"] = rpm.getT1();
    RPM_j["t2"] = rpm.getT2();

    frsky_j["RPM"] = RPM_j;
    something_updated = 1;
  }
  #endif

  #ifdef SP2UART_EN
  if(sp2uart.data_available)
  {
    JsonObject& SP2UART_j = jsonBuffer.createObject();

    // Get RPM/temperature sensor data
    SP2UART_j["adc3"] = sp2uart.getAdc3();
    SP2UART_j["adc4"] = sp2uart.getAdc4();

    frsky_j["SP2UART"] = SP2UART_j;
    something_updated = 1;
  }
  #endif

  #ifdef VARIO_EN
  if(vario.data_available)
  {
    JsonObject& VARIO_j = jsonBuffer.createObject();

    // Get variometer data (altitude in m, VSI in m/s)
    VARIO_j["alt"] = vario.getAltitude();
    VARIO_j["vsi"] = vario.getVsi();

    frsky_j["VARIO"] = VARIO_j;
    something_updated = 1;
  }
  #endif

  #ifdef SPEC1_EN
  if(spec1.data_available)
  {
    JsonObject& SPEC1_j = jsonBuffer.createObject();

    // Get LiPo voltage sensor (FLVSS) data (each cell1-cell6 voltage in volts)
    SPEC1_j["V1"] = spec1.getCell1();
    SPEC1_j["V2"] = spec1.getCell2();
    SPEC1_j["V3"] = spec1.getCell3();
    SPEC1_j["V4"] = spec1.getCell4();
    SPEC1_j["V5"] = spec1.getCell5();
    SPEC1_j["V6"] = spec1.getCell6();

    frsky_j["SPEC1"] = SPEC1_j;
    something_updated = 1;
  }
  #endif

  #ifdef SPEC2_EN
  if(spec2.data_available)
  {}
  #endif

  #ifdef SPEC3_EN
  if(spec3.data_available)
  {}
  #endif

  if(something_updated == 1)
  {
    frsky_j["TIME"] = currentTime;
    root["frsky"] = frsky_j;

    root.printTo(Serial);
    Serial.print("\n");
  }
}
