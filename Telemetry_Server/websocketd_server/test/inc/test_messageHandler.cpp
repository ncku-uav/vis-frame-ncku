/*
 * test_messageHandler.cpp
 * Author: thibault
 *
 */


#include <MessageHandler.hpp>
#include <telemetry_message_ids.hpp>

#include <iostream>
#include <list>
#include <iterator>
using namespace std;

#include <Sensor.hpp>

#ifdef UNIT_TESTING
#include<gtest/gtest.h>

#define xMessage_Handler_Tests
#ifdef Message_Handler_Tests
TEST(MessageHandlerTest, name)
{
	TelemetryDecoder * decoder = new TelemetryDecoder();

	Buffer * receive_buffer = new Buffer();
	uint8_t test_string[] =
	{
			0x7E,
			TypeId::request_handshake, //typeId
			0x51, 0xA2, //messageIdvariable
			0x00
	};

	uint16_t size_to_add = sizeof(test_string);
	receive_buffer->add_to_buffer(test_string, size_to_add);

	MessageHandler message_handler;
	message_handler.handleBuffer(receive_buffer);

	//EXPECT_EQ(((ResponseMessage_t *)message_handler.interface)->typeId,TypeID_t::request_handshake);
	//EXPECT_EQ(((ResponseMessage_t *)message_handler.interface)->messageId, 0xA251);
}
#endif /* Message_Handler_Tests */

//function for printing the elements in a list
void showlist(list <Sensor> g)
{
    list <Sensor> :: iterator it;
    Sensor current_sensor;
    for(it = g.begin(); it != g.end(); ++it)
    {
    	current_sensor = *it;
        cout << '\t' << current_sensor.deviceId;
    }
    cout << '\n';
}

TEST(MessageHandlerTest, processSensorHeader)
{
	TelemetryEncoder * encoder = new TelemetryEncoder();
	TelemetryMessageInterface * p_Message;
	MessageHandler message_handler;
	Sensor current_sensor;

	list <Sensor> :: iterator p_List;

	p_Message = encoder->encodeSensorHeader(0x0101,0x0202,0x0303); //add sensor
	message_handler.processSensorHeader(p_Message);

	p_List = message_handler.sensor_scan_list.begin();

	current_sensor = *p_List;
	EXPECT_EQ(current_sensor.deviceId, 0x0303);
	EXPECT_EQ(current_sensor.manufacturerId, 0x0202);
	EXPECT_EQ(current_sensor.state, 1);

	p_Message = encoder->encodeSensorHeader(0x0203,0x0302,0x0403); //add new sensor
	message_handler.processSensorHeader(p_Message);

	current_sensor = *++p_List;
	EXPECT_EQ(current_sensor.deviceId, 0x0403);
	EXPECT_EQ(current_sensor.manufacturerId, 0x0302);
	EXPECT_EQ(current_sensor.state, 1);

	p_Message = encoder->encodeSensorHeader(0x0102,0x0202,0x0303);
	message_handler.processSensorHeader(p_Message);

	current_sensor = *++p_List;
	EXPECT_EQ(current_sensor.deviceId, 0x0303);
	EXPECT_EQ(current_sensor.manufacturerId, 0x0202);
	EXPECT_EQ(current_sensor.state, 1);
}


TEST(MessageHandlerTest, processSensorName)
{
	TelemetryEncoder * encoder = new TelemetryEncoder();
	TelemetryMessageInterface * p_Message;
	MessageHandler message_handler;
	Sensor current_sensor;
	char name[21] = "test";

	list <Sensor> :: iterator p_List;

	p_Message = encoder->encodeSensorHeader(0x0101,0x0202,0x0303); //add sensor
	message_handler.processSensorHeader(p_Message);

	p_Message = encoder->encodeSensorName(0x0010, name); //add name
	message_handler.processSensorName(p_Message);

	p_List = message_handler.sensor_scan_list.begin();

	current_sensor = *p_List;
	EXPECT_EQ(current_sensor.deviceId, 0x0303);
	EXPECT_EQ(current_sensor.manufacturerId, 0x0202);
	EXPECT_EQ(current_sensor.state, 3);
	EXPECT_STREQ(current_sensor.name, "test");
}

TEST(MessageHandlerTest, processSensorMeasurement)
{
	TelemetryEncoder * encoder = new TelemetryEncoder();
	TelemetryMessageInterface * p_Message;
	MessageHandler message_handler;
	Sensor current_sensor;
	Measurement current_measurement;
	char description[21] = "description";

	list <Sensor> :: iterator p_List_Sensor;
	list <Measurement> :: iterator p_List_Measurement;

	p_Message = encoder->encodeSensorHeader(0x0101,0x0202,0x0303); //add sensor
	message_handler.processSensorHeader(p_Message);

	p_Message = encoder->encodeMeasurementWithId(0x0010, 0xAB, description); //add measurement
	message_handler.processSensorMeasurement(p_Message);

	p_List_Measurement = message_handler.measurement_scan_list.begin();

	current_measurement = *p_List_Measurement;
	EXPECT_EQ(current_measurement.measurementId, 0xAB);
//	EXPECT_EQ(current_measurement.requestReferenceId, 0x0010);
	EXPECT_STREQ(current_measurement.description, "description");

	current_sensor = *current_measurement.p_sensor_scan_list;

	EXPECT_EQ(current_sensor.deviceId, 0x0303);
	EXPECT_EQ(current_sensor.manufacturerId, 0x0202);
	EXPECT_EQ(current_sensor.state, 1);
}

TEST(MessageHandlerTest, integrationSensorHeaderNameMeasurement)
{
	TelemetryEncoder * encoder = new TelemetryEncoder();
	TelemetryMessageInterface * p_Message;
	MessageHandler message_handler;
	Sensor current_sensor;
	Measurement current_measurement;
	char name[21] = "test";
	char name2[21] = "test2";
	char description[21] = "description";
	char description2[21] = "description2";

	list <Sensor> :: iterator p_List_Sensor;
	list <Measurement> :: iterator p_List_Measurement;

	p_Message = encoder->encodeSensorHeader(0x0101,0x0202,0x0303); //add sensor
	message_handler.processSensorHeader(p_Message);
	p_Message = encoder->encodeSensorName(0x0010, name); //add name
	message_handler.processSensorName(p_Message);
	p_Message = encoder->encodeMeasurementWithId(0x0010, 0xAB, description); //add measurement
	message_handler.processSensorMeasurement(p_Message);

	p_Message = encoder->encodeSensorHeader(0x0101,0x02BB,0x03AA); //add second sensor
	message_handler.processSensorHeader(p_Message);
	p_Message = encoder->encodeSensorName(0x0011, name2); //add name
	message_handler.processSensorName(p_Message);
	p_Message = encoder->encodeMeasurementWithId(0x0011, 0x13, description); //add measurement
	message_handler.processSensorMeasurement(p_Message);
	p_Message = encoder->encodeMeasurementWithId(0x0012, 0xCD, description2); //add second measurement
	message_handler.processSensorMeasurement(p_Message);

	p_List_Sensor = message_handler.sensor_scan_list.begin();
	p_List_Measurement = message_handler.measurement_scan_list.begin();

	current_measurement = *p_List_Measurement; //check first measurement in list with according sensor
	EXPECT_EQ(current_measurement.measurementId, 0xAB);
//	EXPECT_EQ(current_measurement.requestReferenceId, 0x0010);
	EXPECT_STREQ(current_measurement.description, "description");
	current_sensor = *current_measurement.p_sensor_scan_list;
	EXPECT_EQ(current_sensor.deviceId, 0x0303);
	EXPECT_EQ(current_sensor.manufacturerId, 0x0202);
	EXPECT_EQ(current_sensor.state, 3);

	current_measurement = *++p_List_Measurement; //check second measurement in list with according sensor
	EXPECT_EQ(current_measurement.measurementId, 0x13);
//	EXPECT_EQ(current_measurement.requestReferenceId, 0x0011);
	EXPECT_STREQ(current_measurement.description, "description");
	current_sensor = *current_measurement.p_sensor_scan_list;
	EXPECT_EQ(current_sensor.deviceId, 0x03AA);
	EXPECT_EQ(current_sensor.manufacturerId, 0x02BB);
	EXPECT_EQ(current_sensor.state, 3);

	current_measurement = *++p_List_Measurement; //check third measurement in list with according sensor
	EXPECT_EQ(current_measurement.measurementId, 0xCD);
//	EXPECT_EQ(current_measurement.requestReferenceId, 0x0012);
	EXPECT_STREQ(current_measurement.description, "description2");
	current_sensor = *current_measurement.p_sensor_scan_list;
	EXPECT_EQ(current_sensor.deviceId, 0x03AA);
	EXPECT_EQ(current_sensor.manufacturerId, 0x02BB);
	EXPECT_EQ(current_sensor.state, 3);

	EXPECT_EQ(message_handler.sensor_scan_list.size(), 2);
	EXPECT_EQ(message_handler.measurement_scan_list.size(), 3);

}

#endif /* UNIT_TESTING */



