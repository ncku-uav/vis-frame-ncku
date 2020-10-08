/*
 * USBDevice.hpp
 *
 *  Created on: 08.04.2019
 *      Author: thibault
 */

#ifndef INC_USBDEVICE_HPP_
#define INC_USBDEVICE_HPP_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <time.h>
#include <pthread.h>

#include "Buffer.hpp"


class USB_Device {
public:
	USB_Device();
	virtual ~USB_Device();
	void dev_parser();
	void open_port(char port[], uint16_t baudrate);
	void create_dev_parser_thread();

	Buffer device_buffer;

	bool b_write = false;

	uint8_t dev;
	int n;
	//char buf_dev[1024] = "temp text";
	uint8_t buf_dev[1] = { 0 };
	uint8_t write_buffer[100] = { 0 };
	uint8_t write_buffer_size = 0;
	struct termios toptions_dev;



};

#endif /* INC_USBDEVICE_HPP_ */
