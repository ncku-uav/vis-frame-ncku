/*
 * test_ConsoleOutputGenerator.hpp
 * Author: thibault
 *
 */


#include "test_ConsoleOutput.hpp"

#ifdef UNIT_TESTING
#include<gtest/gtest.h>

TEST_F(TestConsoleOutputGenerator, Numeric)
{
	float numeric = 27.0412;
	message = encoder.encodeNumericSubscriptionUpdate(measurement.subscriptionId, numeric);
	outputGenerator.output_sensor_update(output, message, measurement, sensor);

	//cout << output;

	EXPECT_STREQ(output.c_str(), "{\"JETI\":{\"sensor\":\"sensor_name\",\"description\":\"description\",\"data\":{\"unit\":\"unit\",\"type\":\"numeric\",\"numeric\":\"27.0412\"}}}\n");
}

TEST_F(TestConsoleOutputGenerator, Date)
{
	Date date(4, 2, 1996);

	message = encoder.encodeDateSubscriptionUpdate(measurement.subscriptionId, date);
	outputGenerator.output_sensor_update(output, message, measurement, sensor);

	//cout << output;

	EXPECT_STREQ(output.c_str(), "{\"JETI\":{\"sensor\":\"sensor_name\",\"description\":\"description\",\"data\":{\"unit\":\"unit\",\"type\":\"date\",\"date\":{\"d\":\"4\",\"m\":\"2\",\"y\":\"1996\"}}}}\n");
}

TEST_F(TestConsoleOutputGenerator, Time)
{
	Time time(23, 14, 03);

	message = encoder.encodeTimeSubscriptionUpdate(measurement.subscriptionId, time);
	outputGenerator.output_sensor_update(output, message, measurement, sensor);

	//cout << output;

	EXPECT_STREQ(output.c_str(), "{\"JETI\":{\"sensor\":\"sensor_name\",\"description\":\"description\",\"data\":{\"unit\":\"unit\",\"type\":\"time\",\"time\":{\"h\":\"23\",\"m\":\"14\",\"s\":\"3\"}}}}\n");
}

TEST_F(TestConsoleOutputGenerator, Gps)
{
	Gps gps(GpsOrientation::LATITUDE, 48.0001);

	message = encoder.encodeGpsSubscriptionUpdate(measurement.subscriptionId, gps);
	outputGenerator.output_sensor_update(output, message, measurement, sensor);

	//cout << output;

	EXPECT_STREQ(output.c_str(), "{\"JETI\":{\"sensor\":\"sensor_name\",\"description\":\"description\",\"data\":{\"unit\":\"unit\",\"type\":\"gps\",\"gps\":{\"ori\":\"lat\",\"pos\":\"48.0001\"}}}}\n");
}

TEST_F(TestConsoleOutputGenerator, Debug)
{
	outputGenerator.output_debug_message("test");
}

#endif /* UNIT_TESTING */



