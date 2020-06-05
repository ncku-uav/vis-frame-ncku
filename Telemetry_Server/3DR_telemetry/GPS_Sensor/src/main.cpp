#include <Arduino.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <ArduinoJson-v5.13.5.h>


//https://bigdanzblog.wordpress.com/2015/01/18/connecting-u-blox-neo-6m-gps-to-raspberry-pi/

SoftwareSerial mySerial(3, 4); // RX, TX
TinyGPSPlus gps;

unsigned long last = 0UL;

void JSON_test();
void push_JSON();
void displayInfo();

void setup()
{
  Serial.begin(57600);
  mySerial.begin(9600);
}

void loop()
{
  while (mySerial.available() > 0)
    gps.encode(mySerial.read());
  push_JSON();
}

void JSON_test()
{
}

void push_JSON()
{
  DynamicJsonBuffer jsonBuffer;

  JsonObject& root = jsonBuffer.createObject();
  JsonObject& threeDR = jsonBuffer.createObject();

  uint8_t something_updated = 0;

  static uint8_t start_location_set = 0;
  static uint32_t raw_time = 0, raw_time_old = 1;
  static double start_lat=0, start_lon=0;

  if (gps.location.isUpdated())
  {
    JsonObject& GPS_j = jsonBuffer.createObject();

    double current_lat = gps.location.lat();
    double current_lon = gps.location.lng();

    GPS_j["age"] = gps.location.age();
    GPS_j["lat"] = current_lat;
    GPS_j["lon"] = current_lon;

    if (start_location_set == 1)
    {
      JsonObject& DIST_j = jsonBuffer.createObject();

      double distanceToStart =
        TinyGPSPlus::distanceBetween(
          current_lat,
          current_lon,
          start_lat,
          start_lon);
      double courseToStart =
        TinyGPSPlus::courseTo(
          current_lat,
          current_lon,
          start_lat,
          start_lon);

      DIST_j["dist"] = distanceToStart/1000000; //m
      DIST_j["course"] = courseToStart; //deg
      DIST_j["card"] = TinyGPSPlus::cardinal(courseToStart); //{"N", "NNE", "NE", "ENE", "E", "ESE", "SE", "SSE", "S", "SSW", "SW", "WSW", "W", "WNW", "NW", "NNW"}

      GPS_j["DIST"] = DIST_j;

    }
    else if(start_location_set == 0)
    {
      start_location_set = 1;
      start_lat = current_lat;
      start_lon = current_lon;
    }

    threeDR["GPS"] = GPS_j;
    something_updated = 1;

  }

  else if (gps.date.isUpdated())
  {
    JsonObject& DATE_j = jsonBuffer.createObject();

    DATE_j["age"] = gps.date.age();
    DATE_j["year"] = gps.date.year();
    DATE_j["month"] = gps.date.month();
    DATE_j["day"] = gps.date.day();

    threeDR["DATE"] = DATE_j;
    something_updated = 1;
  }

  else if (gps.time.isUpdated() && raw_time_old != raw_time)
  {
    JsonObject& TIME_j = jsonBuffer.createObject();

    raw_time_old = raw_time;
    raw_time = gps.time.value();

    TIME_j["age"] = gps.time.age();
    TIME_j["hour"] = gps.time.hour();
    TIME_j["minute"] = gps.time.minute();
    TIME_j["second"] = gps.time.second();
    TIME_j["hundr"] = gps.time.centisecond();

    threeDR["TIME"] = TIME_j;
    something_updated = 1;
  }

  else if (gps.speed.isUpdated())
  {
    JsonObject& SPEED_j = jsonBuffer.createObject();

    SPEED_j["age"] = gps.speed.age();
    SPEED_j["speed"] = gps.speed.kmph();

    threeDR["SPEED"] = SPEED_j;
    something_updated = 1;


/*    Serial.print(gps.speed.knots());
    Serial.print(gps.speed.mph());
    Serial.print(gps.speed.mps());
    Serial.println(gps.speed.kmph());*/
  }

  else if (gps.course.isUpdated())
  {
    JsonObject& HDG_j = jsonBuffer.createObject();

    HDG_j["age"] = gps.course.age();
    HDG_j["heading"] = gps.course.deg();

    threeDR["HDG"] = HDG_j;
    something_updated = 1;
  }

  else if (gps.altitude.isUpdated())
  {
    JsonObject& ALT_j = jsonBuffer.createObject();

    ALT_j["age"] = gps.altitude.age();
    ALT_j["alt"] = gps.altitude.meters();

    threeDR["ALT"] = ALT_j;
    something_updated = 1;

    /*Serial.print(gps.altitude.meters());
    Serial.print(gps.altitude.miles());
    Serial.print(gps.altitude.kilometers());
    Serial.println(gps.altitude.feet());*/
  }

  else if (gps.satellites.isUpdated())
  {
    JsonObject& SAT_j = jsonBuffer.createObject();

    SAT_j["age"] = gps.satellites.age();
    SAT_j["count"] = gps.satellites.value();

    threeDR["SAT"] = SAT_j;
    something_updated = 1;
  }

  else if (gps.hdop.isUpdated())
  {
    JsonObject& HDOP_j = jsonBuffer.createObject();

    HDOP_j["age"] = gps.hdop.age();
    HDOP_j["hdop"] = gps.hdop.hdop();

    threeDR["HDOP"] = HDOP_j;
    something_updated = 1;
  }

  else if (millis() - last > 5000)
  {
    JsonObject& STAT_j = jsonBuffer.createObject();

    STAT_j["chars_prcsd"] = gps.charsProcessed();
    STAT_j["s_w_fix"] = gps.sentencesWithFix();
    STAT_j["failed_cksum"] = gps.failedChecksum();
    STAT_j["passed_cksum"] = gps.passedChecksum();

    last = millis();

    threeDR["STAT"] = STAT_j;
    something_updated = 1;

  }

  root["threeDR"] = threeDR;

  if(something_updated == 1)
  {
    root.printTo(Serial);
    Serial.print("\n");
  }
}

void displayInfo()
{
  Serial.print(F("Location: "));
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}
