/*
 * test_telemetry_encoder.hpp
 *
 *      Author: thibault
 */

#ifndef TEST_INC_TEST_CONSOLE_OUTPUT_GENERATOR_HPP_
#define TEST_INC_TEST_CONSOLE_OUTPUT_GENERATOR_HPP_

#ifdef UNIT_TESTING
#include <gtest/gtest.h>
#include "ConsoleOutputGenerator.hpp"

#include "telemetry_message_types.hpp"
#include "telemetry_value_type.hpp"

#include "telemetry_encoder.hpp"

#include "Measurement.hpp"
#include "Sensor.hpp"

class TestConsoleOutputGenerator :public ::testing::Test
{
public:
	TestConsoleOutputGenerator();
	~TestConsoleOutputGenerator();

protected:

	ConsoleOutputGenerator outputGenerator;
	TelemetryEncoder encoder;

	Measurement measurement;
	Sensor sensor;

	SubscriptionUpdateMessage * message;
	TelemetryValue * value;

	string output;


//	uint8_t measurementId;
//	uint8_t subscriptionId;
//	uint16_t requestReferenceId;
//
//	list <Sensor> :: iterator p_sensor_scan_list;
//
//	char description[MEASUREMENT_DESCRITION_LENGTH];
//
//	uint8_t measurement_state;
//
//	uint16_t manufacturerId;
//	uint16_t deviceId;
//
//	char name[SENSOR_INFORMATION_LENGTH];
//
//	uint8_t sensor_state;

};

#endif /* UNIT_TESTING */
#endif /* TEST_INC_TEST_CONSOLE_OUTPUT_GENERATOR_HPP_ */
