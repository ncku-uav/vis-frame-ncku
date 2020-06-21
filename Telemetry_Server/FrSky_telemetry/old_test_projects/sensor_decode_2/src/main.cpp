/*
  FrSky S-Port Telemetry Decoder library example
  (c) Pawelsky 20160919
  Not for commercial use

  Note that you need Teensy 3.x/LC or 328P based (e.g. Pro Mini, Nano, Uno) board and FrSkySportDecoder library for this example to work
*/

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

/*
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

uint32_t currentTime, displayTime;
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
  char command;
  // Read and decode the telemetry data, note that the data will only be decoded for sensors
  // that that have been passed to the begin method. Print the AppID of the decoded data.
  decodeResult = decoder.decode();
  //if(decodeResult != SENSOR_NO_DATA_ID) { Serial.print("Decoded data with AppID 0x"); Serial.print(decodeResult, HEX); }

  // Display data once a second to not interfeere with data decoding
  currentTime = millis();
  //if(currentTime > displayTime)
  if(Serial.available() > 0)// && Serial.read()=="g")
  {
    command = Serial.read();
    displayTime = currentTime + 500;

    //Serial.print("<RS>");
    Serial.print("{\"frsky\":{");

    #ifdef RSSI_EN
    // Get current/voltage sensor (FCS) data
    //Serial.print("{\"sensortype\":\"FCS\",");
    Serial.print("\"RSSI\":{");
    Serial.print("\"rssi\":\"");
    Serial.print(rssi.rssi);
    Serial.print("\",");
    Serial.print("\"adc1\":\"");
    Serial.print(rssi.adc1);
    Serial.print("\",");
    Serial.print("\"adc2\":\"");
    Serial.print(rssi.adc2);
    Serial.print("\",");
    Serial.print("\"rxBatt\":\"");
    Serial.print(rssi.rxBatt);
    Serial.print("\",");
    Serial.print("\"swr\":\"");
    Serial.print(rssi.swr);
    Serial.print("\"");
    Serial.print("}");
    #endif

    #if defined(RSSI_EN)&& defined(ASS_EN)
    Serial.print(",");
    #endif

    #ifdef ASS_EN
    // Get airspeed sensor (ASS) data
    //Serial.print("{\"sensortype\":\"ASS\",");
    Serial.print("\"ASS\":{");
    Serial.print("\"airspeed\":\"");

    Serial.print(random(1,100));
    //Serial.print(ass.speed);
    Serial.print("\"");
    Serial.print("}"); // Airspeed in km/h
    #endif

    #if defined(ASS_EN)&& defined(FCS_EN)
    Serial.print(",");
    #endif

    #ifdef FCS_EN
    // Get current/voltage sensor (FCS) data
    //Serial.print("{\"sensortype\":\"FCS\",");
    Serial.print("\"FCS\":{");
    Serial.print("\"current\":\"");
    Serial.print(fcs.current);
    Serial.print("\",");
    Serial.print("\"voltage\":\"");
    Serial.print(fcs.voltage);
    Serial.print("\"");
    Serial.print("}");
    #endif

    #if defined(FCS_EN)&& defined(FLVSS_EN)
    Serial.print(",");
    #endif

    #ifdef FLVSS_EN
    // Get LiPo voltage sensor (FLVSS) data (each cell1-cell6 voltage in volts)
    // Serial.print("{\"sensortype\":\"FLVSS\",");
    Serial.print("\"FLVSS\":{");
    for(uint8_t i=0; i<5; i++)
    {
      Serial.print("\"V");Serial.print(i);Serial.print("\":\"");
      Serial.print(flvss.cell[i]);
      Serial.print("\",");
    }
    Serial.print("\"V5\":\"");
    Serial.print(flvss.cell[5]);
    Serial.print("\"");
    Serial.print("}");
    #endif

    #if defined(FLVSS_EN)&& defined(GPS_EN)
    Serial.print(",");
    #endif

    #ifdef GPS_EN
    // Get GPS data

    Serial.print("\"GPS\":{");
    Serial.print("\"lat\":\"");
    Serial.print(gps.lat);
    Serial.print("\",");

    Serial.print("\"lon\":\"");
    Serial.print(gps.lon);
    Serial.print("\",");

    Serial.print("\"alt\":\"");
    Serial.print(gps.altitude);
    Serial.print("\",");

    Serial.print("\"speed\":\"");
    Serial.print(gps.speed);
    Serial.print("\",");

    Serial.print("\"cog\":\"");
    Serial.print(gps.cog);
    Serial.print("\",");

    Serial.print("\"year\":\"");
    Serial.print(gps.year+2000);
    Serial.print("\",");

    Serial.print("\"month\":\"");
    Serial.print(gps.month);
    Serial.print("\",");

    Serial.print("\"day\":\"");
    Serial.print(gps.day);
    Serial.print("\",");

    Serial.print("\"hour\":\"");
    Serial.print(gps.hour);
    Serial.print("\",");

    Serial.print("\"min\":\"");
    Serial.print(gps.minute);
    Serial.print("\",");

    Serial.print("\"sec\":\"");
    //Serial.print(gps.second);
    Serial.print(currentTime);

    Serial.print("\"");
    Serial.print("}");


    /*Serial.print("GPS: lat = "); Serial.print(gps.getLat(), 6); Serial.print(", lon = "); Serial.print(gps.getLon(), 6); // Latitude and longitude in degrees decimal (positive for N/E, negative for S/W)
    Serial.print(", altitude = "); Serial.print(gps.getAltitude()); // Altitude in m (can be negative)
    Serial.print("m, speed = "); Serial.print(gps.getSpeed());      // Speed in m/s
    Serial.print("m/s, COG = "); Serial.print(gps.getCog());        // Course over ground in degrees (0-359, 0 = north)
    char dateTimeStr[18];
    sprintf(dateTimeStr, "%02u-%02u-%04u %02u:%02u:%02u", gps.getDay(), gps.getMonth(), gps.getYear() + 2000, gps.getHour(), gps.getMinute(), gps.getSecond());
    Serial.print(", date/time = "); Serial.print(dateTimeStr); // Date (year - need to add 2000, month, day) and time (hour, minute, second)*/
    #endif

    #if defined(GPS_EN)&& defined(RPM_EN)
    Serial.print(",");
    #endif

    #ifdef RPM_EN
    // Get RPM/temperature sensor data

    Serial.print("\"RPM\":{");
    Serial.print("\"rpm\":\"");
    Serial.print(rpm.rpm);
    Serial.print("\",");
    Serial.print("\"t1\":\"");
    Serial.print(rpm.t1);
    Serial.print("\",");
    Serial.print("\"t2\":\"");
    Serial.print(rpm.t2);
    Serial.print("\"");
    Serial.print("}");


    /*Serial.print("RPM: rpm = "); Serial.print(rpm.getRpm());                              // Rotations per minute
    Serial.print(", T1 = "); Serial.print(rpm.getT1());                                   // Temperature #1 in degrees Celsuis (can be negative, will be rounded)
    Serial.print(" deg. C, T2 = "); Serial.print(rpm.getT2()); Serial.print(" deg. C"); // Temperature #2 in degrees Celsuis (can be negative, will be rounded)
    */
    #endif

    #if defined(RPM_EN)&& defined(SP2UART_EN)
    Serial.print(",");
    #endif

    #ifdef SP2UART_EN
    Serial.print("\"SP2UART\":{");
    Serial.print("\"adc3\":\"");
    Serial.print(sp2uart.adc3);
    Serial.print("\",");
    Serial.print("\"adc4\":\"");
    Serial.print(sp2uart.adc4);
    Serial.print("\"");
    Serial.print("}");

    // Get SP2UART sensor data (ADC3 and ADC4 voltage in V)
    /*Serial.print("SP2UART: ADC3 = "); Serial.print(sp2uart.getAdc3());                // ADC3 voltage in volts
    Serial.print("V, ADC4 = "); Serial.print(sp2uart.getAdc4()); Serial.print("V"); // ADC4 voltage in volts*/
    #endif

    #if defined(SP2UART_EN)&& defined(VARIO_EN)
    Serial.print(",");
    #endif

    #ifdef VARIO_EN
    // Get variometer data (altitude in m, VSI in m/s)

    Serial.print("\"VARIO\":{");
    Serial.print("\"alt\":\"");
    Serial.print(vario.altitude);
    Serial.print("\",");
    Serial.print("\"vsi\":\"");
    Serial.print(vario.vsi);
    Serial.print("\"");
    Serial.print("}");


    /*Serial.print("VARIO: altitude = "); Serial.print(vario.getAltitude());          // Altitude in meters (can be negative)
    Serial.print("m, VSI = "); Serial.print(vario.getVsi()); Serial.print("m/s"); // Vertical speed in m/s (positive - up, negative - down)*/
    #endif

    #if defined(VARIO_EN)&& defined(SPEC1_EN)
    Serial.print(",");
    #endif

    #ifdef SPEC1_EN
    // Get LiPo voltage sensor (FLVSS) data (each cell1-cell6 voltage in volts)
    Serial.print("\"SPEC1\":{");
    for(uint8_t i=0; i<5; i++)
    {
      Serial.print("\"V");Serial.print(i);Serial.print("\":\"");
      Serial.print(spec1.cell[i]);
      Serial.print("\",");
    }
    Serial.print("\"V5\":\"");
    Serial.print(spec1.cell[5]);
    Serial.print("\"");
    Serial.print("}");
    #endif

    #if defined(SPEC1_EN)&& defined(SPEC2_EN)
    Serial.print(",");
    #endif

    #ifdef SPEC2_EN
    #endif

    #if defined(SPEC2_EN)&& defined(SPEC3_EN)
    Serial.print(",");
    #endif

    #ifdef SPEC3_EN
    #endif

    Serial.print("}}\n");
    //Serial.print("<LF>");
  }
}
