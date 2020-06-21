/*
  FrSky XJT module S.Port output decoder class for Teensy 3.x/LC and 328P based boards (e.g. Pro Mini, Nano, Uno)
  (c) Pawelsky 20160919
  Not for commercial use

  Note that this special sensor class can only be used for decoding.
*/

#ifndef _FRSKY_SPORT_SENSOR_RSSI_H_
#define _FRSKY_SPORT_SENSOR_RSSI_H_

#include "FrSkySportSensor.h"

#define XJT_DEFAULT_ID ID25

#define XJT_RSSI_DATA_ID          0xF101
#define XJT_ADC1_DATA_ID          0xF102
#define XJT_ADC2_DATA_ID          0xF103
#define XJT_RXBATT_DATA_ID        0xF104
#define XJT_SWR_DATA_ID           0xF105
#define XJT_FAS_VOLTAGE_DATA_ID   0x003B
#define XJT_FAS_CURRENT_DATA_ID   0x0028
#define XJT_FGS_FUEL_DATA_ID      0x0004
#define XJT_FLVS_VOLTAGE_DATA_ID  0x0006
#define XJT_FVAS_ALTITUDE_DATA_ID 0x0021
#define XJT_FVAS_VSI_DATA_ID      0x0030 // Not documented in FrSky spec, added based on OpenTX sources.
#define XJT_GPS_LAT_DATA_ID       0x0023
#define XJT_GPS_LON_DATA_ID       0x0022
#define XJT_GPS_ALTITUDE_DATA_ID  0x0009
#define XJT_GPS_SPEED_DATA_ID     0x0019
#define XJT_GPS_COG_DATA_ID       0x001C
#define XJT_GPS_DATE_DATA_ID      0x0016
#define XJT_GPS_TIME_DATA_ID      0x0018
#define XJT_TAS_ACCX_DATA_ID      0x0024
#define XJT_TAS_ACCY_DATA_ID      0x0025
#define XJT_TAS_ACCZ_DATA_ID      0x0026
#define XJT_TEMS_T1_DATA_ID       0x0002
#define XJT_TEMS_T2_DATA_ID       0x0005
#define XJT_RPMS_RPM_DATA_ID      0x0003

class FrSkySportSensorRSSI : public FrSkySportSensor
{
  public:
    FrSkySportSensorRSSI(SensorId id = XJT_DEFAULT_ID);
    virtual uint16_t decodeData(uint8_t id, uint16_t appId, uint32_t data);

    // System data
    uint8_t getRssi();
    float getAdc1();
    float getAdc2();
    float getRxBatt();
    uint8_t getSwr();

    // Variables holding decoded telemetry data
    uint8_t rssi;
    float adc1;
    float adc2;
    float rxBatt;
    uint8_t swr;
    uint8_t data_available;

  private:
    // Temporary variables for collecting telemetry values that are combined from multiple data IDs
    uint16_t voltBD;
    uint16_t voltAD;
    int16_t altBD;
    int16_t altAD;
    char latNS;
    uint16_t latBD;
    uint16_t latAD;
    char lonEW;
    uint16_t lonBD;
    uint16_t lonAD;
    int16_t gpsAltBD;
    int16_t gpsAltAD;
    uint16_t speedBD;
    uint16_t speedAD;
    uint16_t cogBD;
    uint16_t cogAD;
};

#endif // _FRSKY_SPORT_SENSOR_RSSI_H_
