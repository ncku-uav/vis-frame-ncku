/*
 * Buffer.hpp
 *
 *  Created on: 12.04.2019
 *      Author: thibault
 */

#ifndef INC_BUFFER_HPP_
#define INC_BUFFER_HPP_

#include <stdint.h>

class Buffer {
public:
	uint8_t buffer[256] = {0};
	uint8_t * p_buffer_start = &buffer[0];
	uint8_t * p_buffer_end = &buffer[0];

	Buffer();
	virtual ~Buffer();

	void add_to_buffer(uint8_t * p_buffer_to_add, uint8_t size_to_add);
	void delete_from_buffer(uint8_t number_of_bytes);
	void clear_buffer();
	uint8_t get_start_position();
	uint8_t get_end_position();
	void set_start_position(uint8_t position);
	void set_end_position(uint8_t position);
	void increment_start_position(uint8_t length);
	void increment_end_position(uint8_t length);
	uint8_t get_buffer_size();
};

#endif /* INC_BUFFER_HPP_ */
