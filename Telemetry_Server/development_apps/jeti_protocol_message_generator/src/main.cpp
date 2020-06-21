/*
The circuit:
* pushbutton attached to pin 2 from +5V
* 10K resistor attached to pin 2 from ground
* RX attached to TX of FTDI USB Device
* TX attached to RX of FTDI USB Device
*/

#import <Arduino.h>
#import <telemetry_message_ids.hpp>

const int buttonPin = 2;     // the number of the pushbutton pin

uint8_t request_handshake_buf[] =
{
  0x0A, 0x7E,
  TypeID_t::request_handshake, //typeId
  0x51, 0xA2, //messageId
  0x03, 0x04, 0xAA, 0x0A //scrap
};

uint8_t response_handshake_buf[] =
{
  0x02, 0x45, 0x7E,
	0x14, //type Id = HandshakeResponse
	0x01, 0x02, // messageId
	0x03, 0x04, // messageReferenceId
	'y', 'e', 't', 'i', '\0', //yeti\0
	0x7A // CRC
	//message2: typeId=20/Handshake_response mId=258 mrId=772 hId="yeti\0" CRC , 2 bytes scrap
};

uint8_t ACK_buf[] =
{
  0x0A, 0x7E,
  0x0F, //typeId = ACK
  0x01, 0x02,  //messageId
  0x03, 0x04  //message reference Id
};

uint8_t scan_sensor_data_buf[] =
{
  0x0A, 0x7E, TypeID_t::scan_sensor_data,
	0xFD, 0xAE, //mID
	0x03, 0x04, //mRID
	0xAA, 0x0A, //manufacturerId
	0x4E, 0x6A,  //deviceId
	'D','e','s','c','r','i','p','t','i','o','n','#','U','n','i','t',0x00,0x00,0x00,0x00, //name
	0x08 //CRC
};

uint8_t message_array[] =
{
  request_handshake_buf,
  response_handshake_buf,
  ACK_buf,
  scan_sensor_data_buf,
};

void burst() {
    Serial.print(message_array[0]);
}

void setup() {
// initialize the pushbutton pin as an input:
pinMode(buttonPin, INPUT_PULLUP);
attachInterrupt(digitalPinToInterrupt(buttonPin), burst, FALLING);

Serial.begin(115200);

}

void loop() {
}
