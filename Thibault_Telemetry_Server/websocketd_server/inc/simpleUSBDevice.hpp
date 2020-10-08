/*
 * USBDevice.hpp
 *
 *  Created on: 08.04.2019
 *      Author: thibault
 */

#ifndef INC_SIMPLEUSBDEVICE_HPP_
#define INC_SIMPLEUSBDEVICE_HPP_

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


class simple_USB_Device {
public:
	simple_USB_Device();
	virtual ~simple_USB_Device();
	void dev_parser();
	void open_port(char port[], unsigned int baudrate);
	void create_dev_parser_thread();

	uint8_t dev;
	int n;
	uint8_t buf_dev[200] = { 0 };

	struct termios toptions_dev;

	pthread_t thread_id_dev;


};

#endif /* INC_SIMPLEUSBDEVICE_HPP_ */
