/*
 * ConsoleOutputGenerator.hpp
 *
 *  Created on: 24.05.2019
 *      Author: thibault
 */

#ifndef INC_CONSOLEOUTPUTGENERATOR_HPP_
#define INC_CONSOLEOUTPUTGENERATOR_HPP_

#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "Measurement.hpp"
#include "Sensor.hpp"
#include "telemetry_message_ids.hpp"
#include "telemetry_custom.hpp"

class ConsoleOutputGenerator {
public:
	ConsoleOutputGenerator();
	virtual ~ConsoleOutputGenerator();

	void output_debug_message(std::string string);
	void output_sensor_update(std::string & string, SubscriptionUpdateMessage * update_message, Measurement measurement, Sensor sensor);
	void sensor_update_numeric(std::string & string, TelemetryValue * current_data);
	void sensor_update_date(std::string & string, TelemetryValue * current_data);
	void sensor_update_time(std::string & string, TelemetryValue * current_data);
	void sensor_update_gps(std::string & string, TelemetryValue * current_data);

};

#endif /* INC_CONSOLEOUTPUTGENERATOR_HPP_ */
