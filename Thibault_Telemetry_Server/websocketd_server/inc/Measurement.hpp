/*
 * Measurement.hpp
 *
 *      Author: Thibault
 */

#include <stdint.h>
#include <telemetry_message_types.hpp>
#include <list>
#include <iterator>
using namespace std;

#include "Sensor.hpp"

#ifndef MODULES_TELEMETRYDECODER_INC_MEASUREMENT_HPP_
#define MODULES_TELEMETRYDECODER_INC_MEASUREMENT_HPP_

#define SUBSCRIBE_REQUESTED			1
#define SUBSCRIBE_PENDING			2
#define SUBSCRIBE_CONFIRMED			4

class Measurement{
public:
	Measurement();
	virtual ~Measurement();

	uint8_t measurementId;
	uint8_t subscriptionId;
	uint16_t requestReferenceId;

	list <Sensor> :: iterator p_sensor_scan_list;

	char description[MEASUREMENT_DESCRITION_LENGTH];

	uint8_t state;
};


#endif /* MODULES_TELEMETRYDECODER_INC_MEASUREMENT_HPP_ */
