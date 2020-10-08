/*
 * telemetry_message_ids.hpp
 *
 *  Created on: 20.03.2019
 *      Author: Thibault
 */

#include <stdint.h>

#ifndef INC_TELEMETRY_MESSAGE_IDS_HPP_
#define INC_TELEMETRY_MESSAGE_IDS_HPP_


// May not contain FRAMING_BYTE from telemetry_custom; nor escape characters
typedef enum : uint8_t
{
  message_idle					= 0x00,   // 0
  request_handshake             = 0x0A,   // 10
  ACK                 			= 0x0F,   // 15
  response_handshake            = 0x14,   // 20
  request_start_scan            = 0x1E,   // 30
  scan_sensor_data       		= 0x28,   // 40
  sensor_header 				= 0x29,	  // 41
  sensor_name					= 0x2A,   // 42
  sensor_measurement			= 0x2B,   // 43
  sensor_measurement_description = 0x2C,  // 44
  scan_measurement_data         = 0x32,   // 50
  request_cancel_scan          	= 0x3C,   // 60
  request_information           = 0x46,   // 70
  information_measurement       = 0x50,   // 80
  information_pending           = 0x5A,   // 90
  request_subscribe             = 0x64,   // 100
  subscribe_confirmation        = 0x6E,   // 110
  subscribe_pending            	= 0x78,   // 120
  subscribe_update             	= 0x82,   // 130
  subscribe_update_text			= 0x83,	  // 131
  subscribe_unsubscribe         = 0x8C,   // 140
  checkout_notification 		= 0x96,   // 150
  maintenance_subscribe     	= 0xA0,   // 160
  maintenance_log           	= 0xAA,   // 170
  maintenance_unsubscribe   	= 0xB4,	  // 180
  NACK                			= 0xF0,   // 240
} TypeID_t;

#endif /* INC_TELEMETRY_MESSAGE_IDS_HPP_ */
