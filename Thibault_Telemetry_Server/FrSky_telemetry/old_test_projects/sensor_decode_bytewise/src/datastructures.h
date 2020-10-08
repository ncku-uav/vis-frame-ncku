#ifndef _DATASTRUCTURES_H_
#define _DATASTRUCTURES_H_

#include "Arduino.h"

struct sensor_ass{
  uint16_t speed;
};

struct sensor_fcs{
  uint16_t current;
  uint16_t voltage;
};

struct sensor_flvss{
  uint16_t cell1;
  uint16_t cell2;
  uint16_t cell3;
  uint16_t cell4;
  uint16_t cell5;
  uint16_t cell6;
};

struct sensor_gps{
  uint32_t lat;
  uint32_t lon;
  uint16_t alt;
  uint16_t speed;
  uint16_t cog;
  uint8_t day;
  uint8_t month;
  uint16_t year;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
};

struct sensor_rpm{
  uint16_t rpm;
  uint8_t t1;
  uint8_t t2;
};

struct sensor_sp2uart{
  uint16_t adc3;
  uint16_t adc4;
};

struct sensor_vario{
  uint16_t alt;
  uint16_t vsi;
};

#endif // _DATASTRUCTURES_H_
