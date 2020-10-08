#include "communicationhandler.h"

#define FRAME_END_SEQUENCE 0x63 //c

uint8_t sensorlist[28] = {0};

void protocol_handshake(){
  static uint8_t message = 0x00;
  static uint8_t handshake = 0x00;
  static uint32_t currentTime=0, displayTime=0;

  Serial.println("waiting for handshake. listening...");
  while (handshake==0x00){ //wenn noch keine Message empfangen und alle 500 ms
    switch(message){
      case(0x6d)://m
        get_sensorlist();
        Serial.println(sensorlist[1]);
        break;

      case(0x63)://c
        handshake=0x01;

      default:
        currentTime=millis();
        if(currentTime > displayTime)
        {
          displayTime = currentTime + 1000;
          Serial.println("frsky");
          message=Serial.read();
        }
        break;
    }
  }
  Serial.println("handshake completed");
}

void get_sensorlist(){
  static uint8_t buf = 0x00;
  static uint8_t buf_old = 0x00;
  static uint8_t i = 0;
  do{
    buf=Serial.read();
    if(buf_old!=buf){
      sensorlist[i]=buf;
      i++;
    }
    buf_old=buf;
  }while(buf!=FRAME_END_SEQUENCE && i<28);

}
