/*
 * test_telemetry_encoder.cpp
 *
 *      Author: thibault
 */

#ifndef TEST_INC_TEST_CONSOLE_OUTPUT_GENERATOR_CPP_
#define TEST_INC_TEST_CONSOLE_OUTPUT_GENERATOR_CPP_

#ifdef UNIT_TESTING
#include <gtest/gtest.h>
#include "test_ConsoleOutput.hpp"

TestConsoleOutputGenerator::TestConsoleOutputGenerator() {

	char description[21] = "description#unit";
	char name[20] = "sensor_name";

	measurement.measurementId = 0x01;
	strcpy(measurement.description, description);
	measurement.requestReferenceId = 0x1649;
	measurement.subscriptionId = 0x10;

	sensor.deviceId = 0xABCD;
	sensor.manufacturerId = 0x1234;
	strcpy(sensor.name, name);
}

TestConsoleOutputGenerator::~TestConsoleOutputGenerator() {}

#endif /* UNIT_TESTING */
#endif /* TEST_INC_TEST_CONSOLE_OUTPUT_GENERATOR_CPP_ */
