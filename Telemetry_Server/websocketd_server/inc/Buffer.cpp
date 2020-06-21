/*
 * Buffer.cpp
 *
 *  Created on: 12.04.2019
 *      Author: thibault
 */

#include <Buffer.hpp>
#include <stdint.h>
//#include <stdlib.h>
#include <stdio.h>
#include <string.h>



Buffer::Buffer() {
	// TODO Auto-generated constructor stub

}

Buffer::~Buffer() {
	// TODO Auto-generated destructor stub
}


void Buffer::add_to_buffer(uint8_t * p_buffer_to_add, uint8_t size_to_add)
{
	//printf("sizeto add: %d", size_to_add);
	memcpy(p_buffer_end, p_buffer_to_add, size_to_add);

	p_buffer_end += size_to_add;

}

void Buffer::delete_from_buffer(uint8_t number_of_bytes)
{
	if (number_of_bytes >= p_buffer_end-p_buffer_start)
	{
		//do nothing
	}
	else
	{
		p_buffer_end -= number_of_bytes;
	}
}

void Buffer::clear_buffer()
{
	p_buffer_start = p_buffer_end = &buffer[0];
}

uint8_t Buffer::get_start_position()
{
	return p_buffer_start-buffer;
}

uint8_t Buffer::get_end_position()
{
	return p_buffer_end-buffer;
}

void Buffer::set_start_position(uint8_t position)
{
	p_buffer_start = buffer+position;
}

void Buffer::increment_end_position(uint8_t length)
{
	p_buffer_end = buffer+length; //todo
}
void Buffer::increment_start_position(uint8_t length)
{
	p_buffer_start = buffer+length; //todo
}

void Buffer::set_end_position(uint8_t position)
{
	p_buffer_end = buffer+position;
}

uint8_t Buffer::get_buffer_size()
{
	return (this->p_buffer_end - this->p_buffer_start);
}

