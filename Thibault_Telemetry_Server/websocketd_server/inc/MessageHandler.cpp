/*
 * MessageHandler.cpp
 *
 *  Created on: 12.04.2019
 *      Author: thibault
 */

#include "MessageHandler.hpp"

MessageHandler::MessageHandler():
handshakeComplete(false),
scanRequestComplete(false),
scanCancelComplete(false),
subscribeRequested(false),
timestamp(0),
communicationState(handshaking),
interface(nullptr)
{
	this->decoder->registerDecodingErrorHandler(this);
}

void MessageHandler::handleBuffer()
{

	new_dev.open_port("/dev/ttyUSB0", B115200);
	//new_dev->dev_parser();

	//new_dev.create_dev_parser_thread();

	TelemetryMessageInterface * p_Message;
	while(1)
	{
		new_dev.dev_parser();
		if (new_dev.device_buffer.get_buffer_size())
		{
			p_Message = decoder->decode_datastream(new_dev.device_buffer.p_buffer_start, new_dev.device_buffer.get_buffer_size());
			handleMessage(p_Message);
			if(p_Message != nullptr)
				new_dev.device_buffer.clear_buffer();
		}


		check_timestamps();
		checkStates();
	}
}

void MessageHandler::notifyDecodingError(uint16_t referenceId)
{
	output_generator.output_debug_message("NACK sent because of CRC failure");
	send_message(encoder->decline(referenceId));
}

void MessageHandler::checkStates()
{
	switch(communicationState)
	{
	case(handshaking):
		break;
	case(scanning):
		sendStartScan();
		sendCancelScan();
		break;
	case(subscribing):
		if(!subscribeRequested)
			sendSubscribeRequests();
		break;
	case(subscribed):
		break;
	}
}

void MessageHandler::sendStartScan()
{
	if(!scanRequestComplete)
	{
		output_generator.output_debug_message("start scan sent");
		send_message(encoder->encodeStartScanRequest()); //send start scan
		timestamp = clock(); //set the timestamp to enable time limited scanning
		previously_sent_typeId = TypeId::request_start_scan;
		scanRequestComplete = true; //todo: maybe 50 ms repeat or so
	}
}

void MessageHandler::sendCancelScan()
{
	if (((double) (clock() - timestamp)) / (CLOCKS_PER_SEC / 1000) < 30000) return; //return always before 30 secs have passed
	output_generator.output_debug_message("cancel scan sent");

	if(!scanCancelComplete)
	{
		send_message(encoder->encodeCancelScanRequest()); //send start scan
		previously_sent_typeId = TypeId::request_cancel_scan;
		communicationState = subscribing;
	}
}

void MessageHandler::sendSubscribeRequests()
{
	p_measurement_scan_list = measurement_scan_list.begin();
	Sensor current_sensor;
	Measurement current_measurement;
	MeasurementRequestMessage * p_Request;

	for(p_measurement_scan_list = measurement_scan_list.begin();
		p_measurement_scan_list != measurement_scan_list.end();
		++p_measurement_scan_list)
	{
		current_measurement = *p_measurement_scan_list;
		current_sensor = *current_measurement.p_sensor_scan_list;
		if(!(current_measurement.state & SUBSCRIBE_REQUESTED))
		{
			output_generator.output_debug_message("subscribe requested");
			subscribeRequested = true;
			previously_sent_typeId = TypeId::request_subscribe;
			p_Request = encoder->encodeSubscriptionRequest(
					current_sensor.manufacturerId,
					current_sensor.deviceId,
					current_measurement.measurementId
					);
			send_message(p_Request);
			current_measurement.state |= SUBSCRIBE_REQUESTED;
			current_measurement.requestReferenceId = p_Request->getMessageId();
			*p_measurement_scan_list = current_measurement;
			return;
		}
	}
	//initialize pointer to sensor element
	//for the length of the list
	//send subscribe for the first unsubscribed element
	//return
}

void MessageHandler::handleMessage(TelemetryMessageInterface * p_Message)
{
	if (p_Message == nullptr) return;

	switch (p_Message->getTypeId())
	{
	case message_idle: 				break; //not handled
	case request_handshake:  		break; //not handled
	case request_start_scan:  		break; //not handled
	case request_cancel_scan:  		break; //not handled
	case request_information:  		break; //not handled
	case request_subscribe:  		break; //not handled
	case request_subscribe_text:  	break; //not handled
	case request_unsubscribe_text:  break; //not handled
	case maintenance_subscribe:  	break; //not handled
	case maintenance_log:  			break; //not handled
	case maintenance_unsubscribe:  	break; //not handled
	case subscribe_unsubscribe:  	break; //not handled
		/* Should never reach this position here */
	case ACK:
		processACK(); break;
	case NACK:
		processNACK(); break;
	case response_handshake:
		output_generator.output_debug_message("handshake response received");
		send_message(encoder->confirm(p_Message->getMessageId())); //send ACK
		if (((HandshakeResponseMessage *) p_Message)->getState() == HANDSHAKE_SUCCEEDED)
		{
			communicationState = scanning;
			handshakeComplete = true;
			output_generator.output_debug_message("handshake VALID");
		}
		else
		{
			send_handshake_request();
			output_generator.output_debug_message("handshake request sent");
		}
		break;
	case sensor_header:
		processSensorHeader(p_Message);
		break;
	case sensor_name:
		processSensorName(p_Message);
		break;
	case sensor_measurement:
		processSensorMeasurement(p_Message);
		break;
	case sensor_measurement_description:
	case information_pending:
	case subscribe_confirmation:
		processSubscribeConfirmation(p_Message);
	case subscribe_pending:
		processSubscribePending(p_Message);
	case subscribe_update:
		processSubscribeUpdate(p_Message);
	case subscribe_update_text:
	case checkout_notification:
		break;
		// TODO
	}
}

void MessageHandler::processACK()
{
	switch (communicationState) {
	case handshaking:
	case scanning:
		if(previously_sent_typeId == TypeId::request_start_scan)
			scanRequestComplete = true;
		else if(previously_sent_typeId == TypeId::request_cancel_scan)
			scanCancelComplete = true;
		break;
	case subscribing:
		break;
	case subscribed:
		break;
	default:
		break;
	}
}

void MessageHandler::processNACK()
{
	output_generator.output_debug_message("NACK received");
	switch (communicationState) {
	case handshaking:
	case scanning:
	case subscribing:
	case subscribed:
		break;
	default:
		break;
	}
}

void MessageHandler::processSensorHeader(TelemetryMessageInterface * p_Message)
{
	output_generator.output_debug_message("sensor header element added");
	addSensorHeader(p_Message);
	send_message(encoder->confirm(((SensorHeaderMessage *)p_Message)->getMessageId())); //ACK
}

void MessageHandler::addSensorHeader(TelemetryMessageInterface * p_message)
{
	Sensor new_sensor;

	new_sensor.manufacturerId = ((SensorHeaderMessage *)p_message)->getManufacturerId();
	new_sensor.deviceId = ((SensorHeaderMessage *)p_message)->getDeviceId();
	new_sensor.state |= SENSOR_HEADER_DETECTED;

	sensor_scan_list.emplace_back(new_sensor);
}

void MessageHandler::processSensorName(TelemetryMessageInterface * p_Message)
{
	output_generator.output_debug_message("sensor name element added");
	addSensorName(p_Message); //for sensor name pop back, modify add back
	send_message(encoder->confirm(((SensorHeaderMessage *)p_Message)->getMessageId())); //ACK
}

void MessageHandler::addSensorName(TelemetryMessageInterface * p_message)
{
	Sensor new_sensor;

	new_sensor = sensor_scan_list.back();
	sensor_scan_list.pop_back();

	strcpy(new_sensor.name,((SensorNameMessage *)p_message)->getName());
	new_sensor.state |= SENSOR_NAME_DETECTED;

	sensor_scan_list.push_back(new_sensor);
}

void MessageHandler::processSensorMeasurement(TelemetryMessageInterface * p_Message)
{
	output_generator.output_debug_message("sensor measurement element added");
	addSensorMeasurement(p_Message); //for sensor name pop back, modify add back
	send_message(encoder->confirm(((MeasurementDataMessage *)p_Message)->getMessageId())); //ACK
}

void MessageHandler::addSensorMeasurement(TelemetryMessageInterface * p_message)
{
	Measurement new_measurement;

	//new_measurement = sensor_scan_list.back();
	//sensor_scan_list.pop_back();

	strcpy(new_measurement.description,((MeasurementDataMessage *)p_message)->getDescription());
	new_measurement.measurementId = ((MeasurementDataMessage *)p_message)->getMeasurementId();
	new_measurement.p_sensor_scan_list = sensor_scan_list.end();
	new_measurement.p_sensor_scan_list--;

	measurement_scan_list.push_back(new_measurement);
}

void MessageHandler::processSubscribePending(TelemetryMessageInterface * p_message)
{
	if(subscribeRequested == true)
	{
		Measurement new_measurement;
		new_measurement = measurement_scan_list.back();
		measurement_scan_list.pop_back();

		new_measurement.state |= SUBSCRIBE_PENDING;

		measurement_scan_list.push_back(new_measurement);

		send_message(encoder->confirm(((MeasurementDataMessage *)p_message)->getMessageId())); //ACK
		subscribeRequested = false;
		output_generator.output_debug_message("subscribe pending received");

	}
}

void MessageHandler::processSubscribeConfirmation(TelemetryMessageInterface * p_message)
{
	p_measurement_scan_list = measurement_scan_list.begin();
	Sensor current_sensor;
	Measurement current_measurement;

	for(p_measurement_scan_list = measurement_scan_list.begin();
		p_measurement_scan_list != measurement_scan_list.end();
		++p_measurement_scan_list)
	{
		current_measurement = *p_measurement_scan_list;
		current_sensor = *current_measurement.p_sensor_scan_list;
		if(current_measurement.requestReferenceId == ((SubscriptionConfirmationMessage *)p_message)->getReferenceId()
			&& !(current_measurement.state & SUBSCRIBE_CONFIRMED))
		{
			output_generator.output_debug_message("subscribe confirmation received");
			current_measurement.state |= SUBSCRIBE_CONFIRMED;
			current_measurement.subscriptionId = ((SubscriptionConfirmationMessage *)p_message)->getSubscriptionId();
			*p_measurement_scan_list = current_measurement;
		}
	}
	send_message(encoder->confirm(((MeasurementDataMessage *)p_message)->getMessageId())); //ACK
	subscribeRequested = false;

}

void MessageHandler::processSubscribeUpdate(TelemetryMessageInterface * p_message)
{
	p_measurement_scan_list = measurement_scan_list.begin();
	Sensor current_sensor;
	Measurement current_measurement;

	for(p_measurement_scan_list = measurement_scan_list.begin();
		p_measurement_scan_list != measurement_scan_list.end();
		++p_measurement_scan_list)
	{
		current_measurement = *p_measurement_scan_list;
		if(current_measurement.subscriptionId == ((SubscriptionUpdateMessage *)p_message)->getSubscriptionId()
			&& current_measurement.state & SUBSCRIBE_CONFIRMED)
		{
			current_sensor = *current_measurement.p_sensor_scan_list;
			output_generator.output_debug_message("subscribe update received");

			output_generator.output_sensor_update(output, (SubscriptionUpdateMessage *)p_message, current_measurement, current_sensor);
			cout << output;
			return;
		}
	}
}

void MessageHandler::send_message(TelemetryMessageInterface * p_message)
{
	new_dev.write_buffer_size =
		encoder->encodeFraming(new_dev.write_buffer, p_message)
		- new_dev.write_buffer;
	new_dev.b_write=true;
}

void MessageHandler::send_handshake_request()
{
	send_message(encoder->encodeHandshakeRequest());
	output_generator.output_debug_message("handshake request sent");
	timestamp = clock();
}

void MessageHandler::check_timestamps()
{
	if (((double) (clock() - timestamp)) / (CLOCKS_PER_SEC / 1000) < 1000) return;

	if (!handshakeComplete)
	{
		send_handshake_request();
	}
}

uint8_t MessageHandler::last_received_messageId_smaller(uint16_t old_messageId, uint16_t new_messageId)
{
	if(old_messageId == 65535 && new_messageId >= 0)
		return 1;
	else if(new_messageId - old_messageId > 0)
		return 1;
	else
		return 0;
}
