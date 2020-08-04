/*
 * Sensor.hpp
 *
 *  Created on: 20.03.2019
 *      Author: Thibault
 */

#include <stdint.h>
#include <telemetry_message_types.hpp>

#ifndef MODULES_TELEMETRYDECODER_INC_SENSOR_HPP_
#define MODULES_TELEMETRYDECODER_INC_SENSOR_HPP_

#define SENSOR_HEADER_DETECTED			1
#define SENSOR_NAME_DETECTED			2
#define SENSOR_MEASUREMENT_DETECTED		4


//class in which all important sensor data is written. Can be generated and deleted by sensorlist.

class sensor_interface {
	public:
	virtual ~sensor_interface() {};
};


class Sensor : public sensor_interface {
public:
	Sensor();
	virtual ~Sensor();

	uint16_t manufacturerId;
	uint16_t deviceId;

	char name[SENSOR_INFORMATION_LENGTH];

	uint8_t state;
};


#endif /* MODULES_TELEMETRYDECODER_INC_SENSOR_HPP_ */
