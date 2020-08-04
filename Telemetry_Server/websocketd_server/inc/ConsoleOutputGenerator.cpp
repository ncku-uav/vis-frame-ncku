/*
 * ConsoleOutputGenerator.cpp
 *
 *  Created on: 24.05.2019
 *      Author: thibault
 */

#include <ConsoleOutputGenerator.hpp>

#include <boost/algorithm/string.hpp>



template < typename Type > std::string to_str (const Type & t)
{
  std::ostringstream os;
  os << t;
  return os.str();
}

ConsoleOutputGenerator::ConsoleOutputGenerator() {}

ConsoleOutputGenerator::~ConsoleOutputGenerator() {}

void ConsoleOutputGenerator::output_debug_message(std::string string)
{

	cout << "{\"" + to_str(HANDSHAKE_IDENTIFICATION_KEY) + "\":{" +
			"\"debug\":\"" + string + "\"}}\n";
}

void ConsoleOutputGenerator::output_sensor_update(std::string & string, SubscriptionUpdateMessage * update_message, Measurement measurement, Sensor sensor)
{
	TelemetryType current_type = update_message->getDataType();
	TelemetryValue * current_data = update_message->getData();

	char * description;
	char * unit;

	std::string type;

	description = strtok(measurement.description, "#");
	unit = strtok(NULL, "");

	switch(current_type)
	{
	case(TelemetryType::UNKOWN_VALUE_ID):
		break;
	case(TelemetryType::NUMERIC):
		sensor_update_numeric(string, current_data);
		type = "numeric";
		break;
	case(TelemetryType::GPS):
		sensor_update_gps(string, current_data);
		type = "gps";
		break;
	case(TelemetryType::TIME):
		sensor_update_time(string, current_data);
		type = "time";
		break;
	case(TelemetryType::DATE):
		sensor_update_date(string, current_data);
		type = "date";
		break;
	}

	string = "{\"" + to_str(HANDSHAKE_IDENTIFICATION_KEY) + "\":{" +
			"\"sensor\":\""+ to_str(sensor.name) + "\"," +
			"\"description\":\""+ to_str(description) + "\"," +
			"\"data\":{" +
			"\"unit\":\"" + to_str(unit) + "\"," +
			"\"type\":\"" + type + "\"," +
			string +
			"}}}\n";
}



void ConsoleOutputGenerator::sensor_update_numeric(std::string & string, TelemetryValue * current_data)
{
	string = "\"numeric\":\"" + to_str(current_data->numeric) + "\"";
}

void ConsoleOutputGenerator::sensor_update_date(std::string & string, TelemetryValue * current_data)
{
	string = to_str("\"date\":{") +
			"\"d\":\"" + to_str((uint16_t)(current_data->date.getDay())) + "\"," +
			"\"m\":\"" + to_str((uint16_t)(current_data->date.getMonth())) + "\"," +
			"\"y\":\"" + to_str(current_data->date.getYear()) + "\"" +
			"}";
}

void ConsoleOutputGenerator::sensor_update_time(std::string & string, TelemetryValue * current_data)
{
	string = to_str("\"time\":{") +
			"\"h\":\"" + to_str((uint16_t)(current_data->time.getHour())) + "\"," +
			"\"m\":\"" + to_str((uint16_t)(current_data->time.getMinute())) + "\"," +
			"\"s\":\"" + to_str((uint16_t)(current_data->time.getSecond())) + "\"" +
			"}";
}

void ConsoleOutputGenerator::sensor_update_gps(std::string & string, TelemetryValue * current_data)
{
	std::string orientation;
	switch(current_data->gps.getOrientation())
	{
		case(GpsOrientation::LATITUDE):
			orientation = "lat";
			break;
		case(GpsOrientation::LONGITUDE):
			orientation = "lon";
			break;
		default:
			orientation = "ndef";
			break;
	}
	string = to_str("\"gps\":{") +
			"\"ori\":\"" + orientation + "\"," +
			"\"pos\":\"" + to_str(current_data->gps.getPosition()) + "\"" +
			"}";
}

