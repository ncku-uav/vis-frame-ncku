/*
 * sensorlist.hpp
 *
 *  Created on: 20.03.2019
 *      Author: Thibault
 */

#ifndef MODULES_TELEMETRYDECODER_INC_SENSORLIST_HPP_
#define MODULES_TELEMETRYDECODER_INC_SENSORLIST_HPP_

#include <Sensor.hpp>

class sensor_list {
public:
	sensor_list();
	virtual ~sensor_list();

	sensor_interface * p_sensor = new sensor_interface();

	void insert_element();
	void get_data();
	void set_data();
	void delete_element();

};

#endif /* MODULES_TELEMETRYDECODER_INC_SENSORLIST_HPP_ */
