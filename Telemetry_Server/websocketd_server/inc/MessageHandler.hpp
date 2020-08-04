/*
 * MessageHandler.hpp
 *
 *  Created on: 12.04.2019
 *      Author: thibault
 */

#ifndef INC_MESSAGEHANDLER_HPP_
#define INC_MESSAGEHANDLER_HPP_

#include <string>
#include <iostream>
#include <list>
#include <iterator>
using namespace std;

#include "telemetry_decoder.hpp"
#include "telemetry_encoder.hpp"
#include "telemetry_message_types.hpp"
#include "telemetry_message_ids.hpp"
#include "USBDevice.hpp"
#include "Buffer.hpp"
#include "ground_station_decoder_interface.hpp"
#include "telemetry_handshake.hpp"
#include "DecodingErrorNotifier.hpp"
#include "decoding_error_handler.hpp"
#include "Sensor.hpp"
#include "Measurement.hpp"
#include "ConsoleOutputGenerator.hpp"

#define handshaking 	10
#define scanning		20
#define	subscribing 	30
#define subscribed		40

class MessageHandler : public DecodingErrorInterface
{
public:
	MessageHandler();
	virtual ~MessageHandler() {};

	TelemetryDecoder * decoder = new TelemetryDecoder();
	TelemetryEncoder * encoder = new TelemetryEncoder();

	list <Sensor> sensor_scan_list;
	list <Sensor> :: iterator p_sensor_scan_list;

	list <Measurement> measurement_scan_list;
	list <Measurement> :: iterator p_measurement_scan_list;

	USB_Device new_dev;

	TypeId previously_sent_typeId;

	ConsoleOutputGenerator output_generator;

	string output;

	bool handshakeComplete;
	bool scanRequestComplete;
	bool scanCancelComplete;
	bool subscribeRequested;

	clock_t timestamp;
	uint8_t communicationState;
	TelemetryMessageInterface * interface;

	void handleBuffer();
	void notifyDecodingError(uint16_t referenceId) override;
	void checkStates();
	void sendStartScan();
	void sendCancelScan();
	void sendSubscribeRequests();
	void handleMessage(TelemetryMessageInterface * p_Message);
	void processACK();
	void processNACK();
	void processSensorHeader(TelemetryMessageInterface * p_Message);
	void addSensorHeader(TelemetryMessageInterface * p_Message);
	void processSensorName(TelemetryMessageInterface * p_Message);
	void addSensorName(TelemetryMessageInterface * p_Message);
	void processSensorMeasurement(TelemetryMessageInterface * p_Message);
	void addSensorMeasurement(TelemetryMessageInterface * p_Message);
	void processSubscribePending(TelemetryMessageInterface * p_message);
	void processSubscribeConfirmation(TelemetryMessageInterface * p_message);
	void processSubscribeUpdate(TelemetryMessageInterface * p_message);
	void send_message(TelemetryMessageInterface * p_message);
	void send_handshake_request();
	void check_timestamps();
	uint8_t last_received_messageId_smaller(uint16_t old_messageId, uint16_t new_messageId);

};

#endif /* INC_MESSAGEHANDLER_HPP_ */
